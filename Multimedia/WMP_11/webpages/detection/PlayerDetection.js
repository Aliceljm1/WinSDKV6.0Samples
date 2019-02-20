//    Copyright (c) Microsoft Corporation.  All rights reserved.

//-----------------------------------------------------------------
// PlayerDetection.js                                              
//                                                                 
// Purpose:                                                        
//   Javascript library of functions used by Sample.htm.  Each
//   function is commented as to its purpose.            
//-----------------------------------------------------------------


//------------------------
//-- Global Vars 
//------------------------
var WMPVer = "unknown";       // Set to WMP version string detected
var fHasWMP = false;          // True if either WMP v6.4 or v7+ found
var fHasWMP64 = false;        // True if WMP v6.4 found
var fHasWMP7 = false;         // True if WMP v7+ (8, 9) found

//-----------------------------------------------
// v7+ onPlayStateChange state options array
//-----------------------------------------------
var psArray = new Array(12);
psArray[0] = "Undefined - Windows Media Player is in an undefined state.";
psArray[1] = "Stopped - Playback of the current media clip is stopped."; 
psArray[2] = "Paused - Playback of the current media clip is paused. When media is paused, resuming playback begins from the same location.";
psArray[3] = "Playing - The current media clip is playing."; 
psArray[4] = "ScanForward - The current media clip is fast forwarding.";
psArray[5] = "ScanReverse - The current media clip is fast rewinding."; 
psArray[6] = "Buffering - The current media clip is getting additional data from the server.";
psArray[7] = "Waiting - Connection is established, however the server is not sending bits. Waiting for session to begin.";
psArray[8] = "MediaEnded - Media has completed playback and is at its end.";  
psArray[9] = "Transitioning - Preparing new media."; 
psArray[10] = "Ready - Ready to begin playing."; 
psArray[11] = "Reconnecting - Reconnecting to stream.";


//-----------------------------------------------
// v6.4 onPlayStateChange state options array
//-----------------------------------------------
var ps64Array = new Array(9);
ps64Array[0] = "mpStopped - Playback is stopped.";
ps64Array[1] = "mpPaused - Playback is paused."; 
ps64Array[2] = "mpPlaying - Stream is playing."; 
ps64Array[3] = "mpWaiting - Waiting for stream to begin."; 
ps64Array[4] = "mpScanForward - Stream is scanning forward."; 
ps64Array[5] = "mpScanReverse - Stream is scanning in reverse."; 
ps64Array[6] = "mpSkipForward - Skipping to next."; 
ps64Array[7] = "mpSkipReverse - Skipping to previous."; 
ps64Array[8] = "mpClosed - Stream is not open."; 


//-----------------------------------------------
// Name:  GetBrowser
// Purpose: Get Browser Information
//-----------------------------------------------
function GetBrowser()
{
    var agt=navigator.userAgent.toLowerCase();
    if( ((agt.indexOf("msie") != -1) && (agt.indexOf("opera") == -1)) )
        return "IE";
    else if( ((agt.indexOf('mozilla')!=-1) && (agt.indexOf('spoofer')==-1)
            && (agt.indexOf('compatible') == -1) && (agt.indexOf('opera')==-1)
            && (agt.indexOf('webtv')==-1) && (agt.indexOf('hotjava')==-1)) )
        return "Netscape";
    else
        return "unknown";
}


//------------------------------------------------
// Name:  GetPlayerMajorVer
// Purpose: Get Media Player Major Version Number
//------------------------------------------------
function GetPlayerMajorVer()
{
    var strVer = new String(WMPVer);
    s = strVer.split(".");
    return s[0];
}


//-------------------------------------------------
// Name:  GetPlayerMinorVer
// Purpose: Get Media Player Major Version Number.
//-------------------------------------------------
function GetPlayerMinorVer()
{
    var strVer = new String(WMPVer);
    s = strVer.split(".");
    if (s[1])
        return s[1];
    else
        return("unknown");
}


//----------------------------------------------------------------------
// Name:  set_uiMode
// Purpose:  Wrapper function for setting the uiMode of the Media Player 
//  control.  Supports uiMode of "none", "mini" and "full".
//-----------------------------------------------------------------------
function set_uiMode(uiMode)
{ 
    // If WMP 6.4 then set ui mode related properties
    if (fHasWMP64) 
    { 
        if (uiMode=="none") 
        {
            MediaPlayer.ShowControls=false;
            MediaPlayer.ShowTracker=false;
            MediaPlayer.EnableTracker=false;
            MediaPlayer.ShowPositionControls=false;
            MediaPlayer.EnablePositionControls=false;
            MediaPlayer.ShowStatusBar=false;
        } 
        if (uiMode=="mini") 
        {
            MediaPlayer.ShowControls=true;
            MediaPlayer.ShowTracker=false;
            MediaPlayer.EnableTracker=false;
            MediaPlayer.ShowPositionControls=false;
            MediaPlayer.EnablePositionControls=false;
            MediaPlayer.ShowStatusBar=true;
        }  
        if (uiMode=="full") 
        {
            MediaPlayer.ShowControls=true;
            MediaPlayer.ShowTracker=true;
            MediaPlayer.EnableTracker=true;
            MediaPlayer.ShowPositionControls=true;
            MediaPlayer.EnablePositionControls=true;
            MediaPlayer.ShowStatusBar=true;
        }
    }
    else
    {
        MediaPlayer.uiMode=uiMode;
    }
}


//---------------------------------------------------------------------------
// Name: get_uiMode
// Purpose: Wrapper function for getting current uiMode of the Media Player.
//---------------------------------------------------------------------------
function get_uiMode()
{
    // Note: Per WMP SDK, uiMode is a string:  "none", "mini", "full"
    // If accessing the old 6.4 properties
    if (fHasWMP64) 
    {                
        if (MediaPlayer.ShowControls==false &&
            MediaPlayer.ShowTracker==false &&
            MediaPlayer.EnableTracker==false && 
            MediaPlayer.ShowPositionControls==false && 
            MediaPlayer.EnablePositionControls==false )
        return("none");

        if (MediaPlayer.ShowControls==true &&
            MediaPlayer.ShowTracker==false && 
            MediaPlayer.EnableTracker==false &&
            MediaPlayer.ShowPositionControls==false &&
            MediaPlayer.EnablePositionControls==false)
        return("mini");   

        if (MediaPlayer.ShowControls==true &&
            MediaPlayer.ShowTracker==true &&
            MediaPlayer.EnableTracker==true &&
            MediaPlayer.ShowPositionControls==true &&
            MediaPlayer.EnablePositionControls==true)
        return("full");   

        return("unknown");
    }
    else
    {
        return(MediaPlayer.uiMode);
    }
}


//------------------------------------------------------------------
// Name: ToggleMode
// Purpose:  Using get_uiMode and set_uiMode this  function cycles 
//           through the UI modes of the player control when called.
//-------------------------------------------------------------------
function ToggleMode()
{
   var Mode = get_uiMode();
   if (Mode=="none") set_uiMode("mini");
   if (Mode=="mini") set_uiMode("full");
   if (Mode=="full") set_uiMode("none");   
}


//----------------------------------------------------------------------
// Name: GetPlayerState
// Purpose:  Returns a string describing the NewState number specified.
//----------------------------------------------------------------------
function GetPlayerState(NewState)
{
    if (fHasWMP64)
    {
        if (NewState>=0 && NewState<=8)
        return ps64Array[NewState];
        else
        return "mpUndefined - Windows Media Player is in an undefined state.";
    }    
    else
    {
        if (NewState>=0 && NewState<=11)
        return psArray[NewState];
        else
        return "Undefined - Windows Media Player is in an undefined state.";
    }   
}

//---------------------------------------------------------------------
// Name:  AddDownloadLink
// Purpose:  Add WMP v9 download link if an older version of WMP 
//           detected.
//---------------------------------------------------------------------
function AddDownloadLink(MajorVer)
{
    if (!(MajorVer) || MajorVer=="unknown" || MajorVer<9)
    { 
        document.write('<tr><td>You do not have the latest Windows Media Player version.<BR><A HREF="http://windowsmedia.com/download"><IMG SRC="http://www.microsoft.com/windows/windowsmedia/images/Download_88x31_static.gif" WIDTH="88" HEIGHT="31" BORDER="0" ALT="Get Windows Media Player" VSPACE="7"></A></td></tr>');
    } 
}
