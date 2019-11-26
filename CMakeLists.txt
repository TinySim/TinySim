CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SIM_LIB)
SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "-std=c++11 -O2 -D_GNU_SOURCE -DWITH_POSIX -w")  #  -s 去掉了
SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0  -g -ggdb ")  # -Wall
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 ") # -Wall

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib /usr/lib /usr/local/lib)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/library ${PROJECT_SOURCE_DIR}/TL_MQTT ${PROJECT_SOURCE_DIR}/TL_NB ${PROJECT_SOURCE_DIR}/TL_schedule ${PROJECT_SOURCE_DIR}/TL_SIM_radio ${PROJECT_SOURCE_DIR}/TL_SIM_dbg ${PROJECT_SOURCE_DIR}/TL_SIM_PowerProfile ${PROJECT_SOURCE_DIR}/TL_SIM_TimeProfile  ${PROJECT_SOURCE_DIR}/TL_SIM_datagen ${PROJECT_SOURCE_DIR}/TL_GPS ${PROJECT_SOURCE_DIR}/TL_LOCK ${PROJECT_SOURCE_DIR}/TL_COAP  ${PROJECT_SOURCE_DIR}/TL_SIM_mode  ${PROJECT_SOURCE_DIR}/main ${PROJECT_SOURCE_DIR}/TL_LED ${PROJECT_SOURCE_DIR}/TL_Time ${PROJECT_SOURCE_DIR}/TL_Serial ${PROJECT_SOURCE_DIR}/TL_Temperature ${PROJECT_SOURCE_DIR}/TL_FAN ${PROJECT_SOURCE_DIR}/TL_Light ${PROJECT_SOURCE_DIR}/TL_SIM_Climate /usr/local/include/libcoap)

ADD_EXECUTABLE(app ./main/TL_SIM_main.cpp ./library/WMath.cpp ./library/itoa.cpp ./library/dtostrf.cpp  ./library/WString.cpp ./TL_MQTT/MQTTDeserializePublish.cpp ./TL_MQTT/MQTTUnsubscribeServer.cpp ./TL_MQTT/MQTTFormat.cpp ./TL_MQTT/MQTTConnectServer.cpp ./TL_MQTT/TL_MQTTClient.cpp ./TL_MQTT/MQTTUnsubscribeClient.cpp ./TL_MQTT/MQTTConnectClient.cpp ./TL_MQTT/MQTTSubscribeClient.cpp ./TL_MQTT/MQTTSerializePublish.cpp ./TL_MQTT/MQTTPacket.cpp ./TL_MQTT/MQTTSubscribeServer.cpp ./TL_NB/UE.cpp ./TL_NB/TL_NB.cpp ./TL_SIM_radio/TL_radio.cpp ./TL_SIM_radio/TL_radio_csma.cpp ./TL_SIM_radio/TL_radio_link.cpp ./TL_SIM_radio/TL_radio_trace.cpp ./TL_schedule/TL_SIM_schedule.cpp ./TL_SIM_dbg/TL_SIM_log.cpp ./TL_SIM_dbg/TL_SIM_dbg.cpp ./TL_SIM_PowerProfile/TL_SIM_power_node.cpp ./TL_SIM_PowerProfile/TL_SIM_powerp.cpp ./TL_SIM_PowerProfile/TL_SIM_power_util.cpp ./TL_SIM_TimeProfile/TL_SIM_time_node.cpp ./TL_SIM_TimeProfile/TL_SIM_timep.cpp ./TL_SIM_TimeProfile/TL_SIM_time_util.cpp ./TL_SIM_datagen/bsDataGen.cpp ./TL_schedule/sim_event_util.cpp ./TL_GPS/TL_GPS.cpp ./TL_GPS/U_BLOX_GPS_Arduino_UNO.cpp ./TL_LOCK/TL_LOCK.cpp ./TL_COAP/TL_COAP.cpp ./TL_SIM_datagen/CloudDataGen.cpp ./TL_SIM_mode/TL_SIM_mode.cpp ./TL_LED/TL_LED.cpp ./TL_Time/TL_Time.cpp  ./TL_Serial/TL_Serial.cpp ./TL_Temperature/TL_Temperature.cpp  ./TL_FAN/TL_FAN.cpp  ./TL_Light/TL_Light.cpp ./TL_SIM_Climate/TL_SIM_Climate.cpp ./main/user_final_file.cpp) 


TARGET_LINK_LIBRARIES(app -lm -lz -lrt -lpthread -pthread -lmycoap -lcoap -ljson)





