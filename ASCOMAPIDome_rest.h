/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOMAPIDome_rest_h_
#define _ASCOMAPIDome_rest_h_

//GET /dome/{device_number}/altitude           The dome altitude
//GET /dome/{device_number}/athome             Indicates whether the dome is in the home position.
//GET /dome/{device_number}/atpark             Indicates whether the telescope is at the park position
//GET /dome/{device_number}/azimuth            The dome azimuth
//GET /dome/{device_number}/canfindhome        Indicates whether the dome can find the home position.
//GET /dome/{device_number}/canpark            Indicates whether the dome can be parked.
//GET /dome/{device_number}/cansetaltitude     Indicates whether the dome altitude can be set
//GET /dome/{device_number}/cansetazimuth      Indicates whether the dome azimuth can be set
//GET /dome/{device_number}/cansetpark         Indicates whether the dome park position can be set
//GET /dome/{device_number}/cansetshutter      Indicates whether the dome shutter can be opened
//GET /dome/{device_number}/canslave           Indicates whether the dome supports slaving to a telescope
//GET /dome/{device_number}/cansyncazimuth     Indicates whether the dome azimuth position can be synched
//GET /dome/{device_number}/shutterstatus      Status of the dome shutter or roll-off roof
//GET /dome/{device_number}/slaved             Indicates whether the dome is slaved to the telescope
//PUT /dome/{device_number}/slaved             Sets whether the dome is slaved to the telescope
//GET /dome/{device_number}/slewing            Indicates whether the any part of the dome is moving
//PUT /dome/{device_number}/abortslew          Immediately cancel current dome operation.
//PUT /dome/{device_number}/closeshutter       Close the shutter or otherwise shield telescope from the sky.
//PUT /dome/{device_number}/findhome           Start operation to search for the dome home position.
//PUT /dome/{device_number}/openshutter        Open shutter or otherwise expose telescope to the sky.
//PUT /dome/{device_number}/park               Rotate dome in azimuth to park position.
//PUT /dome/{device_number}/setpark            Set the current azimuth, altitude position of dome to be the park position
//PUT /dome/{device_number}/slewtoaltitude     Slew the dome to the given altitude position.
//PUT /dome/{device_number}/slewtoazimuth      Slew the dome to the given azimuth position.
//PUT /dome/{device_number}/synctoazimuth      Synchronize the current position of the dome to the given azimuth.

void handleAltitudeGet( void);
void handleAtHomeGet( void);
void handleAtParkGet(void);
void handleAzimuthGet(void);
void handleCanFindHomeGet(void);
void handleCanParkGet(void);
void handleCanSetAltitudeGet(void);
void handleCanFindSetAzimuthGet(void);
void handleCanSetParkGet(void);
void handleCanSetShutterGet(void);
void handleCanSlaveGet(void);
void handleCanSyncAzimuthGet(void);
void handleShutterStatusGet(void);
void handleSlavedGet(void);
void handleSlavedPut(void);
void handleSlewingGet(void);
void handleAbortSlewPut(void);
void handleCloseShutterPut(void);
void handleFindHomePut(void);
void handleOpenShutterPut(void);
void handleParkPut(void);
void handleSetParkPut(void);
void handleSlewToAltitudePut(void);
void handleSlewToAzimuthPut(void);
void handleSyncToAzimuthPut(void);

#endif