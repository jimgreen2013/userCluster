#!/usr/bin/python3
# -*- coding:utf-8 -*-
import MySQLdb

db = MySQLdb.connect("192.168.6.42", "readonly", "HowyYih2929Holl29GHoiq", "audiobook")
cursor = db.cursor();
try:
    f = open('userRawData.txt', 'w')
    pageSize = 3
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
