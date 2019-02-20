WavSource Sample

    This sample implements a media source that parses audio .wav 
    files. The sample demonstrates how to write a custom media source
    for Media Foundation. It also shows how to register a byte stream
    handler for the source, so that the Media Foundation source 
    resolver can discover the media source.

    To keep the sample code as simple as possible, the media source 
    only parses .wav files that contain uncompressed PCM audio, mono 
    or stereo. 

    This sample does not support protected playback.


Usage

    The sample builds a DLL. To use the sample, do the following.

    1. Register the DLL with Regsvr32.exe.

    2. Open a .wav file from a Media Foundation playback application.
       You can use the BasicPlayback sample for this purpose.

Note: You must run as administrator to register the DLL.


THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation. All rights reserved.

