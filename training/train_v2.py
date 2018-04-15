import numpy as np
import random
import math
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation
from keras.layers.normalization import BatchNormalization
from keras.utils import np_utils

def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0: 
      return v
    return v / norm    

def getSets(data_in):
    data_reduced = np.delete(data_in, [0,1,3,4,7,9,10,13,15,16,19,21,22,25,26,27,28,30,31,34,35,36,37,38,39,40,41,42,43], axis=1)
    #0,1,3,5,6,7,9,11,12,13,15,17,18,19,21,23,24,25,26,27,28,30,32,33,34,35,36,37,38,39,40,41,42,43
    print(data_reduced[0])
    random.shuffle(data_reduced)
    X = normalize(data_reduced[:, 0:data_reduced.shape[1] - 1])
    Y = data_reduced[:, data_reduced.shape[1] - 1].reshape(data_reduced.shape[0], 1) - 1

    m,_ = X.shape 
    m_train = math.ceil(m * 0.7)

    X_train = X[0:m_train,:]
    X_test = X[m_train:,:]
    Y_train = np_utils.to_categorical(Y[0:m_train,:],3)
    Y_test = np_utils.to_categorical(Y[m_train:,:],3)

    return X_train, Y_train, X_test, Y_test



data = np.genfromtxt("./data/preprocessed/briscola_2.data", delimiter=",")

X_train, Y_train, X_test, Y_test = getSets(data)
_,n = X_train.shape

model = Sequential()
# model.add(Dense(64, input_shape=(n,)))
model.add(Dense(32, input_shape=(n,)))
model.add(BatchNormalization())
model.add(Activation('relu'))
model.add(Dropout(0.20))
# model.add(Dense(48))
# model.add(BatchNormalization())
# model.add(Activation('relu'))
#model.add(Dropout(0.20))
# model.add(Dense(32))
# model.add(BatchNormalization())
# model.add(Activation('relu'))
#model.add(Dropout(0.20))
# model.add(Dense(24))
# model.add(BatchNormalization())
# model.add(Activation('relu'))
#model.add(Dropout(0.20))
model.add(Dense(16))
model.add(BatchNormalization())
model.add(Activation('relu'))
model.add(Dropout(0.20))
# model.add(Dense(12))
# model.add(BatchNormalization())
# model.add(Activation('relu'))
#model.add(Dropout(0.20))
# model.add(Dense(8))
# model.add(BatchNormalization())
# model.add(Activation('relu'))
#model.add(Dropout(0.20))
model.add(Dense(3))
model.add(BatchNormalization())
model.add(Activation('softmax'))
model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.summary()
print(model.output_shape)

model.fit(X_train, Y_train,
          epochs=10, verbose=1)              

 	
score = model.evaluate(X_test, Y_test, verbose=0)

print(score)

Y_act = np.argmax(Y_test, axis=1)
Y_pred = np.argmax(model.predict(X_test), axis=1)
for i in range(0, 10):
    print("predicted: ", Y_pred[i])
    print("actual: ", Y_act[i])

