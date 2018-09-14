from __future__ import division, print_function, absolute_import
import pickle
import numpy as np
import scipy.io as sio
import tensorflow as tf
import glob
from PIL import Image
import copy

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
        img = load_image(fpath)
        # print(fpath)
        # img=sio.loadmat(fpath)
        img = resize_image(img, 227, 227)
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

# Make sure the data is normalized
img_prep = ImagePreprocessing()
img_prep.add_featurewise_zero_center(31.83)
img_prep.add_featurewise_stdnorm(50.96)

# Create extra synthetic training data by flipping, rotating and blurring the
# images on our data set.
# img_aug = ImageAugmentation()
# img_aug.add_random_flip_leftright()
# img_aug.add_random_crop((300, 300), 20)
# img_aug.add_random_rotation(max_angle=180.)

def create_alexnet(num_classes):
    # Building 'AlexNet'
    network = input_data(shape=[None, 227, 227, 3],
                         data_preprocessing  = img_prep)
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
                         learning_rate=0.002)
    return network

def write_result(result,filename):
    file=open(filename,'w')
    for i in range(len(result)):
        for j in range(64):
            file.write('%.4f' % result[i][j][0]+' ')
            file.write('%.4f' % result[i][j][1])
            file.write('\n')
    file.close()

def accuracy(test_pre,labels):
    img_num=len(labels)
    Y_array=np.array(labels)
    Y_array=Y_array.astype(int)
    test_pre_array=np.resize((test_pre),(img_num,2))+0.5
    test_pre_array=test_pre_array.astype(int)
    label_p_num=sum(Y_array[:,1])
    label_n_num=sum(Y_array[:,0])
    test_pre_p_num=sum(test_pre_array[:,1])
    test_pre_n_num=sum(test_pre_array[:,0])
    f_p_num=sum(test_pre_array[:,1]-Y_array[:,1]==1)
    t_p_num=test_pre_p_num-f_p_num
    f_n_num=sum(test_pre_array[:,1]-Y_array[:,1]==-1)
    t_n_num=test_pre_n_num-f_n_num
    f_p_r=f_p_num*1.0/label_n_num
    f_n_r=f_n_num*1.0/label_p_num
    acc=sum(test_pre_array[:,1]-Y_array[:,1]==0)*1.0/img_num
    return acc,f_p_r,f_n_r

def predict(model, train_model, test_data):
    img_num=len(test_data)
    test_pre=[]
    model.load(train_model)
    for i in range(int(img_num/64)):    
        Y_pre=model.predict(test_data[i*64:(i+1)*64])
        test_pre.append(Y_pre)
    # Y_p=model.predict(test_data[int(img_num/64)*64:img_num])
    # test_pre.extend(Y_p)
    return test_pre

if __name__ == '__main__':
    
    acc_file=open('alexnet_test_acc.txt','w')
    X0, Y0 = load_data('test.txt', 2)

    config=tf.ConfigProto()
    config.gpu_options.allow_growth=True
    sess=tf.Session(config=config)

    net = create_alexnet(2)
    model=tflearn.DNN(net)

    '''
    X=copy.deepcopy(X0)
    # print('-----', X0[0])
    # model.load('/devdata/dujun/CTC/model_6/model_alexnet-19000')
    test_pre=predict(model, '/devdata/dujun/CTC/model_6/model_alexnet-19000', X)
    # print('======', X0[0])
    write_result(test_pre,'test_19000.txt')
    acc=accuracy(test_pre,Y0)
    print(acc)  
    
    # X1, Y1 = load_data('test_7.txt', 2)
    # print('$$$$$$$$$$$',X)
    X=copy.deepcopy(X0)
    # model.load('/devdata/dujun/CTC/model_6/model_alexnet-15400')
    test_pre=predict(model,'/devdata/dujun/CTC/model_6/model_alexnet-15400',X)
    write_result(test_pre,'test_15400.txt')
    acc=accuracy(test_pre,Y0)
    print(acc)
    '''
    file=glob.glob('/devdata/dujun/CTC/model_alexnet/*.index')
    file.sort(key=lambda x:int(x[47:-6]))
    for model_name in file:
        X=copy.deepcopy(X0)
        # print('***********',model_name[0:len(model_name)-6])
        test_pre=predict(model,model_name[0:len(model_name)-6],X)
        txt_name=model_name[0:19]+'alexnet_test_result'+model_name[32:len(model_name)-6]+'.txt'
        # print('^^^^^^^^^^^',txt_name)
        write_result(test_pre,txt_name)
        acc,f_p_r,missing_r=accuracy(test_pre,Y0)
        acc_file.write(model_name[33:47]+'%.05d'%int(model_name[47:len(model_name)-6])+'	accuracy '+'%.5f'%acc+ '	false_positive_rate '+'%.5f'%f_p_r+'	missing_rate '+'%.5f'%missing_r)
        acc_file.write('\n')
        print('-----------',model_name[33:47]+'%.05d'%int(model_name[47:len(model_name)-6])+'	accuracy '+'%.5f'%acc+ '	false_positive_rate '+'%.5f'%f_p_r+'	missing_rate '+'%.5f'%missing_r)
    
    
   
    
    
