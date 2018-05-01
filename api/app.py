#!flask/bin/python
from flask import Flask, jsonify, request
import game

app = Flask(__name__)
g = None

@app.route('/deal', methods=['POST'])
def deal():
    deal = g.deal()

    return jsonify(deal)

@app.route('/play', methods=['POST'])
def play():
    body = request.get_json(silent=True)
    g.play(body)

    return jsonify(body)

if __name__ == '__main__':
    g = game.Game()
    app.run(debug=True)