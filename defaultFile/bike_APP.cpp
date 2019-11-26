TL_COAP coap;
char serverName[20] = "127.0.0.1";
char port[5] = "5683";

void messageArrived(string message){
    if(message == "ON"){ 
        TL_LOCK.unlock();
     }
}

void setup() {
    TL_NB.init();
    TL_NB.setReceive(100);
    TL_NB.setSleep(1000);

    coap = TL_NB.fetchCoap();
    coap.connect(serverName, port);
    coap.reqisterCallback(messageArrived);
}

void loop() {
    TL_GPS.read();
    double lat = TL_GPS.latData();
    double lng = TL_GPS.lngData();
    coap.send(lat,lng);
}
