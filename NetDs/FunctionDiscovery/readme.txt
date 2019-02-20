__About Function Discovery__

Function Discovery provides a uniform programmatic interface for enumerating system resources, such as hardware devices, whether they are local or connected through a network. It enables applications to discover and manage lists of devices or objects sorted by functionality or class.  Function Discovery supports an extensible discovery provider model. The providers allow FD to offer an abstraction layer over existing standards such as Plug and Play (PnP), SSDP, WS-Discovery, and the registry.  For a complete listing of in-box FD providers, look at FunctionDiscoveryCategories.h.


__The Sample__

First, this sample demonstrates how to use Function Discovery to create a PnP query and obtain a list of all PnP devices on the system.  It also shows how to read the metadata provided by the device.

Second, the sample demonstrates how to use Function Discovery to receive notifications from PnP.  Using Function Discovery, the application registers its query and receives a notification when a device is added or removed.

FunDiscovery.cpp
Contains the implementation of a Function Discovery wrapper class.

FunDiscovery.h
Contains the definition of the wrapper class.

Main.cpp
Contains the _wmain function.


__Note__

If you add a multifunction device, such as a web camera, the sample will report an add for the camera and an add for the microphone.

This sample could easily be modified to query a Network Provider such as the SSDP or WSD provider.  
