#encoding=utf-8
#! /usr/bin/python
import re
import string

def CodeParseForNB():
    src_file = open("../user/app.cpp")
    src_str = src_file.read()
    src_file.close()

    src_str += "\nvoid"
    dest_file_h = open("../template/template_file_for_NB.h")
    dest_str_h = dest_file_h.read()
    dest_file_h.close()

    dest_file_cpp = open("../template/template_file_for_NB.cpp")
    dest_str_cpp = dest_file_cpp.read()
    dest_file_cpp.close()


    # 剔除多余代码
    delete_dest = re.compile("\s+(.*reqisterCallback.*)\n",re.M)
    delete_dest_out = delete_dest.findall(src_str)

    if delete_dest_out:
        src_str = src_str.replace(delete_dest_out[0],"")
        round_send = re.compile(r"//\s*segment_for_round_send",re.S|re.M)
        round_send_event = "roundQuery(get_nodeid(), &node::messageArrived);"
        dest_str_cpp = round_send.sub(round_send_event, dest_str_cpp)


    ClassTypeList = ["TL_NB","TL_LOCK","TL_GPS","TL_Temperature","TL_Time","TL_Light","TL_Humidity",
                "TL_Moter","TL_LED","TL_Soil_Humidity","TL_FAN","TL_Light"]
    # 处理类型
    for per_class in ClassTypeList:
        src_str = ConvertClassToObject(per_class, src_str)

        # 单独处理coap

    coap_query = re.compile("TL_COAP\s+(.*?);",re.S|re.M)
    coap_var = coap_query.findall(src_str)

    if coap_var:
        pass
        src_str = src_str.replace(coap_var[0], "_inner_coap")


    user_var = ""
    user_var = getUserVar(src_str)  # 

    if user_var != "":
        var_segment = re.compile(r"//\s*segment_for_user_init",re.S|re.M)
        dest_str_h = var_segment.sub(user_var, dest_str_h)

    sleepTime_var = re.compile("setSleep\((.*)\)")
    sleepTime = sleepTime_var.findall(src_str)
    insert_code = ""
    if re.search(r"^(\D)",sleepTime[0]):
        sleep_var = re.compile("{}\s*=\s*(.*);".format(sleepTime[0]))
        sleep_value = sleep_var.findall(src_str)
        insert_code = "int _inner_SleepTime = %s ;" %(sleep_value[0])
        
    else:  # num
        insert_code = "int _inner_SleepTime = %s ;"%(sleepTime[0])

    if insert_code != "":
        sleep_var = re.compile(r"//\s*segment_for_sleep_time", re.S|re.M)
        dest_str_h = sleep_var.sub(insert_code, dest_str_h)

    # messageArrived segment
    message_segment = re.compile("messageArrived\s*\(.+?\){(.*?)}[\n|\s]*void",re.M|re.S)
    message_code = message_segment.findall(src_str)
    if message_code:
        callback_block = re.compile(r"//\s*segment_for_callback_function", re.S|re.M)
        dest_str_cpp = callback_block.sub(message_code[0], dest_str_cpp)


   # setup segement
    user_setup = re.compile("setup\s*\(\)\s*{(.*?)}[\n|\s]*void",re.M|re.S)
    user_setup_code = user_setup.findall(src_str)
    user_setup_code[0] = user_setup_code[0].strip()
    if user_setup_code[0]:
        setup = re.compile(r"//\s*segment_for_user_setup",re.S|re.M)
        dest_str_cpp = setup.sub(user_setup_code[0], dest_str_cpp)

    # loop segement
    user_loop = re.compile("loop\s*\(\)\s*{(.*?)}[\n|\s]*void",re.M|re.S)
    user_loop_code = user_loop.findall(src_str)
    user_loop_code[0] = user_loop_code[0].strip()
    if user_loop_code[0]:
        loop = re.compile(r"//\s*segment_for_user_loop",re.S|re.M)
        dest_str_cpp = loop.sub(user_loop_code[0], dest_str_cpp)

    deal_time_delay_event(dest_str_h, dest_str_cpp)


def CodeParseForTinyLink():

    src_file = open("../user/app.cpp")
    src_str = src_file.read()
    src_file.close()

    src_str += "\nvoid"
    dest_file_h = open("../template/template_file_for_tinylink.h")
    dest_str_h = dest_file_h.read()
    dest_file_h.close()

    dest_file_cpp = open("../template/template_file_for_tinylink.cpp")
    dest_str_cpp = dest_file_cpp.read()
    dest_file_cpp.close()

    # 剔除注册回调函数代码
    delete_dest = re.compile("\s+(.*reqisterCallback.*)\n",re.M)
    delete_dest_out = delete_dest.findall(src_str)

    if delete_dest_out:
        src_str = src_str.replace(delete_dest_out[0],"")
        round_send = re.compile(r"//\s*segment_for_round_send",re.S|re.M)
        round_send_event = "roundQuery(get_nodeid(), &node::messageArrived);"
        dest_str_cpp = round_send.sub(round_send_event, dest_str_cpp)


    # 处理类型
    classname_list = ["TL_NB","TL_LOCK","TL_GPS","TL_Time","TL_Serial","TL_LED","TL_WIFI","TL_Temperature", "TL_Humidity",
                "TL_Light","TL_Motor","TL_FAN"]

    for per_class in classname_list:
        src_str = ConvertClassToObject(per_class, src_str)

    # 填充目标文件 user_final_file.h 填充变量
    user_var, user_char_var = getUserVarForMQTT(src_str)
    # input(user_var)
    if user_var:
        if user_char_var:
            char_vars = ""
            for line in user_char_var:
                char_vars = char_vars + "\n" + line
            dest_str_cpp = re.compile(r"//\s*segment_for_char_args", re.S|re.M).sub(char_vars, dest_str_cpp)
        var = re.compile(r"//\s*segment_for_user_init",re.S|re.M)
        dest_str_h = var.sub(user_var, dest_str_h)
        #print(dest_str_h)

    # setup segement
    # print(src_str)
    user_setup = re.compile("setup\s*\(\)\s*{(.*?)}[\n\s]*void",re.M|re.S)
    # user_setup = re.compile("setup\s*\(\)\s*\{(.*?)\}(\n|\s)*void", re.M|re.S)
    user_setup_code = user_setup.findall(src_str)
    user_setup_code = user_setup_code[0].strip()
    if user_setup_code:
        # input(user_setup_code)
        if user_var.find("TL_MQTT") != -1:
            mqtt_obj_name = re.compile("TL_MQTT\s*.*;").findall(user_var)[0][7:-1].strip(" ")
            user_setup_code = dealMQTT(mqtt_obj_name, user_setup_code)
        setup = re.compile(r"//\s*segment_for_user_setup", re.S|re.M)
        # input(user_setup_code)
        dest_str_cpp = setup.sub(user_setup_code, dest_str_cpp)

    # loop segement
    # user_loop = re.compile("loop\s*\(\)\s*{(.*?)}[\n|\s]*void",re.M|re.S)
    user_loop = re.compile("loop\s*\(\)\s*{(.*)}(\n|.)*void", re.M|re.S)
    user_loop_code = user_loop.findall(src_str)
    user_loop_code = user_loop_code[0][0].strip()
    if user_loop_code:
        # input(user_loop_code)
        loop = re.compile(r"//\s*segment_for_user_loop",re.S|re.M)
        dest_str_cpp = loop.sub(user_loop_code, dest_str_cpp)
    # print(dest_str_cpp)

    # deal_time_delay_event(dest_str_h, dest_str_cpp)
    add_time_delay(dest_str_h, dest_str_cpp)
    # write_app_file(dest_str_h, dest_str_cpp)


def CodeParseForALL():
    src_file = open("../user/app.cpp")
    src_str = src_file.read()
    src_file.close()

    src_str += "\nvoid"
    dest_file_h = open("../template/template_file_for_NB.h")
    dest_str_h = dest_file_h.read()
    dest_file_h.close()

    dest_file_cpp = open("../template/template_file_for_NB.cpp")
    dest_str_cpp = dest_file_cpp.read()
    dest_file_cpp.close()


    delete_dest = re.compile("\s+(.*reqisterCallback.*)\n",re.M)
    delete_dest_out = delete_dest.findall(src_str)

    if delete_dest_out:
        src_str = src_str.replace(delete_dest_out[0],"")
        round_send = re.compile(r"//\s*segment_for_round_send",re.S|re.M)
        round_send_event = "roundQuery(get_nodeid(), &node::messageArrived);"
        dest_str_cpp = round_send.sub(round_send_event, dest_str_cpp)

    classname_list = ["TL_NB","TL_LOCK","TL_GPS","TL_Time","TL_Serial","TL_LED","TL_WIFI","TL_Temperature",
                "TL_Light","TL_Motor","TL_FAN"]

    for per_class in classname_list:
        src_str = ConvertClassToObject(per_class, src_str)


    coap_query = re.compile("TL_COAP\s+(.*?);",re.S|re.M)
    coap_var = coap_query.findall(src_str)
    if coap_var:
        pass
        src_str = src_str.replace(coap_var[0], "_inner_coap")
        

    sleepTime_var = re.compile("setSleep\((.*)\)")
    sleepTime = sleepTime_var.findall(src_str)
    insert_code = ""
    if re.search(r"^(\D)",sleepTime[0]):
        sleep_var = re.compile("{}\s*=\s*(.*);".format(sleepTime[0]))
        sleep_value = sleep_var.findall(src_str)
        insert_code = "int _inner_SleepTime = %s ;" %(sleep_value[0])
        
    else:  # num
        insert_code = "int _inner_SleepTime = %s ;"%(sleepTime[0])

    if insert_code != "":
        sleep_var = re.compile(r"//\s*segment_for_sleep_time", re.S|re.M)
        dest_str_h = sleep_var.sub(insert_code, dest_str_h)

    # messageArrived segment
    message_segment = re.compile("messageArrived\s*\(.+?\){(.*?)}[\n|\s]*void",re.M|re.S)
    message_code = message_segment.findall(src_str)
    if message_code:
        callback_block = re.compile(r"//\s*segment_for_callback_function", re.S|re.M)
        dest_str_cpp = callback_block.sub(message_code[0], dest_str_cpp)


    user_var = getUserVar(src_str)
    #print(user_var)
    if user_var:
        var = re.compile(r"//\s*segment_for_user_init",re.S|re.M)
        dest_str_h = var.sub(user_var, dest_str_h)
        #print(dest_str_h)

    # setup segement
    user_setup = re.compile("setup\s*\(\)\s*{(.*?)}[\n\s]*void",re.M|re.S)
    user_setup_code = user_setup.findall(src_str)
    user_setup_code[0] = user_setup_code[0].strip()
    if user_setup_code[0]:
        setup = re.compile(r"//\s*segment_for_user_setup",re.S|re.M)
        dest_str_cpp = setup.sub(user_setup_code[0], dest_str_cpp)

    # loop segement
    user_loop = re.compile("loop\s*\(\)\s*{(.*?)}[\n|\s]*void",re.M|re.S)
    user_loop_code = user_loop.findall(src_str)
    user_loop_code[0] = user_loop_code[0].strip()
    if user_loop_code[0]:
        loop = re.compile(r"//\s*segment_for_user_loop",re.S|re.M)
        dest_str_cpp = loop.sub(user_loop_code[0], dest_str_cpp)

    deal_time_delay_event(dest_str_h, dest_str_cpp)


def deal_time_delay_event(dest_str_h,dest_str_cpp):
    '''
    deal TL_Time.delayMillis problem.
    '''
    src_cpp_str = dest_str_cpp
    src_header_str = dest_str_h
    # print(src_cpp_str,"\n",src_header_str)


    if src_cpp_str.find("tl_time.delayMillis") < 0: # no exists TL_Time.delayMillis method need to be dealt.
        write_app_file(src_header_str,src_cpp_str)
        return


    user_loop = re.compile("loop\s*\(\)\s*{(.*)}[\n|\s]*void",re.M|re.S) # "TL_Time.delatMillis" may also appear in "setup module".
    user_loop_code = user_loop.findall(src_cpp_str)
    # input(user_loop_code)
    if not user_loop_code: # content of loop is empty
        write_app_file(src_header_str,src_cpp_str)
        return

    user_setup = re.compile("setup\s*\(\)\s*{(.*)}\n*.*loop",re.M|re.S)
    user_setup_code = user_setup.findall(src_cpp_str)
    # input(user_setup_code)
    if not user_setup_code:
        write_app_file(src_header_str, src_cpp_str)
        return

    user_loop_code[0] = user_loop_code[0].strip()
    user_setup_code[0] = user_setup_code[0].strip()

    loop_ins_list = user_loop_code[0].split("\n"); # instruction list
    setup_ins_list = user_setup_code[0].split("\n")
    for ele in setup_ins_list:
        print(ele)
    ins_list = []
    for ele in loop_ins_list:
        ins_list.append(ele)
    for ele in setup_ins_list:
        ins_list.append(ele)
    ins_list = [a.strip() for a in ins_list]
    
    new_ins_list = []
    flag = False
    for var in ins_list:
        if var.find("tl_time.delayMillis") >= 0:
            flag = True
        if flag and var != "":
            new_ins_list.append(var)

    ins_dict = {}
    delay_num = 0
    ins_num = 1
    total_delay = 0
    mark = ""
    for var in new_ins_list:
        if var.find("tl_time.delay") >= 0:
            delay_num = delay_num + 1
            mark = "mark" + str(delay_num)
            delay = re.compile("\((.*?)\)")
            delay_time = delay.findall(var)[0]
            total_delay = total_delay + int(delay_time)
            ins_dict[mark] = {}
            ins_dict[mark]["time"] = total_delay
            ins_dict[mark]["instruction"] = ""
            src_cpp_str = replace_code_for_insert_event(var, delay_num, src_cpp_str,"mark")
        else:
            ins_dict[mark]["instruction"] = ins_dict[mark]["instruction"] + "\n\t" + var
            src_cpp_str = replace_code_for_insert_event(var, delay_num, src_cpp_str,"instruction")

    function_name = "_function_{}"
    function_body = ""
    function = "void function{}(){"
    event = '''
        std::shared_future<void> _event_sss{} = async(launch::deferred, &node::{}, this);
        sim_event  _event_eee{}(sim_scheduler::currentTime + {}, get_nodeid(), _event_sss{});
        sim_scheduler::sim_queue_insert_delay(_event_eee{});    
        ''' # need six paramters

    function_num = 1
    for key in ins_dict.keys():

        delay = ins_dict[key]["time"]
        current_function_name = function_name.format(function_num)
        function_for_header = "void " + current_function_name + "();" + "\n// segment_for_time_module"
        current_event = event.format(function_num,current_function_name,function_num,delay,function_num,function_num)
        instruction = ins_dict[key]["instruction"]
        instruction = "\n\tusleep(" + str(delay) + " * 1000);" + instruction
        function_body = "void node::" + current_function_name + "(){" + instruction + "\n}" + "\n// segment_for_time_module"
        # print(function_for_header,current_event , "\n",function_body)
       

        # replace header file
        header = re.compile(r"//\s*segment_for_time_module",re.S | re.M)
        src_header_str = header.sub(function_for_header, src_header_str)
        

        # replace cpp file
        cpp_block = re.compile(r"//\s*segment_for_time_module",re.M | re.S)
        src_cpp_str = cpp_block.sub(function_body, src_cpp_str)
        

        # insert event in cpp file
        event_block = re.compile(r"//\s*mark{};".format(function_num),re.M | re.S)
        src_cpp_str = event_block.sub(current_event, src_cpp_str)
        
        function_num = function_num + 1

        # end_for
    src_cpp_str = src_cpp_str[:-6]

    write_app_file(src_header_str,src_cpp_str)



def write_app_file(header_file,cpp_file,user = "qiufujian"):
    dest_str_h_write_file = open("../main/user_final_file.h","w")
    dest_str_h_write_file.write(header_file)
    dest_str_h_write_file.close()

    dest_str_cpp_write_file = open("../main/user_final_file.cpp","w")
    dest_str_cpp_write_file.write(cpp_file)
    dest_str_cpp_write_file.close()

    lines = open("../main/user_final_file.cpp").readlines()
    for line in lines:
        if line.find("delayMillis") != -1:
            pass
            
        

def replace_code_for_insert_event(text,num,primary_text,mark):
    # print("mark num:",num)
    if not text or len(text) == 0:
        return
    result = ""
    if mark == "mark":
        result = primary_text.replace(text,"//mark{};".format(num),1)
    elif mark == "instruction":
        result = primary_text.replace(text,"",1)
    return result

def ConvertClassToObject(classname, srcfile):
    p = re.findall(r"%s" % classname, srcfile, re.S)
    if len(p) > 0:
        rule = re.compile(r"%s" % classname, re.S)
        s = rule.sub(classname.lower(), srcfile)
        srcfile = s
        return srcfile
    return srcfile


def getUserVar(srcfile):
    UserDefineVar = ""
    i = 0
    while i < (len(srcfile) - 4):
        if srcfile[i:i+4] == 'void':
            return UserDefineVar
        else:
            UserDefineVar = UserDefineVar + srcfile[i]
            i = i+1
    return UserDefineVar.strip('\n')


def getUserVarForMQTT(srcfile):
    UserDefineVar = ""
    i = 0
    while i < (len(srcfile) - 4):
        if srcfile[i:i+4] == 'void':
            require = re.compile('REQUIRE\(.*\)')
            UserDefineVar = require.sub('// REQUIRE', UserDefineVar)

            char_var = re.compile("char.+\[\].*;", re.M)
            char_vars = char_var.findall(UserDefineVar)
            UserDefineVar = char_var.sub("", UserDefineVar)

            return UserDefineVar, char_vars
        else:
            UserDefineVar = UserDefineVar + srcfile[i]
            i = i+1
    return UserDefineVar.strip('\n')


def dealMQTT(mqtt_obj_name, srcfile):
    srcfile = re.compile(".*TL_WiFi.*;").sub("", srcfile)
    mqtt_args = re.compile("connect\(.*\);").findall(srcfile)[0].split(",")
    servername = mqtt_args[0][8:].strip(" ")
    port = mqtt_args[1].strip(" ")
    clientname = mqtt_args[2].strip(" ")
    username = mqtt_args[3].strip(" ")
    password = mqtt_args[4].strip(" ")[:-2]

    # replace with tinysim mqtt
    mqtt_config = '''
    IPStack ipstack = IPStack();
    MQTT::Client<IPStack, Countdown> {} = MQTT::Client<IPStack, Countdown>(ipstack);
    int rc = ipstack.connect({}, {});

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = (char*){};
    connectData.username.cstring = (char*){};
    connectData.password.cstring = (char*){};

    int a = {}.connect(connectData);
    '''
    mqtt_config = mqtt_config.format(mqtt_obj_name, servername, port, clientname, username, password, mqtt_obj_name)
    srcfile = re.compile(mqtt_obj_name + "\.connect\(.*\);").sub(mqtt_config, srcfile)
    return srcfile


def add_time_delay(dest_str_h, dest_str_cpp):
    event = '''
    tl_time.delayMillis({});
    sim_scheduler::currentTime += {};
    '''
    delay_events = re.compile("tl_time\.delayMillis\(.*\);").findall(dest_str_cpp)
    for delay_event in delay_events:
        delay_time = delay_event[20:-2]
        delay_event = re.compile("\.").sub("\.", delay_event)
        delay_event = re.compile("\+").sub("\+", delay_event)
        delay_event = re.compile("\(").sub("\(", delay_event)
        delay_event = re.compile("\)").sub("\)", delay_event)
        # input(delay_event)
        dest_str_cpp = re.compile(delay_event).sub(event.format(delay_time, delay_time), dest_str_cpp)
        # input(dest_str_cpp)
    write_app_file(dest_str_h, dest_str_cpp)


if __name__=="__main__":
    CodeParseForTinyLink()
