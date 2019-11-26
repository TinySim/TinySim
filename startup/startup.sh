#! /bin/sh
# pwd
# cd simulator_web
# cd tinysim
cd ../build
cmake .. > /dev/null 
if [ $? -eq 0 ];then
	echo "cmake -- ok"
	make > /dev/null # 正常不输出,错误输出到终端
else
	echo "cmake error"
	return -3
fi
if [ $? -eq 0 ];then
	# echo '' >../result/application_all.log  # clear all contents before
	# ./app  >/dev/null 2>&1 &
	echo 'make -- ok'
	./app
else
	echo "make error"
	return -4
fi

if [ $? -eq 0 ];then
	# echo "tinysim started!"
	return 0
else
	echo "tinysim failed"
	return -5
fi
