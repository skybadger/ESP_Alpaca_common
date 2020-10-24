#if !defined _JSONHelperFunctions
#define _JSONHelperFunctions
#include <ArduinoJson.h>

//#include <ESP8266WebServer.h>
//extern ESP8266WebServer server;

String& debugURI(String& message);
void jsonResponseBuilder( JsonObject&, uint32_t clientTransID, uint32_t transID, String methodName, int errNum , String errMsg );

//JSON error structures used in ASCOM REST calls
//https://ascom-standards.org/api/?urls.primaryName=Remote%20Management%20API
//Responses, as described below, are returned in JSON format and always include a common set of values 
//including the client's transaction number, the server's transaction number together with any error message 
//and error number. If the transaction completes successfully, 
//the ErrorMessage field will be an empty string and the ErrorNumber field will be zero.
//https://github.com/ASCOMInitiative/ASCOMRemote/blob/master/Documentation/ASCOM%20Alpaca%20API%20Reference.pdf
//HTTP Status Codes and ASCOM Error codes
void jsonResponseBuilder( JsonObject& root, unsigned int clientID, unsigned int clientTransID, unsigned int serverTransID, String methodName, int errNum , String errMsg )
{
//ClientTransactionIDForm  unsigned integer($int32) Client's transaction ID.
//ServerTransactionID unsigned integer($int32) Server's transaction ID.
//Method  string Name of the calling method.
//ErrorNumber integer($int32) Error number from device.
//ErrorMessage  string Error message description from device.
//DriverException {...}
   
    root["Value"] = 0;
    root["ClientID"]= clientID;
    root["ClientTransactionID"]= clientTransID;
    root["ServerTransactionID"]= serverTransID;
    root["Method"] = methodName;
    root["ErrorNumber"] = errNum;
    root["ErrorMessage"] = errMsg;
}

String& debugURI( String& message)
  {
   boolean listArgs= false;
    
    message += "URI: ";
    message.concat( server.uri() );
    message += "\nMethod: ";
    switch (server.method())
    {
      case HTTP_GET:  message += "GET"; 
          listArgs= true;
          break;

      case HTTP_POST: message += "POST"; 
          listArgs = true;
          break;
      case HTTP_PUT:  message += "PUT"; 
          listArgs = true;
          break;
      case HTTP_PATCH:  message += "PATCH"; break;
      case HTTP_DELETE:  message += "DELETE"; break;
      case HTTP_OPTIONS: message += "OPTIONS"; break;
      default: break;
    }
    if ( listArgs )
    {              
      message += "\nArguments: ";
      message.concat( server.args() );
      for (uint8_t i = 0; i < server.args(); i++)
      {
        message += " ";
        message.concat( server.argName(i));
        message += ":";
        message.concat( server.arg(i) );
        message += "\n";
      }
    }
  return message;
  }
  
#endif
