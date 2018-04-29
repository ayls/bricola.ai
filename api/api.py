#!flask/bin/python
from flask import Flask, jsonify, request
from keras.models import model_from_json
import numpy as np
import math

app = Flask(__name__)
model = None

def load_model():
    json_file = open('./model/model.json', 'r')
    model_json = json_file.read()
    json_file.close()
    print('loaded model meta data')

    global model
    model = model_from_json(model_json)
    model.load_weights("./model/model.h5")
    print('loaded model weights')

    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    print('compiled model')

    # workaround to be able to use predict from other threads
    model.predict(np.zeros((1, 15)))

@app.route('/predict', methods=['POST'])
def predict():
    X = parseRequest(request)
    Y_pred = np.argmax(model.predict(X), axis=1)

    return jsonify({'prediction': int(Y_pred[0])})

def parseRequest(request):
    body = request.get_json(silent=True)
    X = np.array([calculateFeatures(body['card1']) + calculateFeatures(body['card2']) + calculateFeatures(body['card3']) 
        + calculateFeatures(body['oppCard']) + calculateFeatures(body['briscola'])])
    return X

def calculateFeatures(card):
    features = None
    if (card == -1):
        features = [-1, -1, -1]
    else:
        type = math.floor(card / 10)
        value = (11 if (card % 10 == 0) else (10 if (card % 10 == 2) else (2 if (card % 10 == 7) else (3 if (card % 10 == 8) else (4 if (card % 10 == 9) else 0)))))
        hasBigValue = (1 if (value == 11 or value == 10) else 0)
        hasSmallValue = (1 if (value == 4 or value == 3 or value == 2) else 0)
        features = [type, hasBigValue, hasSmallValue]

    return features

if __name__ == '__main__':
    load_model()
    app.run(debug=True)