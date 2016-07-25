#ifndef SimpleKDS_h
#define SimpleKDS_h
#include "Arduino.h"


// Response states
#define SUCCESS 0
#define TIMEOUT 1
#define BUSY 2
#define BUFFEROVERFLOW 3

class SimpleKDS
{
	public:
	
	SimpleKDS(int resbufsize);
	~SimpleKDS();
	bool initECU();
	void sendRequest(byte *reqbuf, byte len);
	byte getResponse(byte *resbuf);
	void setTiming(byte P1, byte P2, byte P3, byte P4);
	void setAddresses(byte ECU, byte Source);
	void setTimeout(int t);
	
	private:
	
	byte K_IN;
	byte K_OUT;
	uint16_t BAUDRATE;
	uint16_t responseTimeout;
	byte interByteReqDelay;
	byte interByteResDelay;
	byte interReqResDelay;
	byte interResReqDelay;
	byte ECUAddress;
	byte sourceAddress;
	uint16_t responseBufferSize;
	
};



#endif
