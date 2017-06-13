## 解决的问题
对点群进行分类，如下图

![init image](init.jpg)


## 基本概念
1. 训练样本(training set)
* 作用：已有数据，将被用来训练算法模型
* 数学记号
\begin{equation}
\lbrace x^{(1)}, x^{(2)}, ..., x^{(m)} \rbrace \\\\
其中, x^{(1)}是一个向量
x^{(1)} = \[ x^{(1)}_1, x^{(1)}_2, ..., x^{(1)}_n \]
\end{equation}

2. 聚类(clustering)
* 什么是聚类
> 聚类指将一组物体按照相似度分组 (clustering is the task of grouping a set of objects in such a way that objects in the same group (called a cluster) are more similar (in some sense or another) to each other than to those in other groups)
* 举例
  * 懒人听书的用户根据活跃度，可以聚类为：高活跃度用户、中活跃度用户、低活跃度用户
  * 懒人听书的用户根据消费金额，可以聚类为：高消费度用户、中消费度用户、低消费度用户

## K均值聚类算法(k-means clustering algorithm)
* 算法流程：
  1. 随机初始化各个分类的中心:$\mu_1, \mu_2, ..., \mu_k \in R^n$;
  2. 根据当前分类的中心，求出每个样本到各个分类中心的距离, 距离最近的就是样本所属的分类;
  \begin{equation}
    c^{(i)} := \arg\mathop{\min}\limits_{j}\lvert\lvert x^{(i)} - \mu_j \rvert\rvert
  \end{equation}
  3. 经过上一步骤，每个分类包含的样本发生了变化, 重新估算每个分类的中心;
  \begin{equation}
     \mu_j := \frac{\sum_{i=1}^m1\\{c^{(i)}=j\\}x^{(i)}}{\sum_{i=1}^m1\\{c^{(i)}=j\\}}
  \end{equation}
  4. 重复2、3两步骤，直到收敛。收敛条件是各个分类中心不再有变化;

* 代码实现(matlab)
``` matlab
function k_means()
{

}
```
* 算法动态过程

![image0](0.jpg)
![image0](1.jpg)
![image0](2.jpg)
![image0](3.jpg)
![image0](4.jpg)
![image0](5.jpg)

## 算法应用
根据系统中已有的用户数据，利用K均值算法，对用户进行分类：高活跃度用户、中活跃度用户的、低活跃度用户
1. 从现有系统中得到用户原始数据
```python
#!/usr/bin/python3
# -*- coding:utf-8 -*-
import MySQLdb

db = MySQLdb.connect("192.168.6.42", "readonly", "HowyYih2929Holl29GHoiq", "audiobook")
cursor = db.cursor();
try:
    f = open('userRawData.txt', 'w')
    pageSize = 10 
    for i in range(1):
        start = i * pageSize;
        cursor.execute("select user_id, follower_count, follow_count, collection_folder_count, collection_read_count, flag from t_user_ext_0 limit %s, %s" , (start, pageSize));
        results = cursor.fetchall()
        for result in results:
            f.write(' '.join(str(s) for s in result) + '\n')
except Exception as e:
    print(e)
finally :
    if f:
        f.close();

# disconnect from server
db.close()
```

2. 对原始数据进行预处理
```python
#!/bin/python3
# -*- encodeing:utf8 -*-

import numpy as np

try:
    rawData = np.fromfile('userRawData.txt', sep=' ').reshape(-1, 6)
    rawData = rawData[:, 1:]
    for i in range(rawData.shape[0]):
        rawData[i][4] = int(rawData[i][4]) & 8; 
    # 归一化
    normalizedData = rawData / rawData.max(0)
    normalizedData[np.isnan(normalizedData)] = 0
    np.savetxt('userData.txt', normalizedData, fmt='%f', delimiter=' ')
except Exception as e:
    print(e)

```
3. 将数据分为训练数据和验证数据

4. 对训练数据运行聚类算法，得到聚类结果
```python
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
parsedData = data.map(lambda line : array([float(x) for x in line.split(' ')]));

# 通过训练得到模型参数
clusters = KMeans.train(parsedData, 3, maxIterations=10000, initializationMode="random")

# Evaluate clustering by computing Within Set Sum of Squared Errors
def error(point):
    center = clusters.centers[clusters.predict(point)]
    return sqrt(sum([x**2 for x in (point - center)]))

WSSSE = parsedData.map(lambda point: error(point)).reduce(lambda x, y: x + y)
print("Within Set Sum of Squared Error = " + str(WSSSE))

# 保存模型参数
modePath = './userClusterModel'
clusters.save(sc, modePath);
sameModel = KMeansModel.load(sc, modePath)
```
5. 在验证数据上检验聚类算法的效果
