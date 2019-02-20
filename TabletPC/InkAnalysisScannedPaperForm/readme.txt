Ink Analysis Scanned Paper Form

This sample shows how to use a Ink Analysis to create a form filling application based on a scanned paper form. This sample application demonstrates the following features of the Ink Analysis API and the Windows Forms Inking Controls:

* Loading a scanned paper form (from an image in .png format)
* Gathering and displaying Ink over the scanned form
* Using an InkAnalyzer to interpret handwriting results
* Generating AnalysisHintNodes to improve handwriting results
* Populating text boxes from analysis hints for a basic correction experience


Project References
The sample is available as a Windows Forms or Windows Presentation Foundation application. For the Windows Forms version, it references the Microsoft.Ink.dll assembly and the Microsoft.Ink.Analysis.dll assembly.  The Windows Presentation Foundation version references IAWinFX.dll in addition to the core Windows Presentation Foundation dlls.

NOTE: The Windows Presentation Foundation version of this sample (found in the XAML directory) requires that the WinFX exetensions for Microsoft Visual Studio 2005 is installed on the system.

User Interface
The user interface for the sample consists of two tabs, labeled: "Ink Form" and "Converted Text Form". The "Ink Form" tab contains a panel which contains an image of a scanned paper form used for taking telephone messages. This tab also has a check box that enables showing the analysis hint bounds, as well as a pair of buttons used to clear the ink from the form, and to initialize the analysis operation.

When the Analyze button on the Ink Form is clicked, the ink analysis operation is performed (synchronously) and the recognition results are shown on the "Converted Text Form" tab.
