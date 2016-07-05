# SimpleKDS

The Kawasaki Diagnostic System (KDS) is a system which is utilized to communicate with a Kawasaki motorcycle. The KDS uses serial communication which is described by the ISO-14230 protocol. Via the KDS protocol a multitude of data can be requested from the motorcycle, for instance the sensor measurements can be requested (RPM, speed, battery voltage, engine coolent temperature) but also the diagnostic trouble codes (DTC). A great deal of effort has been done by members of the [ECUhacking forum](http://ecuhacking.activeboard.com/t56234221/kds-protocol/) on decoding the KDS.

Several developers have worked on a datalogging function based on an Arduino (e.g. [HerrRiebmann](https://github.com/HerrRiebmann/KDS2Bluetooth/blob/master/README.md) with KDS2Bluetooth and [tomnz](https://bitbucket.org/tomnz/kawaduino/overview) with Kawaduino), however a basic KDS library has not yet been made public to the best of my knowledge. 

Therefore I introduce SimpleKDS which is an Arduino library for the KDS protocol. SimpleKDS offers a simple method to send requests of which the request is received in a non-blocking manner. Due to the simplicity of the library it can be used in many applications like datalogging or custom gauges. 
