#ifndef SimpleKDS_h
#define SimpleKDS_h
#include "Arduino.h"


// Response states
#define RES_SUCCESS 0
#define RES_TIMEOUT 1
#define RES_BUSY 2

// Negative response codes
#define generalReject 0x10
#define serviceNotSupported 0x11
#define subFunctionNotSupportedInvalidFormat 0x12
#define busyRepeatRequest 0x21
#define conditionsNotCorrectOrRequestSequenceError 0x22
#define routineNotCompleteOrServiceInProgress 0x23
#define requestOutOfRange 0x31
#define securityAccessDeniedSecurityAccessRequested 0x33
#define invalidKey 0x35
#define exceedNumberofAttempts 0x36
#define requiredTimeDelayNotExpired 0x37
#define downloadNotAccepted 0x40
#define improperDownloadType 0x41
#define canNotDownloadToSpecifiedAddress 0x42
#define canNotDownloadNumberOfBytesRequested 0x43
#define uploadNotAccepted 0x50
#define improperUploadType 0x51
#define canNotUploadFromSpecifiedAddress 0x52
#define canNotUploadNumberOfBytesRequested 0x53
#define transferSuspended 0x71
#define transferAborted 0x72
#define illegalAddressInBlockTransfer 0x74
#define illegalByteCountInBlockTransfer 0x75
#define illegalBlockTransferType 0x76
#define blockTransferDataChecksumError 0x77
#define requestCorrectlyReceivedResponsePending 0x78
#define incorrectByteCountDuringBlockTransfer 0x79
#define serviceNotSupportedInActiveDiagnosticSession 0x80




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
