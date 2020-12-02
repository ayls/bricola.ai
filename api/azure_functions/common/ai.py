import numpy as np
import math
import os
from keras.models import model_from_json

class AI:

    def __init__(self):
        scriptpath = os.path.abspath(__file__)
        scriptdir = os.path.dirname(scriptpath)

        model_filename = os.path.join(scriptdir, 'model', 'model.json')
        json_file = open(model_filename, 'r')
        model_json = json_file.read()
        json_file.close()
        print('loaded model meta data')

        self.model = model_from_json(model_json)
        weights_filename = os.path.join(scriptdir, 'model', 'model.h5')        
        self.model.load_weights(weights_filename)
        print('loaded model weights')

        self.model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
        print('compiled model')

        # hack to be able to use predict from other Flask threads
        self.model.predict(np.zeros((1, 10)))

    def nnPredict(self, card1, card2, card3, oppCard, briscola):
        X = self.__getFeatures(card1, card2, card3, oppCard, briscola)
        Y_pred = np.argmax(self.model.predict(X), axis=1)    

        return Y_pred[0]    

    def __getFeatures(self, card1, card2, card3, oppCard, briscola):
        X = np.array([self.__calculateFeatures(card1) + self.__calculateFeatures(card2) + self.__calculateFeatures(card3) 
            + self.__calculateFeatures(oppCard) + self.__calculateFeatures(briscola)])
        return X

    def __calculateFeatures(self, card):
        features = None
        if (card == -1):
            features = [-1, -1]
        else:
            type = math.floor(card / 10)
            value = (11 if (card % 10 == 0) else (10 if (card % 10 == 2) else (2 if (card % 10 == 7) else (3 if (card % 10 == 8) else (4 if (card % 10 == 9) else 0)))))
            features = [type, value]

        return features