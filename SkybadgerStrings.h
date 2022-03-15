#if !defined _SKYBADGER_STRINGS_H_
#define _SKYBADGER_STRINGS_H_
//Strings moved to PROGMEM to save on heap . Use care with accessing. 
//ALPACA management interface Strings 
const char* Manufacturer PROGMEM = "Skybadger Astronomy";

//WiFi Strings
const char* ssid1 		PROGMEM =        "";
const char* password1 	PROGMEM =        "";
const char* ssid2 		PROGMEM =        "";
const char* password2 	PROGMEM =        "";
const char* ssid3 		PROGMEM =        "";
const char* password3 	PROGMEM =        "";
const char* ssid4 		PROGMEM =        "";
const char* password4 	PROGMEM =        "";
const char* ssid5 		PROGMEM =        "";
const char* password5 	PROGMEM =        "";

//MQTT settings
const char* pubsubUserID 	PROGMEM =   "";
const char* pubsubUserPwd 	PROGMEM =   "";
const char* mqtt_server 	PROGMEM =   "";
const char* mqtt_server2 	PROGMEM =   "";

//Use structure topic: /<MQTT host>/<feature>/<feature type>/<hostname>  payload:  json string of <values>
const char* outHealthTopic 	PROGMEM =  	"/skybadger/devices/";
const char* outSenseTopic 	PROGMEM =   "/skybadger/sensors/";
const char* outFnTopic  	PROGMEM =   "/skybadger/functions/";
const char* inTopic 		PROGMEM =   "/skybadger/devices/heartbeat";

//Timeserver settings - UK 
#if defined _USE_PROGMEM
const char* timeServer1 PROGMEM = "0.uk.pool.ntp.org";
const char* timeServer2 PROGMEM = "1.uk.pool.ntp.org";
const char* timeServer3 PROGMEM = "ntp.exnet.com";
#else
const char* timeServer1 = "0.uk.pool.ntp.org";
const char* timeServer2 = "1.uk.pool.ntp.org";
const char* timeServer3 = "ntp.exnet.com";
#endif
#endif