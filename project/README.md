# Dead or Alive
#### Video Demo:  <URL HERE>
#### Description:

For my final project I decided to make a web game called Dead or Alive. The idea was originally from my uncle who wanted
a game where you could guess if a person is, as the title implies, dead or alive. I thought it was a fun idea and decided
to make it into a web game. The game is simple, first you choose a name to go under. Then, when all players have joined,
the admin, the first person that joined, can start the game. Every player will then be able to write the name of three 
people, either a celebrity or a person they know, or choose a random from a database. When all players have submitted 
three names, the game will start. The game will then show a name and the players will have to guess if the person is dead
or alive. The game will keep track of the score and the player with the most points at the end will show up on the top of
the leaderboard. The game is made with HTML, CSS, JavaScript, as well as a little bit of golang for a rest API that 
fetches a few random names from a database. 

I started the project by creating the basic layout of the game. I made a lobby where players could join and a submission 
page where players could submit their names. I then created a game page where the game would be played. The hardest part 
of the entire project was to let all players join the game and submit their names. I had to use websockets to make this 
work. I had never used websockets before so it was a bit of a challenge. I also had to make a rest API that would fetch 
the names and statuses from a data. That was not that challenging since I had done it before. When I had all the basic
functionality working I started to style the game. I also had to add some features to the game, like a leaderboard and 
an admin system that lets the admin start the game, kick player, skip and cancel the countdown. 

I spent a lot of time figuring out how to let all players join the game. I do not think I made the best solution, but it
is the best I could come up with. I should probably have use something other than websockets, but I wanted to learn how
to use them and it was a fun challenge. I also spent a lot of time styling the game. I am not the best at front-end and
I had to learn a lot of new things to make the game look the way it does. However, I am happy with the result. I think the
game looks good and it works mostly as intended. There was quite a few bugs that I had to fix and a lot of features that
I came up with that I had to implement. Like, countdown, leaderboard, admin system, and the ability to get and post to/from
the database. 

The project is set up like this:

Node-modules: Contains all the node modules that are needed to run the project.

Public: Contains all the files that are needed to run the project. 
    - CSS: Contains the CSS files for each page, as well as a global CSS file.
    - data: Where the players, their submissions, and guesses are stored.
    - socket.io: Contains the socket.io.js file.
    - All the HTML files for the different pages.

rest-api:
    - db.py: Initializes the database and adds some names and statuses to it.
    - main.go: The main file that starts the rest API. It has three endpoints, one that fetches all the names, one that
        fetches a random name, and one post endpoint that adds a new person to the database.
    - people.db: The database that contains the names and statuses.
    - go.sum: Contains the dependencies for the rest api.

server.js: The file that starts the server and sets up the websockets. As well as handling the different events that 
    happen in the game and the redirections to the different pages.


The game is not perfect, and it does not look the best, but I am happy with the result. I learned a lot from this project
and I had a lot of fun making it. I hope you enjoy playing it as much as I enjoyed making it. :)