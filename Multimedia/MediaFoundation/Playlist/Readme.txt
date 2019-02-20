Readme for the Sequencer Source Sample

Overview
-----------
------------
Sample: SequencerSource
Sample location: <SDK install directory>\Samples\Multimedia\SeqSource
Module: MF_SeqSrc_Playback.exe
Language: Win32
OS Version: Windows Vista

Files				Description

MF_SeqSrc_Playback.h		Global project header
MF_SeqSrc_Playback.rc		Resource file
Utils.h				Debug utilities
MF_SeqSrc_Playback.sln		Solution file
MF_SeqSrc_Playback.vcproj	Project file
CPlayer.cpp			Playback class
CPlayer.h			Playback class
resource.h			Resource IDs
winmain.cpp			Entry point


Features Demonstrated
---------------------
----------------------
This sample demonstrates standard playback scenario implemented by using Media Foundation components mainly the sequencer source and the media session. It shows how to:
* Create of partial playback topologies
* Add the topologies in the sequencer source
* Delete topologies from the sequencer source
* Provide playback control such as play, pause, stop, and skip
* Receive media session events asynchronously
* Manage presentation time using the media session


Relevant Documentation
-----------------------
-----------------------
The following topics in Media Foundation SDK documentation provides information about the procedures demonstrated in this sample:
* Topologies
* Media Session
* Sequencer Source
* Walkthrough: Playing Media Files in a Sequence
* Media Event Generators
* Presentation Clocks
* Presentation Descriptors


Relevant APIs
-----------------------
-----------------------
The sample demonstrates the use of the following Media Foundation APIs:
* IMFMediaSession Interface
* IMFMediaSource Interface
* IMFSourceResolver Interface
* IMFTopology Interface
* IMFTopologyNode Interface
* IMFSequencerSource Interface
* IMFMediaSourceTopologyProvider Interface
* MFCreateTopology Function
* MFCreateSequencerSource Function
* MFCreateMediaSession Function


Building the Sample
------------------------
------------------------
To build this sample
1. From the Start->All Programs menu choose Microsoft Windows SDK -> CMD Shell
2. In the WinSDK CMD shell, navigate to Samples/Multimedia/MediaFoundationF/SeqSource
3. Type vcbuild.
This creates the executable module in the <enter path>


Usage
-------------
-------------
1. To create a playlist, click File->Add to Playlist. This displays the Open dialog box. 
2. The user can select multiple files. Supported file types are wma and mp3. The application creates topologies for the selected files and adds them to the sequencer source. The created playlist is displayed in the listbox.
3. To play the sequence, click Play. 
4. To pause the current segment, click Pause. 
5. To stop the current segment, click Stop.
6. To skip to a segment, double-click the corresponding item in the playlist.
7. To add more segments to the sequencer source, click File->Add to Playlist.
8. To delete segments, click File->Remove from Playlist.
9. To close the application, click the x button or click File->Exit.

The events from Media Foundation, presentation time, segment duration, and current time are displayed on the UI. 
