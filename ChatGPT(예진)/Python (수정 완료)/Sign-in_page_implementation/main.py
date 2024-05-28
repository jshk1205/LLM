from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# Hardcoded username and password for demonstration purposes
USERNAME = 'admin'
PASSWORD = 'admin123'

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/login', methods=['POST'])
def login():
    username = request.json.get('username')
    password = request.json.get('password')

    if username == USERNAME and password == PASSWORD:
        return jsonify({'success': True})
    else:
        return jsonify({'success': False, 'message': 'Invalid username or password'})

if __name__ == '__main__':
    app.run(debug=True)