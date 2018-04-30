#!flask/bin/python
from flask import Flask, jsonify, request
import ai
import game

app = Flask(__name__)
model = None

@app.route('/deal', methods=['POST'])
def deal():
    deal = game.deal()

    return jsonify(deal)

@app.route('/play', methods=['POST'])
def play():
    body = request.get_json(silent=True)
    game.play(body, model)

    return jsonify(body)

if __name__ == '__main__':
    model = ai.load_model()
    app.run(debug=True)