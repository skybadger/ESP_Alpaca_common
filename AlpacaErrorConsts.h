#if ! defined _ALPACA_ERR_CONSTS_H_
#define _ALPACA_ERR_CONSTS_H_

/*
All five elements of the API path are case sensitive and must always be in lower case. For example, this is the only valid casing for a call to the Telescope.CanSlew property:
/api/v1/telescope/0/canslew

Alpaca parameters are key-value pairs where:
•	The parameter key is case insensitive
•	The parameter value can have any casing required.
This is the same behaviour as defined for HTTP header keys in RFC7230.
For example, these are all valid API parameters:
/api/v1/telescope/0/canslew?clientid=0&clienttransactionid=23
/api/v1/telescope/0/canslew?ClientID=0&ClientTransactionID=23
/api/v1/telescope/0/canslew?CLIENTID=0&CLIENTTRANSACTIONID=23
Clients and drivers must expect incoming API parameter keys to have any casing.
*/

/*
Condition	                        Alpaca Error Number	
Successful transaction	            0x0 
Property or method not implemented	0x400 
Invalid value	                     0x401 
Value not set	                     0x402 
Not connected	                     0x407 
Invalid while parked	               0x408 
Invalid while slaved	               0x409 
Invalid operation	                  0x40B 
Action not implemented	            0x40C 
*/
enum alpacaErrors {  Success = 0x0, 
                     notImplemented = 0x400, 
                     invalidValue = 0x401, 
                     valueNotSet = 0x402, 
                     notConnected = 0x407, 
                     invalidWhileParked = 0x408, 
                     invalidWhileSlaved = 0x409, 
                     invalidOperation = 0x40B, 
                     actionNotImplemented = 0x40C };

#endif
