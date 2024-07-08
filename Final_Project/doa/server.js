const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const fs = require('fs');
const cookie = require('cookie');
const cookieParser = require('cookie-parser');
const crypto = require('crypto');
const path = require('path');

const secretKey = crypto.randomBytes(64).toString('hex');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(cookieParser(secretKey));
app.use(express.static('public'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/public/index.html');
});

app.get('/lobby', (req, res) => {
    res.sendFile(__dirname + '/public/lobby.html');
});

app.get('/selection', (req, res) => {
    res.sendFile(__dirname + '/public/selection.html');
});

app.get('/waiting', (req, res) => {
    res.sendFile(__dirname + '/public/waiting.html');
});

app.get('/guessing', (req, res) => {
    res.sendFile(__dirname + '/public/guessing.html');
});

app.get('/scoreboard', (req, res) => {
    res.sendFile(__dirname + '/public/scoreboard.html');
});

const users = {};
let submissions = [];
let allPeople = [];
const guessStorage = {};
let onlineUsers = 0;
let maxUsers = 0;
let currentPersonIndex = 0;

const dataDir = path.join(__dirname, 'data');
if (!fs.existsSync(dataDir)) {
    fs.mkdirSync(dataDir);
}

const submissionsPath = path.join(dataDir, 'submissions.json');
if (fs.existsSync(submissionsPath)) {
    const fileData = fs.readFileSync(submissionsPath, 'utf8');
    try {
        submissions = JSON.parse(fileData);
        submissions.forEach(submission => {
            allPeople.push({ name: submission.person1, status: submission.status1 });
            allPeople.push({ name: submission.person2, status: submission.status2 });
            allPeople.push({ name: submission.person3, status: submission.status3 });
        });
    } catch (err) {
        console.error('Error parsing submissions file:', err);
        submissions = [];
    }
}

function advanceToNextPerson() {
    currentPersonIndex++;
    if (currentPersonIndex < allPeople.length) {
        console.log('Advancing to next person:', allPeople[currentPersonIndex].name);
        console.log('Current person index:', currentPersonIndex);

        io.emit('next person', { personIndex: currentPersonIndex, personName: allPeople[currentPersonIndex].name });
    } else {
        // Ensure guesses are written to the file when all guesses are completed
        fs.writeFileSync(path.join(dataDir, 'guesses.json'), JSON.stringify(guessStorage, null, 2));
        io.emit('all guesses completed');
    }
}

async function readJSONFile(filePath) {
    return new Promise((resolve, reject) => {
        fs.readFile(filePath, 'utf8', (err, data) => {
            if (err) {
                reject(err);
            } else {
                try {
                    const json = JSON.parse(data);
                    resolve(json);
                } catch (parseErr) {
                    reject(parseErr);
                }
            }
        });
    });
}

const calculateScores = async () => {
    try {
        const submissions = await readJSONFile(path.join(dataDir, 'submissions.json'));
        const guesses = await readJSONFile(path.join(dataDir, 'guesses.json'));

        const scores = {};

        for (const [username, userGuesses] of Object.entries(guesses)) {
            let score = 0;
            const userSubmission = submissions.find(sub => sub.username === username);

            if (userSubmission) {
                for (const [personId, guessedStatus] of Object.entries(userGuesses)) {
                    if (
                        (userSubmission.person1 === personId && userSubmission.status1.toLowerCase() === guessedStatus.toLowerCase()) ||
                        (userSubmission.person2 === personId && userSubmission.status2.toLowerCase() === guessedStatus.toLowerCase()) ||
                        (userSubmission.person3 === personId && userSubmission.status3.toLowerCase() === guessedStatus.toLowerCase())
                    ) {
                        score++;
                    }
                }
            }
            

            scores[username] = score;
        }
        return scores;
    } catch (err) {
        console.error('Error calculating scores:', err);
        throw err;
    }
};

io.on('connection', (socket) => {
    onlineUsers++;
    if (onlineUsers > maxUsers) {
        maxUsers = onlineUsers;
    }
    console.log('Users connected:', onlineUsers);
    const cookieHeader = socket.handshake.headers.cookie;
    if (cookieHeader) {
        const parsedCookies = cookie.parse(cookieHeader);
        const signedCookies = cookieParser.signedCookies(parsedCookies, secretKey);
        const username = signedCookies['username'];

        if (username) {
            users[socket.id] = { username, ready: false };
            if (Object.keys(users).length === 1) {
                users[socket.id].admin = true;
            }
            socket.emit('set cookie', { username, admin: users[socket.id].admin });
            socket.emit('redirect to lobby');
            io.emit('user list', getAllUsers());
        }
    } else {
        console.log('No cookies found in the socket handshake.');
    }

    socket.on('kick user', (data) => {
        const admin = getUserBySocketId(socket.id);
        if (admin && admin.admin && socket.id !== data.socketId) {
            io.to(data.socketId).emit('kicked');
            io.to(data.socketId).emit('redirect', '/'); // Ensure the kicked user is redirected
            removeUserBySocketId(data.socketId);
            io.emit('user list', getAllUsers());
        }
    });

    socket.on('skip countdown', () => {
        const admin = getUserBySocketId(socket.id);
        if (admin && admin.admin) {
            clearInterval(countdownInterval);
            io.emit('update countdown', { countdown: 0 }); // Set countdown to 0
            io.emit('redirect to selection');
        }
    });

    socket.on('set name', (data) => {
        const name = data.name;
        console.log(submissions);
        if (Object.values(submissions).some(submission => submission.username === name)) {
            socket.emit('submission error', { error: 'You have already submitted your selections.' });
            socket.emit('redirect', '/waiting');
            console.log('User has already submitted selections:', name);
            return;
        }
        if (isNameTaken(name)) {
            socket.emit('name taken', { error: 'This name is already taken. Please choose another one.' });
        } else {
            users[socket.id] = { username: name, ready: false };
            if (Object.keys(users).length === 1) {
                users[socket.id].admin = true;
            }
            socket.emit('set cookie', { username: name, admin: users[socket.id].admin });
            socket.emit('redirect to lobby');
            io.emit('user list', getAllUsers());
        }
    });

    socket.on('toggle ready', () => {
        if (users[socket.id]) {
            users[socket.id].ready = !users[socket.id].ready;
            io.emit('user list', getAllUsers());

            if (areAllPlayersReady()) {
                io.emit('start countdown');
                startCountdown();
            }
        }
    });

    socket.on('cancel countdown', () => {
        if (users[socket.id] && users[socket.id].admin) {
            clearInterval(countdownInterval);
            io.emit('cancel countdown');
            users[socket.id].ready = false;
            io.emit('user list', getAllUsers());
        }
    });

    socket.on('disconnect', () => {
        onlineUsers--;

        console.log('Users connected:', onlineUsers);
        try {
            console.log('User disconnected:', users[socket.id]["username"]);
        } catch {
            console.log('User disconnected');
        }
        const wasAdmin = users[socket.id] && users[socket.id].admin;
        delete users[socket.id];
        io.emit('user list', getAllUsers());

        if (wasAdmin) {
            const remainingUsers = Object.keys(users);
            if (remainingUsers.length > 0) {
                users[remainingUsers[0]].admin = true;
                io.emit('user list', getAllUsers());
            }
        }
    });

    socket.on('submitSelections', (data) => {
        const username = data.username;

        if (!username) {
            socket.emit('submission error', { error: 'You must be logged in to submit your selections.' });
            socket.emit('redirect', '/');
            console.log('User not logged in:', socket.id);
            return;
        }

        if (Object.values(submissions).some(submission => submission.username === username)) {
            socket.emit('submission error', { error: 'You have already submitted your selections.' });
            socket.emit('redirect', '/waiting');
            console.log('User has already submitted selections:', username);
            return;
        }

        const filePath = path.join(__dirname, './data/submissions.json');
        fs.readFile(filePath, 'utf8', (err, fileData) => {
            if (err && err.code !== 'ENOENT') {
                console.error('Error reading file:', err);
                return;
            }

            let submissionsArray;

            if (fileData) {
                try {
                    submissionsArray = JSON.parse(fileData);
                } catch (parseErr) {
                    console.error('Error parsing JSON:', parseErr);
                    submissionsArray = [];
                }
            } else {
                submissionsArray = [];
            }


            const { 'person1-id': person1Id, 'person2-id': person2Id, 'person3-id': person3Id, ...filteredData } = data;

            submissionsArray.unshift({ socketId: socket.id, ...filteredData });

            fs.writeFile(filePath, JSON.stringify(submissionsArray, null, 2), (writeErr) => {
                if (writeErr) {
                    console.error('Error writing file:', writeErr);
                } else {
                    submissions = submissionsArray;
                    allPeople = [];
                    submissions.forEach(submission => {
                        allPeople.push({ name: submission.person1, status: submission.status1 });
                        allPeople.push({ name: submission.person2, status: submission.status2 });
                        allPeople.push({ name: submission.person3, status: submission.status3 });
                    });
                    socket.emit('redirect', '/waiting');
                }
            });
        });

        if (Object.values(submissions).length === maxUsers - 1) {
            console.log('Everyone has submitted their selections.');
            setTimeout(() => {
                io.emit('everyone submitted');
            }, 2000);
        }
    });

socket.on('submitGuess', (data) => {
    console.log('Received guess:', data);
    const { username, personName, guess } = data;

    if (!username) {
        socket.emit('submission error', { error: 'You must be logged in to submit your guess.' });
        socket.emit('redirect', '/');
        return;
    }

    if (!guessStorage[username]) {
        guessStorage[username] = {};
    }

    guessStorage[username][personName] = guess;

    const usersGuessed = Object.keys(guessStorage).filter(user => guessStorage[user][personName]).length;

    console.log('Users guessed:', usersGuessed);
    console.log('Max users:', maxUsers);

    if (usersGuessed === maxUsers) {
        advanceToNextPerson();
    }
});

function advanceToNextPerson() {
    currentPersonIndex++;
    while (currentPersonIndex < allPeople.length && allGuessedOn(allPeople[currentPersonIndex].name)) {
        currentPersonIndex++;
    }
    if (currentPersonIndex < allPeople.length) {
        io.emit('next person', { personIndex: currentPersonIndex, personName: allPeople[currentPersonIndex].name });
    } else {
        // Ensure guesses are written to the file when all guesses are completed
        fs.writeFileSync(path.join(dataDir, 'guesses.json'), JSON.stringify(guessStorage, null, 2));
        io.emit('all guesses completed');
    }
}

// Helper function to check if all users have guessed on a person
function allGuessedOn(personName) {
    return Object.values(guessStorage).every(guesses => guesses[personName]);
}

socket.on('request next person', () => {
    if (currentPersonIndex < allPeople.length) {
        io.emit('next person', { personIndex: currentPersonIndex, personName: allPeople[currentPersonIndex].name });
    } else {
        io.emit('all guesses completed');
    }
});

    socket.on('request scoreboard', async () => {
        const points = await calculateScores();
        const scoreboardData = Object.keys(guessStorage).map(username => ({
            username,
            guesses: guessStorage[username],
            points: points[username]
        }));
        socket.emit('scoreboard data', scoreboardData);
    });
});

function getUserBySocketId(socketId) {
    return users[socketId];
}

function removeUserBySocketId(socketId) {
    delete users[socketId];
}

function isNameTaken(name) {
    const taken = Object.values(users).some(user => user.username === name);
    return taken;
}

function getAllUsers() {
    return Object.keys(users).map(socketId => ({
        socketId,
        username: users[socketId].username,
        ready: users[socketId].ready,
        admin: users[socketId].admin
    }));
}

function areAllPlayersReady() {
    return Object.values(users).every(user => user.ready);
}

function startCountdown() {
    let countdown = 10;
    io.emit('update countdown', { countdown });
    countdownInterval = setInterval(() => {
        countdown -= 1;
        io.emit('update countdown', { countdown });
        if (countdown <= 0) {
            clearInterval(countdownInterval);
            io.emit('redirect to selection');
        }
    }, 1000);
}

server.listen(3000, '0.0.0.0', () => {
    console.log('Listening on *:3000');
    let submissions = [];
    fs.writeFile(path.join(dataDir, 'submissions.json'), '[]', (err) => {
        if (err) {
            console.error('Error initializing submissions.json:', err);
        } else {
            console.log('submissions.json initialized successfully.');
        }
    });
});
