#encoding=utf-8
#! /usr/bin/python
import os
from code_parse import CodeParseForNB,CodeParseForTinyLink,CodeParseForALL
from genTopo import genTopo
import subprocess

app_code= ''

print('user code')

if os.path.exists('../user/app.cpp'):


	app_code = open("../user/app.cpp",'r').read()
else:
	app_code = open("../defaultFile/bike_APP.cpp","r").read()
	app_writer = open('../user/app.cpp','w')
	app_writer.write(app_code)
	app_writer.close()
# print(app_code)

isTinyLink = True  # 

CodeParseForALL();


if not os.path.exists("../user/config.json"):
	config_code = open("../defaultFile/bike_config.json",'r').read()
	config_writer = open("../user/config.json",'w')
	config_writer.write(config_code)
	config_writer.close()
	# print("topo")


if os.path.exists("../user/config.json"):
	genTopo()
cmd = "sh startup.sh"
#ret = os.system(cmd)
ret = subprocess.call(cmd,shell = True)
'''
If every step in the script is executed normally, a integer 0 will be returned. 
'''
# if ret != 0:
# 	print("failure")
# else:
# 	print("success")


	
