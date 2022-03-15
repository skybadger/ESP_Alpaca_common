//File to define some simple serial debug macros. 

#ifndef _DEBUG_SERIAL_H
#define _DEBUG_SERIAL_H

//#define DEBUG_ESP
#if defined DEBUG_ESP_MH
#define DEBUGS1(a)    Serial.print(a)
#define DEBUGS2(a,b)  Serial.print(a, b)
#define DEBUGSL1(a)   Serial.println(a)
#define DEBUGSL2(a,b) Serial.println(a, b)
#define DEBUG_ESP(...) Serial.printf( __VA_ARGS__ )
#define DEBUG_ESPP( ...) Serial.printf_P( PSTR(fmt) )
#define DEBUG_ESPP( fmt, ...) Serial.printf_P( PSTR(fmt), __VA_ARGS__ )
#else
#define DEBUGS1(a) 
#define DEBUGS2(a,b) 
#define DEBUGSL1(a)
#define DEBUGSL2(a,b) 
#define DEBUG_ESP(...) 
#define DEBUG_ESPP( fmt, ...)
#endif

//These are ordinarily defined in the remote debug output macros but need redirecting to Serial if remote debugging is disabled but debugging output is still required. 
#if defined DEBUG_DISABLED
#define debugE( fmt, ... ) Serial.printf( fmt, __VA_ARGS__ )
#define debugW( fmt, ... ) Serial.printf( fmt, __VA_ARGS__ )
#define debugI( fmt, ... ) Serial.printf( fmt, __VA_ARGS__ )
#define debugD( fmt, ... ) Serial.printf( fmt, __VA_ARGS__ )
#define debugV( fmt, ... ) Serial.printf( fmt, __VA_ARGS__ )
#endif 

#if defined DEBUG_MQTT
#define DEBUG_MQTT(...) Serial.printf( __VA_ARGS__ )  
#else
#define DEBUG_MQTT(...) 
#endif 

#if defined DEBUG_HW
#define DEBUG_HW(...) Serial.printf( __VA_ARGS__ )  
#else
#define DEBUG_HW(...) 
#endif 

#endif