/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOM_Switch
#define _ASCOM_Switch

//GET ​/switch​/{device_number}​/maxswitch
//The number of switch devices managed by this driver
void handlerMaxswitch(void);

//GET ​/switch​/{device_number}​/canwrite
//Indicates whether the specified switch device can be written to
void handlerCanWrite(void);

//GET ​/switch​/{device_number}​/getswitch
//PUT ​/switch​/{device_number}​/setswitch
//Get/Set the state of switch device id as a boolean
void handlerSwitchState(void);

//GET ​/switch​/{device_number}​/getswitchdescription
//Gets the description of the specified switch device
void handlerSwitchDescription(void);

//GET ​/switch​/{device_number}​/getswitchname
//PUT ​/switch​/{device_number}​/setswitchname
//Get/set the name of the specified switch device
void handlerSwitchName(void);

//GET ​/switch​/{device_number}​/getswitchvalue
//PUT ​/switch​/{device_number}​/setswitchvalue
//Get/Set the value of the specified switch device as a double
void handlerSwitchValue(void);

//GET ​/switch​/{device_number}​/minswitchvalue
//Gets the minimum value of the specified switch device as a double
void handlerMinSwitchValue(void);

//GET ​/switch​/{device_number}​/maxswitchvalue
//Gets the maximum value of the specified switch device as a double
void handlerMaxSwitchValue(void);

//GET ​/switch​/{device_number}​/switchstep
//Returns the step size that this device supports (the difference between successive values of the device).
void handlerMaxSwitchStep(void);

//Additional non-ASCOM custom setup calls

//PUT ​/switch​/{device_number}​/minswitchvalue
//Set the minimum value of the specified switch device as a double - setup config 
void handlerMinSwitchValuePut(void);

//PUT ​/switch​/{device_number}​/maxswitchvalue
//Sets the maximum value of the specified switch device as a double - setup config
void handlerMaxSwitchValuePut(void);

//GET ​/switch​/{device_number}​/status
//Get a descriptor of all the switches managed by this driver for discovery purposes
void handlerMinSwitchValuePut(void);

#endif 