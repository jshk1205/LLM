# Description: This file contains the database utility functions to save messages in the database.
import mysql.connector
from mysql.connector import Error

def create_connection():
    try:
        connection = mysql.connector.connect(
            host='localhost',
            database='messenger',
            user='root',
            password='rootpassword'
        )
        if connection.is_connected():
            return connection
    except Error as e:
        print("Error while connecting to MySQL", e)
    return None

def save_message(sender, receiver, message):
    connection = create_connection()
    if connection is None:
        return
    try:
        cursor = connection.cursor()
        query = "INSERT INTO messages (sender, receiver, message) VALUES (%s, %s, %s)"
        cursor.execute(query, (sender, receiver, message))
        connection.commit()
    except Error as e:
        print(f"Error: {e}")
    finally:
        if connection.is_connected():
            cursor.close()
            connection.close()
