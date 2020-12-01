#!flask/bin/python
from flask import Flask, jsonify, request
from flask_cors import CORS, cross_origin
from common import game

app = Flask(__name__)
CORS(app)

g = game.Game()

@app.route('/deal', methods=['POST'])
def deal():
    deal = g.deal()

    return jsonify(deal)

@app.route('/play', methods=['POST'])
def play():
    body = request.get_json(silent=True)
    response = g.play(body)

    return jsonify(response)

if __name__ == '__main__':
    app.run(host='0.0.0.0')