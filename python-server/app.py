from flask import Flask, jsonify
from flask_cors import CORS
import datetime

app = Flask(__name__)
CORS(app)

@app.route('/api/python/health')
def health():
    return jsonify({
        "status": "OK",
        "server": "Python Flask",
        "timestamp": datetime.datetime.now().isoformat(),
        "message": "Python server is running! 🐍"
    })

@app.route('/api/python/info')
def info():
    return jsonify({
        "language": "Python",
        "framework": "Flask",
        "port": 5001,
        "features": ["Simple", "Fast", "Works immediately!"]
    })

@app.route('/api/python/users')
def users():
    return jsonify([
        {"id": 1, "name": "John from Python"},
        {"id": 2, "name": "Jane from Python"}
    ])

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=False)
