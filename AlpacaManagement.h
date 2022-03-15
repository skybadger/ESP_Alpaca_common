#ifndef __ALPACA_MGMT_H__
#define __ALPACA_MGMT_H__

#ifndef MAX_NAME_LENGTH 
#define MAX_NAME_LENGTH 25
#endif 

/*
Assumptions 
- only supports one device instance per build
*/
//Reference code: https://github.com/DanielVanNoord/AlpacaDiscoveryTests/blob/master/Arduino/ESP8266/ESP8266.ino

//External dependencies
extern WiFiUDP Udp;
extern int udpPort; //Define this in your ASCOM descriptor code - should be a variable because it's alterable. 
extern char* Location; 
extern char* myHostname; 

// https://github.com/ASCOMInitiative/ASCOMRemote/blob/master/Documentation/ASCOM%20Alpaca%20API%20Reference.pdf
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "SkybadgerStrings.h"

//Declarations of local prototypes
//UDP discovery service responder struct.
struct DiscoveryPacket
 {
  const char protocol[15] = {'a','l','p','a','c','a','d','i','s','c','o','v','e','r','y'};
  byte version = '1'; //1-9, A-Z
  byte reserved[48];
 }; 
 
void handleManagement();
void handleDiscovery( int udpBytesCount );
void handleMgmtDescription(void);
void handleMgmtConfiguredDevices(void);
void handleMgmtVersions(void);


//Add this to your processing loop function
void handleManagement()
{
 //check for data available
  int udpBytesIn = Udp.parsePacket();
  if( udpBytesIn > 0  ) 
    handleDiscovery( udpBytesIn );
}

//Function to handle detection of a UDP inbound query 
void handleDiscovery( int udpBytesCount )
{
    String message;
    DiscoveryPacket discoveryPacket;
 
    byte inBytes[256];
	DynamicJsonBuffer jsonBuffer(256);
    JsonObject& root = jsonBuffer.createObject();
    
    debugV("UDP: %i bytes received from %s:%i\n", udpBytesCount, Udp.remoteIP().toString().c_str(), Udp.remotePort() );

    // We've received a packet, read the data from it
    Udp.read( inBytes, udpBytesCount); // read the packet into the buffer

    // display the packet contents
    for (int i = 0; i < udpBytesCount; i++ )
    {
      Serial.print( inBytes[i]);
      if (i % 32 == 0)
      {
        Serial.println();
      }
      else Serial.print(' ');
    } // end for
    Serial.println();
   
    //Is it for us ?
    char protocol[17];
    strncpy( protocol, (char*) inBytes, 15);
	protocol[15] = '1';
	protocol[16] = '\0';
	
    debugD( "Searching for %s in discovery string in UDP bytes ", protocol );
	if(strncmp( discoveryPacket.protocol, (char *)protocol, 14) == 0)
    {
      debugI( "%s", "UDP discovery string match");
	  Udp.beginPacket( Udp.remoteIP(), Udp.remotePort() );
      //Respond with discovery message
	  root["AlpacaPort"] = 80;
      root.printTo( message );
	  Udp.write( message.c_str(), strlen( message.c_str() ) * sizeof(char) );
      Udp.endPacket();   
	  debugD( "UDP return message: %s", message.c_str() );
    }
 }

//Management API for ALPACA devices - https://www.ascom-standards.org/api/?urls.primaryName=ASCOM%20Alpaca%20Management%20API#/Management%20Interface%20(JSON)
//Function to respond to management queries - description
void handleMgmtDescription(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "MgmtDescription", Success, "" );  
    //root.remove["ClientID"];"
	//root.remove["ErrorNumber"];"
	//root.remove["ErrorMessage"];"
	
	//JsonArray& values  = root.createNestedArray("Value");    
	//Ref: https://github.com/bblanchon/ArduinoJson/wiki/Examples
	JsonObject& object = root.createNestedObject("Value"); 
    object["ServerName"]   = WiFi.hostname();
	object["Manufacturer"] = Manufacturer;
	object["ManufacturerVersion"] = instanceVersion;
	object["Location"] = Location; //TODO This needs to be configurable as part of setup so we know where to look for the device after installation.
	
	root.printTo( message);
	DEBUGS1( F("ManagementDescription response : ") ); DEBUGSL1( message.c_str() );
    server.send(200, F("application/json"), message);
    return ;
}

void handleMgmtVersions(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIversions", Success, "" );  
    //Expect to return an array of integer values.
	JsonArray& values  = root.createNestedArray("Value");     
    values.add( atoi( String(InterfaceVersion).c_str() ));  //Do this because InterfaceVersion is stored in PROGMEM
    root.printTo(message);
	DEBUGS1( F("ManagementVersions response : ") ); DEBUGSL1( message.c_str() );    
    server.send(200, F("application/json"), message);
    return ;
	//Note: Must return test/plain error message in case of returning 400 or 500 error code. 
}

void handleMgmtConfiguredDevices(void)
{
    //TODO - this assumes only one device per hardware instance is supported. To increase, need to update....
	String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "APIversions", Success, "" );  
    JsonArray& values  = root.createNestedArray("Value");
    
    //Need an embedded array entry per registered device.  Default 1 only implemented. 
	DynamicJsonBuffer jsonBuff2(128);
    JsonObject& device= jsonBuff2.createObject();
    device["DeviceName"] = Description;
    device["DeviceType"] = DriverType;
    device["DeviceNumber"] = instanceNumber;
    device["UniqueID"] = GUID;    

    values.add(device);    
    root.printTo(message);
	DEBUGS1( F("ManagementConfiguredDevices response : ") ); DEBUGSL1( message.c_str() );    
    server.send(200, F("application/json"), message);
    return ;
	//Note: Must return test/plain error message in case of returning 400 or 500 error code. 
}

 /*
  * Handler to update the hostname from the form.
  */
 


#endif //__ALPACA_MGMT_H__
