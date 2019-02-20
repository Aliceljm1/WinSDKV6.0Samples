This sample illustrates the creation of a sample property handler for an XML based .recipe file format. It supports Open Metadata. (handles all properties given to it by saving under <ExtendedProperties> section of the XML file)


To install this sample:

1. Build the DLL in Visual Studio.
2. Copy DLL, RecipeProperties.propdesc, and RecipePropertyHandler.reg to a directory on your machine.
3. Execute the following commands from an elevated command prompt (right-click cmd.exe -> Run as Administrator):
    a. 'reg.exe import C:\Path\To\Your\Files\RecipePropertyHandler.reg'  (registers the .recipe filetype)
    b. 'regsvr32.exe C:\Path\To\Your\Files\RecipePropertyHandler.dll'  (registers the property handler with COM and for the .recipe filetype)
    c. 'rundll32.exe C:\Path\To\Your\Files\RecipePropertyHandler.dll,InstallPropertySchema C:\Path\To\Your\Files\RecipeProperties.propdesc'  (registers custom .recipe properties with the system schema; NOTE: you must specify the full path to the .propdesc file)


* Build in visual studio first.
	This sample runs on Windows Vista.
* Copy RecipePropertyHandler.dll to the bin folder
	Alternatively you can keep the dll in your debug folder. However note that once registered and used in Explorer, Explorer keeps it loaded, so when you recompile, Visual Studio will fail at the last stage of writing back to the dll file, since it is currently locked by Explorer. You can kill Explorer when compiling and then restart it once it is done. This way Explorer will load the latest build of the DLL.
* Make sure the path is correct in RecipeHandlers\RecipeHandlers.reg file for the DLL. 
	(based on where you copied these files in your own machine.)
* Create an elevated command prompt. (i.e. right click the cmd prompt and select Run As Administrator) 
	This is an important step to ensure that you can register your handler with Explorer when running under UAP.
* Run the RecipePropertyHandler.reg file in the elevated command prompt.
	This registers the handler and the property lists to show for that file type with Explorer.
* Copy the PerfectSteaks.recipe file to your %systemdrive%\users\USERNAME\Documents folder. 
	This is a folder that is indexed by default. This way your recipes will be indexed. 
	Explorer will show the set of properties specified in the RecipePropertyHandler.reg file in your details pane, property dialog and tooltips.


Debugging Tips:

* You can use the other SDK sample propertyedit to debug the property handler you build.
	* Build and copy the propertyedit.exe to a location that is in your path.
	* In VS2005, you can then go to Project->Properties and in the dialog go under Configuration Properties/Debugging:
		* Set Command to the propertyedit.exe
		* Set Command arguments to: -set "System.Author" "John Doe" PerfectSteaks.recipe (use the full path to the .recipe)
	* Note that you should register the DLL in your debug folder to make sure that this tool loads the correct version of the dll and that your breakpoints are hit properly. 

* Note that property handlers run in 2 different contexts:
	* Under the Explorer process. All property editing is done here, as well as reading properties. 
	* Under the SearchIndexer.exe process. All handlers run out-of-proc with default restricted privileges. No editing is done here, only reading of values.
