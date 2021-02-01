import numpy as np
import tensorflow as tf
from tensorflow.keras.applications import imagenet_utils
from tensorflow.keras.models import load_model

mobile = tf.keras.applications.mobilenet.MobileNet()
#mobile = load_model('mobilenet.h5', compile=False)
scene_infile = open("random.raw", "rb")
scene_image_array = np.fromfile(scene_infile, dtype=np.uint8)
temp = np.array([])
for i in scene_image_array:
    temp = np.append(temp, i)
    temp = np.append(temp, i)
    temp = np.append(temp, i)

temp1 = np.expand_dims(np.resize(temp, (224, 224, 3)), axis=0)
temp2 = tf.keras.applications.mobilenet.preprocess_input(temp1)
predictions = mobile.predict(temp2)
results = imagenet_utils.decode_predictions(predictions)
print(f'This is {results[0][0][1]}')
