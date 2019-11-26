#! /usr/bin/python
# -*- coding=utf-8 -*-

"""
file: service.py
"""
import time
import sys
import datetime
import asyncio
import aiocoap.resource as resource
import aiocoap
import requests
import threading


PRINT_FLAG = False
MobileMessages = {}
BikeStates = {}

class BlockResource(resource.Resource):

    def __init__(self):
        super().__init__()
        self.my_pos_dict = {}

    async def render_get(self, request):
        # id
        query_id = request.opt.uri_query[0]
        query_value = query_id.split("=")
        id_value = query_value[1]

        #print(id_value , " is querying for opening")

        # time
        query_time = request.opt.uri_query[1]
        query_time_value = query_time.split("=")

        last_time = query_time_value[1]

        back_value = 'not related information'

        # query
        if id_value in self.my_pos_dict.keys():
            #print(id_value , "is asking for opening")  
            if self.my_pos_dict[id_value][0] == 1:

                back_value = self.my_pos_dict.get("msg_type","ON")
                last_time = self.my_pos_dict[id_value][1]
        else:
            back_value = 'not_{}'.format(self.my_pos_dict.get("msg_type","ON"))

        real_value =( back_value + "##" + last_time + query_time_value[1] + "\0" ).encode("ascii")
        #print(real_value.decode("ascii"))
        return aiocoap.Message(payload=real_value)


    async def render_put(self, request):
        
        query = request.opt.uri_query[0]

        query_value = query.split("=")

        # time list
        query_time = request.opt.uri_query[1]

        query_time_value = query_time.split("=")

        if len(request.opt.uri_query) > 2:

            user_define_msg = request.opt.uri_query[2]
            user_define_msg_value = user_define_msg.split("=")
            self.my_pos_dict["msg_type"] = user_define_msg_value[1]
            

        if query_value[0] != "close":
            return

        request_node_id = request.payload.decode("ascii")

        #print(request_node_id," is asking open lock")

        if query_value[1] == "1":
            print("virtual device is asking to close")
        else:
            print("mobile phone is asking to open")

        if  request_node_id:
            if query_value[1] =="1":  # ask close
                node_value = [0,query_time_value[1]]
                self.my_pos_dict[request_node_id] = node_value
                # return value
                lock_payload = "locking..."+"##"+query_time_value[1] + query_time_value[1] + "\0"
                lock_payload = lock_payload.encode("ascii")
                return aiocoap.Message(code=aiocoap.CHANGED, payload=lock_payload)
            
            elif query_value[1] == "0": # ask open
                if  (request_node_id in self.my_pos_dict.keys()) and self.my_pos_dict[request_node_id][0] == 1:   # bike is using
                    not_payload = "is_using##"+query_time_value[1]+query_time_value[1]+"\0"
                    not_payload = not_payload.encode("ascii")
                    return aiocoap.Message(code=aiocoap.CHANGED,payload=not_payload)
                else:
                    print("mobile is asking opening",request_node_id)
                    node_value = [1,query_time_value[1]];
                    self.my_pos_dict[request_node_id] = node_value  # request come

                    unlock_payload = "unlocking..."+"##"+query_time_value[1]+ query_time_value[1] +"\0"

                    unlock_payload = unlock_payload.encode("ascii")
                    return aiocoap.Message(code=aiocoap.CHANGED, payload=unlock_payload)
                

class HelloResource(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.set_content(b"hello,world this is my test for python coap!")

    def set_content(self, content):
        self.payload = content
        
    async def render_get(self, request):
        print("this method is called by get client")
        return aiocoap.Message(payload=b'return_data_by_hello_resource')

    async def render_put(self, request):
        print("this method is called by PUT client")
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED,payload = b'return_data')


class ReceiveData(resource.ObservableResource):
    def __init__(self):
        super().__init__()

        
    async def render_get(self, request):
        if PRINT_FLAG:
            print("ReceiveData")
            print(request.opt.uri_query)

        return aiocoap.Message(payload=b'your message')


    async def render_put(self, request):
        print("this method is called by PUT client")
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED,payload = b'your message')


class QueryData(resource.ObservableResource):
    def __init__(self):
        super().__init__()

        
    async def render_get(self, request):
        if PRINT_FLAG:
            print("QueryData")
            print(request.opt.uri_query)
        # print("this method is called by get client")

        nodeId = request.opt.uri_query[0].split("=")[1]

        global BikeStates
        global MobileMessages

        if nodeId not in BikeStates.keys():
            BikeStates[nodeId] = "LOCKED"
            backMessage = "OFF"

        if nodeId not in MobileMessages.keys():
            backMessage = "NO MESSAGE"
        else:
            if MobileMessages[nodeId] == "OPEN":
                del MobileMessages[nodeId]
                BikeStates[nodeId] = "UNLOCKED"
                backMessage = "ON"
            elif MobileMessages[nodeId] == "CLOSE":
                del MobileMessages[nodeId]
                BikeStates[nodeId] = "LOCKED"
                backMessage = "OFF"
        
        if PRINT_FLAG:
            print(MobileMessages)
            print(BikeStates)
            print(nodeId + ": " + backMessage)

        return aiocoap.Message(payload=backMessage.encode('ascii'))


    async def render_put(self, request):
        print("this method is called by PUT client")
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED,payload = b'hahahah')

class MobileEventDeal(resource.ObservableResource):
    def __init__(self):
        super().__init__()

    async def render_get(self, request):
        if PRINT_FLAG:
            print("MobileEventDeal")
            print(request.opt.uri_query)

        message = request.opt.uri_query[0].split("=")[1]
        nodeId = request.opt.uri_query[1].split("=")[1]
        # print(nodeId + " " + message)

        global MobileMessages
        global BikeStates
        MobileMessages[nodeId] = message
        if message == "OPEN":
            print("Mobile device is asking to unlock node" + nodeId)
        elif message == "CLOSE":
            print("virtual device is asking to lock node" + nodeId)

        if PRINT_FLAG:
            print(MobileMessages)
            print(BikeStates)

        return aiocoap.Message(payload=b'your message')


    async def render_put(self, request):

        return aiocoap.Message(code=aiocoap.CHANGED,payload = b'your message')


def MyCoap():
    # Resource tree creation
    root = resource.Site()

    lock = threading.Lock()

    root.add_resource(('.well-known', 'core'),
            resource.WKCResource(root.get_resources_as_linkheader))

    # root.add_resource(('time',), TimeResource())

    # root.add_resource(('other', 'lockrequest'), BlockResource())

    root.add_resource(('hello',), HelloResource())

    root.add_resource(('receiveData',), ReceiveData())

    root.add_resource(('queryData',), QueryData())

    root.add_resource(('mobileEvent',), MobileEventDeal())

    # root.add_resource(('other','gpsinfo'), GPSPositionResource())

    asyncio.Task(aiocoap.Context.create_server_context(root))

    asyncio.get_event_loop().run_forever()

if __name__ == '__main__':
    print('coap sever is running....')
    MyCoap()
    
