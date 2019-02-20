/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// readme.txt
// important notes on getting samples app to build and run correctly, please read
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BcdSampleApp is a windows forms sample app meant to show some standard usages of C#/WMI/Bcd as used in the BcdSampleLib 
that comes with the application.  You are free to use the BcdSampleLib alone, the App is a test application meant to show usage.

This project requires Visual Studio 2005.  

This project is a new release under Vista and does require escalation to work correctly since accessing Bcd is an admin level behavior. 
To get this to work you can right click the BcdSampleApp.exe and select "run as administrator".  

To avoid LUA build bugs or issues with Visual Studio on this project, you should run Visual Studio 2005 itself elevated for convenience.  
You can do this by right clicking on the Visual Studio 2005 link and choose "run as administrator" option, when you startup Visual Studio.  
Otherwise build errors may occur when you run the app within VS2005 based on insufficient rights.  As of the pending new Vista release these 
errors aren't always clear from the UI, sometimes the app just closes without showing a clear reason why.  If it is doing that it may well be an
insufficient rights issue with LUA.

Alternatively if you want automatic LUA behavior by double clicking the BcdSampleApp.exe (or your modified App that you deploy), then you will need 
to add a file like the following below in the same directory as the exe, with the name myApp.exe.manifest (if your app is named myApp.exe).

The contents of this file should be the following xml (this is entire file contents):

BcdSampleApp.exe.manifest contents: (use same name as your exe + ".manifest", and put in same folder as your EXE).
----------------------------------------------------------

<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">   
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
      <requestedPrivileges>
        <requestedExecutionLevel level="requireAdministrator" uiAccess="false"></requestedExecutionLevel>
        </requestedPrivileges>
       </security>
  </trustInfo>
</assembly>

A better solution would be to incorporate this manifest directly into the BcdSampleApp.exe as a post install step from Visual Studio 2005,
perhaps using the mt.exe utility (other utilities exist to sign manifest).  A standard usage for the mt.exe utility might be:

mt -manifest BcdSampleApp.exe.manifest -outputresource:BcdSampleApp.exe;#1

This updates BcdSampleApp.exe's internal manifest to now be the same as the BcdSampleApp.exe.manifest file.  If this approach doesn't work
you will have to use the manifest as an external file and keep it in same folder as your exe app.

Microsoft SDK publishing rules do not usually allow exe's to be published, and your Build Output Path may be either bin/Debug, bin/Release or a 
custom location.  Please copy the included (or your created) manifest file to your correct Build Output Path, and on deployment, make sure you 
deploy the manifest file along with your final exe application.  Remember the manifest must be in same folder to work that exe is in, unless you
are able to embed the manifest file using mt.exe.

If you don't see any .manifest files, or lose your manifest file, you can use cut and paste the xml text above and create a file with the same 
name as your exe adding on ".manifest", by default this would be BcdSampleApp.exe.manifest, and add the contents of the xml snippet above 
to the new file and save, and then move that .manifest file into the same folder as your test or release BcdSampleApp.exe.  Otherwise double 
clicking the application during deployment may result in it closing based on some error.  Right clicking and choosing run as administrator should
always work, with or without manifest files, but double clicking will typically fail ungracefully without a manifest file (or embedded manifest in
exe).

For more documentation on the new Bcd classes this msdn section may prove useful:

http://msdn.microsoft.com/library/en-us/BCD/bcd/bcd_classes.asp

Project Updates
-----------------

For latest version of this project contact me -- I will try to post project updates to a MS website 
in near future.  Contact info below.

Good luck, hope these samples are useful in learning the Bcd/WMI ropes from managed code.

Scott.Laing@microsoft.com
scottlaingwa@hotmail.com
9/27/06



