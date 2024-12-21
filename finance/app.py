import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
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
    """Show portfolio of stocks"""
    try:
        stocks = db.execute("SELECT * FROM shares WHERE user_id = (?)", session["user_id"])
        balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
        balance = balance[0]['cash']

        number = db.execute("SELECT COUNT(id) FROM shares WHERE user_id = (?)", session["user_id"])
        number = number[0]['COUNT(id)']

        processed_shares = []
        for stock in stocks:
            cleaned_string = stock['shares']
            share_int = int(float(cleaned_string))
            processed_shares.append(share_int)

        for i, stock in enumerate(stocks):
            current_stock = lookup(stock['symbol'])
            stock['processed_shares'] = processed_shares[i] * current_stock['price']

        total_shares = sum(stock['processed_shares'] for stock in stocks)

        for i, stock in enumerate(stocks):
            stock['processed_shares'] = usd(stock['processed_shares'])

        return render_template("index.html", stocks=stocks, number=number, balance=usd(balance), total=usd(total_shares))

    except:
        return render_template("index.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get('symbol')

        try:
            shares = int(request.form.get('shares'))
        except:
            return apology("Shares must be numeric.", 400)

        if not symbol:
            return apology("Symbol can not be blank.", 400)

        if not shares:
            return apology("Shares can not be empty.", 400)

        if shares <= 0:
            return apology("Shares can not be less than 1.", 400)

        try:
            stock = lookup(symbol)

            name = stock['name']

            cash = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])

            cash_value = cash[0]['cash']

            if cash_value < shares * stock['price']:
                cash_value = usd(cash_value)
                return apology(f"Insufficient funds: current funds {cash_value}", 400)

            table_check = """ SELECT name FROM sqlite_master WHERE type='table' AND name='shares'; """

            result = db.execute(table_check)

            if result:
                print("table shares already exists.")
            else:
                table = """ CREATE TABLE shares (
                                id INTEGER PRIMARY KEY AUTOINCREMENT,
                                name TEXT NOT NULL,
                                symbol TEXT UNIQUE,
                                shares TEXT NOT NULL,
                                user_id INTEGER NOT NULL,
                                FOREIGN KEY (user_id) REFERENCES users(id)
                            ); """
                db.execute(table)

            symbol = symbol.upper()
            current_cash = cash_value - (shares * stock['price'])

            existing_stock = db.execute(
                "SELECT shares FROM shares WHERE symbol = ? AND user_id = ?",
                symbol, session["user_id"]
            )

            if existing_stock:
                existing_shares = int(float(existing_stock[0]['shares']))
                total_shares = existing_shares + shares

                db.execute("UPDATE shares SET shares = ? WHERE symbol = ? AND user_id = ?",
                           total_shares, symbol, session["user_id"])
            else:
                db.execute("INSERT INTO shares (name, symbol, shares, user_id) VALUES (?, ?, ?, ?)",
                           name, symbol, shares, session["user_id"])

            db.execute("UPDATE users SET cash = ? WHERE id = ?", current_cash, session["user_id"])

            History(name, symbol, "purchase", shares * stock['price'], shares, session["user_id"])

            return redirect("/")

        except:
            return apology("Symbol does not exist.", 400)

    else:
        return render_template("buy.html")


def History(name, symbol, type, price, shares, id):
    table_check = """ SELECT name FROM sqlite_master WHERE type='table' AND name='history'; """

    result = db.execute(table_check)

    if not result:
        table = """ CREATE TABLE history (
                                id INTEGER PRIMARY KEY AUTOINCREMENT,
                                name TEXT NOT NULL,
                                symbol TEXT,
                                type TEXT NOT NULL,
                                shares INTEGER,
                                date TEXT,
                                price TEXT,
                                user_id INTEGER NOT NULL,
                                FOREIGN KEY (user_id) REFERENCES users(id)
                            ); """
        db.execute(table)

    date = os.popen("date /T").read()

    db.execute("INSERT INTO history (name, symbol, type, shares, date, price, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)",
               name, symbol, type, shares, date, usd(price), id)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:
        stocks = db.execute("SELECT * FROM history WHERE user_id = (?)", session["user_id"])

        return render_template("history.html", stocks=stocks)

    except:
        return render_template("history.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        symbol = request.form.get('symbol')

        if not symbol:
            return apology("Symbol can not be blank.", 400)

        try:
            stock = lookup(symbol)
            stock['price'] = usd(stock['price'])
            return render_template("quoted.html", stock=stock)

        except:
            return apology("Symbol does not exist.", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username:
            return apology("Must provide username", 400)

        if not password or not confirmation:
            return apology("Fill your password", 400)

        if password != confirmation:
            return apology("Passwords do not match", 400)

        hash = generate_password_hash(password)

        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?,?)", username, hash)

            return redirect("/")
        except:
            return apology("The username is already in use", 400)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    if request.method == "POST":
        symbol = request.form.get('symbol')
        real_shares = db.execute(
            "SELECT shares FROM shares WHERE user_id = (?) AND symbol = (?)", session["user_id"], symbol)
        real_shares = int(real_shares[0]['shares'])

        try:
            shares = int(request.form.get('shares'))

        except:
            return apology("Shares can not be less than 0.", 400)

        if shares > real_shares:
            return apology("Insufficient shares.", 400)

        if db.execute("SELECT symbol FROM shares WHERE user_id = (?) AND symbol = (?)", session["user_id"], symbol):

            db.execute("UPDATE shares SET shares = ? WHERE symbol = ? AND user_id = ?",
                       (real_shares - shares), symbol, session["user_id"])

            stock = lookup(symbol)

            balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
            balance = balance[0]['cash']

            current_cash = balance + (stock['price'] * shares)

            db.execute("UPDATE users SET cash = ? WHERE id = ?", current_cash, session["user_id"])

            History(stock['name'], symbol, "purchase", shares *
                    stock['price'], shares, session["user_id"])

        else:
            return apology("There are no shares with that symbol", 400)

        return redirect("/")

    else:
        stocks = db.execute("SELECT * FROM shares WHERE user_id = ?", user_id)
        return render_template("sell.html", stocks=stocks)


@app.route("/more-cash", methods=["GET", "POST"])
@login_required
def moreCash():
    balance = db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])
    balance = balance[0]['cash']

    if request.method == "POST":

        cash = request.form.get('cash')

        if not cash:
            return apology("No amount specified for fund recharge.", 400)
        try:
            cash = float(cash)
        except:
            return apology("Specified recharge should be int or float.", 400)

        if cash <= 0:
            return apology("Specified recharge can not be less than 1", 400)

        current_balance = balance + cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", current_balance, session["user_id"])

        return render_template("more-cash.html", balance=usd(current_balance))

    else:
        return render_template("more-cash.html", balance=usd(balance))
