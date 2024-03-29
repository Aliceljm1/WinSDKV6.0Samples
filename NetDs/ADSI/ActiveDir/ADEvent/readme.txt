ADEvent is a simple example of an Active Directory administrative 
notification handler.


A notification handler receives notifications when the user deletes, renames, 
moves or changes the properties of an object using one of the Active 
Directory administrative MMC the snap-ins. This sample will display text in 
the debugger when any of the methods are called. Any additional functionality 
must be added by the developer.


Before the notification handler can be installed, it must be registered. To 
register the COM object, build the debug version of the DLL and execute the 
following from the command line, substituting the path where the DLL is 
located for "<path>":

regsvr32.exe /s /c <path>\ADEvent.dll


The notification handler must also be added to the display specifiers in 
Active Directory. The notification handler is registered under the 
dsUIAdminNotification attribute in the DisplaySpecifiers - <locale> - DS-UI-
Default-Settings container. <locale> refers to the locale identifier for the 
locale that the handler will be registered under. The handler must be added 
for all locales that it will be available for. The dsUIAdminNotification 
attribute is a multi-valued string attribute that takes the form "<entry 
number>,<CLSID>. <entry number> is an ordinal number and the first available 
number should be used, but duplicates are allowed. <CLSID> is the CLSID of 
the notification handler. For this sample, the value of this will be {08698521
-653C-4386-B206-DFA3C0F904E5}.


To test the notification handler, run, or attach to an existing instance of, 
the Active Directory Users and Computers MMC snap-in in a debugger. If any 
object displayed by the snap-in is deleted, renamed, moved or has a property 
change, the notification handler will be instantiated and notified of the 
event.

