#
#  Filename: database.py
#  Created on: 02/19/2023
#  Author: Julia Veenkamp
#

import json
import sqlite3

conn = sqlite3.connect('books.db')
c = conn.cursor()

with open('book.json') as f:
    data = json.load(f)

for row in data:
    c.execute("INSERT INTO BOOKS (ISBN, TITLE, AUTHOR, YEAR) VALUES (?, ?, ?, ?)", (row['isbn'], row['title'], row['author'], row['year']))

conn.commit()
conn.close()