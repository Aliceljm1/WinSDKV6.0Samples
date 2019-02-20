// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

// CWMPEventDispatch.cpp : Implementation of the event dispatcher
//
#include "stdafx.h"
#include "CWMPEventDispatch.h"
#include "CCWindow.h"

extern CCWindow* g_pCCW;

HRESULT CWMPEventDispatch::Invoke(
                                  DISPID  dispIdMember,      
                                  REFIID  riid,              
                                  LCID  lcid,                
                                  WORD  wFlags,              
                                  DISPPARAMS FAR*  pDispParams,  
                                  VARIANT FAR*  pVarResult,  
                                  EXCEPINFO FAR*  pExcepInfo,  
                                  unsigned int FAR*  puArgErr )
{
    if (!pDispParams)
        return E_POINTER;

    if (pDispParams->cNamedArgs != 0)
        return DISP_E_NONAMEDARGS;

    HRESULT hr = DISP_E_MEMBERNOTFOUND;

    switch (dispIdMember)
    {
    case DISPID_WMPCOREEVENT_OPENSTATECHANGE:
        /* NewState */
        OpenStateChange(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_PLAYSTATECHANGE:
        /* NewState */
        PlayStateChange(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_AUDIOLANGUAGECHANGE:
        /* LangID */
        AudioLanguageChange(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_STATUSCHANGE:
        StatusChange();
        break;

    case DISPID_WMPCOREEVENT_SCRIPTCOMMAND:
        /* scType & Param */
        ScriptCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_NEWSTREAM:
        NewStream();
        break;

    case DISPID_WMPCOREEVENT_DISCONNECT:
        /* Result */ 
        Disconnect(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_BUFFERING:
        /* Start */
        Buffering(pDispParams->rgvarg[0].boolVal);
        break;

    case DISPID_WMPCOREEVENT_ERROR:
        Error();
        break;

    case DISPID_WMPCOREEVENT_WARNING:
        /* WarningType & Param, & Description */
        Warning(pDispParams->rgvarg[1].lVal, pDispParams->rgvarg[0].lVal, pDispParams->rgvarg[2].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_ENDOFSTREAM:
        /* Result */
        EndOfStream(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_POSITIONCHANGE:
        /* oldPosition & newPosition */
        PositionChange(pDispParams->rgvarg[1].dblVal, pDispParams->rgvarg[0].dblVal);
        break;

    case DISPID_WMPCOREEVENT_MARKERHIT:
        /* MarkerNum */
        MarkerHit(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_DURATIONUNITCHANGE:
        /* NewDurationUnit */
        DurationUnitChange(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_CDROMMEDIACHANGE:
        /* CdromNum */
        CdromMediaChange(pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_PLAYLISTCHANGE:
        /* Playlist & change */
        PlaylistChange(pDispParams->rgvarg[1].pdispVal, (WMPPlaylistChangeEventType) pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_CURRENTPLAYLISTCHANGE:
        /* change */
        CurrentPlaylistChange((WMPPlaylistChangeEventType) pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPCOREEVENT_CURRENTPLAYLISTITEMAVAILABLE:
        /* bstrItemName */
        CurrentPlaylistItemAvailable(pDispParams->rgvarg[0].bstrVal );
        break;

    case DISPID_WMPCOREEVENT_MEDIACHANGE:
        /* Item */
        MediaChange(pDispParams->rgvarg[0].pdispVal);
        break;

    case DISPID_WMPCOREEVENT_CURRENTMEDIAITEMAVAILABLE:
        /* bstrItemName */
        CurrentMediaItemAvailable(pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_CURRENTITEMCHANGE:
        /* pdispMedia */
        CurrentItemChange(pDispParams->rgvarg[0].pdispVal);
        break;

    case DISPID_WMPCOREEVENT_MEDIACOLLECTIONCHANGE:
        MediaCollectionChange();
        break;

    case DISPID_WMPCOREEVENT_MEDIACOLLECTIONATTRIBUTESTRINGADDED:
        /* bstrAttribName & bstrAttribVal */
        MediaCollectionAttributeStringAdded(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_MEDIACOLLECTIONATTRIBUTESTRINGREMOVED:
        /* bstrAttribName & bstrAttribVal */
        MediaCollectionAttributeStringRemoved(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_MEDIACOLLECTIONATTRIBUTESTRINGCHANGED:
        /* bstrAttribName & bstrOldAttribVal & bstrNewAttribVal */
        MediaCollectionAttributeStringChanged(pDispParams->rgvarg[2].bstrVal, pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_PLAYLISTCOLLECTIONCHANGE:
        PlaylistCollectionChange();
        break;

    case DISPID_WMPCOREEVENT_PLAYLISTCOLLECTIONPLAYLISTADDED:
        /* bstrPlaylistName */
        PlaylistCollectionPlaylistAdded(pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_PLAYLISTCOLLECTIONPLAYLISTREMOVED:
        /* bstrPlaylistName */
        PlaylistCollectionPlaylistRemoved(pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPCOREEVENT_PLAYLISTCOLLECTIONPLAYLISTSETASDELETED:
        /* bstrPlaylistName & varfIsDeleted */
        PlaylistCollectionPlaylistSetAsDeleted(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].boolVal);
        break;

    case DISPID_WMPCOREEVENT_MODECHANGE:
        /* ModeName & NewValue */
        ModeChange(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].boolVal);
        break;

    case DISPID_WMPCOREEVENT_MEDIAERROR:
        /* pMediaObject */
        MediaError(pDispParams->rgvarg[0].pdispVal);
        break;

    case DISPID_WMPCOREEVENT_OPENPLAYLISTSWITCH:
        /* pItem */
        OpenPlaylistSwitch(pDispParams->rgvarg[0].pdispVal);
        break;

    case DISPID_WMPCOREEVENT_DOMAINCHANGE:
        /* strDomain */
        DomainChange(pDispParams->rgvarg[0].bstrVal);
        break;

    case DISPID_WMPOCXEVENT_SWITCHEDTOPLAYERAPPLICATION:
        SwitchedToPlayerApplication();
        break;

    case DISPID_WMPOCXEVENT_SWITCHEDTOCONTROL:
        SwitchedToControl();
        break;

    case DISPID_WMPOCXEVENT_PLAYERDOCKEDSTATECHANGE:
        PlayerDockedStateChange();
        break;

    case DISPID_WMPOCXEVENT_PLAYERRECONNECT:
        PlayerReconnect();
        break;

    case DISPID_WMPOCXEVENT_CLICK:
        /* nButton & nShiftState & fX & fY */
        Click(pDispParams->rgvarg[3].iVal, pDispParams->rgvarg[2].iVal,  pDispParams->rgvarg[1].lVal,  pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPOCXEVENT_DOUBLECLICK:
        /* nButton & nShiftState & fX & fY */
        DoubleClick(pDispParams->rgvarg[3].iVal, pDispParams->rgvarg[2].iVal,  pDispParams->rgvarg[1].lVal,  pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPOCXEVENT_KEYDOWN:
        /* nKeyCode & nShiftState */
        KeyDown(pDispParams->rgvarg[1].iVal, pDispParams->rgvarg[0].iVal);
        break;

    case DISPID_WMPOCXEVENT_KEYPRESS:
        /* nKeyAscii */
        KeyPress(pDispParams->rgvarg[0].iVal);
        break;

    case DISPID_WMPOCXEVENT_KEYUP:
        /* nKeyCode & nShiftState*/
        KeyUp(pDispParams->rgvarg[1].iVal, pDispParams->rgvarg[0].iVal);
        break;

    case DISPID_WMPOCXEVENT_MOUSEDOWN:
        /* nButton & nShiftState & fX & fY */
        MouseDown(pDispParams->rgvarg[3].iVal, pDispParams->rgvarg[2].iVal,  pDispParams->rgvarg[1].lVal,  pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPOCXEVENT_MOUSEMOVE:
        /* nButton & nShiftState & fX & fY */
        MouseMove(pDispParams->rgvarg[3].iVal, pDispParams->rgvarg[2].iVal,  pDispParams->rgvarg[1].lVal,  pDispParams->rgvarg[0].lVal);
        break;

    case DISPID_WMPOCXEVENT_MOUSEUP:
        /* nButton & nShiftState & fX & fY */
        MouseUp(pDispParams->rgvarg[3].iVal, pDispParams->rgvarg[2].iVal,  pDispParams->rgvarg[1].lVal,  pDispParams->rgvarg[0].lVal);
        break;
    }

    return( hr );
}

// Sent when the control changes OpenState
void CWMPEventDispatch::OpenStateChange(long NewState)
{
    return;
}

// Sent when the control changes PlayState
void CWMPEventDispatch::PlayStateChange(long NewState)
{
    return;
}

// Sent when the audio language changes
void CWMPEventDispatch::AudioLanguageChange(long LangID)
{
    return;
}

// Sent when the status string changes
void CWMPEventDispatch::StatusChange()
{
    return;
}

// Sent when a synchronized command or URL is received
// we need to forward the caption string to the closed
// caption window for display
void CWMPEventDispatch::ScriptCommand(BSTR scType,BSTR Param)
{
    CComBSTR bstrText(TEXT("TEXT"));
    CComBSTR bstrCaption(TEXT("CAPTION"));
    CComBSTR bstrSyncLyric(TEXT("SYNCHRONIZEDLYRIC"));
    CComBSTR bstrSyncLyricLyric(TEXT("SYNCHRONIZEDLYRICLYRIC"));

    CComBSTR bstrType(scType);
    HRESULT hr = bstrType.ToUpper();
    if(FAILED(hr)) return;

    if( bstrText == bstrType  ||
        bstrCaption == bstrType ||
        bstrSyncLyric == bstrType ||
        bstrSyncLyricLyric == bstrType)
    {
        g_pCCW->OnClosedCaption(Param);
    }

    return;
}

// Sent when a new stream is encountered (obsolete)
void CWMPEventDispatch::NewStream()
{
    return;
}

// Sent when the control is disconnected from the server (obsolete)
void CWMPEventDispatch:: Disconnect(long Result )
{
    return;
}

// Sent when the control begins or ends buffering
void CWMPEventDispatch:: Buffering(VARIANT_BOOL Start)
{
    return;
}

// Sent when the control has an error condition
void CWMPEventDispatch::Error()
{
    return;
}

// Sent when the control has an warning condition (obsolete)
void CWMPEventDispatch::Warning(long WarningType, long Param, BSTR Description)
{
    return;
}

// Sent when the media has reached end of stream
void CWMPEventDispatch::EndOfStream(long Result)
{
    return;
}

// Indicates that the current position of the movie has changed
void CWMPEventDispatch::PositionChange(double oldPosition,double newPosition)
{
    return;
}

// Sent when a marker is reached
void CWMPEventDispatch::MarkerHit(long MarkerNum )
{
    return;
}

// Indicates that the unit used to express duration and position has changed
void CWMPEventDispatch::DurationUnitChange(long NewDurationUnit)
{
    return;
}

// Indicates that the CD ROM media has changed
void CWMPEventDispatch::CdromMediaChange(long CdromNum)
{
    return;
}

// Sent when a playlist changes
void CWMPEventDispatch::PlaylistChange(IDispatch * Playlist,WMPPlaylistChangeEventType change)
{
    return;
}

// Sent when the current playlist changes
void CWMPEventDispatch::CurrentPlaylistChange(WMPPlaylistChangeEventType change )
{
    return;
}

// Sent when a current playlist item becomes available
void CWMPEventDispatch::CurrentPlaylistItemAvailable(BSTR bstrItemName)
{
    return;
}

// Sent when a media object changes
void CWMPEventDispatch::MediaChange(IDispatch * Item)
{
    return;
}

// Sent when a current media item becomes available
void CWMPEventDispatch::CurrentMediaItemAvailable(BSTR bstrItemName)
{
    return;
}

// Sent when the item selection on the current playlist changes
void CWMPEventDispatch::CurrentItemChange(IDispatch *pdispMedia)
{
    return;
}

// Sent when the media collection needs to be requeried
void CWMPEventDispatch::MediaCollectionChange()
{
    return;
}

// Sent when an attribute string is added in the media collection
void CWMPEventDispatch::MediaCollectionAttributeStringAdded(BSTR bstrAttribName, BSTR bstrAttribVal)
{
    return;
}

// Sent when an attribute string is removed from the media collection
void CWMPEventDispatch::MediaCollectionAttributeStringRemoved(BSTR bstrAttribName, BSTR bstrAttribVal)
{
    return;
}

// Sent when an attribute string is changed in the media collection
void CWMPEventDispatch::MediaCollectionAttributeStringChanged(BSTR bstrAttribName, BSTR bstrOldAttribVal, BSTR bstrNewAttribVal)
{
    return;
}

// Sent when playlist collection needs to be requeried
void CWMPEventDispatch::PlaylistCollectionChange()
{
    return;
}

// Sent when a playlist is added to the playlist collection
void CWMPEventDispatch::PlaylistCollectionPlaylistAdded(BSTR bstrPlaylistName)
{
    return;
}

// Sent when a playlist is removed from the playlist collection
void CWMPEventDispatch::PlaylistCollectionPlaylistRemoved(BSTR bstrPlaylistName)
{
    return;
}

// Sent when a playlist has been set or reset as deleted
void CWMPEventDispatch::PlaylistCollectionPlaylistSetAsDeleted(BSTR bstrPlaylistName, VARIANT_BOOL varfIsDeleted)
{
    return;
}

// Playlist playback mode has changed
void CWMPEventDispatch::ModeChange(BSTR ModeName, VARIANT_BOOL NewValue)
{
    return;
}

// Sent when the media object has an error condition
void CWMPEventDispatch::MediaError(IDispatch * pMediaObject)
{
    return;
}

// Current playlist switch with no open state change
void CWMPEventDispatch::OpenPlaylistSwitch(IDispatch *pItem)
{
    return;
}

// Sent when the current DVD domain changes
void CWMPEventDispatch::DomainChange(BSTR strDomain)
{
    return;
}

// Sent when display switches to player application
void CWMPEventDispatch::SwitchedToPlayerApplication()
{
    return;
}

// Sent when display switches to control
void CWMPEventDispatch::SwitchedToControl()
{
    return;
}

// Sent when the player docks or undocks
void CWMPEventDispatch::PlayerDockedStateChange()
{
    return;
}

// Sent when the OCX reconnects to the player
void CWMPEventDispatch::PlayerReconnect()
{
    return;
}

// Occurs when a user clicks the mouse
void CWMPEventDispatch::Click( short nButton, short nShiftState, long fX, long fY )
{
    return;
}

// Occurs when a user double-clicks the mouse
void CWMPEventDispatch::DoubleClick( short nButton, short nShiftState, long fX, long fY )
{
    return;
}

// Occurs when a key is pressed
void CWMPEventDispatch::KeyDown( short nKeyCode, short nShiftState )
{
    return;
}

// Occurs when a key is pressed and released
void CWMPEventDispatch::KeyPress( short nKeyAscii )
{
    return;
}

// Occurs when a key is released
void CWMPEventDispatch::KeyUp( short nKeyCode, short nShiftState )
{
    return;
}

// Occurs when a mouse button is pressed
void CWMPEventDispatch::MouseDown( short nButton, short nShiftState, long fX, long fY )
{
    return;
}

// Occurs when a mouse pointer is moved
void CWMPEventDispatch::MouseMove( short nButton, short nShiftState, long fX, long fY )
{
    return;
}

// Occurs when a mouse button is released
void CWMPEventDispatch::MouseUp( short nButton, short nShiftState, long fX, long fY )
{
    return;
}