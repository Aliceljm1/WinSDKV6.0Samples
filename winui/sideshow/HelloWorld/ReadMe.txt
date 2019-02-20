Hello World Sample
------------------
Demonstrates the creation of a simple Windows SideShow application. The application registers with the Simple Content Endpoint and pre-loads or sends a page with the string ‘Hello World’ to the device.

Output: When the application runs successfully, the device will contain the  'Hello World Sample' application with a single page of content saying ‘Hello World’.

This sample demonstrates the use of the following classes and interfaces:
ISideShowSession
ISideShowContentManager
ISideShowContent


Build Steps
-----------
From an SDK Command Window:
1. To open an SDK Command Window look in Start -> Microsoft Platform SDK For Windows Vista -> Open a build environment
2. Change directory to the samples folder <PSDK Install Folder>\Samples\WinUI\SideShow\HelloWorld
3. Type ‘nmake’


Steps to run sample
-------------------
1. Register: Register the application with the Windows SideShow platform: Run the .reg registry file and select to enter the data into the registry
2. Simulator: See instructions to setup and start the simulator
3. Open the Windows SideShow control panel: Control Panel -> Classic View -> Windows SideShow
4. Enable the check box for this sample application
5. Run the sample executable
6. View the output on the simulator

Simulator Setup
---------------
Installation steps (first time only)
1. Run <PSDK Install Folder>\Bin\WindowsSideShowVirtualDevice /regserver

Starting
1. Run simulator for Windows SideShow from <PSDK Install Folder>\Bin\VirtualSideShow.exe
2. Installation required on first run


FAQ
---
Q. Why does the simulator not work on Windows XP or earlier Windows releases?
A. The Windows SideShow platform and simulator is supported only on Windows Vista.

Q. Why don’t I see the sample application on the device?
A. Make sure you register the application with the appropriate .reg file (in the sample folder). Then start the simulator and enable it through the Windows SideShow control panel. See the steps to run a sample.

Q. Why is there no data sent to the device?
A. Ensure that the application is enabled and appears on the simulator. Ensure that you run the simulator and then run the sample application.
