/*
File to be included into relevant device REST setup 
*/
//Assumes Use of ARDUINO ESP8266WebServer for entry handlers
#if !defined _ASCOMAPI_Common
#define _ASCOMAPI_Common

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

void handleAction(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();
    JsonObject& root = jsonBuffer.createObject();
    
    jsonResponseBuilder( root, clientID, transID, "Action", 0x400, "Not implemented" );
    root["Value"]= "";
    root.printTo(message);
    server.send(200, "text/json", message);
    return;
 }

void handleCommandBlind(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();
        
    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "Action", 0x400, "Not implemented" );
    root["Value"]= "";
    root.printTo(message);
    server.send(200, "text/json", message);
    return;
}

void handleCommandBool(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();
    
    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "CommandBool", 0x400, "Not implemented" );
    root["Value"]= false; 
    root.printTo(message);   
    server.send(200, "text/json", message);
    return;
}

void handleCommandString(void)
{
    String message;
    JsonObject& root = jsonBuffer.createObject();
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();
    jsonResponseBuilder( root, clientID, transID, "CommandBool", 0x400, "Not implemented" );
    root["Value"]= false; 
    root.printTo(message);   
    server.send(200, "text/json", message);
    return;
}

void handleConnected(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();
    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "Connected", 0, "" );
    
    if ( server.method() == HTTP_PUT )
    { 
      //dont like the logic here - if iits already connected for this client we should refuse a connect. 
      if( server.arg("Connected") == "true" ) 
      { //setting to true 
        if ( connected )//already true
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, transID, "Connected", 0x403, "Setting connected when already connected" );        
          root["Value"]= false;    
          root.printTo(message);
          server.send( 400, "text/json", message);
          return;
        }
        else //OK
          connected = true;
      }
      else 
      {
        if ( connected ) //
        {
          connected = false; //OK          
        }
        else
        {
          //Check error numbers
          jsonResponseBuilder( root, clientID, transID, "Connected", 0x403, "Clearing 'connected' when aready cleared" );        
          root["Value"]= false; 
          root.printTo(message);   
          server.send( 400, "text/json", message);
          return;          
        }
      }
      jsonResponseBuilder( root, clientID, transID, "Connected", 0, "" );        
      root.printTo(message);
      server.send( 200, "text/json", message);
      return;          
    }
    else if ( server.method() == HTTP_GET )
    {
      //Check error numbers
      jsonResponseBuilder( root, clientID, transID, "Connected", 0, "" );        
      root["Value"]= connected;      
      root.printTo(message);
      server.send( 200, "text/json", message);
      return;          
    }
    else
    {
      jsonResponseBuilder( root, clientID, transID, "Connected", 400, "Unexpected method" );        
      root["Value"]= connected;      
      root.printTo(message);
      server.send( 200, "text/json", message);
      return;          
    }
}

void handleDescriptionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();

    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "Description", 0, "" );    
    root["Value"]= Description;    
    root.printTo(message);
    server.send(200, "text/json", message);
    return ;
}

void handleDriverInfoGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();

    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "DriverInfo", 0, "" );    
    root["Value"]= Description;    
    root.printTo(message);
    server.send(200, "text/json", message);
    return ;
}

void handleDriverVersionGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();

    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "DriverVersion", 0, "" );    
    root["Value"]= DriverVersion;    
    root.printTo(message);
    server.send(200, "text/json", message);
    return ;
}

void handleNameGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();

    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "Name", 0, "" );    
    root["Value"] = DriverName;    
    root.printTo(message);
    server.send(200, "text/json", message);
    return ;
}

void handleSupportedActionsGet(void)
{
    String message;
    uint32_t clientID = (uint32_t)server.arg("ClientID").toInt();
    uint32_t transID = (uint32_t)server.arg("ClientTransactionID").toInt();

    JsonObject& root = jsonBuffer.createObject();
    jsonResponseBuilder( root, clientID, transID, "SupportedActions", 0, "" );    
    root["Value"]= "";    //Empty array
    root.printTo(message);
    server.send(200, "text/json", message);
    return ;
}

//Non-ASCOM functions
//JSON error structures
//https://ascom-standards.org/api/?urls.primaryName=Remote%20Management%20API
//Responses, as described below, are returned in JSON format and always include a common set of values 
//including the client's transaction number, the server's transaction number together with any error message 
//and error number. If the transaction completes successfully, 
//the ErrorMessage field will be an empty string and the ErrorNumber field will be zero.
//https://github.com/ASCOMInitiative/ASCOMRemote/blob/master/Documentation/ASCOM%20Alpaca%20API%20Reference.pdf
//HTTP Status Codes and ASCOM Error codes
void jsonResponseBuilder( JsonObject& root, unsigned int clientID, unsigned int transID, String methodName, int errNum , String errMsg )
{
//ClientTransactionIDForm  unsigned integer($int32) Client's transaction ID.
//ServerTransactionID unsigned integer($int32) Server's transaction ID.
//Method  string Name of the calling method.
//ErrorNumber integer($int32) Error number from device.
//ErrorMessage  string Error message description from device.
//DriverException {...}
   
    root["ClientTransactionID"]= clientID;
    root["ServerTransactionID"]= transID;
    root["Method"]= methodName;
    root["ErrorNumber"]= errNum;
    JsonObject& err = root.createNestedObject("ErrorMessage");
    err["Value"] = errMsg;
}

#endif
  