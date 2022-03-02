#if !defined _SKYBADGER_STRINGS_H_
#define _SKYBADGER_STRINGS_H_

//ALPACA management interface Strings 
const char* Manufacturer PROGMEM = "Skybadger Astronomy";

//Strings
const char* ssid1     =        "";
const char* password1 =        "";

const char* ssid2     =        "";
const char* password2 =        "";

const char* ssid3     =        "";
const char* password3 =        "";

const char* ssid4     =        "";
const char* password4 =        "";

//MQTT settings
const char* pubsubUserID =    "";
const char* pubsubUserPwd =   "";
const char* mqtt_server =     "obbo.i-badger.co.uk";
const char* outHealthTopic =  "/skybadger/devices/";
const char* outSenseTopic =   "/skybadger/sensors/";
const char* outFnTopic =      "/skybadger/functions/";
const char* inTopic =         "/skybadger/devices/heartbeat";

//Timeserver settings - UK 
const char* timeServer1 = "0.uk.pool.ntp.org";
const char* timeServer2 = "1.uk.pool.ntp.org";
const char* timeServer3 = "ntp.exnet.com";

#endif
