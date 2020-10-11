/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOMAPI_Common
#define _ASCOMAPI_Common
#include "AlpacaErrorConsts.h"
#include "ArduinoJson.h"
#include "JSONHelperFunctions.h"

/*
extern bool connected;
extern String DriverName;
extern String DriverVersion;
extern String DriverInfo;
extern String Description;
extern String InterfaceVersion;
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

int serverTransID= 0;

void handleAction(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Action", notImplemented, "Not implemented" );
    root["Value"]= "";
    root.printTo(message);
    server.send(200, "application/json", message);
    return;
 }

void handleCommandBlind(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
        
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBlind", notImplemented, "Not implemented" );
    root["Value"]= "";
    root.printTo(message);
    server.send(200, "application/json", message);
    return;
}

void handleCommandBool(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandBool", notImplemented, "Not implemented" );
    root["Value"]= false; 
    root.printTo(message);   
    server.send(200, "application/json", message);
    return;
}

void handleCommandString(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "CommandString", notImplemented, "Not implemented" );
    root["Value"]= ""; 
    root.printTo(message);   
    server.send(200, "application/json", message);
    return;
}

void handleConnected(void)
{
    String message;
    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();

    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );
    
    if ( server.method() == HTTP_PUT )
    { 
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.printf( "handleConnected: arg:%s, connected: %i\n", server.arg("Connected").c_str(), connected );
#endif

      //don't like the logic here - if it's already connected for this client we should refuse a connect. 
      if( server.arg("Connected").equalsIgnoreCase( "true" ) ) 
      { //setting to true 
        if ( connected )//already true
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" /*"Setting connected when already connected"*/ );        
          root["Value"]= true;    
          root.prettyPrintTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.printf( "handleConnected: output:%s\n", message.c_str() );
#endif
          server.send( 200, "application/json", message ); 
          return;
        }
        else //OK
        {
           connected = true;
           root["Value"] = true; 
        }
      }
      else //'Connected' = false
      {
        if ( connected ) //
        {
          //If we are disconnecting, we should park or home and close the shutter ?
#ifdef _ASCOM_Dome
          if( parkDomeOnDisconnect )
              addDomeCmd( 0, 0, CMD_DOME_PARK, 0);
          if( closeShutterOnDisconnect )
              addDomeCmd( 0, 0, CMD_SHUTTER_CLOSE, 0 );
#endif 
          connected = false; //OK          
          root.remove( "Value" );
        }
        else
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "Clearing 'connected' when already cleared" );        
          root.remove( "Value" );
          root.printTo(message);   
          server.send( 200, "application/json", message);
          return;          
        }
      }
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );        
      root.remove( "Value" );
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", Success, "" );        
      root["Value"] = connected;      
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
    else
    {
      jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "Connected", invalidOperation , "Unexpected HTTP method" );        
      root["Value"] = connected;      
      root.printTo(message);
      server.send( 200, "application/json", message);
      return;          
    }
}

void handleDescriptionGet(void)
{
    String message;
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
    server.send(200, "application/json", message);

    return ;
}

void handleDriverInfoGet(void)
{
    String message;
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
    
    server.send(200, "application/json", message);
    return ;
}

void handleDriverVersionGet(void)
{
    String message;
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

    server.send(200, "application/json", message);
    return ;
}

void handleInterfaceVersionGet(void)
{
    String message;
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

    server.send(200, "application/json", message);
    return ;
}

void handleNameGet(void)
{
    String message;
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
    server.send(200, "application/json", message);
    return ;
}

void handleSupportedActionsGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "SupportedActions", Success, "" );    
    JsonArray& values  = root.createNestedArray("Value");   
    values.add(""); //Explicitly empty array
    root.printTo(message);
    server.send(200, "application/json", message);
    return ;
}

void handleAPIversions(void)
{
    String message;
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
    
    server.send(200, "application/json", message);
    return ;
}

void handleAPIconfiguredDevices(void)
{
    String message;
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
    
    server.send(200, "application/json", message);
    return ;
}



#endif
  
