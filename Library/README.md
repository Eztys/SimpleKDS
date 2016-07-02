The library contains the following functions:

**SimpleKDS()**

This is the basic constructor that sets the following parameters:<br />
- Uses digital pin 0 and 1 for serial communication (hardware serial)<br />
- 10400 baudrate for KDS<br />
- 1500 ms timeout for responses<br />

**void setTiming(byte P1, byte P2, byte P3, byte P4)**

Function to set the timing parameters<br />
P1 = interbyteRequestdelay (default 5 ms)<br />
P2 = interByteResponsedelay (default 0 ms)<br />
P3 = interRequestResponsedelay (default 0 ms)<br />
P4 = interResponseRequestdelay (default 55 ms)<br />

**void sendRequest(byte *reqbuf, byte len)**

Sends "len" bytes contained in byte array reqbuf.

**byte getResponse(byte *resbuf)**

A function that receives the response message in a non-blocking manner, such that on every call the next byte is read (if available).<br />
Returns:<br />
RES_BUSY when not all bytes have been received <br />
RES_SUCCESS when the entire response message is received<br />
RES_TIMEOUT when the there is a timeout > 1500 ms<br />

**bool initECU()**

This function initiates the ECU communication with the fastinit method. Returns true or false<br />
	
