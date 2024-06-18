# 웹 개발과 관련하여 간단한 CRUD 기능을 가진 웹 애플리케이션을 구현하라.
# Flask 프레임워크를 사용하여 구현하라.
# 데이터베이스는 SQLite를 사용하며, SQLAlchemy ORM을 사용하여 데이터베이스에 접근하라.
# 데이터베이스에는 다음과 같은 테이블이 존재해야 한다.
# - User
#   - id (Integer, Primary Key)
#   - name (String, Not Null)
#   - email (String, Not Null)
# - Post
#   - id (Integer, Primary Key)
#   - title (String, Not Null)
#   - content (Text, Not Null)
#   - created_at (DateTime, Not Null)
#   - user_id (Integer, ForeignKey('user.id'))
#   - user (Relationship('User'))
# 위와 같은 테이블이 존재할 때, 다음과 같은 기능을 가진 웹 애플리케이션을 구현하라.
# - 사용자는 사용자를 생성할 수 있다.
# - 사용자는 사용자 목록을 조회할 수 있다.
# - 사용자는 사용자를 조회할 수 있다.
# - 사용자는 사용자를 수정할 수 있다.
# - 사용자는 사용자를 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 수 있다.
# - 사용자는 사용자의 게시물 목록을 조회할 수 있다.
# - 사용자는 사용자의 게시물을 조회할 수 있다.
# - 사용자는 사용자의 게시물을 수정할 수 있다.
# - 사용자는 사용자의 게시물을 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 때, 사용자를 선택할 수 있다.
# - 사용자는 사용자의 게시물을 조회할 때, 사용자를 조회할 수 있다.
# - 사용자는 사용자의 게시물을 수정할 때, 사용자를 수정할 수 있다.
# - 사용자는 사용자의 게시물을 삭제할 때, 사용자를 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 때, 게시물을 생성한 시간을 지정할 수 있다.
# - 사용자는 사용자의 게시물을 조회할 때, 게시물을 생성한 시간을 조회할 수 있다.
# - 사용자는 사용자의 게시물을 수정할 때, 게시물을 생성한 시간을 수정할 수 있다.
# - 사용자는 사용자의 게시물을 삭제할 때, 게시물을 생성한 시간을 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 때, 게시물의 제목을 지정할 수 있다.
# - 사용자는 사용자의 게시물을 조회할 때, 게시물의 제목을 조회할 수 있다.
# - 사용자는 사용자의 게시물을 수정할 때, 게시물의 제목을 수정할 수 있다.
# - 사용자는 사용자의 게시물을 삭제할 때, 게시물의 제목을 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 때, 게시물의 내용을 지정할 수 있다.
# - 사용자는 사용자의 게시물을 조회할 때, 게시물의 내용을 조회할 수 있다.
# - 사용자는 사용자의 게시물을 수정할 때, 게시물의 내용을 수정할 수 있다.
# - 사용자는 사용자의 게시물을 삭제할 때, 게시물의 내용을 삭제할 수 있다.
# - 사용자는 사용자의 게시물을 생성할 때, 게시물의 사용자를 지정할 수 있다.

from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, Text, DateTime, ForeignKey
from sqlalchemy.orm import relationship
from datetime import datetime

'''
<코드에 관한 설명>
이 코드는 Flask를 사용하여 간단한 CRUD 기능을 가진 웹 애플리케이션을 구현한 것입니다.
각 기능을 실행하기 위해 curl 명령어나 웹 브라우저에서 특정 API 엔드포인트를 호출하여 테스트할 수 있습니다.

1. 코드 실행 시 "Hello world!'라고 적혀져 있는 웹페이지가 생성됨
2. 그 상태로 powershell 실행, 사용자 생성 요청 보내기
$headers = @{
    "Content-Type" = "application/json"
}

$body = @{
    name = "Alice"
    email = "alice@example.com"
} | ConvertTo-Json

Invoke-RestMethod -Uri "http://127.0.0.1:5000/users" -Method Post -Headers $headers -Body $body

3. 사용자 정보 관련 예제 코드
- 사용자 생성
curl -X POST -H "Content-Type: application/json" -d '{"name": "Alice", "email": "alice@example.com"}' http://127.0.0.1:5000/users
- 모든 사용자 조회
curl http://127.0.0.1:5000/users
- 특정 사용자 조회
curl http://127.0.0.1:5000/users/1
- 사용자 정보 수정
curl -X PUT -H "Content-Type: application/json" -d '{"name": "Alice Smith", "email": "alice.smith@example.com"}' http://127.0.0.1:5000/users/1
- 사용자 삭제
curl -X DELETE http://127.0.0.1:5000/users/1

4. 게시물 관련 예제 코드
- 게시물 생성
curl -X POST -H "Content-Type: application/json" -d '{"title": "First Post", "content": "Hello, World!"}' http://127.0.0.1:5000/users/1/posts
- 특정 사용자의 모든 게시물 조회
curl http://127.0.0.1:5000/users/1/posts
- 특정 게시물 조회
curl http://127.0.0.1:5000/users/1/posts/1
- 게시물 수정
curl -X PUT -H "Content-Type: application/json" -d '{"title": "Updated Post", "content": "Updated content"}' http://127.0.0.1:5000/users/1/posts/1
- 게시물 삭제
curl -X DELETE http://127.0.0.1:5000/users/1/posts/1

'''


app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///test.db'
db = SQLAlchemy(app)

Base = declarative_base()

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String, nullable=False)
    email = db.Column(db.String, nullable=False)
    posts = db.relationship('Post', backref='user', lazy=True)

class Post(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String, nullable=False)
    content = db.Column(db.Text, nullable=False)
    created_at = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'))

@app.route('/')
def hello():
    return 'Hello, World!'

@app.route('/users', methods=['POST'])
def create_user():
    name = request.json.get('name')
    email = request.json.get('email')

    user = User(name=name, email=email)
    db.session.add(user)
    db.session.commit()

    return jsonify({'id': user.id, 'name': user.name, 'email': user.email})

@app.route('/users', methods=['GET'])
def get_users():
    users = User.query.all()
    result = []

    for user in users:
        result.append({'id': user.id, 'name': user.name, 'email': user.email})

    return jsonify(result)

@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    user = User.query.get(user_id)

    if user is None:
        return jsonify({'error': 'User not found'}), 404

    return jsonify({'id': user.id, 'name': user.name, 'email': user.email})

@app.route('/users/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    user = User.query.get(user_id)

    if user is None:
        return jsonify({'error': 'User not found'}), 404

    user.name = request.json.get('name')
    user.email = request.json.get('email')

    db.session.commit()

    return jsonify({'id': user.id, 'name': user.name, 'email': user.email})

@app.route('/users/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    user = User.query.get(user_id)

    if user is None:
        return jsonify({'error': 'User not found'}), 404

    db.session.delete(user)
    db.session.commit()

    return '', 204

@app.route('/users/<int:user_id>/posts', methods=['POST'])
def create_post(user_id):
    user = User.query.get(user_id)

    if user is None:
        return jsonify({'error': 'User not found'}), 404

    title = request.json.get('title')
    content = request.json.get('content')

    post = Post(title=title, content=content, user_id=user_id)
    db.session.add(post)
    db.session.commit()

    return jsonify({'id': post.id, 'title': post.title, 'content': post.content, 'created_at': post.created_at})

@app.route('/users/<int:user_id>/posts', methods=['GET'])
def get_posts(user_id):
    user = User.query.get(user_id)

    if user is None:
        return jsonify({'error': 'User not found'}), 404

    posts = Post.query.filter_by(user_id=user_id)
    result = []

    for post in posts:
        result.append({'id': post.id, 'title': post.title, 'content': post.content, 'created_at': post.created_at})

    return jsonify(result)

@app.route('/users/<int:user_id>/posts/<int:post_id>', methods=['GET'])
def get_post(user_id, post_id):
    post = Post.query.get(post_id)

    if post is None:
        return jsonify({'error': 'Post not found'}), 404

    return jsonify({'id': post.id, 'title': post.title, 'content': post.content, 'created_at': post.created_at})

@app.route('/users/<int:user_id>/posts/<int:post_id>', methods=['PUT'])
def update_post(user_id, post_id):
    post = Post.query.get(post_id)

    if post is None:
        return jsonify({'error': 'Post not found'}), 404

    post.title = request.json.get('title')
    post.content = request.json.get('content')

    db.session.commit()

    return jsonify({'id': post.id, 'title': post.title, 'content': post.content, 'created_at': post.created_at})

@app.route('/users/<int:user_id>/posts/<int:post_id>', methods=['DELETE'])
def delete_post(user_id, post_id):
    post = Post.query.get(post_id)

    if post is None:
        return jsonify({'error': 'Post not found'}), 404

    db.session.delete(post)
    db.session.commit()

    return '', 204

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    app.run()
