import numpy as np
import math
import common
from keras.models import model_from_json

class AI:

    def __init__(self):
        json_file = open('./model/model.json', 'r')
        model_json = json_file.read()
        json_file.close()
        print('loaded model meta data')

        self.model = model_from_json(model_json)
        self.model.load_weights("./model/model.h5")
        print('loaded model weights')

        self.model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
        print('compiled model')

        # hack to be able to use predict from other Flask threads
        self.model.predict(np.zeros((1, 15)))

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
            features = [-1, -1, -1]
        else:
            type = math.floor(card / 10)
            value = (11 if (card % 10 == 0) else (10 if (card % 10 == 2) else (2 if (card % 10 == 7) else (3 if (card % 10 == 8) else (4 if (card % 10 == 9) else 0)))))
            hasBigValue = (1 if (value == 11 or value == 10) else 0)
            hasSmallValue = (1 if (value == 4 or value == 3 or value == 2) else 0)
            features = [type, hasBigValue, hasSmallValue]

        return features