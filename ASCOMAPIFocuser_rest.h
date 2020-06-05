/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOM_Focuser 
#define _ASCOM_Focuser
//focuser-specific variables
extern uint_t_32 position;
extern enum FocuserType;
extern enum FocuserState;
extern FocuserType focuserType;
extern FocuserState focuserState;
extern boolean isMoving;
extern boolean tempCompAvailable;
extern float focuserTemp;

//GET /focuser/{device_number}/absolute
//Indicates whether the focuser is capable of absolute position.
void handleFocuserTypeGet(void);

//GET /focuser/{device_number}/ismoving
// Indicates whether the focuser is currently moving.
void handleFocuserMovingGet(void);

//GET /focuser/{device_number}/maxincrement
//Returns the focuser's maximum increment size.
void handleFocuserMaxIncrementGet(void);

//GET /focuser/{device_number}/maxstep
//Returns the focuser's maximum step size.
void handleFocuserMaxStepGet(void);

//GET focuser/{device_number}/position
//Returns the focuser's current position.
void handleFocuserPositionGet(void);

//GET /focuser/{device_number}/stepsize 
//Returns the focuser's step size.
void handleFocuserStepSizeGet(void);

//GET /focuser/{device_number}/tempcomp
//Retrieves the state of temperature compensation mode
void handleFocuserTempCompModeGet(void);

//PUT /focuser/{device_number}/tempcomp
//Sets the device's temperature compensation mode.
void handleFocuserTempCompModePut(void);

//GET /focuser/{device_number}/tempcompavailable
//Indicates whether the focuser has temperature compensation.
void handleFocuserTempCompAvailableGet(void);

//GET /focuser/{device_number}/temperature
//Returns the focuser's current temperature.
void handleFocuserTempGet(void);

//PUT /focuser/{device_number}/halt
//Immediately stops focuser motion.
void handleFocuserHaltPut(void);

//PUT /focuser/{device_number}/move
//Moves the focuser to a new position.
void handleFocuserMovePut(void);

#endif 