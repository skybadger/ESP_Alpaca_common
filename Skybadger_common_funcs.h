#if !defined _SKYBADGER_COMMON_FUNCS_H
#define _SKYBADGER_COMMON_FUNCS_H
/*
   Helper functions
*/

/*
@brief Function to scan the wifi channels for SSIDS and strengths, prits output to default serial terminal
*/
void scanNet(void)
{
  int i;
  int n;
  WiFi.disconnect();
  n = WiFi.scanNetworks(false, true);
  if (n == 0) 
  {
    Serial.println( F("no networks found") );
  } 
  else 
  {
    Serial.println( F(" networks found ") );
    for ( i = 0; i < n; ++i)
    {
      Serial.printf( "ID: %s, Strength: %i \n", WiFi.SSID(i).c_str(), WiFi.RSSI(i) );
    }
  }
}

/*
int stricmp(char* a, char*b)
{
   int i; 
   int diff = 0;
   int sLen = min( strlen( a), strlen(b) );
   
   for ( i=0;i < sLen; i++ )
   {
      diff += (byte) tolower(a[i]) - (byte) tolower(b[i]);
   }
   return diff;
}   
*/

//Wrapper for web server args handling for URL query arguments that differ by case. The ALPACA API allows this!
/*
* Pass in the string you want to check exists. 
* modifies the string to the case-insensitive version found. 
* This doesn't work for urls which may be mixed-case. 
*/
bool hasArgIC( String& check, ESP8266WebServer& ref, bool caseSensitive )
{
 String matchName;
 if ( !caseSensitive )
 {
   int max = ref.args();
   for ( int i = 0; i < max; i++ )
   {
     if( ref.argName(i).equalsIgnoreCase( check ) )  
     {
       check = ref.argName(i);          
       return true;
     }
   }
   return false;
 }
 else
   return ( ref.hasArg( check ) );
}

/*
@brief function to read i2c bus and identify responding devices - output as string summary
*/
String scanI2CBus()
  {
  int nDevices = 0;
  int address = 0;
  int error = 0;
  String output = "";
  
  for(address = 2; address < 256; address+=2 )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission( address );
    delay(2);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print( F("I2C device found at address (dec)") );
      if (address<16)
        Serial.print("0");
      Serial.print(address,DEC);
      Serial.println("  !");
      output.concat( F("I2C device found at address (dec)" ));
      output.concat( address ) ;
      output.concat( "\n" );
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print( F("Unknown error at address 0x") ) ;
      if (address<16)
        Serial.print("0");
      Serial.println(address ,HEX);
    }    
  }
  if (nDevices == 0)
  {
    Serial.println( F("No I2C devices \n") );
    output = String( F("No I2C devices found\n") );
  }
  else
    Serial.println( F("scanI2CBus done\n") );
  
  return output;
  }


//bool setupMQTT( char* thisId, char* user, char* passwd, char* server );
/*
  @brief Connect to MQTT and subscribe.. 
  MQTT
  Returns the current state of the client. If a connection attempt fails, this can be used to get more information about the failure.
  int - the client state, which can take the following values (constants defined in PubSubClient.h):
  -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
  -3 : MQTT_CONNECTION_LOST - the network connection was broken
  -2 : MQTT_CONNECT_FAILED - the network connection failed
  -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
  0 : MQTT_CONNECTED - the client is connected
  1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
  2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
  3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
  4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
  5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
 Dependencies - 
  volatile unsigned int timeoutFlag ;
  etsTimer timeoutTimer
  ets timer handler 

*/
void reconnectNB() 
{
  //Non-blocking version
   if ( timerSet ) //timer is running but not timed out. 
   {
     if( timeoutFlag ) //Timeout - try again
     {   
         DEBUG_MQTT(F("Repeating MQTT connection attempt..."));
         if ( !client.connect( thisID, String(pubsubUserID).c_str(), String(pubsubUserPwd).c_str() ) ) //PROGMEM
         {  //Set a one-off timer to try next time around. 
            DEBUG_MQTT( F("connect failed, rc="), client.state() );
            Serial.println( client.state() );
            timeoutFlag = false;
            timerSet = true;
            ets_timer_arm_new( &timeoutTimer, 5000, 0/*one-shot*/, 1);           
         }
         else
         { //Stop - all connected again
            timerSet = false;
         }
      }
   }
   else //timer not set 
   {
     Serial.print( F("Attempting MQTT connection..."));
     if ( !client.connect( thisID, String(pubsubUserID).c_str(), String(pubsubUserPwd ).c_str() ) ) //PROGMEM
     {  
        DEBUG_MQTT(F("connect failed, rc=%i"), client.state() );
        //Serial.println( client.state());

        //Set a one-off timer to try next time around. 
        timeoutFlag = false;
        timerSet = true;
        ets_timer_arm_new( &timeoutTimer, 5000, 0/*one-shot*/, 1);           
     }
     else
     {
        //publishHealth();
        client.subscribe( String(inTopic).c_str(), 1); //PROGMEM
        DEBUG_MQTT(F("MQTT connection regained.") );
     }
   }
return;
}

void reconnect( void )
{
  String output;
  DEBUG_MQTT( F("Entering reconnect" ) );
  // Loop until we're reconnected
  while ( !client.connected() )
  {
    DEBUG_MQTT(F("Attempting sync MQTT connection..."));
    // Attempt to connect
    if (client.connect( thisID, String(pubsubUserID).c_str(), String(pubsubUserPwd ).c_str() ) )//PROGMEM
    {
      //publish to our device topic(s)
      DEBUG_MQTT( F("connected") );
      // Resubscribe
      client.subscribe( String(inTopic).c_str(), 1 ); //PROGMEM
    }
    else
    {
      Serial.printf_P( PSTR("failed, rc= %i, try again in 5 seconds \r\n"), client.state() );
//      // Wait 5 seconds before retrying
//      for (int i = 0; i < 5000; i++)
//      {
//        delay(1);
//        //delay(20);
//        //yield();
//      }
    }
  }
  DEBUG_MQTT( F("Actually connected") );
  DEBUG_MQTT( F("Leaving reconnect") );
}

//Creating timestrings 
void syncTime()
{
  //The reason to do this is that we timestamp the data with the network time. 
  //However the network time is only recorded to the second since 1970 on the ESP8266
  //
  struct timeval tv;
  gettimeofday( &tv, nullptr );  
  
  struct timezone tzone;
  tzone.tz_minuteswest = 0;
  tzone.tz_dsttime = DST_MN;
  
  //Look for the boundary change of second
  time_t now, last;
  now = time(nullptr);
  struct tm* gnow = gmtime( &now );
  tv.tv_sec = (((gnow->tm_hour*60) + gnow->tm_min )*60) + gnow->tm_sec;
  last = now;
  while( ( now - last ) == 0 )
  {
      last = now;
      now = time( nullptr );
  }

  //As soon as gmtime seconds change - update the system clock usec counter.
  tv.tv_usec = 0;
  settimeofday( &tv, &tzone );
}
 
String& getTimeAsString2(String& output)
{
   //relies on the system clock being synchronised with the sntp clock at the ms level. 
   char buf[64];
   //Get seconds since 1970, mix in the milliseconds
   now = time( nullptr );
   sprintf( buf, "%lu%03i", now, (int)(millis() % 1000 ) );
   output = String( buf);
   return output;
}

String& getTimeAsString(String& output)
{
  /* ISO Dates (Date-Time)
     ISO dates can be written with added hours, minutes, and seconds (YYYY-MM-DDTHH:MM:SSZ)
     Z indicates Zulu or GMT time,
     For other timezones use +/-HH:MM instead of Z
     Can replace 'T' with ' ' - works in javascript nicely.
  */
  struct timeval tv;
  gettimeofday( &tv, nullptr );
  //double milliTimestamp = ((double) (tv.tv_sec))*1000.0 + tv.tv_usec/1000.0;
  //Serial.println( tv.tv_sec );//works in seconds - get 10 digits
  //Serial.println( tv.tv_usec/1000 );//works in usecs - get 5 digits
  //Serial.println( milliTimestamp, 3 ); //Doesnt work when you put them together - overflows

  //get time, maintained by NTP, but only to the second.
  //Hence we'd like to use the system time in ms from 1970 - which is what the js date parser would like too, but esp cant provide
  //an integer long enough it seems.
  now = time(nullptr);
  struct tm* gnow = gmtime( &now );
  //output += String(gnow->tm_year + 1900) + "-" + \
  String(gnow->tm_mon ) + "-" + \
  String(gnow->tm_mday ) + " " + \
  String(gnow->tm_hour) + ":" + \
  String(gnow->tm_min) + ":" + \
  String( (float) gnow->tm_sec + tv.tv_usec / 1000000.0 ) + "Z";

  //This works but its heavy for an embedded device.
  char buf[128];
  sprintf( buf, "%4i-%02i-%02i %02i:%02i:%02i.%03iZ", gnow->tm_year + 1900, gnow->tm_mon + 1, gnow->tm_mday + 1, gnow->tm_hour, gnow->tm_min, gnow->tm_sec, (int) ( tv.tv_usec / 1000000.0F) );

  //String and Serial classes can't handle 'long long' - missing SerialHelper overload
  output = String(buf);
  //Serial.print("timestamp value:");Serial.println( output );
  return output;
}
#endif 