#!/bin/sh
while [ 1 ]; do
sleep 60
count=`ps -ef|grep "./start.out"|grep -v grep|wc -l`
if [ "$count" -eq 0 ];then
    echo "killed tinyweb"
    exit
else
    echo "runing tinyweb"
fi
done
#启动此脚本时在后台运行：nohup  

