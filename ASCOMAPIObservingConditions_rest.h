/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOM_ObservingConditions
#define _ASCOM_ObservingConditions

//focuser-specific variables
extern float temperature;
extern List monitorSources;
extern List sourceWeights;
/*
Averaging period only reflects some sensor entries. It  also requires you to have access to the last n data over the period. 
May not know the frequency of data provision. 

*/ 

//Hoping a bit to implement this one. 
//PUT /observingconditions​/{device_number}​/refresh
//Refreshes sensor values from hardware.

//GET /observingconditions​/{device_number}​/averageperiod
//PUT ​/observingconditions​/{device_number}​/averageperiod
//​Server.on( "/api/v1/observingconditions​/{device_number}​/averageperiod", HTTP_GET, ....);
//Returns the time period over which observations will be averaged
void handleAveragePeriod(void);

//GET ​/observingconditions​/{device_number}​/cloudcover - doable, maybe
//Returns the amount of sky obscured by cloud
void handleCloudcover(void);

//GET ​/observingconditions​/{device_number}​/dewpoint - doable
//Returns the atmospheric dew point at the observatory

//GET /observingconditions​/{device_number}​/humidity - doable
//Returns the atmospheric humidity at the observatory

//GET ​/observingconditions​/{device_number}​/pressure - doable
//Returns the atmospheric pressure at the observatory.

//GET ​/observingconditions​/{device_number}​/rainrate - doable maybe - can get rain but what terms of rate?
//Returns the rain rate at the observatory.

//GET ​/observingconditions​/{device_number}​/skybrightness - doable. 
//Returns the sky brightness at the observatory

//GET ​/observingconditions​/{device_number}​/skyquality - metric ?
//Returns the sky quality at the observatory

//GET ​/observingconditions​/{device_number}​/skytemperature - doable.
//Returns the sky temperature at the observatory

//GET ​/observingconditions​/{device_number}​/starfwhm - reported by what ?
//Returns the seeing at the observatory

//GET ​/observingconditions​/{device_number}​/temperature - multiple sources. 
//Returns the temperature at the observatory

//GET ​/observingconditions​/{device_number}​/winddirection - doable.
//Returns the wind direction at the observatory

//GET ​/observingconditions​/{device_number}​/windgust - have definition. Suggests implementation of counts/sec for 5 minutes 
//Returns the peak 3 second wind gust at the observatory over the last 2 minutes

//GET ​/observingconditions​/{device_number}​/windspeed - doable.
//Returns the wind speed at the observatory.

//GET ​/observingconditions​/{device_number}​/sensordescription - doable but doubles the existing work. 
//Return a sensor description

//GET ​/observingconditions​/{device_number}​/timesincelastupdate - this seems confused - individual sensors or the last sensor ?
//Return the time since the sensor value was last updated
#endif 