# SQL 쿼리 파서, 실행 엔진, 인덱스, 트랜잭션 관리 등을 포함하는 간단한 데이터베이스 엔진을 구현하라
# 데이터베이스 엔진은 메모리에만 데이터를 저장하며, 디스크에 데이터를 저장하지 않아도 된다.
# 데이터베이스 엔진은 다음과 같은 SQL 쿼리를 지원해야 한다.
# - CREATE TABLE
# - INSERT
# - SELECT
# - DELETE
# - UPDATE
# - BEGIN
# - ROLLBACK
# - COMMIT
# CREATE TABLE 쿼리는 다음과 같은 형식을 가진다.
# CREATE TABLE table_name (column_name column_type, ...)
# INSERT 쿼리는 다음과 같은 형식을 가진다.
# INSERT INTO table_name VALUES (value, ...)
# SELECT 쿼리는 다음과 같은 형식을 가진다.
# SELECT column_name, ... FROM table_name WHERE column_name operator value
# DELETE 쿼리는 다음과 같은 형식을 가진다.
# DELETE FROM table_name WHERE column_name operator value
# UPDATE 쿼리는 다음과 같은 형식을 가진다.
# UPDATE table_name SET column_name = value WHERE column_name operator value
# BEGIN 쿼리는 트랜잭션을 시작한다.
# ROLLBACK 쿼리는 트랜잭션을 롤백한다.
# COMMIT 쿼리는 트랜잭션을 커밋한다.
# 트랜잭션은 다음과 같은 규칙을 따른다.
# - BEGIN 쿼리가 실행되면 트랜잭션이 시작된다.
# - ROLLBACK 쿼리가 실행되면 트랜잭션을 롤백한다.
# - COMMIT 쿼리가 실행되면 트랜잭션을 커밋한다.
# - 트랜잭션이 시작된 후에는 트랜잭션을 중첩할 수 없다.
# 코드를 실행시켰을 때 오류가 나지 않아야 한다.
# 또한 시작과 끝이 있어야 한다.
# 조건은 여기까지다. 이제 데이터베이스 엔진을 구현하라.

import re
import copy

# 실행 결과가 올바르지 않게 나오나 실행은 됩니다 ㅜㅜ !!

class Database:
    def __init__(self):
        self.tables = {}
        self.transactions = []
        self.transaction = False

    def execute(self, query):
        query = query.strip().upper()
        if query.startswith('CREATE TABLE'):
            self.create_table(query)
        elif query.startswith('INSERT INTO'):
            self.insert(query)
        elif query.startswith('SELECT'):
            self.select(query)
        elif query.startswith('DELETE FROM'):
            self.delete(query)
        elif query.startswith('UPDATE'):
            self.update(query)
        elif query.startswith('BEGIN'):
            self.begin()
        elif query.startswith('ROLLBACK'):
            self.rollback()
        elif query.startswith('COMMIT'):
            self.commit()

    def create_table(self, query):
        table_name = query.split(' ')[2]
        columns = query.split('(')[1].split(')')[0].split(', ')
        self.tables[table_name] = {col.split(' ')[0]: [] for col in columns}

    def insert(self, query):
        table_name = query.split(' ')[2]
        values = query.split('VALUES ')[1].strip('()').split(', ')
        columns = list(self.tables[table_name].keys())
        for col, value in zip(columns, values):
            self.tables[table_name][col].append(value)

    def select(self, query):
        table_name = query.split('FROM ')[1].split(' ')[0]
        columns = query.split('SELECT ')[1].split(' FROM')[0].split(', ')
        condition = query.split('WHERE ')[1] if 'WHERE' in query else None

        if table_name not in self.tables:
            print(f'Table {table_name} does not exist')
            return

        rows = list(zip(*self.tables[table_name].values()))  # Transpose the table to iterate over rows

        for row in rows:
            if condition:
                col_name, operator, value = re.split(r' (=|!=|>|<) ', condition)
                idx = list(self.tables[table_name].keys()).index(col_name)
                cell_value = row[idx]

                if operator == '=' and cell_value != value:
                    continue
                elif operator == '!=' and cell_value == value:
                    continue
                elif operator == '>' and not (cell_value > value):
                    continue
                elif operator == '<' and not (cell_value < value):
                    continue

            selected_columns = [row[list(self.tables[table_name].keys()).index(col)] for col in columns]
            print(' '.join([str(item) for item in selected_columns]))

    def delete(self, query):
        table_name = query.split('FROM ')[1].split(' ')[0]
        condition = query.split('WHERE ')[1] if 'WHERE' in query else None

        for i in range(len(list(self.tables[table_name].values())[0]) - 1, -1, -1):
            if condition:
                col_name, operator, value = re.split(r' (=|!=|>|<) ', condition)
                if operator == '=' and self.tables[table_name][col_name][i] != value:
                    continue
                elif operator == '!=' and self.tables[table_name][col_name][i] == value:
                    continue
                elif operator == '>' and self.tables[table_name][col_name][i] <= value:
                    continue
                elif operator == '<' and self.tables[table_name][col_name][i] >= value:
                    continue

            for col_name in self.tables[table_name].keys():
                self.tables[table_name][col_name].pop(i)
            break

    def update(self, query):
        table_name = query.split(' ')[1]
        set_clause = query.split(' SET ')[1].split(' WHERE')[0]
        set_column, set_value = set_clause.split(' = ')
        where_clause = query.split(' WHERE ')[1]
        where_column, where_operator, where_value = re.split(r' (=|!=|>|<) ', where_clause)

        for i in range(len(self.tables[table_name][where_column])):
            if where_operator == '=' and self.tables[table_name][where_column][i] == where_value:
                self.tables[table_name][set_column][i] = set_value

    def begin(self):
        self.transactions.append(copy.deepcopy(self.tables))
        self.transaction = True

    def rollback(self):
        if self.transaction:
            self.tables = self.transactions.pop()
            self.transaction = False
        else:
            print('NO TRANSACTION')

    def commit(self):
        if self.transaction:
            self.transactions = []
            self.transaction = False
        else:
            print('NO TRANSACTION')


db = Database()
db.execute('CREATE TABLE person (name TEXT, age INTEGER)')
db.execute('INSERT INTO person VALUES (\'John\', 25)')
db.execute('INSERT INTO person VALUES (\'Alice\', 24)')
db.execute('SELECT name FROM person WHERE age = 24')
db.execute('BEGIN')
db.execute('UPDATE person SET age = 26 WHERE name = \'Alice\'')
db.execute('SELECT name, age FROM person WHERE age = 26')
db.execute('ROLLBACK')
db.execute('SELECT name, age FROM person WHERE age = 26')
db.execute('BEGIN')
db.execute('UPDATE person SET age = 26 WHERE name = \'Alice\'')
db.execute('SELECT name, age FROM person WHERE age = 26')
db.execute('COMMIT')
db.execute('SELECT name, age FROM person WHERE age = 26')



