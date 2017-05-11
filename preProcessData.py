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
