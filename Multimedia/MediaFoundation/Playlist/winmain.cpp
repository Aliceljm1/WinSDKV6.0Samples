//////////////////////////////////////////////////////////////////////////
//
// winmain.cpp : Defines the entry point for the application.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////
//
//The sample shows steps that are required to create a sequencer source 
//and add topologies. In addition, it also demonstrates the use of media session APIs 
//to provide transport control such as play, pause, stop, and skip.
//
//The user creates the initial playlist of audio files by using 
//a standard Open dialog box that allows multiple selection.
//The application creates topologies for the selected files and 
//adds them to the sequencer source. 
//The last topology is flagged to mark the end of the playlist. 
//To start the sequence, the first topology is added to the media session. 
//For continuous play, the next topology is queued on the media session 
//when the application receives a MENewPresenationEvent.
//The user can play, pause, stop, and skip. While the session is playing, 
//the user can add another segment; and delete a segment (this is not working).
//The core functionality is provided by the CPlayer class. 
//As the media session The UI is shows the segment name and duration for the current segment. 
//The statusbar shows the current status of the player. Segment information is stored in a list. 
//////////////////////////////////////////////////////////////////////////

#include "MF_SeqSrc_Playback.h"
#include "resource.h"
#include "CPlayer.h"

// Global Variables:
HINSTANCE    g_hInst;
HWND         g_hWnd = NULL;
HMENU        g_hMenu = NULL;

CPlayer      *g_pPlayer = NULL;
    
// Timer-based clock UI
const UINT_PTR  g_TimerID = 0;
const DWORD     g_TimerInterval = 200;

//global string for displaying Media Foundation messages

WCHAR szMessage [MAX_STRING_SIZE];


//////////////////////////////////////////////////////////////////////////
//  Name: NotifyError
//  Description: Show a message box with an error message.
//
//  sErrorMessage: NULL-terminated string containing the error message.
//  hrErr: HRESULT from the error.
/////////////////////////////////////////////////////////////////////////
void NotifyError(const WCHAR *szErrorMessage, HRESULT hrErr)
{
    WCHAR szMessage[MAX_STRING_SIZE];

    HRESULT hr = StringCchPrintf (szMessage, MAX_STRING_SIZE, L"%s (HRESULT = 0x%X)", szErrorMessage, hrErr);
    
    if (SUCCEEDED(hr))
	{
        MessageBox(g_hWnd, szMessage, NULL, MB_OK | MB_ICONERROR);
	}

}

//////////////////////////////////////////////////////////////////////////
//  Name: FormatTimeString
//  Description: Converts MFTIME format to mm:ss format.
//
//  time: MFTIME type 100 nano second unit.
//  szTimeString: NULL-terminated string containing the time string.
///////////////////////////////////////////////////////////////////////////
void FormatTimeString(MFTIME time, WCHAR *szTimeString)
{
    //Convert nanoseconds to seconds
    MFTIME TimeInSeconds = 0, Hours = 0, Minutes = 0, Seconds = 0;
    
    TimeInSeconds = (MFTIME)time/10000000;
    
    if (TimeInSeconds > 60)
    {
        Minutes = TimeInSeconds/60;

        if( Minutes > 60 )
        {
            Hours = Minutes/60;
            Minutes = Minutes % 60;
        }
        Seconds = (TimeInSeconds % 60);
    }
    else
    {
        Seconds = TimeInSeconds;
    }

	StringCchPrintf (szTimeString, MAX_STRING_SIZE, L"%.2i:%.2i:%.2i", (int) Hours, (int) Minutes, (int)Seconds);    
}


//////////////////////////////////////////////////////////////////////////
//  Name: SetPlayerStateString
//  Description: Returns the player .
//
//  time: MFTIME type 100 nano second unit.
//  szTimeString: NULL-terminated string containing the time string.
///////////////////////////////////////////////////////////////////////////
void SetPlayerStateString(DWORD dwState)
{
    WCHAR szPlayerState [MAX_STATE_STRING];
    StringCchPrintf(szPlayerState, MAX_STATE_STRING, L"");

    switch (dwState)
    {
        case PlayerCreated:
            StringCchCopy(szPlayerState, MAX_STATE_STRING, L"Player Created.");
            break;
       
        case Initialized:
            StringCchCopy(szPlayerState, MAX_STATE_STRING, L"Player initialized, empty playlist.");
            break;
            
        case Paused:
			StringCchCopy(szPlayerState, MAX_STATE_STRING, L"MESessionPaused: Paused.");
            break;
            
        case Playing:
			StringCchCopy(szPlayerState, MAX_STATE_STRING, L"MESessionStarted: Playing.");
            break;
            
        case Stopped:
			StringCchCopy(szPlayerState, MAX_STATE_STRING, L"MESessionStopped: Stopped.");
            break;

        default:
            break;
    }

	int iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szPlayerState);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);

}


//UI Event handlers
//----------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//  Name: OnInitDialog
//  Description: WM_INITDIALOG event handler. 
//
/////////////////////////////////////////////////////////////////////////

INT_PTR OnInitDialog()
{

    HRESULT hr = S_OK;

    g_hMenu = GetMenu(g_hWnd);


    //Initialize CPlayer object

    g_pPlayer = new CPlayer(g_hWnd);

    hr = g_pPlayer->Initialize();
    if( FAILED(hr))
    {
        return (INT_PTR)FALSE;  
    }   

    EnableWindow(GetDlgItem(g_hWnd, IDC_STOP), FALSE);
    EnableWindow(GetDlgItem(g_hWnd, IDC_PLAY), FALSE);
    EnableMenuItem(g_hMenu, IDM_REMOVEFROMPLAYLIST, MF_GRAYED);

    SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_SETHORIZONTALEXTENT, 500, 0);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETHORIZONTALEXTENT, 500, 0);

    return (INT_PTR)TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////
//  Name: OnAddToPlaylist
//  Description: 
//      Opens the standard open file dialog and allows the user to select multiple files.
//  Parameter:  
//  Note: If only one file is selected, the format is <path>\<filename>\0\0. 
//        If more than one file is selected, the format is <path>\<filename1>\0<filename2>\0\0 
/////////////////////////////////////////////////////////////////////////////////////////

void OnAddToPlaylist(void)
{
    HRESULT hr = S_OK;
    DWORD extendedError = 0;
    const size_t bufferSize = 2000;

    if (g_pPlayer)
    {
        // Show the File Open dialog.
        WCHAR path[bufferSize];
        path[0] = L'\0';

        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = g_hWnd;
        ofn.lpstrFilter = L"Windows Media\0*.wma\0MP3\0*.mp3\0All files\0*.*\0";
        ofn.lpstrFile = path;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn))
        {
			WCHAR szUrl[MAX_STRING_SIZE];
			WCHAR seps[]   = L"\n";
			WCHAR *pathToken, *fileName, *nextToken;
			int iNumSelect = 0;
			
			//replace all '\0' with '\n' so that we can use wcstok_s
			for(WCHAR *ptr = path; ; ptr++)
			{
				if(*ptr=='\0')
				{
					*ptr='\n';
					iNumSelect++;
				}
				if(*ptr=='\n'&&*(ptr+1)=='\0')
				{
					break;
				}
			}
			if(iNumSelect == 1)
			{
				hr = StringCchCopy(szUrl, MAX_STRING_SIZE, path);
				
				// Add a topology for the file to the sequencer source.
				if (SUCCEEDED(hr))
				{            
					hr = g_pPlayer->AddToPlaylist(szUrl);
				}
			}
			else
			{

				pathToken = wcstok_s( path, seps, &nextToken);
				fileName = nextToken;
	    
				while( fileName != NULL )
				{
					// Copy the path.
					hr = StringCchCopy(szUrl, MAX_STRING_SIZE, pathToken);
					
					//Get the next filename
					fileName = wcstok_s( NULL, seps, &nextToken);
					
					if(fileName != NULL)
					{
						// Concatenate the file name to the path.
						hr = StringCchCat(szUrl, MAX_STRING_SIZE, fileName);
				
						// Add a topology for the file to the sequencer source.
						if (SUCCEEDED(hr))
						{            
							hr = g_pPlayer->AddToPlaylist(szUrl);
						}
					}
				}
			}
			if (FAILED(hr))
	        {
		        NotifyError(L"Could not add to the playlist.", hr);
			}

		}
    }
}


void OnPlay(void)
{
    HRESULT hr = S_OK;

    if(g_pPlayer->GetState() == Playing)
    {
        if (FAILED(hr = g_pPlayer->Pause()))
        {
            NotifyError(L"Could not pause playback.", hr);
        }
    }
    else
    {
        if (FAILED(hr = g_pPlayer->Play()))
        {
            NotifyError(L"Could not start playback.", hr);
        }
    }
}


void OnStop(void)
{
    HRESULT hr = S_OK;

    if (FAILED(hr = g_pPlayer->Stop()))
    {
        NotifyError(L"Could not start playback.",hr);
    }
}


void OnRemoveFromPlaylist(void)
{
    HRESULT hr = S_OK;
    
    MFSequencerElementId SegmentID = 0;

    int iIndex = 0;

    iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_GETCURSEL, 0, 0);
    
    if (iIndex != LB_ERR)
    {
        SegmentID = (DWORD)SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_GETITEMDATA, iIndex, 0);

        if (FAILED(hr = g_pPlayer->DeleteSegment(SegmentID)))
        {
            NotifyError(L"Could not delete segment.", hr);
        }
    }
    else
    {
        NotifyError(L"Segment not selected.", LB_ERR);
    }
}

void SkipToSegment(int iIndex)
{
    HRESULT hr = S_OK;
    MFSequencerElementId SegmentID = 0; 
    
    SegmentID = (DWORD)SendDlgItemMessage(g_hWnd, IDC_PLAYLIST, LB_GETITEMDATA, iIndex, 0);
    
    if (FAILED(hr = g_pPlayer->Skip(SegmentID)))
    {
        NotifyError(L"Could not skip playback.",hr);
    }
}


// MediaFoundation Event Handlers
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//  Name: OnMESessionStarted
//  Description: OnMESessionStarted event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMESessionStarted(DWORD dwState)
{
    
    SetDlgItemText(g_hWnd, IDC_PLAY, L"Pause");

    EnableWindow(GetDlgItem(g_hWnd, IDC_STOP), TRUE);

    SetPlayerStateString(dwState);

    KillTimer( g_hWnd, g_TimerID );
    SetTimer( g_hWnd, g_TimerID, g_TimerInterval, NULL );
}

//////////////////////////////////////////////////////////////////////////
//  Name: OnMESessionPaused
//  Description: OnMESessionPaused event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMESessionPaused(DWORD dwState)
{

    SetDlgItemText(g_hWnd, IDC_PLAY, L"Continue");

    SetPlayerStateString(dwState);

    KillTimer( g_hWnd, g_TimerID );
}

//////////////////////////////////////////////////////////////////////////
//  Name: OnMESessionStopped
//  Description: MESessionStopped event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMESessionStopped(DWORD dwState)
{
    SetDlgItemText(g_hWnd, IDC_PLAY, L"Play");

    EnableWindow(GetDlgItem(g_hWnd, IDC_STOP), FALSE);

    SetPlayerStateString(dwState);

    KillTimer( g_hWnd, g_TimerID );
}


//////////////////////////////////////////////////////////////////////////
//  Name: OnMESessionPresentationSwitch
//  Description: MESessionPresentationSwitch event handler
//
/////////////////////////////////////////////////////////////////////////



void OnMESessionTopologyStatus(TopologyStatusInfo *ptopo)
{
    WCHAR szDurationString [MAX_TIME_STRING];
    WCHAR szSegmentInfoString[MAX_STRING_SIZE];
    
    HRESULT hr = S_OK;

    MFTIME hnsSegmentDuration = 0;
    
    int iIndex = 0, iSegID = 0;

	hr = g_pPlayer->GetSegmentInfo(ptopo->iSegmentId, &hnsSegmentDuration, szSegmentInfoString, MAX_STRING_SIZE);

    FormatTimeString(hnsSegmentDuration, szDurationString);
    
    SetDlgItemText(g_hWnd, IDC_DURATION, szDurationString);

    //find the corresponding iIndex in the listbox
    while (iSegID != ptopo->iSegmentId)
    {
        iSegID = (DWORD)SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_GETITEMDATA, iIndex, 0);
        iIndex++;
    }

    SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_SETCURSEL, --iIndex, 0);

	switch (ptopo->iTopologyStatusType)
	{
		case MF_TOPOSTATUS_READY:
			StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MESessionTopologyStatus: Segment %d ready to start.", ptopo->iSegmentId);

			break;
		case MF_TOPOSTATUS_STARTED_SOURCE:
			StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MESessionTopologyStatus: Reading data for segment %d.", ptopo->iSegmentId);

			break;
		case MF_TOPOSTATUS_SINK_SWITCHED:
			StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MESessionTopologyStatus: Switching to segment %d.", ptopo->iSegmentId); 
			break;

		case MF_TOPOSTATUS_ENDED:
			StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MESessionTopologyStatus: Playback ended for segment %d.", ptopo->iSegmentId); 

			break;
	}

	iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);

}
//////////////////////////////////////////////////////////////////////////
//  Name: OnMENewPresentation
//  Description: MENewPresentation event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMENewPresentation(MFSequencerElementId SegmentID)
{
    StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MENewPresentation: Prerolled segment %d", SegmentID); 
    int iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);
}


//////////////////////////////////////////////////////////////////////////
//  Name: OnMEEndOfPresentation
//  Description: MEEndOfPresentation event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMEEndOfPresentation(UINT value)
{
    SetDlgItemText(g_hWnd, IDC_PLAY, L"Play");
    EnableWindow(GetDlgItem(g_hWnd, IDC_STOP), FALSE);

	if(value == 1)
	{
		StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MEEndOfPresentation: Segment canceled by Sequencer Source. End of Playlist."); 
	}
	else
	{
		StringCchPrintf(szMessage, MAX_STRING_SIZE, L"MEEndOfPresentation: End of Playlist."); 
	}
    int iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);

    KillTimer( g_hWnd, g_TimerID );

}

//////////////////////////////////////////////////////////////////////////
//  Name: OnMEEndOfPresentation
//  Description: MEEndOfPresentation event handler
//
/////////////////////////////////////////////////////////////////////////

void OnMEEndOfPresentationSegment(UINT value)
{
	if (value == 1)
	{
		StringCchCopy(szMessage, MAX_STRING_SIZE, L"MEEndOfPresentationSegment: Segment canceled by Sequencer Source.");
	}
	else
	{
		StringCchCopy(szMessage, MAX_STRING_SIZE, L"MEEndOfPresentationSegment: Segment ended normally.");
	}
	int iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
	SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);
}

//////////////////////////////////////////////////////////////////////////
//  Name: OnSegmentAdded
//  Description: Updates the list box with the specified segment name
//
/////////////////////////////////////////////////////////////////////////

void OnSegmentAdded(MFSequencerElementId SegmentID)
{
    int iIndex = 0;
    
    WCHAR szSegmentURL[MAX_STRING_SIZE];

    WCHAR szDurationString[MAX_TIME_STRING];

    MFTIME hnsSegmentDuration = 0;

    HRESULT hr = S_OK;
    
    hr = g_pPlayer->GetSegmentInfo(SegmentID, &hnsSegmentDuration, szSegmentURL, MAX_STRING_SIZE);
    
    if(hr == E_FAIL)
    {
        NotifyError(L"Could not add segment", hr);
        return;
    }



    iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_INSERTSTRING, -1, (LPARAM)szSegmentURL);
    SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_SETITEMDATA, iIndex, SegmentID);


    if (SegmentID == 1)
    {
        FormatTimeString(hnsSegmentDuration, szDurationString);

        SetDlgItemText(g_hWnd, IDC_DURATION, szDurationString);
        
        SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_SETCURSEL, 0, 0);

    }
    
	StringCchPrintf(szMessage, MAX_STRING_SIZE, L"Added: %d", SegmentID);
	
	iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);

    EnableWindow(GetDlgItem(g_hWnd, IDC_PLAY), TRUE);

    EnableMenuItem(g_hMenu, IDM_REMOVEFROMPLAYLIST, MF_ENABLED);

}



//////////////////////////////////////////////////////////////////////////
//  Name: OnSegmentDeleted
//  Description: Deletes the specified segment from the listbox
//
/////////////////////////////////////////////////////////////////////////

void OnSegmentDeleted(MFSequencerElementId SegmentID)
{
    int SegID = 0, iIndex = 0;
    
    //find the corresponding iIndex in the listbox
    while (SegID != SegmentID)
    {
        SegID = (DWORD)SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_GETITEMDATA, iIndex, 0);
        iIndex++;
    }
    SendMessage(GetDlgItem(g_hWnd, IDC_PLAYLIST), LB_DELETESTRING, --iIndex, 0);
	
	StringCchPrintf(szMessage, MAX_STRING_SIZE, L"Deleted: %d", SegmentID);
	iIndex = (int)SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_ADDSTRING, 0, (LPARAM)szMessage);
    SendMessage(GetDlgItem(g_hWnd, IDC_MESSAGES), LB_SETTOPINDEX, --iIndex, 0);

}

//////////////////////////////////////////////////////////////////////////
//  Name: PlaybackUI
//  Description: Dialogbox callback. Handles the following messsages:
//      WM_INITDIALOG
//      WM_COMMAND
//      WM_TIMER
//      WM_DESTROY
//      WM_NOTIFY_APP (custom)
/////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK PlaybackUI(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    HRESULT hr = S_OK;

    g_hWnd = hDlg;

    switch (message)
    {
        case WM_INITDIALOG:
            return OnInitDialog();

        case WM_COMMAND:
            switch ( LOWORD(wParam) )
            {

                case IDM_ADDTOPLAYLIST:
                    OnAddToPlaylist();
                    break;

                case IDM_REMOVEFROMPLAYLIST:
                    OnRemoveFromPlaylist();
                    break;

                case IDC_PLAY:
                    OnPlay();
                    break;

                case IDC_STOP:
                    OnStop();
                    break;

                case IDC_PLAYLIST:
                    switch ( HIWORD(wParam) )
                    {
                        case LBN_DBLCLK:
                            {
                                int iIndex = (int)SendDlgItemMessage(g_hWnd, IDC_PLAYLIST, LB_GETCURSEL, 0, 0);
                                SkipToSegment(iIndex);
                                break;
                            }
                        
                        default:
                            break;
                    }
                    break;

                case IDM_EXIT:
                    DestroyWindow(g_hWnd);
                    break;
        
                default:
                    break;
            }
            break;
        
        case WM_NOTIFY_APP:
            switch(wParam)
            {
                case MESessionStarted:
                    OnMESessionStarted((DWORD)lParam);
                    break;
                    
                case MESessionPaused:
                    OnMESessionPaused((DWORD)lParam);
                    break;

                case MESessionStopped:
                    OnMESessionStopped((DWORD)lParam);
                    break;

                case MESessionTopologyStatus:
                    OnMESessionTopologyStatus((TopologyStatusInfo*)lParam);
                    break;

                case MEEndOfPresentationSegment:
						OnMEEndOfPresentationSegment((UINT)lParam);
						break;

				case MEEndOfPresentation:
                    OnMEEndOfPresentation((UINT)lParam);
                    break;
                    
                case MENewPresentation:
                    OnMENewPresentation((DWORD)lParam);
                    break;
                    
                case PLAYER_INITIALIZED:
                    SetPlayerStateString((DWORD)lParam);
                    break;
                
                case PLAYER_SEGMENT_ADDED:
                    OnSegmentAdded((DWORD)lParam);
                    break;

                case PLAYER_SEGMENT_DELETED:
                    OnSegmentDeleted((DWORD)lParam);
                    break;

                default:
                    break;
            }
            
            break;

        case WM_TIMER:
            {
                WCHAR szTimeString [MAX_TIME_STRING];
                StringCchPrintf(szTimeString, MAX_STRING_SIZE, L"");

                MFTIME hnsCurrentTime = 0;

				MFTIME hnsPresentationTime = 0;

                g_pPlayer->GetCurrentSegmentTime(&hnsCurrentTime);

				g_pPlayer->GetPresentationTime(&hnsPresentationTime);

                FormatTimeString(hnsCurrentTime, szTimeString);
                SetDlgItemText(g_hWnd, IDC_TIME, (LPCWSTR)szTimeString);

				FormatTimeString(hnsPresentationTime, szTimeString);
                SetDlgItemText(g_hWnd, IDC_PRESENTATION_TIME, (LPCWSTR)szTimeString);

                SetTimer(g_hWnd, g_TimerID, g_TimerInterval, NULL );
                
                break;
            }
        case WM_DESTROY:
            {
                PostQuitMessage(0);

                break;
            }

        default:
            DefWindowProc(hDlg, message, wParam, lParam ); 

        }
    return (INT_PTR)FALSE;
}


//winmain
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE /* hPrevInstance */,
                     LPTSTR    /* lpCmdLine */,
                     int       nCmdShow)

{
    g_hInst = hInstance;
    
    TRACE_INIT(L"log.txt");
    
    DialogBox( g_hInst, (LPCTSTR)IDD_MAIN, NULL, PlaybackUI );

    if (g_pPlayer)
    {
        delete g_pPlayer;
    }
    TRACE_CLOSE();

    
    return 0;
}

