Windows Media Player Plug-in Wizard

Installing the Plug-in Wizard
You must install the Windows Media Player plug-in wizard to use it. The following steps will guide you: 

1. Using Windows Explorer, locate the folder where you installed the SDK. Expand the folder to view its subfolders and locate the subfolder named \Wizards\VSNET. 

2. Locate the three files that have a .vsz file name extension. You must choose one of these files, depending on which version of Visual Studio you have. The following table lists the files and the version of Visual Studio they correspond to. 

File name       Visual Studio version
---------       --------------------- 
Wmpwiz.vsz      Visual Studio .NET 2002 
Wmpwiz2003.vsz  Visual Studio .NET 2003 
Wmpwiz2005.vsz  Visual Studio 2005 


3. Locate the following two files: 
   1. wmpwiz.ico 
   2. wmpwiz.vsdir 
Select both of these files and the appropriate file from step 2. Right-click the files, and then click Copy.

4. Locate the subfolder named vcprojects where you installed Visual Studio. Paste the three files you copied into this folder. 

5. If needed, rename the .vsz file to wmpwiz.vsz. If the .vsz file you copied is not already named wmpwiz.vsz, rename the .vsz file by deleting the year from the file name. 

6. Using Notepad, edit the file named wmpwiz.vsz. Locate the following line: 
Param="ABSOLUTE_PATH = <path to wmpwiz directory goes here>"

7. Change the value for ABSOLUTE_PATH to the path where the wizard files are located. For example, if you installed the SDK to the following path: 
C:\WMSDK\WMPSDK11

8. Change the text to read as follows:

Param="ABSOLUTE_PATH = C:\WMSDK\WMPSDK11\Wizards\VSNET"

Save and close the file. 
This wizard is now installed.





Copyright (c) Microsoft Corporation. All rights reserved.