#ifndef SimpleKDS_h
#define SimpleKDS_h
#include "Arduino.h"

// Response states
#define RES_SUCCESS 0
#define RES_TIMEOUT 1
#define RES_BUSY 2

class SimpleKDS
{
	public:
	
	SimpleKDS();
	SimpleKDS(int baudrate);
	~SimpleKDS();
	bool initECU();
	void sendRequest(byte *reqbuf, byte len);
	byte getResponse(byte *resbuf);
	void setTiming(byte P1, byte P2, byte P3, byte P4);
	void setAddresses(byte ECU, byte source);
	
	private:
	
	byte K_IN;
	byte K_OUT;
	uint16_t BAUDRATE;
	uint16_t TIMEOUT;
	byte interByteReqDelay;
	byte interByteResDelay;
	byte interReqResDelay;
	byte interResReqDelay;
	byte ECUAddress;
	byte sourceAddress;
	
};



#endif
