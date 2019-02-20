PlaybackFX Sample

    This sample demonstrates how add a video effect during video 
    playback, using Media Foundation transforms (MFTs). 

    Much of the code in this sample is identical to the BasicPlayback
    sample. You may want to look at the BasicPlayback sample first,
    and then examine the differences between that sample and the 
    PlaybackFX sample.

    The only major difference between the two samples is how the 
    topology is created. In the PlaybackFX sample, the application
    adds a grayscale MFT to the video branch of the toplogy. 

Usage

    This sample depends on the MFT_Grayscale sample. 

    1. Build the MFT_Grayscale sample.

    2. Register MFT_Grayscale.dll with Regsvr32.exe.

       Note: You must run as administrator to register the DLL.

    3. Launch the PlaybackFX application.

    4. From the File menu, select Open File. 
  
    5. Select a Windows Media Video (.wmv) file.



THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All rights reserved.

