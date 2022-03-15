/*
File to be included into relevant device REST setup to implement the default common mandatory 
ASCOM interface commands. 
Note that the action commands return NOT_IMPLEMENTED. Override if this is not what you need. 
Also includes default ALPACA Discovery handlers - you will need to listen for the ALPACA UDP calls from your main loop handler.
*/
/*Change Log
01/06/2021 Updated getActions to return empty array rather than array[] with an empty string to pass Conform compliance checks - tested with dome. 
*/

//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOMAPI_Common
#define _ASCOMAPI_Common
#include "AlpacaErrorConsts.h"
#include "ArduinoJson.h"
#include "JSONHelperFunctions.h"

//#define DEBUG_ESP_HTTP_SERVER - re-used from ESP8266WebServer.h but due to a dependency copied here. 
#ifndef DEBUG_OUTPUT
#ifdef DEBUG_ESP_PORT
#define DEBUG_OUTPUT DEBUG_ESP_PORT
#else
#define DEBUG_OUTPUT Serial
#endif
#endif

/*
Required External variables - Specify these in your driver - they are driver type specific. 
extern bool connected;
extern String DriverName;
extern String DriverVersion;
extern String DriverInfo;
extern String Description;
extern String InterfaceVersion;
extern char[] GUID
extern int instanceNumber;
extern float instanceVersion;
extern char[] Location;
*/
//Generic response tracking id 
int serverTransID= 0;

//Used to track whic device last connected- if !NOT_CONNECTED then contains the ID of the connected client.
const unsigned int NOT_CONNECTED = (unsigned int) -1;

//PUT /{DeviceType}/{DeviceNumber}/Action Invokes the specified device-specific action.
void handleAction(void);

//PUT /{DeviceType}/{DeviceNumber}/CommandBlind Transmits an arbitrary string to the device
void handleCommandBlind(void);

//PUT /{DeviceType}/{DeviceNumber}/CommandBool Transmits an arbitrary string to the device and returns a boolean value from the device.
void handleCommandBool(void);

//PUT /{DeviceType}/{DeviceNumber}/CommandString Transmits an arbitrary string to the device and returns a string value from the device
void handleCommandString(void);

//GET /{DeviceType}/{DeviceNumber}/Connected Retrieves the connected state of the device
//PUT /{DeviceType}/{DeviceNumber}/Connected Sets the connected state of the device
void handleConnected(void);

//GET /{DeviceType}/{DeviceNumber}/Description Device description
void handleDescriptionGet(void);

//GET /{DeviceType}/{DeviceNumber}/Driverinfo Device driver description
void handleDriverInfoGet(void);

//GET /{DeviceType}/{DeviceNumber}/DriverVersion Driver Version
void handleDriverVersionGet(void);

//GET /{DeviceType}/{DeviceNumber}/InterfaceVersion Driver Version
void handleInterfaceVersionGet(void);

//GET /{DeviceType}/{DeviceNumber}/Name Device name
void handleNameGet(void);
void handleNamePut(void); //Non-ASCOM , required by setup

//GET /{DeviceType}/{DeviceNumber}/SupportedActions Returns the list of action names supported by this driver.  
void handleSupportedActionsGet(void);

void handleAction(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Action"), notImplemented, F("Not implemented") );
    root["Value"] = "";
    root.printTo(message);
    server.send(200, F("application/json"), message);
    return;
 }

void handleCommandBlind(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
        
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("CommandBlind"), notImplemented, F("Not implemented") );
    root["Value"]= "";
    root.printTo(message);
    server.send(200, F("application/json"), message);
    return;
}

void handleCommandBool(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("CommandBool"), notImplemented, F("Not implemented") );
    root["Value"]= false; 
    root.printTo(message);   
    server.send(200, F("application/json"), message);
    return;
}

void handleCommandString(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    
    String argsToSearchFor[] = {"clientID","clientTransID"};
    uint32_t clientID = 0;
    uint32_t clientTransID = 0;

    if ( hasArgIC( argsToSearchFor[0], server, false ) )
        clientID = (uint32_t)server.arg( argsToSearchFor[0] ).toInt();

    if( hasArgIC( argsToSearchFor[1], server, false ) )
       clientTransID = (uint32_t)server.arg( argsToSearchFor[1]).toInt();

    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("CommandString"), notImplemented, F("Not implemented") );
    root["Value"]= ""; 
    root.printTo(message);   
    server.send(200, F("application/json"), message);
    return;
}


/*
Record the clientID of the connected client in the connected variable. 
Use a value of -1 ti indicate not connected. 
TODO- record a count for connected - disconnected

*/
void handleConnected(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    String argsToSearchFor[] = {"clientID","clientTransID","Connected"};
    uint32_t clientID = 0;
    uint32_t clientTransID = 0;
    bool tobeConnected = false;
    int retVal = 200;
    
    //Updated to fix case-sensitivity issues when putting a value since tobeConnected defaults to false if not found when supplied as a different case.
    if( hasArgIC( argsToSearchFor[0], server, false ) )
        clientID = (uint32_t)server.arg( argsToSearchFor[0] ).toInt();

    if( hasArgIC( argsToSearchFor[1], server, false ) )
       clientTransID = (uint32_t)server.arg( argsToSearchFor[1]).toInt();
    
    if( hasArgIC( argsToSearchFor[2], server, false ) )
       tobeConnected = (boolean) server.arg( argsToSearchFor[2]).equalsIgnoreCase( "true");
         
    if ( server.method() == HTTP_PUT )
    { 
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.printf( "handleConnected: new: %s, this client ID: %i, connectedID: %i\n", server.arg(argsToSearchFor[2]).c_str(), clientID, connected );
#endif
      if ( server.hasArg( argsToSearchFor[2] ) )
      {
         if ( tobeConnected == true ) 
         { //setting to true 
           if ( connected != NOT_CONNECTED )//already true with another clientID, so refuse to set
           {
             //Check error numbers
             jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), notConnected, F("Another client already connected") );        
             root["Value"] = false;
           }
           else //OK
           {
              connected = clientID;
              jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), Success, "" );      
              root["Value"] = true; 
              connectionCtr++;
           }
         }
         else //'tobeConnected' = false
         {
           if ( connected != NOT_CONNECTED ) //
           {
             //If we are disconnecting, we should park or home and close the shutter ?
             //We could count the number of connects and disconnects from different clientIDs to track when noone is finally connected. 
             //Relies on clients not diconnecting multiple times. 
             //
             if( connected == clientID )
             {
   #ifdef _ASCOM_Dome
                if( parkDomeOnDisconnect )
                    addDomeCmd( 0, 0, CMD_DOME_PARK, 0);
                if( closeShutterOnDisconnect )
                    addShutterCmd( 0, 0, CMD_SHUTTER_CLOSE, 0 );
   #endif 
                jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), Success, "" );        
                connected = NOT_CONNECTED; //OK
                root["Value"] = false;
                //root.remove( "Value" );
                connectionCtr--;
             }
             else
             {
                jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), notConnected, F("This client is not the connected client") );        
                root["Value"] = false;
             }
           }
           else //not already connected
           {
             //Check error numbers
             jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), Success, "" );        
             //root.remove( "Value" );
             root["Value"] = false;
             connected = NOT_CONNECTED;
           }
         }
       }
       else
       {
         jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), invalidOperation , F("Unable to parse 'connected' value  during PUT") );
       }
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), Success, "" );        
      root["Value"] = (connected != NOT_CONNECTED);      
    }
    else
    {
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Connected"), invalidOperation , F( "Unexpected HTTP method" ) );        
      root["Value"] = connected;      
    }

   root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
   DEBUG_OUTPUT.printf( "handleConnected: %s\n", message.c_str() );
#endif
   server.send( retVal, F("application/json"), message);
   return;
}

void handleDescriptionGet(void)
{
    String message;
    
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Description"), 0, "" );    
    root["Value"]= String(Description);    //From PROGMEM
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, F("application/json"), message);

    return ;
}

void handleDriverInfoGet(void)
{
    String message;
    //Dont care about client IDs for read-only data 

    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("DriverInfo"), Success, "" );    
    root["Value"]= String(DriverInfo);    //From PROGMEM
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    
    server.send(200, F("application/json"), message);
    return ;
}

void handleDriverVersionGet(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("DriverVersion"), Success, "" );    
    root["Value"]= String(DriverVersion);    //From PROGMEM
    root.printTo(message);
    
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif

    server.send(200, F("application/json"), message);
    return ;
}

void handleInterfaceVersionGet(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("InterfaceVersion"), Success, "" );    
    root["Value"]= String( InterfaceVersion); //From PROGMEM    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif

    server.send(200, F("application/json"), message);
    return ;
}

void handleNameGet(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("Name"), Success, "" );    
    root["Value"] = String( DriverName );    //From PROGMEM
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    server.send(200, F("application/json"), message);
    return ;
}

void handleSupportedActionsGet(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, F("SupportedActions"), Success, "" );    
    JsonArray& values  = root.createNestedArray("Value");   
    //01/06/2021 updated to return empty array to pass Conform compliance test. 
    root.printTo(message);
    server.send(200, F("application/json"), message);
    return ;
}

#endif
