import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

app = Flask(__name__)

app.jinja_env.filters["usd"] = usd

app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    user_id = session["user_id"]

    stocks = db.execute(
        "SELECT symbol, price, SUM(shares) as totalShares FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    total = cash

    for stock in stocks:
        total += stock["price"]*stock["totalShares"]

    return render_template("index.html", stocks=stocks, cash=cash, usd=usd, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        item = lookup(symbol)

        if not symbol:
            return apology("Enter a symbol")
        elif not item:
            return apology("Invalid symbol")
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Shares must be an integer")
        if shares <= 0:
            return apology("Shares must be positiv")

        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        item_price = item["price"]
        total_price = item_price*shares

        if cash < total_price:
            return apology("Not enough cash")
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash-total_price, user_id)
            db.execute("INSERT INTO transactions (user_id, shares, price, type, symbol) VALUES(?, ?, ?, ?, ?)",
                       user_id, shares, item_price, 'buy', symbol)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Missing symbol")
        if not lookup(symbol):
            return apology("Enter a valid symbol")

        item = lookup(symbol)

        return render_template("quoted.html", item=item, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/history")
@login_required
def history():

    user_id = session["user_id"]

    transactions = db.execute(
        "SELECT type, symbol, price, shares, time FROM transactions WHERE user_id = ?", user_id)

    return render_template("history.html", transactions=transactions, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    session.clear()

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)

        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]

        return redirect("/")

    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    session.clear()

    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("must provide username")
        elif not password:
            return apology("must provide password")
        elif len(db.execute('SELECT username FROM users WHERE username = ?', username)) > 0:
            return apology("Username already exists")
        elif confirmation != password:
            return apology("Passwords must be the same")

        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)
        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        try:
            shares_to_sell = int(request.form.get("shares"))
        except ValueError:
            return apology("Shares must be a positive integer")

        if shares_to_sell <= 0:
            return apology("Shares must be a positive integer")

        stock = lookup(symbol)
        if stock is None:
            return apology("Invalid symbol")

        item_price = stock["price"]
        price = shares_to_sell * item_price

        shares_owned = db.execute(
            "SELECT SUM(shares) as totalShares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]["totalShares"]

        if shares_owned is None or shares_owned < shares_to_sell:
            return apology("Not enough shares to sell")

        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        new_cash_balance = current_cash + price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash_balance, user_id)

        db.execute("INSERT INTO transactions (user_id, shares, price, type, symbol) VALUES (?, ?, ?, ?, ?)",
                   user_id, -shares_to_sell, item_price, "sell", symbol)

        return redirect("/")
    else:
        user_id = session["user_id"]
        symbols = db.execute(
            "SELECT symbol, SUM(shares) as totalShares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING totalShares > 0", user_id)
        return render_template("sell.html", symbols=symbols)
