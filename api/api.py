#!flask/bin/python
from flask import Flask, jsonify, request
from keras.models import model_from_json
import numpy as np

app = Flask(__name__)

# json_file = open('./model/model.json', 'r')
# loaded_model_json = json_file.read()
# json_file.close()
# print('loaded model meta data')

# loaded_model = model_from_json(loaded_model_json)
# loaded_model.load_weights("./model/model.h5")
# print('loaded model weights')

# loaded_model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
# print('compiled model')

@app.route('/predict', methods=['POST'])
def predict():
    json_file = open('./model/model.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    print('loaded model meta data')

    loaded_model = model_from_json(loaded_model_json)
    loaded_model.load_weights("./model/model.h5")
    print('loaded model weights')

    loaded_model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    print('compiled model')

    X = parseRequest(request)
    Y_pred = np.argmax(loaded_model.predict(X), axis=1)

    return jsonify({'prediction': int(Y_pred[0])})

def parseRequest(request):
    return np.array([[3, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0, 0, 3, 0, 0]])

if __name__ == '__main__':
    app.run(debug=True)