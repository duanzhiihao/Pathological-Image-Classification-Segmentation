from __future__ import division, print_function, absolute_import
import pickle
import numpy as np
import scipy.io as sio
import tensorflow as tf
from PIL import Image

import tflearn
from tflearn.layers.core import input_data, dropout, fully_connected
from tflearn.layers.conv import conv_2d, max_pool_2d
from tflearn.layers.normalization import local_response_normalization
from tflearn.layers.estimator import regression
from tflearn.data_preprocessing import ImagePreprocessing
from tflearn.data_augmentation import ImageAugmentation


def load_image(img_path):
    img = Image.open(img_path)
    return img


def resize_image(in_image, new_width, new_height, out_image=None,
                 resize_mode=Image.ANTIALIAS):
    img = in_image.resize((new_width, new_height), resize_mode)
    if out_image:
        img.save(out_image)
    return img


def pil_to_nparray(pil_image):
    pil_image.load()
    return np.asarray(pil_image, dtype="float32")


def load_data(datafile, num_clss, save=False, save_path='dataset.pkl'):
    train_list = open(datafile,'r')
    labels = []
    images = []
    for line in train_list:
        tmp = line.strip().split(' ')
        fpath = tmp[0]
        # print(fpath)
        img = load_image(fpath)
        # img=sio.loadmat(fpath)
        img = resize_image(img,227,227)
        np_img = pil_to_nparray(img)
        # np_img=img['I_6']
        # np_img = np_img.astype('float16')
        images.append(np_img)

        # [0 1]:benign [1 0]:malignant
        index = int(tmp[1])
        label = np.zeros(num_clss)
        label[index] = 1
        labels.append(label)
    if save:
        pickle.dump((images, labels), open(save_path, 'wb'))
    return images, labels


def load_from_pkl(dataset_file):
    X, Y = pickle.load(open(dataset_file, 'rb'))
    return X,Y


# Make sure the data is normalized
img_prep = ImagePreprocessing()
# img_prep.add_featurewise_zero_center(31.83)
# img_prep.add_featurewise_stdnorm(50.96)

# Create extra synthetic training data by flipping, rotating and blurring the
# images on our data set.
img_aug = ImageAugmentation()
# img_aug.add_random_flip_leftright()
#img_aug.add_random_crop((227, 227), 20)
img_aug.add_random_rotation(max_angle=180.)


def create_alexnet(num_classes):
    # Building 'AlexNet'
    network = input_data(shape=[None, 227, 227, 3],
                         data_preprocessing  = img_prep,
                         data_augmentation = img_aug)
    network = conv_2d(network, 96, 11, strides=4, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = conv_2d(network, 256, 5, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = conv_2d(network, 384, 3, activation='relu')
    network = conv_2d(network, 384, 3, activation='relu')
    network = conv_2d(network, 256, 3, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    network = fully_connected(network, 1024, activation='tanh')
    network = dropout(network, 0.5)
    network = fully_connected(network, 1024, activation='tanh')
    network = dropout(network, 0.5)
    network = fully_connected(network, num_classes, activation='softmax')
    network = regression(network, optimizer='momentum',
                         loss='categorical_crossentropy',
                         learning_rate=0.01)
    return network

def predict(network, modelfile, images):
    model = tflearn.DNN(network)
    model.load(modelfile)
    return model.predict(images)

if __name__ == '__main__':

    X, Y = load_data('400Xtest.txt', 2)
    net = create_alexnet(2)
    R = predict(net,'model_alexnet/model_400X-80',X)

    num = len(R)
    print(num)
    for n in range(num):
        print(R[n])
        for m in [0,1]:
            if R[n][m] > 0.5:
                R[n][m] = 1
            else:
                R[n][m] = 0

    count = 0
    for n in range(num):
        # print(R[n])
        if R[n][0] == Y[n][0]:
            count = count + 1

    print('Accuracy on test data is:')
    print(count/num)
