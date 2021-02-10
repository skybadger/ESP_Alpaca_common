#ifndef __ALPACA_MGMT_H__
#define __ALPACA_MGMT_H__

//External dependencies
extern WiFiUDP Udp;
extern int udpPort; //Define this in your ASCOM descriptor code - should be a variable because it's alterable. 
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "SkybadgerStrings.h"

//Declarations of local prototypes
//UDP discovery service responder struct.
struct DiscoveryPacket
 {
  const char* protocol = "alpacadiscovery" ;
  byte version = 1; //1-9, A-Z
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
    strncpy( protocol, (char*) inBytes, 16);
	protocol[16] = '\0';
	
    if ( strcasecmp( discoveryPacket.protocol, protocol ) == 0 )
    {
      Udp.beginPacket( Udp.remoteIP(), Udp.remotePort() );
      //Respond with discovery message
	  root["IPAddress"] = WiFi.localIP().toString().c_str();
      root["Type"] = DriverType;
      root["AlpacaPort"] = 80;
      root["Name"] = WiFi.hostname();
      root["UniqueID"] = system_get_chip_id();
      root.printTo( message );
      Udp.write( message.c_str(), sizeof( message.c_str() ) * sizeof(char) );
      Udp.endPacket();   
    }
 }

//Management API for ALPACA devices
void handleMgmtDescription(void)
{
    String message;
    //Dont care about client IDs for read-only data 
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t clientTransID = (uint32_t)server.arg("ClientTransactionID").toInt();

    DynamicJsonBuffer jsonBuff(256);
    JsonObject& root = jsonBuff.createObject();
    jsonResponseBuilder( root, clientID, clientTransID, ++serverTransID, "MgmtDescription", Success, "" );  
    
	JsonArray& values  = root.createNestedArray("Value");    
	//Ref: https://github.com/bblanchon/ArduinoJson/wiki/Examples
	JsonObject& object = values.createNestedObject(); 
    object["ServerName"]   = WiFi.hostname();
	object["Manufacturer"] = Manufacturer;
	object["ManufacturerVersion"] = instanceVersion;
	object["Location"] = Location; //This needs to be configurable as part of setup so we know where to look for the device after installation.
	
	root.printTo( message);
	
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
    values.add(atoi(InterfaceVersion.c_str()));    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    
    server.send(200, F("application/json"), message);
    return ;
}

void handleMgmtConfiguredDevices(void)
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
    device["DeviceName"] = Description;
    device["DeviceType"] = DriverName;
    device["DeviceNumber"] = instanceNumber;
    device["UniqueID"] = GUID;    

    values.add(device);    
    root.printTo(message);
#ifdef DEBUG_ESP_HTTP_SERVER
DEBUG_OUTPUT.println( message );
#endif
    
    server.send(200, F("application/json"), message);
    return ;
}
#endif //__ALPACA_MGMT_H__