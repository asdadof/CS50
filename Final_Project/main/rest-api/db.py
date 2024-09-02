import os
import sqlite3

def reset_db():
    db_path = './people.db'
    
    # Remove the existing database file if it exists
    if os.path.exists(db_path):
        os.remove(db_path)

    # Connect to SQLite database (or create it if it doesn't exist)
    conn = sqlite3.connect(db_path)

    # Create a cursor object to interact with the database
    cursor = conn.cursor()

    # Create the people table with the specified schema
    cursor.execute('''
    CREATE TABLE IF NOT EXISTS people (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        status TEXT NOT NULL
    )
    ''')

    # Insert initial data
    people = [
        ("Elvis Presley", "Dead"),
        ("Michael Jackson", "Dead"),
        ("Freddie Mercury", "Dead"),
        ("Kurt Cobain", "Dead"),
        ("Jim Parsons", "Alive"),
        ("Morgan Freeman", "Alive"),
        ("Tom Hanks", "Alive"),
        ("Kevin Hart", "Alive")
    ]

    # Insert data into the table
    cursor.executemany('''
    INSERT INTO people (id, name, status)
    VALUES (?, ?, ?)
    ''', people)

    # Commit the transaction
    conn.commit()

    # Close the connection
    conn.close()

# Run the function to reset the database
reset_db()
