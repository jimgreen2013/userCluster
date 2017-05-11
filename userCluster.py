#!/bin/python3
# -*- coding:utf-8 -*-
from pyspark import SparkConf, SparkContext
from numpy import array
from math import sqrt

from pyspark.mllib.clustering import KMeans, KMeansModel

conf = SparkConf().setMaster('local').setAppName('user_cluster')
sc = SparkContext(conf = conf)

# 从文件中读取数据，并把数据转化为特定的数据结构
data = sc.textFile('./userData.txt')
print('lines:' + str(data.count())); 
parseData = data.map(lambda line : array([float(x) for x in line.split(' ')]));

# 通过训练得到模型参数
clusters = KMeans.train(parseData, 2, maxIterations=10000, initializationMode="random")

# 保存模型参数
modePath = '/home/song/workspace/userCluster/userClusterModel'
clusters.save(sc, modePath);
sameModel = KMeansModel.load(sc, modePath)
