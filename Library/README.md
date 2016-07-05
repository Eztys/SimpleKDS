The library contains the following functions:

**SimpleKDS()**

Constructor that sets the following parameters:<br />
- Uses digital pin 0 and 1 for serial communication (hardware serial)<br />
- 10400 baudrate for KDS<br />
- 1500 ms timeout for responses<br />

**void setTiming(byte P1, byte P2, byte P3, byte P4)**

Function to set the timing parameters<br />
P1 = interbyteRequestdelay (default 5 ms)<br />
P2 = interByteResponsedelay (default 0 ms)<br />
P3 = interRequestResponsedelay (default 0 ms)<br />
P4 = interResponseRequestdelay (default 55 ms)<br />

Please read the ISO-14230 document to learn more about the timing settings. <br/>

**void setAddresses(byte ECU, byte source)**

This function allows the user to set the address of the source (default 0xF1) and ECU (default 0x11). <br>

**void sendRequest(byte *reqbuf, byte len)**

Sends *len* bytes stored in *reqbuf* byte array.

**byte getResponse(byte *resbuf)**

A function that receives the response message in a non-blocking manner. One byte is read per function call (if available) and stored in the *resbuf* byte array.<br><br>
Returns:<br>
*RES_BUSY* when not all bytes have been received i.e. continue calling the function<br>
*RES_SUCCESS* when the entire response message is received i.e. start parsing the response message<br>
*RES_TIMEOUT* when response was not received within timeout set in constructure (default 1500 ms)<br>

**bool initECU()**

This function initiates the ECU communication with the fastinit method. Returns *true* on success<br />
	
