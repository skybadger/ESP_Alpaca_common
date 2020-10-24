/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOM_Safety
#define _ASCOM_Safety

//focuser-specific variables
extern float temperature;
extern List monitorSources;
extern List sourceWeights;
// 

//GET /safetymonitor/{device_number}/issafe
//Indicates whether the environment is safe for telescope operations due to weather, power, temperature, inter-locks etc. 
void handleIsSafe(void);


#endif 