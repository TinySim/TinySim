#encoding=utf-8
#! /usr/bin/python

import json
import random
import os

def generateTopoFile(nodeNum, gateWayNum, area_x,area_y):
    '''
    generate coordinate of all nodes
    '''

    random.seed(10) # set the random seed to fix the generated topo.

    node_id = {}
    node_x = {}
    node_y = {}
    i = 0
    startNode = 1
    while i < nodeNum:
        node_id[i] = i + startNode
        node_x[i] = random.randint(0, area_x)
        node_y[i] = random.randint(0, area_y)
        if i >= 2:
            flag = False
            for j in range(1,i):
                if node_x[i] == node_x[j] and \
                        node_y[i] == node_y[j]:
                    flag = True
                    break
            if flag:
                continue
        i = i+1

    '''
    generate coordinate of all gateways
    '''
    gateway_id = {}
    gateway_x = {}
    gateway_y = {}

    i = 0
    while i < gateWayNum:
        gateway_id[i] = i + 1 + startNode * 10000
        gateway_x[i] = random.randint(0, area_x)
        gateway_y[i] = random.randint(0, area_y)
        if i >= 1:
            flag = False
            for j in range(0,i):
                if gateway_x[i] == gateway_x[j] and \
                        gateway_y[i] == gateway_y[j]:
                    flag = True
                    break
            if flag:
                continue
        i = i+1
    '''
    write the coordinate and node ID into topo.txt
    '''
    topo_list = []
    topo_list.append('distance (m)\n') 
    '''
    First, the base station is generated, and then the ordinary node is generated.
    '''
    for i in range(gateWayNum):
        s = str(gateway_id[i]) + " " + str(gateway_x[i]) + " " + str(gateway_y[i]) + "\n"
        topo_list.append(s)

    for i in range(nodeNum):
        s = str(node_id[i]) + " " + str(node_x[i]) + " " + str(node_y[i]) + "\n"
        topo_list.append(s)
    try:
        writer = open('../user/topo.txt','w+')
        writer.writelines(topo_list)
        writer.close()
    except IOError as e:
        print(e)


def genTopo():
    if not os.path.exists("../user/config.json"):
    fp = open("../user/config.json","r")
    config_json_str = fp.read()
    fp.close()
    config_json = json.loads(config_json_str)
    # print(type(config_json))
    if type(config_json) == dict:
        nodenum = config_json["nodenum"]
        gateWayNum = config_json["gateway"]
        area_x = config_json["area"]["area_y"]
        area_y = config_json["area"]["area_y"]
        #print(nodenum,gateway,area_x,area_y)
        generateTopoFile(nodenum, gateWayNum, area_x, area_y)

        
if __name__ == "__main__":
    fp = open("../user/config.json","r")
    config_json_str = fp.read()
    fp.close()
    config_json = json.loads(config_json_str)
    # print(type(config_json))
    if type(config_json) == dict:
        nodenum = config_json["nodenum"]
        gateWayNum = config_json["gateway"]
        area_x = config_json["area"]["area_y"]
        area_y = config_json["area"]["area_y"]
        #print(nodenum,gateway,area_x,area_y)
        generateTopoFile(nodenum, gateWayNum, area_x, area_y)
