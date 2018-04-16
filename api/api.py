from keras.models import model_from_json
import numpy as np

json_file = open('./model/model.json', 'r')
loaded_model_json = json_file.read()
json_file.close()

loaded_model = model_from_json(loaded_model_json)
loaded_model.load_weights("./model/model.h5")
print("Loaded model from disk")

# evaluate loaded model on test data
loaded_model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
X = X = np.array([[3, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0, 0, 3, 0, 0]])
print(X.shape)
Y_pred = np.argmax(loaded_model.predict(X), axis=1)

print("features:", X[0])
print("predicted: ", Y_pred[0])
