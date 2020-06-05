//Project-wide definitions 
//QCOMPASS calibration offsets array length 
#define N_OFFSETS 3

//Hostname and common identifiers max string length
#define MAX_NAME_LENGTH 25

//Timezone constants for location used for SNTP time configuration
#define TZ              0       // (utc+) TZ in hours
#define DST_MN          60      // use 60mn for summer time in some countries
#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)

//MQTT global message length override. 
#define MQTT_MAX_PACKET_SIZE 300

//EEPROM magic character
#ifndef _magic
#define magic '#'
#endif
