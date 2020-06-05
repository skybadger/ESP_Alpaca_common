//File to define some simple serial debug macros. 

#ifndef _DEBUG_SERIAL_H
#define _DEBUG_SERIAL_H

#define _DEBUG
#ifdef _DEBUG
#define DEBUGS1(a)    Serial.print(a)
#define DEBUGS2(a,b)  Serial.print(a, b)
#define DEBUGSL1(a)   Serial.println(a)
#define DEBUGSL2(a,b) Serial.println(a, b)
#else
#define DEBUGS1 
#define DEBUGS2 
#define DEBUGSL1
#define DEBUGSL2 
#endif

#endif