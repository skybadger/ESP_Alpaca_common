/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOM_Focuser 
#define _ASCOM_Focuser
//focuser-specific variables
extern int position;
//extern FocuserStates focuserState;
//extern FocuserStates targetFocuserState; 
extern bool isMoving;
extern bool tempCompAvailable;
extern float focuserTemp;


class ASCOMFocuser extends ASCOMBase 
{
public static: 


private: 
 

public: 

//basic constructor 
ASCOMFocuser:: ASCOMFocuser( char* hostname, char* ServicePort, char* mqttServer, int mqttPort ): 
{
	
}

//GET /focuser/{device_number}/absolute
//Indicates whether the focuser is capable of absolute position.
abstract void handleFocuserTypeGet(void);

//GET /focuser/{device_number}/ismoving
// Indicates whether the focuser is currently moving.
abstract void handleFocuserMovingGet(void);

//GET /focuser/{device_number}/maxincrement
//Returns the focuser's maximum increment size.
abstract void handleFocuserMaxIncrementGet(void);

//GET /focuser/{device_number}/maxstep
//Returns the focuser's maximum step size.
abstract void handleFocuserMaxStepGet(void);

//GET focuser/{device_number}/position
//Returns the focuser's current position.
abstract void handleFocuserPositionGet(void);

//GET /focuser/{device_number}/stepsize 
//Returns the focuser's step size.
abstract void handleFocuserStepSizeGet(void);

//GET /focuser/{device_number}/tempcomp
//Retrieves the state of temperature compensation mode
abstract void handleFocuserTempCompModeGet(void);

//PUT /focuser/{device_number}/tempcomp
//Sets the device's temperature compensation mode.
abstract void handleFocuserTempCompModePut(void);

//GET /focuser/{device_number}/tempcompavailable
//Indicates whether the focuser has temperature compensation.
abstract void handleFocuserTempCompAvailableGet(void);

//GET /focuser/{device_number}/temperature
//Returns the focuser's current temperature.
abstract void handleFocuserTempGet(void);

//PUT /focuser/{device_number}/halt
//Immediately stops focuser motion.
abstract void handleFocuserHaltPut(void);

//PUT /focuser/{device_number}/move
//Moves the focuser to a new position.
abstract void handleFocuserMovePut(void);
}
#endif 