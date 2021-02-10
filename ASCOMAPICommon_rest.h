/*
File to be included into relevant device REST setup 
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
extern bool connected;
extern String DriverName;
extern String DriverVersion;
extern String DriverInfo;
extern String Description;
extern String InterfaceVersion;
extern long GUID
extern long INSTANCE_NUMBER;
*/
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
//GET /{DeviceType}/{DeviceNumber}/Name Device name
void handleNameGet(void);
void handleNamePut(void); //Non-ASCOM , required by setup
void handleFilterCountPut(void); 
//GET /{DeviceType}/{DeviceNumber}/SupportedActions Returns the list of action names supported by this driver.  
void handleSupportedActionsGet(void);

//TODO - fixup connected checks when functions not currently implemented get implemented

int serverTransID= 0;
const unsigned int NOT_CONNECTED = (unsigned int) -1;

void handleAction(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Action", notImplemented, "Not implemented" );
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBlind", notImplemented, "Not implemented" );
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBool", notImplemented, "Not implemented" );
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

    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandString", notImplemented, "Not implemented" );
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
    uint32_t clientID;
    uint32_t clientTransID;
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
             jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", notConnected, "Another client already connected" );        
             root["Value"] = false;
           }
           else //OK
           {
              connected = clientID;
              jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );      
              root["Value"] = true; 
              connectionCtr++;
           }
         }
         else //'tobeConnected' = false
         {
           if ( connected != NOT_CONNECTED ) //
           {
             //If we are disconnecting, we should park or home and close the shutter ?
             //We could count the number of connects and disconnects from different clientIDs to trac when noone is finally connected. 
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
                jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );        
                connected = NOT_CONNECTED; //OK
                root["Value"] = false;
                //root.remove( "Value" );
                connectionCtr--;
             }
             else
             {
                jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", notConnected, "This client is not the connected client" );        
                root["Value"] = false;
             }
           }
           else //not already connected
           {
             //Check error numbers
             jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );        
             //root.remove( "Value" );
             root["Value"] = false;
             connected = NOT_CONNECTED;
           }
         }
       }
       else
       {
         jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", invalidOperation , "Unable to parse 'connected' value  during PUT" );
       }
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );        
      root["Value"] = (connected != NOT_CONNECTED);      
    }
    else
    {
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", invalidOperation , "Unexpected HTTP method" );        
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Description", 0, "" );    
    root["Value"]= Description;    
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverInfo", Success, "" );    
    root["Value"]= DriverInfo;    
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "DriverVersion", Success, "" );    
    root["Value"]= DriverVersion;    
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "InterfaceVersion", Success, "" );    
    root["Value"]= DriverVersion;    
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Name", Success, "" );    
    root["Value"] = DriverName;    
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
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "SupportedActions", Success, "" );    
    JsonArray& values  = root.createNestedArray("Value");   
    values.add(""); //Explicitly empty array
    root.printTo(message);
    server.send(200, F("application/json"), message);
    return ;
}

void handleAPIversions(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIversions", Success, "" );  
    JsonArray& values  = root.createNestedArray("Value");     
    values.add(atoi(InterfaceVersion.c_str()));    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    
    server.send(200, F("application/json"), message);
    return ;
}

void handleAPIconfiguredDevices(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIversions", Success, "" );  
    JsonArray& values  = root.createNestedArray("Value");
    
    DynamicJsonBuffer jsonBuff2(256);
    JsonObject& device= jsonBuff2.createObject();
    device["DeviceName"]= Description;
    device["DeviceType"]= DriverName;
    device["DeviceNumber"]= INSTANCE_NUMBER;
    device["UniqueID"]= GUID;    

    values.add(device);    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    
    server.send(200, F("application/json"), message);
    return ;
}

#endif
