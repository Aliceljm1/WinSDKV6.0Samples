
/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    GroupBrowser.c

Abstract:

    This C file includes sample code for a browser application built
    with the Peer-to-Peer Grouping API.

Feedback:
    If you have any questions or feedback, please contact us using
    any of the mechanisms below:

    Email: peerfb@microsoft.com
    Newsgroup: Microsoft.public.win32.programmer.networks
    Website: http://www.microsoft.com/p2p

--********************************************************************/

#include "groupbrowser.h"
#include <p2p.h>
#include <string.h>
// ----------------------------------------------------------------------------
// Global Variables.
// ----------------------------------------------------------------------------

HWND       g_hTree     = NULL; // The handle to the Tree View
HWND       g_hList     = NULL; // The handle to the List View
HTREEITEM  g_hRootTree = NULL; // The handle to the Root of the Tree View
HINSTANCE  g_hInst     = NULL; // The application instance
HWND       g_hwndMain  = NULL; // The main window
HMENU      g_hMenu     = NULL; // Handle to menu

// Constants for window resizing
const int c_dBorder = 7;
const int c_dBetweenBorder = 3;

// File Extensions
const PCWSTR c_wzFileExtInv = L"inv"; // Group Invitation
const PCWSTR c_wzFileExtIdt = L"idt"; // Peer Identity
const PCWSTR c_wsChooseCloudCaption = L"Choose Cloud..."; // Caption in the cloud drop down.

//-----------------------------------------------------------------------------
// Function: WinMain
//
// Purpose:  This is the main entry point for the application.
//
// Returns:  Nothing

int WINAPI WinMain(
        HINSTANCE hInstance,      // handle to current instance
        HINSTANCE hPrevInstance,  // handle to previous instance
        __in LPSTR lpCmdLine,          // command line
        int nCmdShow              // show state
)
{
    PEER_VERSION_DATA  peerVersion;
    HRESULT            hr;

    g_hInst = hInstance;

    // Initialize and start the peer group functionality.
    hr = PeerGroupStartup(PEER_GROUP_VERSION, &peerVersion);
    if (SUCCEEDED(hr))
    {
        InitCommonControls();
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDialogProc);
        PeerGroupShutdown();
    }
    else
    {
        DisplayHrError(L"One or more Group Browser components could not be initialized.  Make sure Peer-to-Peer is installed and enabled.", hr);
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Function: MainDialogProc
//
// Purpose:  DialogProc for the main window.
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//

LRESULT CALLBACK MainDialogProc(
        HWND hwndDlg,  // handle to dialog box
        UINT uMsg,     // message
        WPARAM wParam, // first message parameter
        LPARAM lParam  // second message parameter
)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            InitializeUIComponents(hwndDlg);
            return FALSE;
            break;
        case WM_CONTEXTMENU:
            if ((HWND)wParam == g_hTree)
            {
                DisplayTrackMenu(TreeView_GetSelection(g_hTree));
            } 
            else if ((HWND)wParam == g_hList)
            {
                DisplayListViewMenu(ListView_GetSelectionMark(g_hList));
            }
            break;
        case WM_SIZE:
            ResizeMainWindow(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_COMMAND:
        
            switch(LOWORD(wParam))
            {
                case ID_ABOUT:
                    DialogBox(g_hInst,
                          MAKEINTRESOURCE(IDD_ABOUT1),
                          hwndDlg,
                          AboutProc);
                    break;
                case ID_VIEW_REFRESH:
                    g_hRootTree = InitializeTreeView(g_hTree);
                    break;
                case ID_FILE_EXIT:
                case IDCANCEL:
                    CleanupTree(g_hTree);
                    EndDialog(hwndDlg,0);
                    return TRUE;
                    break;
            }
            break;
        case WM_NOTIFY:
            if (wParam == IDC_TREE)
            {
                HandleTreeViewMessages(hwndDlg, wParam, lParam);
                break;
            }
            else if (wParam == IDC_LIST)
            {
                HandleListViewMessages(hwndDlg, wParam, lParam);
            }
            break;
    }
    return FALSE;
}


// ----------------------------------------------------------------------------
// UI Component Initialization.
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: InitializeUIComponents
//
// Purpose: Initilize the UI components such as TreeView and ListView and
// initialize the global variables.
//
// Returns:  Nothing
//

void
InitializeUIComponents(HWND hwndDlg)
{
    // store the handle to the dialog box in the global variable.
    g_hwndMain = hwndDlg;

    // Get the handlers to the TreeView and ListView controls and store them
    // in the global variales.
    g_hTree = GetDlgItem(hwndDlg, IDC_TREE);
    g_hList = GetDlgItem(hwndDlg, IDC_LIST);

    g_hRootTree = InitializeTreeView(g_hTree);
    InitializeListView(g_hList);

    // Load the menubar.
    if((g_hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU))) != NULL)
    {
        SetMenu(hwndDlg, g_hMenu);
    }
}


//-----------------------------------------------------------------------------
// Function: InitializeTreeView
//
// Purpose:  Initialized the TreeView in the main window.
//
// Returns:  The Root Node
//

HTREEITEM
InitializeTreeView(HWND hwndTree)
{
    HTREEITEM hRoot = NULL;
    HRESULT   hr    = S_OK;

    // Free all data associated with each tree node and delete tree
    CleanupTree(hwndTree);

    // Create the root node.
    hRoot = InsertTreeViewItem(
                    hwndTree,
                    NULL ,
                    L"Identities",
                    NODETYPE_ROOT,
                    NULL);

    // Populate the TreeView with the existing identities and groups.
    hr = EnumIdentities(hRoot);
    if (FAILED(hr))
    {
        DisplayHrError(L"Unable to insert identities and groups in the TreeView.", hr);
    }

    // return the root node.
    return hRoot;

}


//-----------------------------------------------------------------------------
// Function: InitializeListView
//
// Purpose:  Initializes the List View in the Main Dialog
//
// Returns:  Nothing
//

void InitializeListView(HWND hwnd)
{
    RECT     rect;
    LVCOLUMN lvc  = {0};

    // Get the client's size
    GetClientRect(hwnd, &rect);

    // share it between 2 columns
    lvc.cx = (rect.right-rect.left) / 2;

    // insert the 1st column
    lvc.pszText = L"Item";
    lvc.cchTextMax = lstrlen(lvc.pszText);
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    ListView_InsertColumn(hwnd, 1, &lvc);

    // insert the second column
    lvc.pszText = L"Value";
    lvc.cchTextMax = lstrlen(lvc.pszText);
    ListView_InsertColumn(hwnd, 2, &lvc);
}

//-----------------------------------------------------------------------------
// Function: ResizeMainWindow
//
// Purpose:  Resizes the main window when 
//
// Returns:  nothing.
//
void ResizeMainWindow(int dxMain, int dyMain)
{
    RECT rect;
    INT  iTreeWidth;
    INT  iListWidth;
    HDWP hDWP = BeginDeferWindowPos(2);  //Resizing 2 windows


    iTreeWidth = (dxMain - 2*c_dBorder - c_dBetweenBorder)/3;
    iListWidth = dxMain - 2*c_dBorder - c_dBetweenBorder - iTreeWidth;

    GetWindowRect(g_hTree, &rect);
    hDWP = DeferWindowPos(hDWP, g_hTree, NULL, c_dBorder, c_dBorder, 
       iTreeWidth, (dyMain - 2*c_dBorder), SWP_NOZORDER | SWP_NOOWNERZORDER);

    GetWindowRect(g_hList, &rect);
    hDWP = DeferWindowPos(hDWP, g_hList, NULL, 
        c_dBorder+c_dBetweenBorder+iTreeWidth, c_dBorder, iListWidth, 
        (dyMain - 2*c_dBorder), SWP_NOZORDER | SWP_NOOWNERZORDER);

    EndDeferWindowPos(hDWP);

}


// ----------------------------------------------------------------------------
// UI Message Handlers.
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: HandleTreeViewMessages
//
// Purpose:  Handles the messages sent to the TreeView control.
//
// Returns:  nothing.
//

void HandleTreeViewMessages(HWND hwndDlg, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpnmhdr=(LPNMHDR)lParam;

    switch (lpnmhdr->code)
    {
        case NM_RCLICK: // Right click
        {
            TVHITTESTINFO ht = {0};
            
            // Find item under mouse and select it before displaying
            // the popup menu.

            // Get the position at which the right click happened.
            DWORD dwpos = GetMessagePos();
            ht.pt.x = GET_X_LPARAM(dwpos);
            ht.pt.y = GET_Y_LPARAM(dwpos);

            // Map the mouse position from the absolute co-ordinates to the
            // co-ordinates relative to the TreeView control.
            MapWindowPoints(HWND_DESKTOP, lpnmhdr->hwndFrom, &ht.pt, 1);

            // Determine the item in the tree located at this position and
            // select that item.
            TreeView_HitTest(lpnmhdr->hwndFrom, &ht);
            if (ht.hItem != NULL)
            {
                // We found the item under the right click. Select it and
                // display the popup menu.
                TreeView_SelectItem(g_hTree, ht.hItem);
                DisplayTrackMenu(ht.hItem);
            }
            break;
        }
        case TVN_SELCHANGED:
        {
            // The selection in the TreeView has been changed. Display the
            // appropriate contents in the ListView.
            LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) lParam;
            SynchronizeListView(pnmtv->itemNew.hItem);
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: HandleListViewMessages
//
// Purpose:  Handles the messages sent to the TreeView control.
//
// Returns:  nothing.
//
void HandleListViewMessages(HWND hwndDlg, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpnmhdr=(LPNMHDR)lParam;

    if (lpnmhdr->code == NM_RCLICK)
    {
        // Right click, we need to display the popup menu.
        LVHITTESTINFO ht = {0};

        // Get the position at which the right click happened.
        DWORD dwpos = GetMessagePos();
        ht.pt.x = GET_X_LPARAM(dwpos);
        ht.pt.y = GET_Y_LPARAM(dwpos);

        // Map the mouse position to position on ListView control.
        MapWindowPoints(HWND_DESKTOP, lpnmhdr->hwndFrom, &ht.pt, 1);

        // Determine the item, select it, and display popup
        if (ListView_HitTest(lpnmhdr->hwndFrom, &ht) != -1)
        {
            // display the popup menu.
            ListView_SetItemState(g_hList, ht.iItem, LVIS_FOCUSED, LVIS_SELECTED);
            DisplayListViewMenu(ht.iItem);
        }
    }
}

// ----------------------------------------------------------------------------
// TreeView Content Modification.
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: InsertTreeViewItem
//
// Purpose:  Inserts Items in Tree
//
// Returns:  Return the treeview item inserted
//

HTREEITEM
InsertTreeViewItem(HWND hwndTree, HTREEITEM hParent, PCWSTR pwcName, NODETYPE nt, LPVOID pData)
{
    TVINSERTSTRUCT  tvis        = {0};
    HTREEITEM       hItem       = NULL;
    PNODEINFO       pNodeInfo   = NULL;
    HRESULT         hr          = S_OK;

    hr = AllocNodeInfo(&pNodeInfo);
    if (FAILED(hr))
    {
        // memory allocation failed.
        return NULL;
    }


    // Store the node type
    pNodeInfo->nt = nt;

    // store the data (if any)
    pNodeInfo->pData = pData;

    // Who is the parent of the treeview node that we are about to add?
    // null means that this is a root node
    tvis.hParent = hParent;

    // Insert items in sorted order
    tvis.hInsertAfter = TVI_SORT;

    // tell the tree view which members of the tvis structure
    // are valid - in this case, the param, text and state members are valid
    tvis.itemex.mask = TVIF_PARAM | TVIF_TEXT | TVIF_STATE;

    // tells the tree view that it should display this node
    // in expanded view - ie., show it without requiring the
    // user to click on the parent's node first
    tvis.itemex.state = TVIS_EXPANDED;

    // any bits specified in state must be included in
    // the stateMask as well
    tvis.itemex.stateMask = TVIS_EXPANDED;

    // set the name of this treeview node
    tvis.itemex.pszText = (PWSTR) pwcName;

    // specify the number of CHARACTERS (as opposed to bytes)
    // in tvis.itemex.pszTest
    tvis.itemex.cchTextMax = lstrlen(tvis.itemex.pszText );

    // Store the associated data (for p2p api's later use)
    tvis.itemex.lParam = (LPARAM)pNodeInfo;

    // Try to insert the tree view node
    if((hItem = TreeView_InsertItem(hwndTree, &tvis)) == NULL)
    {
        // it failed, so cleanup the data
        FreeNodeInfo(pNodeInfo);
    }

    

    // Return the treeview node / NULL.
    return hItem;
}

//-----------------------------------------------------------------------------
// Function: FreeNodeData
//
// Purpose:  Free the data associated for each node in the treeview.
//
// Returns:  S_OK if successfull.  Otherwise returns appropriate error code
//

HRESULT
FreeNodeData(HWND hwndTree)
{
    HTREEITEM           hti_ch          = NULL;
    PNODEINFO           pIdNodeInfo     = NULL;
    HRESULT             hr              = S_OK;

    if (hwndTree == NULL)
    {
        return E_HANDLE;
    }

    // Should get the first identity under root
    hti_ch = TreeView_GetChild(g_hTree, g_hRootTree);

    while(hti_ch != NULL)
    {
        pIdNodeInfo   = GetNodeInfo(hti_ch);
        if (pIdNodeInfo != NULL)
        {
            if (pIdNodeInfo->nt == NODETYPE_IDENTITY)
            {
                // We found an identity, now delete any
                // group data associated with that identity
                DeleteGroups(hti_ch);

                // Delete Identity data
                DeleteId(hti_ch);
            }
        }

        // Go to next sibling (which should be an Identity type)
        hti_ch = TreeView_GetNextSibling(g_hTree, hti_ch);
    }

    return hr;

}


//-----------------------------------------------------------------------------
// Function: DeleteGroups
//
// Purpose:  Free the data associated to each group in the treeview.
//
// Returns:  S_OK if successfull.  Otherwise returns appropiate error code
//

HRESULT
DeleteGroups(HTREEITEM hti_id)
{
    HTREEITEM           hti_grp         = NULL;
    PNODEINFO           pNodeInfo       = NULL;
    PGROUP_NODE_DATA    pGrpNodeData    = NULL;
    PPEER_NAME_PAIR     pPeerNamePair   = NULL;
    HRESULT             hr              = S_OK;

    if (hti_id == NULL)
    {
        return E_HANDLE;
    }

    // Should get the first group under identity
    hti_grp = TreeView_GetChild(g_hTree, hti_id);

    while(hti_grp != NULL)
    {
        // Delete this group's data...

        // Get the information about this group node.
        pNodeInfo = GetNodeInfo(hti_grp);

        // Make sure there's node data to delete first
        if (pNodeInfo != NULL)
        {
            // Make sure node is a group type
            if (pNodeInfo->nt == NODETYPE_GROUP)
            {
                pGrpNodeData = (PGROUP_NODE_DATA)pNodeInfo->pData;
                if (pGrpNodeData != NULL)
                {
                    FreePeerNamePair(pGrpNodeData->pPeerNamePair);
                    FreeGroupNodeData(pGrpNodeData);
                }
                FreeNodeInfo(pNodeInfo);
            }
        }
        // Go to next sibling (which should be a group type)
        hti_grp = TreeView_GetNextSibling(g_hTree, hti_grp);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: DeleteId
//
// Purpose:  Free the data associated to each identity in the treeview.
//
// Returns:  S_OK if successfull.  Otherwise returns appropiate error code
//


HRESULT
DeleteId(HTREEITEM hti_id)
{
    PNODEINFO           pIdNodeInfo     = NULL;
    PIDENTITY_NODE_DATA pIdNodeData     = NULL;
    PPEER_NAME_PAIR     pPeerNamePair   = NULL;
    HRESULT             hr              = S_OK;

    // Make sure there's a tree view item
    if (hti_id != NULL)
    {
        pIdNodeInfo = GetNodeInfo(hti_id);

        if (pIdNodeInfo != NULL)
        {
            // Verify node is an Identity type
            if (pIdNodeInfo->nt == NODETYPE_IDENTITY)
            {
                // Free the data associated with the identity.
                pIdNodeData = (PIDENTITY_NODE_DATA)pIdNodeInfo->pData;
                if (pIdNodeData != NULL)
                {
                    FreePeerNamePair(pIdNodeData->pPeerNamePair);
                    FreeIdentityNodeData(pIdNodeData);
                }
                FreeNodeInfo(pIdNodeInfo);
            }

        }
    }
    else
    {
        hr = E_HANDLE;
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Function: GetNodeInfo
//
// Purpose:  Get the information about the specified node.
//
// Returns:  Node information.
//
PNODEINFO
GetNodeInfo(HTREEITEM htItem)
{
    PNODEINFO pNode  = NULL;
    TVITEMEX  tviex  = {0};

    tviex.hItem      = htItem;
    tviex.mask       = TVIF_HANDLE | TVIF_PARAM;

    if(TreeView_GetItem(g_hTree, &tviex))
    {
        pNode=(PNODEINFO)tviex.lParam;
    }

    return pNode;
}

// ----------------------------------------------------------------------------
// ListView Content Modification.
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Function: SynchronizeListView
//
// Purpose:  Makes sure that the list view is synchronized with the currently
// selected tree view node and to display the properties of the selected node
//
// Returns:  Nothing
void
SynchronizeListView(HTREEITEM htItem)
{
    PNODEINFO pNodeInfo = GetNodeInfo(htItem);
    if (NULL == pNodeInfo)
    {
        return;
    }

    // clear the list view
    ListView_DeleteAllItems(g_hList);

    switch(pNodeInfo->nt)
    {
        case NODETYPE_IDENTITY:
            {
                ListIdentityProperties(htItem, pNodeInfo);
                break;
            }

        case NODETYPE_GROUP:
            {
                ListGroupProperties(htItem, pNodeInfo);
                break;
            }
    }
}


//-----------------------------------------------------------------------------
// Function: ListIdentityProperties
//
// Purpose:  Displays the properties of an identity node in the ListView.
//
// Returns:  Nothing
void
ListIdentityProperties(HTREEITEM htItem, PNODEINFO pNodeInfo)
{
    LVITEM              lvi;
    int                 lvitem          = 0;
    PPEER_NAME_PAIR     pPeerNamePair   = NULL;
    PIDENTITY_NODE_DATA pIdNodeData     = NULL;

    // if the node information does not have application data we can not
    // display the properties of this identity node.
    if ( pNodeInfo->pData == NULL )
    {
        return;
    }

    pIdNodeData   = (PIDENTITY_NODE_DATA)pNodeInfo->pData;
    pPeerNamePair = pIdNodeData->pPeerNamePair;

    // Add the Identity Friendly Name.
    // LVITEM struct is used to add a
    // node to the list view. Initialize it to zeros
    ZeroMemory(&lvi, sizeof(lvi));

    // Set the mask to TEXT - since this is all
    // we care to display in the list view
    lvi.mask = LVIF_TEXT;

    // set the text member to the name
    lvi.pszText = L"Identity Friendly Name:";

    // set the length of the text (in CHARACTERS, not bytes)
    lvi.cchTextMax = lstrlen(lvi.pszText);

    // set the item number (lvitem was initialized to 0 above)
    // and increment the lvitem counter
    lvi.iItem = lvitem++;

    // insert the item.
    ListView_InsertItem(g_hList, &lvi);

    // Set the value of this item (the right-column's info)
    ListView_SetItemText(g_hList, lvi.iItem, 1, pPeerNamePair->pwzFriendlyName);

    // Add the peer name of the identity.
    lvi.pszText = L"Identity Peer Name:";
    lvi.cchTextMax = lstrlen(lvi.pszText);
    lvi.iItem = lvitem++;
    ListView_InsertItem(g_hList, &lvi);
    ListView_SetItemText(g_hList, lvi.iItem, 1, pPeerNamePair->pwzPeerName);

}

//-----------------------------------------------------------------------------
// Function: OpenGroup
//
// Purpose:  Opens a Group corresponding to the tree item htItem
//
// Returns:  Nothing
HGROUP OpenGroup(HTREEITEM htItem, PPEER_NAME_PAIR pGrpPeerNamePair)
{
    HTREEITEM               htParentItem            = NULL;
    PNODEINFO               pParentNodeInfo         = NULL;
    PIDENTITY_NODE_DATA     pIdNodeData             = NULL;
    HRESULT                 hr = S_OK;
    HGROUP                  hGroup = NULL;

    // Get the parent node (identity node)
    htParentItem = TreeView_GetParent(g_hTree, htItem);
    if (htParentItem == NULL)
    {
        return NULL;
    }

    // Get the nodeinfo for the parent node.
    pParentNodeInfo = GetNodeInfo(htParentItem);
    if (pParentNodeInfo == NULL)
    {
        return NULL;
    }

    pIdNodeData = (PIDENTITY_NODE_DATA)pParentNodeInfo->pData;
    if (pIdNodeData == NULL)
    {
        return NULL;
    }

    hr = PeerGroupOpen(
                pIdNodeData->pPeerNamePair->pwzPeerName,
                pGrpPeerNamePair->pwzPeerName,
                NULL,
                &hGroup);
    if (FAILED(hr))
    {
        DisplayHrError(L"PeerGroupOpen FAILED!", hr);
    }

    return hGroup;
}



//-----------------------------------------------------------------------------
// Function: ListGroupProperties
//
// Purpose:  Displays the propties of an group node in the ListView.
//
// Returns:  Nothing
void ListGroupProperties(HTREEITEM htItem, PNODEINFO pNodeInfo)
{
    LVITEM                  lvi                     = {0}; // used to add nodes
    int                     lvitem                  = 0;   // to ListView
    HRESULT                 hr                      = S_OK;
    PPEER_NAME_PAIR         pGrpPeerNamePair        = NULL;
    PGROUP_NODE_DATA        pGrpNodeData            = NULL;
    PPEER_GROUP_PROPERTIES  ppProperties            = NULL;
    BOOL                    groupValid              = FALSE;
    DWORD                   dwStatus                = 0;
    PWSTR                   pwzCreatorFriendlyName  = NULL;
    HPEERENUM               hGrpMemberEnum          = NULL;
    ULONG                   cCount                  = 0;
    ULONG                   cPresent                = 0;
    ULONG                   cOnline                 = 0;
    WCHAR                   wStr[MAX_STRING]        = {0};
    HGROUP                  hGroup;

    // if the node information does not have application data we can not
    // display the properties of this group node.
    if (NULL == pNodeInfo->pData)
    {
        return;
    }
    pGrpNodeData     = (PGROUP_NODE_DATA)pNodeInfo->pData;
    pGrpPeerNamePair = pGrpNodeData->pPeerNamePair;

    // if the group handle is not valid, try to open the group.
    if (pGrpNodeData->hGroup == NULL)
    {
        hGroup = OpenGroup(htItem, pGrpPeerNamePair);
    } 
    else
    {
        hGroup = pGrpNodeData->hGroup;
    }
    
    // if the group handle is non null, we have the group opened.
    if (hGroup)
    {
        hr = PeerGroupGetProperties(hGroup, &ppProperties);
        if FAILED(hr)
        {
            if (hr == PEER_E_GROUP_NOT_READY)
            {
                groupValid = TRUE;
            }
            else
            {
                DisplayHrError(L"PeerGroupGetProperties FAILED!", hr);
            }
        }
    }

    SetListItemContents(L"Group Friendly Name:", pGrpPeerNamePair->pwzFriendlyName, &lvitem);
    SetListItemContents(L"Group Peer Name:", pGrpPeerNamePair->pwzPeerName, &lvitem);
    
    // Add the group status information.
    if (ppProperties == NULL)
    {
        lvi.mask = LVIF_TEXT;
        lvi.pszText = L"Group Status:";
        lvi.cchTextMax = lstrlen(lvi.pszText);
        lvi.iItem = lvitem;
        ListView_InsertItem(g_hList, &lvi);

        if (groupValid)
        {
            // valid but not connected, so we do not have the properties.
            ListView_SetItemText(g_hList, lvitem++, 1, L"Not available");
        }
        else
        {
            ListView_SetItemText(g_hList, lvitem++, 1, L"Invalid");
        }
    }
    else
    {
        // Display group properties 
        lvi.mask = LVIF_TEXT;
        lvi.pszText = L"Group Status:";
        lvi.cchTextMax = lstrlen(lvi.pszText);
        lvi.iItem = lvitem;
        ListView_InsertItem(g_hList, &lvi);

        hr = PeerGroupGetStatus(hGroup, &dwStatus);
        if (FAILED(hr))
        {
            DisplayHrError(L"PeerGroupGetStatus FAILED!", hr);
            ListView_SetItemText(g_hList,lvi.iItem,1,L"Not available at this time.");
        }
        else
        {
            if (dwStatus & PEER_GROUP_STATUS_HAS_CONNECTIONS)
            {
                ListView_SetItemText(g_hList,lvitem++,1,L"Connected");
            }
            else if (dwStatus & PEER_GROUP_STATUS_LISTENING)
            {
                ListView_SetItemText(g_hList,lvitem++,1,L"Listening");
            }
            else
            {
                ListView_SetItemText(g_hList,lvitem++,1,L"Not Connected");
            }
        }
        SetListItemContents(L"Group Classifier:", ppProperties->pwzClassifier, &lvitem);
        SetListItemContents(L"PNRP Cloud Name:", ppProperties->pwzCloud, &lvitem);
        SetListItemContents(L"Group Comment:", ppProperties->pwzComment, &lvitem);
   
        // Get the friendly name of the group creator.
        if ( PeerIdentityGetFriendlyName(
                ppProperties->pwzCreatorPeerName,
                &pwzCreatorFriendlyName) == S_OK )
        {
            SetListItemContents(L"Group Creator:", pwzCreatorFriendlyName, &lvitem);
            PeerFreeData(pwzCreatorFriendlyName);
        }

        // count all present/nonpresent members
        cCount = GetMemberCount(hGroup, 0 /*ALL MEMBERS*/);
        cPresent = GetMemberCount(hGroup, PEER_MEMBER_PRESENT);

        StringCbPrintf(wStr, sizeof(wStr), L"%d Present - %d Total ", cPresent, cCount);
        SetListItemContents(L"Group Membership:", wStr, &lvitem);
        SetListItemContents(L"Group Members:", NULL, &lvitem);

        // List group members.
        hr = PeerGroupEnumMembers(hGroup, 0, NULL, &hGrpMemberEnum);
        if (SUCCEEDED(hr))
        {
            PEER_MEMBER** ppMember = NULL;

            hr = PeerGetNextItem(hGrpMemberEnum, &cCount, &ppMember);
            if (SUCCEEDED(hr))
            {
                ULONG i;
                for (i = 0; i < cCount; i++)
                {
                    PWSTR  pwzMemberFriendlyName  = NULL;

                    // Add member name.
                    lvi.pszText = L"";
                    lvi.cchTextMax = lstrlen(lvi.pszText);
                    lvi.iItem = lvitem++;
                    ListView_InsertItem(g_hList, &lvi);
                    SetMemberNameInListView(ppMember[i], lvi.iItem);
                }
                PeerFreeData(ppMember);
            }
            else
            {
                DisplayHrError(L"PeerGetNextItem FAILED!", hr);
            }

            PeerEndEnumeration(hGrpMemberEnum);
        }
        else
        {
            DisplayHrError(L"PeerGroupEnumMembers FAILED.", hr);
        }

        PeerFreeData(ppProperties);
        
    }// end of else
}

//-----------------------------------------------------------------------------
// Function: SetListItemContents
//
// Purpose:  Given a pointer to a PEER_MEMBER struct and an
//           item number, inserts the name corresponding to
//           that PEER_MEMBER on the List View item correspoinding
//           to iItem
//
// Returns:  Nothing
//
void SetListItemContents(PCWSTR pwzTitleText, PWSTR pwzItemText, INT* lvitem)
{
    LVITEM lvi = {0};
    HRESULT hr = S_OK;

    lvi.mask = LVIF_TEXT;
    lvi.pszText = (LPTSTR) pwzTitleText;
    lvi.cchTextMax = lstrlen(lvi.pszText);
    lvi.iItem = (*lvitem)++;
    ListView_InsertItem(g_hList, &lvi);
    ListView_SetItemText(g_hList, lvi.iItem, 1, pwzItemText);
}



//-----------------------------------------------------------------------------
// Function: SetMemberNameInListView
//
// Purpose:  Given a pointer to a PEER_MEMBER struct and an
//           item number, inserts the name corresponding to
//           that PEER_MEMBER on the List View item correspoinding
//           to iItem
//
// Returns:  Nothing
//
void SetMemberNameInListView(PPEER_MEMBER pMember, INT iItem)
{
    HRESULT hr = S_OK;
    PWSTR   pwzMemberFriendlyName = NULL;

    if (pMember->pCredentialInfo)
    {
        // get friendly name from credential info (if valid)
        ListView_SetItemText(g_hList, iItem, 1,
            (PWSTR) pMember->pCredentialInfo->pwzFriendlyName);
    }
    else
    {
        // Try to get the friendly name using the identity APIs.
        hr = PeerIdentityGetFriendlyName(
                pMember->pwzIdentity, &pwzMemberFriendlyName);
        if (SUCCEEDED(hr))
        {
            ListView_SetItemText(g_hList, iItem, 1, pwzMemberFriendlyName);
            PeerFreeData(pwzMemberFriendlyName);
        }
        else
        {
            // Display the identity.
            ListView_SetItemText(g_hList, iItem, 1, 
                (PWSTR)pMember->pwzIdentity);
        }
    }
}


//-----------------------------------------------------------------------------
// Function: DisplayItemMenu
//
// Purpose:  Sets the submenu's possible actions depending on item selected
//
// Returns:  Nothing
//

void DisplayListViewMenu(INT iItem)
{
    PNODEINFO   pNodeInfo   = NULL;
    HMENU       hMenu       = NULL;
    WCHAR       wzTextBuffer[256] = {0};
    int         iCmd        = 0;
    int         iRtn        = 0;
    POINT       point;
    MENUITEMINFO mi = {0};

    // Initialize a popup menu
    hMenu = CreatePopupMenu();
    if(hMenu == NULL) 
    {
        return;
    }

    // initialize and insert menu item
    mi.cbSize      = sizeof(mi);
    mi.fMask       = MIIM_STRING | MIIM_ID;
    mi.fType       = MFT_STRING;
    mi.fState      = MFS_ENABLED;
    mi.dwTypeData  = L"Copy to Clipboard";
    mi.cch         = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID         = 1;
    InsertMenuItem(hMenu,0,FALSE,&mi);

    // Get the current mouse position
    GetCursorPos(&point);

    // Display popup menu and get selection
    iCmd = TrackPopupMenu(hMenu,
                TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
                point.x,
                point.y,
                0,
                g_hList,
                NULL);
    
    // Since there is only one selection, we know
    // if there was success, the user wants to copy the
    // current item to the clipboard
    if(iCmd != 0)
    {
        
        ListView_GetItemText(g_hList, iItem, 1, wzTextBuffer, sizeof(wzTextBuffer));
        if (OpenClipboard(NULL))      
        {
            DWORD dwLen = (DWORD) (sizeof(WCHAR)*(wcslen(wzTextBuffer)+1));
            HGLOBAL hMem;
            WCHAR*  pwzMem;
            hMem = GlobalAlloc(GMEM_MOVEABLE, dwLen);
            if (hMem != NULL)
            {
                pwzMem = (WCHAR*)GlobalLock(hMem);
                memcpy(pwzMem, wzTextBuffer, dwLen);
                GlobalUnlock(hMem);

                EmptyClipboard();
                SetClipboardData(CF_UNICODETEXT, hMem);
            }   
            CloseClipboard();
        }
    }
    DestroyMenu(hMenu);
}

//-----------------------------------------------------------------------------
// Function: DisplayTrackMenu
//
// Purpose:  Sets the submenu's possible actions depending
// on node selected
//
// Returns:  Nothing
//

void DisplayTrackMenu(HTREEITEM htItem)
{
    PNODEINFO   pNodeInfo   = NULL;
    HMENU       hMenu       = NULL;
    int         iCmd        = 0;
    int         iRtn        = 0;
    POINT       point;

    // Get the information about the selected node on which we want to display
    // the track menu.
    pNodeInfo = GetNodeInfo(htItem);
    if (pNodeInfo == NULL)
    {
        // can not get the node information, return
        // without displaying the track menu.
        return;
    }

    // Initialize a popup menu
    if((hMenu = CreatePopupMenu()) == NULL) return;

    // Get the current mouse position
    GetCursorPos(&point);

    // Create the menu items for the different node types.
    switch(pNodeInfo->nt)
    {
        // Popup menu for the root node.
        case NODETYPE_ROOT:
            CreateRootNodePopupMenu(hMenu, pNodeInfo);
            iCmd = TrackPopupMenu(hMenu,
                TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
                point.x,
                point.y,
                0,
                g_hTree,
                NULL);

            if(iCmd != 0)
            {
                DialogBox(g_hInst,
                    MAKEINTRESOURCE(IDD_CreateIdentity),
                    g_hwndMain,
                    CreateIdentityDialogProc);
            }
            break;
        // Popup menu for the identity node.
        case NODETYPE_IDENTITY:
            CreateIdentityNodePopupMenu(hMenu, pNodeInfo);

            iCmd = TrackPopupMenu(hMenu,
                TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD,
                point.x,
                point.y,
                0,
                g_hTree,
                NULL);

            ExecuteIdentityNodePopupMenuCommand(iCmd, htItem, pNodeInfo);
            break;
        // Popup menu for the group node.
        case NODETYPE_GROUP:
            CreateGroupNodePopupMenu(hMenu, pNodeInfo);

            iCmd = TrackPopupMenu(hMenu,
                TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD,
                point.x,
                point.y,
                0,
                g_hTree,
                NULL);

            ExecuteGroupNodePopupMenuCommand(iCmd, htItem, pNodeInfo);
            break;
    }
    DestroyMenu(hMenu);
}


//-----------------------------------------------------------------------------
// Function: CreateRootNodePopupMenu
//
// Purpose:  Create the popup menu for the root node.
//
// Returns:  Nothing.
//

void
CreateRootNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo)
{
    MENUITEMINFO mi;

    // Zero the mi struct
    ZeroMemory(&mi, sizeof(mi));

    mi.cbSize      = sizeof(mi);
    mi.fMask       = MIIM_STRING | MIIM_ID;
    mi.fType       = MFT_STRING;
    mi.fState      = MFS_ENABLED;
    mi.dwTypeData  = L"Create New Identity";
    mi.cch         = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID         = 1;

    // insert the item into the menu
    InsertMenuItem(hMenu,0,FALSE,&mi);
}

//-----------------------------------------------------------------------------
// Function: CreateIdentityNodePopupMenu
//
// Purpose:  Create the popup menu for the identity node.
//
// Returns:  Nothing.
//

void
CreateIdentityNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo)
{
    MENUITEMINFO    mi;

    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize  = sizeof(mi);
    mi.fMask   = MIIM_STRING | MIIM_ID;
    mi.fType   = MFT_STRING;
    mi.fState  = MFS_ENABLED;

    mi.dwTypeData = L"Create Group";
    mi.cch        = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID        = 1;
    InsertMenuItem(hMenu,0,TRUE,&mi);

    mi.dwTypeData = L"Join Group";
    mi.cch        = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID        = 2;
    InsertMenuItem(hMenu,1,TRUE,&mi);

    mi.dwTypeData = L"Save Identity Information";
    mi.cch        = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID        = 3;
    InsertMenuItem(hMenu,2,TRUE,&mi);

    mi.dwTypeData = L"Delete Identity";
    mi.cch        = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID        = 4;
    InsertMenuItem(hMenu,3,TRUE,&mi);
}


//-----------------------------------------------------------------------------
// Function: CreateGroupNodePopupMenu
//
// Purpose:  Create the popup menu for the group node.
//
// Returns:  Nothing.
//

void
CreateGroupNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo)
{
    MENUITEMINFO        mi;
    HRESULT             hr              = S_OK;
    PGROUP_NODE_DATA    pGrpNodeData    = NULL;
    PPEER_NAME_PAIR     pPeerNamePair   = NULL;
    HGROUP              hGroup          = NULL;
    DWORD               dwStatus        = 0;
    BOOL                bShowInvitation = FALSE;

    if ( pNodeInfo->pData == NULL)
    {
        return;
    }

    pGrpNodeData  = (PGROUP_NODE_DATA)pNodeInfo->pData;
    pPeerNamePair = pGrpNodeData->pPeerNamePair;
    hGroup        = pGrpNodeData->hGroup;

    // From the status of the group, determine what menu items we
    // want to display.
    if (NULL != hGroup)
    {
        bShowInvitation = TRUE;
    }
    else
    {
        bShowInvitation = FALSE;
    }


    // Now add the menu items.

    // 1. Create Invitation.
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize    = sizeof(mi);
    mi.fMask     = MIIM_STRING | MIIM_ID | MIIM_STATE;
    mi.fType     = MFT_STRING;
    if (bShowInvitation)
    {
        mi.fState = MFS_ENABLED;
    }
    else
    {
        mi.fState = MFS_DISABLED;
    }
    mi.dwTypeData  = L"Create Invitation";
    mi.cch         = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID         = 1;
    InsertMenuItem(hMenu,0,TRUE,&mi);

    // 4. Delete Group.
    mi.fState      = MFS_ENABLED;
    mi.dwTypeData  = L"Delete Group";
    mi.cch         = lstrlen((WCHAR*)mi.dwTypeData);
    mi.wID         = 4;
    InsertMenuItem(hMenu,3,TRUE,&mi);
}


//-----------------------------------------------------------------------------
// Function: ExecuteIdentityNodePopupMenuCommand
//
// Purpose:  Execute the specified command from the identity node popup menu.
//
// Returns:  Nothing.
//

void
ExecuteIdentityNodePopupMenuCommand(int iCmd, HTREEITEM htItem, PNODEINFO pNodeInfo)
{
    switch(iCmd)
    {
        case 1:
        {
            DialogBoxParam(
                    g_hInst,
                    MAKEINTRESOURCE(IDD_CREATENEWGROUP),
                    g_hwndMain,
                    CreateGroupDialogProc,
                    (LPARAM) htItem);
            break;
        }

        case 3:
        {
            DialogBoxParam(
                    g_hInst,
                    MAKEINTRESOURCE(IDD_SAVEIDENTITYINFO),
                    g_hwndMain,
                    SaveIdentityInfoProc,
                    (LPARAM) htItem);
            break;
        }

        case 4:
        {
            DeleteIdentity(htItem);
            break;
        }
    }
}


//-----------------------------------------------------------------------------
// Function: ExecuteGroupNodePopupMenuCommand
//
// Purpose:  Execute the specified command from the group node popup menu.
//
// Returns:  Nothing.
//

void
ExecuteGroupNodePopupMenuCommand(int iCmd, HTREEITEM htItem, PNODEINFO pNodeInfo)
{
    switch(iCmd)
    {
        case 1:  // CreateInvitation Command
        {
            DialogBoxParam(
                    g_hInst,
                    MAKEINTRESOURCE(IDD_CREATEINVITATION),
                    g_hwndMain,
                    CreateInvitationProc,
                    (LPARAM) htItem);
            break;
        }

        case 4: // DeleteGroup Command.
        {
            DeleteGroup(htItem);
            break;
        }
    }
}


//-----------------------------------------------------------------------------
// Identity Operations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: CreateIdentityDialogProc
//
// Purpose:  DialogProc to create a new identity
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//

LRESULT CALLBACK CreateIdentityDialogProc(
    HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    switch(uMsg)
    {

        case WM_INITDIALOG:
        {
            return TRUE;
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDOK:
                    if (SUCCEEDED(HandleCreateIdentity(hwndDlg)))
                    {
                        EndDialog(hwndDlg, IDOK);
                        break;
                    }
                case IDCANCEL:
                    {
                        EndDialog(hwndDlg,IDCANCEL);
                        break;
                    }
            }
            break;
        }
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
// Function: HandleCreateIdentity
//
// Purpose:  Extracts the friendly name from the dialog and
//           creates a new identity.
//
// Returns:  HRESULT
//
HRESULT HandleCreateIdentity(HWND hDlg)
{
    HRESULT              hr                             = S_OK;
    WCHAR                wzName[MAX_IDENTITY]           = L"";
    WCHAR                wzClassifier[MAX_CLASSIFIER]   = L"";
    HTREEITEM            hTreeNode                      = NULL;
    HPEERENUM            hPeerEnum                      = NULL;
    ULONG                cIdentities                    = 0;
    PEER_NAME_PAIR       **pnp                          = NULL;
    PWSTR                pwzIdentity                    = NULL;
    PIDENTITY_NODE_DATA  pIdNodeData                    = NULL;


    UINT cch = GetDlgItemText(hDlg, IDC_EDT_FRIENDLYNAME, wzName, celems(wzName));
    if (0 == cch)
    {
        hr = E_INVALIDARG;
        MessageBox(hDlg, L"Please type a name for the identity.", L"Create an Identity", MB_OK);
    }

    GetDlgItemText(hDlg, IDC_EDT_INV_LOCATION, wzClassifier, celems(wzClassifier));

    if (SUCCEEDED(hr))
    {
        hr = PeerIdentityCreate(wzClassifier, wzName, 0, &pwzIdentity);
        if (SUCCEEDED(hr))
        {
            // Create a new node in the tree to represent this identity and
            // associate the appropriate data with it.
            hr = AllocIdentityNodeData(&pIdNodeData);
            if (SUCCEEDED(hr))
            {
                // Node data allocated, get the PeerNamePair data.
                hr = AllocPeerNamePair(wzName, pwzIdentity, &pIdNodeData->pPeerNamePair);
                if (SUCCEEDED(hr))
                {
                    hTreeNode = InsertTreeViewItem(
                            g_hTree,
                            g_hRootTree,
                            wzName,
                            NODETYPE_IDENTITY,
                            pIdNodeData);
                    if ( hTreeNode == NULL )
                    {
                        DisplayHrError(L"Failed to insert item in the TreeView!", hr);
                        hr = E_HANDLE;

                        FreePeerNamePair(pIdNodeData->pPeerNamePair);
                        FreeIdentityNodeData(pIdNodeData);
                    }
                }
                else
                {
                    DisplayHrError(L"Failed to allocate memory for PEER_NAME_PAIR!", hr);
                    FreeIdentityNodeData(pIdNodeData);
                }
            }// end of AllocIdentityNodeData success.
            else
            {
               DisplayHrError(L"Can not allocate memory for IDENTITY_NODE_DATA!", hr);
            }

            PeerFreeData(pwzIdentity);
        }   // end of PeerCreateIdentity SUCCESS.
        else
        {
            DisplayHrError(L"Failed to create an identity", hr);
        }
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function: DeleteIdentity
//
// Purpose:  Deletes the identity associated with the specified tree item.
//
// Returns:  HRESULT.
//

HRESULT
DeleteIdentity(HTREEITEM htItem)
{
    int                 iRtn            = 0;
    WCHAR               wzError[512];
    PNODEINFO           pNodeInfo       = NULL;
    PIDENTITY_NODE_DATA pIdNodeData     = NULL;
    PPEER_NAME_PAIR     pPeerNamePair   = NULL;
    HRESULT             hr              = S_OK;

    pNodeInfo   = GetNodeInfo(htItem);
    if (pNodeInfo == NULL)
    {
        return E_UNEXPECTED;
    }

    pIdNodeData = (PIDENTITY_NODE_DATA)pNodeInfo->pData;
    pPeerNamePair = pIdNodeData->pPeerNamePair;

    StringCchPrintf(
            wzError,
            celems(wzError),
            L"Are you sure you want to delete %s?\r\n",
            pPeerNamePair->pwzFriendlyName);

    iRtn = MessageBox(
                g_hwndMain,
                wzError,
                L"Delete Identity",
                MB_YESNOCANCEL | MB_ICONWARNING);

    if (iRtn == IDYES)
    {
        if (DoGroupsExist(pPeerNamePair) == FALSE)
        {
            hr = PeerIdentityDelete(pPeerNamePair->pwzPeerName);

            if SUCCEEDED(hr)
            {
                // Free the data associated with the identity.
                FreePeerNamePair(pPeerNamePair);
                FreeIdentityNodeData(pIdNodeData);
                FreeNodeInfo(pNodeInfo);

                // its gone, so remove it from the tree
                TreeView_DeleteItem(g_hTree, htItem);
            }
            else
            {
                DisplayHrError(L"PeerIdentityDelete FAILED.", hr);
            }
        }
        else
        {
             MessageBox(
                g_hwndMain,
                L"Unable to delete Identity.  Make sure there are no groups assoicated with this identity before attempting to delete it.",
                L"Error",
                MB_OK | MB_ICONWARNING);
        }

    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function: SaveIdentityInfoProc
//
// Purpose:  DialogProc to save the information for a local identity to a file.
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//
LRESULT CALLBACK SaveIdentityInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static PNODEINFO pNodeInfo;

    switch (message)
    {
    case WM_INITDIALOG:
        {
            pNodeInfo = GetNodeInfo((HTREEITEM)lParam);
            break;
        }

    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDCANCEL:
                    {
                        EndDialog(hDlg, IDCANCEL);
                        break;
                    }

                case IDOK:
                    {
                        if (SUCCEEDED(HandleSaveIdentityInfo(hDlg, pNodeInfo)))
                        {
                            EndDialog(hDlg, IDOK);
                        }
                        break;
                    }
                case IDC_BTN_BROWSE:
                    {
                        BrowseHelper(hDlg, IDC_EDT_LOCATION, L"Identity Information", c_wzFileExtIdt, FALSE);
                        break;
                    }
            }
            break;
        }

    default:
        break;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// Function: HandleSaveIdentityInfo
//
// Purpose:  Extracts the information from the dialog and calls
//           SaveIdentityInfo to do the actual work.
//
// Returns:  HRESULT
//
HRESULT HandleSaveIdentityInfo(HWND hDlg, PNODEINFO pNodeInfo)
{
    HRESULT            hr                  = S_OK;
    WCHAR              wzFile[MAX_PATH];
    PEER_NAME_PAIR    *pNewData            = NULL;
    LRESULT            cch;

    if (pNodeInfo == NULL)
    {
        return E_INVALIDARG;
    }

    pNewData = ((PIDENTITY_NODE_DATA)pNodeInfo->pData)->pPeerNamePair;

    cch = SendDlgItemMessage(hDlg, IDC_EDT_LOCATION, WM_GETTEXT, celems(wzFile), (LPARAM) wzFile);
    if (0 == cch)
    {
        MessageBox(hDlg, L"Please enter a filename where the identity will be saved.", L"Error", MB_OK);
        return E_INVALIDARG;
    }

    hr = SaveIdentityInfo(pNewData->pwzPeerName, wzFile);
    if (FAILED(hr))
    {
        DisplayHrError(L"Failed to save identity information.",hr);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: SaveIdentityInfo
//
// Purpose:  Saves the the information for an identity to a file.
//           Displays a message if there was an error.
//
// Returns:  HRESULT
//
HRESULT SaveIdentityInfo(PCWSTR pwzIdentity, PCWSTR pwzFile)
{
    PWSTR pwzXML        = NULL;
    HRESULT hr          = PeerIdentityGetXML(pwzIdentity, &pwzXML);

    if (FAILED(hr))
    {
        DisplayHrError(L"Unable to retreive the XML data for the identity.", hr);
    }
    else
    {
        FILE *pFile = NULL;
        errno_t err = 0;

        err = _wfopen_s(&pFile, pwzFile, L"wb");
        if (err != 0)
        {
            hr = E_POINTER;
            MessageBox(
                    NULL,
                    L"Please choose a valid path",
                    L"Save Identity Information",
                    MB_OK|MB_TASKMODAL);
        }
        else
        {
            fputws(pwzXML, pFile);
            fclose(pFile);
        }

        PeerFreeData(pwzXML);
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Group Operations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: CreateGroupDialogProc
//
// Purpose:  DialogProc to create a new group
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//

LRESULT CALLBACK CreateGroupDialogProc(
    HWND hwndDlg,  // handle to dialog box
    UINT uMsg,     // message
    WPARAM wParam, // first message parameter
    LPARAM lParam  // second message parameter
)
{
    static HTREEITEM htItem;
    switch(uMsg)
    {

        case WM_INITDIALOG:
        {
            htItem = (HTREEITEM)lParam;
            PopulateCloudList(hwndDlg);
            return FALSE;
            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDOK:
                if (SUCCEEDED(HandleCreateGroup(hwndDlg, htItem)))
                {
                    EndDialog(hwndDlg, IDOK);
                    break;
                }

                case IDCANCEL:
                    {
                        EndDialog(hwndDlg,0);
                        break;
                    }
            }
            break;
        }
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
// Function: HandleCreateGroup
//
// Purpose:  Creates a new group with the friendly name.
//
// Returns:  HRESULT
//
HRESULT HandleCreateGroup(HWND hDlg, HTREEITEM htItem)
{
    PNODEINFO   pNodeInfo                       = NULL;
    HRESULT     hr                              = S_OK;
    WCHAR       wzName[MAX_GROUPNAME]           = {0};
    WCHAR       wzClassifier[MAX_CLASSIFIER]    = {0};
    WCHAR       wzIdentity[MAX_IDENTITY]        = {0};
    WCHAR       wzComment[MAX_COMMENT]          = {0};
    WCHAR       wzCloud[MAX_CLOUDNAME]          = {0};

    WCHAR      *pwzIdentity                     = NULL;
    HTREEITEM   hTreeNode                       = NULL;
    HGROUP      hGroup                          = NULL;

    PEER_GROUP_PROPERTIES   props;
    PPEER_GROUP_PROPERTIES  pGroupProps         = NULL;
    PEER_NAME_PAIR          *pCreatorNamePair   = NULL;
    PGROUP_NODE_DATA        pGrpNodeData        = NULL;
    UINT                    cch                 = 0;

    pNodeInfo = GetNodeInfo(htItem);
    if (NULL == pNodeInfo)
    {
        return E_POINTER;
    }

    if (NULL == pNodeInfo->pData)
    {
        return E_POINTER;
    }
    pCreatorNamePair = ((PIDENTITY_NODE_DATA)pNodeInfo->pData)->pPeerNamePair;

    cch = GetDlgItemText(hDlg, IDC_EDT_GROUPNAME, wzName, celems(wzName));
    if (0 == cch)
    {
        MessageBox(hDlg, L"Please type a name for the group.", L"Create a Group", MB_OK);
        return E_INVALIDARG;
    }

    GetDlgItemText(hDlg, IDC_EDT_CLASSIFIER, wzClassifier, celems(wzClassifier));
    GetDlgItemText(hDlg, IDC_EDT_COMMENT, wzComment, celems(wzComment));
    GetDlgItemText(hDlg, IDC_CLOUDLIST, wzCloud, celems(wzCloud));

    if (wcscmp(wzCloud, c_wsChooseCloudCaption) == 0)
    {
        hr = E_INVALIDARG;
        MessageBox(hDlg, L"Pease choose a valid cloud name.", L"Create a Group", MB_OK);
        return hr;
    }

    // Set Group properties
    ZeroMemory(&props, sizeof(props));
    props.dwSize = sizeof(props);

    if(wzClassifier[0] == L'\0')
    {
        props.pwzClassifier    = NULL;
    }
    else
    {
        props.pwzClassifier    = wzClassifier;
    }


    props.pwzFriendlyName       = wzName;
    props.pwzComment            = wzComment;
    props.pwzCloud              = wzCloud;
    props.pwzCreatorPeerName    = pCreatorNamePair->pwzPeerName;

    // Call PeerGroupCreate
    hr = PeerGroupCreate(&props, &hGroup);
    if (SUCCEEDED(hr))
    {
        // Group has been created. Create the data associated with the
        // group node.
        hr = AllocGroupNodeData(&pGrpNodeData);
        if (SUCCEEDED(hr))
        {
            pGrpNodeData->hGroup = hGroup;
            hr = PeerGroupGetProperties(hGroup, &pGroupProps);
            if (SUCCEEDED(hr))
            {
                // allocate the new name pair and store it with the group node
                // data.
                hr = AllocPeerNamePair(
                        pGroupProps->pwzFriendlyName,
                        pGroupProps->pwzGroupPeerName,
                        &pGrpNodeData->pPeerNamePair);
                if (SUCCEEDED(hr))
                {
                    // insert the node in the tree view.
                    hTreeNode = InsertTreeViewItem(g_hTree,htItem, wzName,
                                        NODETYPE_GROUP, pGrpNodeData);

                    if (NULL == hTreeNode)
                    {
                        DisplayHrError(L"Failed to insert item in the TreeView!", hr);
                        hr = E_HANDLE;

                        // We are not able to insert the node in the tree.
                        // Free the data structures associated with this node.
                        FreePeerNamePair(pGrpNodeData->pPeerNamePair);
                        FreeGroupNodeData(pGrpNodeData);
                    }
                }
                else
                {
                    DisplayHrError(L"Failed to allocated PeerNamePair!", hr);
                    FreeGroupNodeData(pGrpNodeData);
                }

                // Free the group properties data.
                PeerFreeData(pGroupProps);
            }
            else
            {
                DisplayHrError(L"PeerGroupGetProperties FAILED!  Error %08x", hr);
            }
        }// Successful allocation of group data.
        else
        {
            DisplayHrError(L"Can not allocate memory for GROUP_NODE_DATA!", hr);
        }
    }
    else
    {
        DisplayHrError(L"Failed to create a new group!", hr);
    }
    return hr;
}


//-----------------------------------------------------------------------------
// Function: CreateInvitationProc
//
// Purpose:  DialogProc to create and save an invitation to a file.
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//
LRESULT CALLBACK CreateInvitationProc(HWND hDlg, UINT message, WPARAM wParam,
LPARAM lParam)
{
    static PNODEINFO pNodeInfo;

    switch (message)
    {
        case WM_INITDIALOG:
            {
                pNodeInfo = GetNodeInfo((HTREEITEM)lParam);
                break;
            }

        case WM_COMMAND:

            switch(LOWORD(wParam))
            {

                case IDC_BTN_IDENT_BROWSE:
                    {
                        BrowseHelper(hDlg, IDC_EDT_IDENT_LOCATION, L"Identity Information", c_wzFileExtIdt, TRUE);
                        break;
                    }
                case IDC_BTN_INV_BROWSE2:
                    {
                        BrowseHelper(hDlg, IDC_EDT_INV_LOCATION, L"Group Invitation", c_wzFileExtInv, FALSE);
                        break;
                    }
                case IDOK:
                    {
                        if (SUCCEEDED(HandleCreateInvitation(hDlg, pNodeInfo)))
                        {
                            EndDialog(hDlg, IDOK);
                        }
                        break;
                    }

                case IDCANCEL:
                    {
                        EndDialog(hDlg, IDCANCEL);
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
            break;

    default:
        {
            break;
        }
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// Function: HandleCreateInvitation
//
// Purpose:  Extracts the information from the dialog and calls CreateInvitation
//           to do the actual work.
//
// Returns:  HRESULT
//
HRESULT HandleCreateInvitation(HWND hDlg, PNODEINFO pNodeInfo)
{
    WCHAR wzIdentityInfo[MAX_PATH]  = {0};
    WCHAR wzInvitation[MAX_PATH]    = {0};

    // Set Invitation Location in UI
    SendDlgItemMessage(hDlg, IDC_EDT_INV_LOCATION, WM_GETTEXT,
            celems(wzInvitation), (LPARAM) wzInvitation);

    // Get Identity From UI
    SendDlgItemMessage(hDlg, IDC_EDT_IDENT_LOCATION, WM_GETTEXT,
            celems(wzIdentityInfo), (LPARAM) wzIdentityInfo);

    if ( wzIdentityInfo[0] == L'\0' || wzInvitation[0] == L'\0' )
    {
        MessageBox(hDlg, L"Please type a path for the Invitation and Identity.", L"Create an Invitation", MB_OK);
        return E_INVALIDARG;
    }

    return CreateInvitation(wzIdentityInfo, wzInvitation, pNodeInfo );
}

//-----------------------------------------------------------------------------
// Function: CreateInvitation
//
// Purpose:  Creates an invitation file for an identity.
//           Displays a message if there was an error.
//
// Returns:  HRESULT
//
HRESULT CreateInvitation(PCWSTR wzIdentityInfoPath, PCWSTR wzInvitationPath, PNODEINFO pNodeInfo)
{
    HRESULT                     hr                              = S_OK;
    PWSTR                       pwzInvitation                   = NULL;
    PGROUP_NODE_DATA            pGrpNodeData                    = (PGROUP_NODE_DATA)pNodeInfo->pData;
    DWORD                       dwNumberBytesRead               = 0;
    DWORD                       dwBytesWritten                  = 0;
    WCHAR                       *pwcInvitiationBuff             = NULL;
    HANDLE                      hFile                           = NULL;
    DWORD                       dwSize                          = 0;

    // Make sure we have this data, else no point in continuing...
    if(pGrpNodeData == NULL)
    {
        return E_INVALIDARG;
    }

    // Open the file
    if ((hFile=CreateFile(wzIdentityInfoPath,
        FILE_ALL_ACCESS,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL))
        == INVALID_HANDLE_VALUE) {

            hr = E_HANDLE;
            DisplayHrError(L"Please choose a valid path to the identity information file.", hr);
            return hr;
    }

    if((dwSize = GetFileSize (hFile, NULL))
        == INVALID_FILE_SIZE) {

            hr = E_HANDLE;
            DisplayHrError(L"Can't get file size!", hr);
            CloseHandle(hFile);
            return hr;
    }

    // Allocate memory for file
    if((pwcInvitiationBuff = (WCHAR*)LocalAlloc(LMEM_ZEROINIT, (dwSize + sizeof(WCHAR))))
        == NULL) {

            hr = E_OUTOFMEMORY;
            DisplayHrError(L"Can't allocate for invitation!", hr);
            CloseHandle(hFile);
            return hr;
        }

    // Read file
    if (ReadFile(hFile, (LPBYTE)pwcInvitiationBuff, dwSize, &dwNumberBytesRead, NULL) == FALSE)
    {
        hr = E_INVALIDARG;
        DisplayHrError(L"Can't read invitation file!", hr);
        CloseHandle(hFile);
        LocalFree(pwcInvitiationBuff);
        return hr;
    }

    // Done with the file - close its handle
    CloseHandle(hFile);

    hr = PeerGroupCreateInvitation(
            pGrpNodeData->hGroup,           // handle to group
            pwcInvitiationBuff,             // ptr to unicode string that contains the blob
            NULL,                            // filetime expiration
            1,                              // cRoles
            (PEER_ROLE_ID*) &PEER_GROUP_ROLE_ADMIN, // ptr to roles
            &pwzInvitation);                // ptr to string that contains the invitation

    if (FAILED(hr))
    {
        DisplayHrError(L"Failed to create the invitation.", hr);
        LocalFree(pwcInvitiationBuff);
        return hr;
    }

    // Free the invitation buffer
    LocalFree(pwcInvitiationBuff);

    // Open a new file for writing the invitation to disk
    if ((hFile=CreateFile(wzInvitationPath,
        FILE_ALL_ACCESS,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL))
        == INVALID_HANDLE_VALUE)
    {
        DisplayHrError(L"Please choose a valid path to the invitation file.", hr);
        PeerFreeData(pwzInvitation);
        return E_HANDLE;
    }

    if(WriteFile(hFile, (LPBYTE)pwzInvitation, sizeof(WCHAR) * (lstrlenW(pwzInvitation)+ 1), &dwBytesWritten, NULL)
        == FALSE)
    {
        DisplayHrError(L"Could not write invitation to disk", hr);

        // close file handle
        CloseHandle(hFile);
        // free the invitation data
        PeerFreeData(pwzInvitation);
        return E_INVALIDARG;
    }

    // Close file handle
    CloseHandle(hFile);

    // Free created invitation data from P2P api
    PeerFreeData(pwzInvitation);

    // We're done!
    return hr;
}

//-----------------------------------------------------------------------------
// Function: DeleteGroup
//
// Purpose:  Deletes the group associated with the specified tree item.
//
// Returns:  HRESULT.
//

HRESULT
DeleteGroup(HTREEITEM htItem)
{
    int                 iRtn            = 0;
    WCHAR               wzError[512];

    PNODEINFO           pNodeInfo       = NULL;
    PGROUP_NODE_DATA    pGrpNodeData    = NULL;

    HTREEITEM           htParentItem    = NULL;
    PNODEINFO           pParentNodeInfo = NULL;
    PIDENTITY_NODE_DATA pIdNodeData     = NULL;
    HRESULT             hr              = S_OK;

    // Get the parent node that is the identity node, if not found
    // return. This should not happen though.
    htParentItem = TreeView_GetParent(g_hTree, htItem);
    if ( htParentItem == NULL )
    {
        return E_HANDLE;
    }

    // Get the nodeinfo for the parent node.
    pParentNodeInfo = GetNodeInfo(htParentItem);
    if (pParentNodeInfo == NULL)
    {
        return E_POINTER;
    }

    pIdNodeData = (PIDENTITY_NODE_DATA)pParentNodeInfo->pData;

    // Get the information about this group node.
    pNodeInfo = GetNodeInfo(htItem);
    if (pNodeInfo == NULL)
    {
        return E_POINTER;
    }

    pGrpNodeData = (PGROUP_NODE_DATA)pNodeInfo->pData;

    // Make sure that the user wants to delete this group.
    StringCchPrintf(
            wzError,
            celems(wzError),
            L"Are you sure you want to delete %s?\r\n",
            pGrpNodeData->pPeerNamePair->pwzFriendlyName);

    iRtn = MessageBox(
                g_hwndMain,
                wzError, L"Delete Group",
                MB_YESNOCANCEL | MB_ICONWARNING);
    if (iRtn == IDYES)
    {
        if ( pGrpNodeData->hGroup != NULL )
        {
            // Close the group handle.
            hr = PeerGroupClose(pGrpNodeData->hGroup);
            if ( FAILED(hr) )
            {
                DisplayHrError(L"PeerGroupClose() FAILED.", hr);
            }
            else
            {
                pGrpNodeData->hGroup = NULL;
            }
        }


        // Try to delete the group anyway.
        hr = PeerGroupDelete(
            pIdNodeData->pPeerNamePair->pwzPeerName,
            pGrpNodeData->pPeerNamePair->pwzPeerName);

        if SUCCEEDED(hr)
        {
            // Free the data associated with this tree item.
            FreePeerNamePair(pGrpNodeData->pPeerNamePair);
            FreeGroupNodeData(pGrpNodeData);
            FreeNodeInfo(pNodeInfo);

            // its gone, so remove it from the tree
            TreeView_DeleteItem(g_hTree, htItem);
        }
        else
        {
            DisplayHrError(L"PeerGroupDelete() FAILED.", hr);
        }

    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: BrowseHelper
//
// Purpose:  Use the common dialog to get/set a path.
//
// Returns:  Nothing
//
void BrowseHelper(HWND hDlg, int idEditbox, PCWSTR pwzFileType, PCWSTR
pwzFileExtension, BOOL fOpen)
{
    OPENFILENAME     ofn                = {0};
    WCHAR            wzPath[MAX_PATH]   = {0};
    WCHAR            wzFilter[512]      = {0};
    BOOL             fSuccess           = FALSE;

    StringCbPrintf(wzFilter, sizeof(wzFilter), L"%s (*.%s)%c*.%s", pwzFileType,
                   pwzFileExtension, L'\0', pwzFileExtension);

    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFilter = wzFilter;
    ofn.lpstrFile = wzPath;
    ofn.lpstrDefExt = pwzFileExtension;
    ofn.nMaxFile = celems(wzPath);

    if (fOpen)
        fSuccess = GetOpenFileName(&ofn);
    else
        fSuccess = GetSaveFileName(&ofn);

    if (fSuccess)
    {
        SetDlgItemText(hDlg, idEditbox, wzPath);
    }
}

//-----------------------------------------------------------------------------
// Function:  DoGroupsExist
//
// Purpose:   Count the number of groups associated with an identity
//
// Returns:   TRUE if Groups exists.  Else FALSE
//

BOOL DoGroupsExist(PPEER_NAME_PAIR pIdPeerNamePair)
{
    HPEERENUM       hPeerGroupEnum  = NULL;
    HRESULT         hr              = S_OK;
    ULONG           cGroups         = 0;
    BOOL            GroupsExist     = FALSE;

    hr = PeerEnumGroups(pIdPeerNamePair->pwzPeerName, &hPeerGroupEnum);
    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hPeerGroupEnum, &cGroups);
        if (FAILED(hr))
        {
            DisplayHrError(L"PeerGetItemCount for groups FAILED.", hr);
        }

        else
        {
            if (cGroups != 0)
            {
                GroupsExist = TRUE;
            }
        }

        PeerEndEnumeration(hPeerGroupEnum);
    }
    else
    {
        DisplayHrError(L"PeerEnumGroups FAILED.", hr);
    }

    return GroupsExist;
}


//-----------------------------------------------------------------------------
// Function: GetMemberCount
//
// Purpose:  Counts the members of the group.
//
// Returns:  Count of members.
//

ULONG GetMemberCount(HGROUP phGroup, DWORD dwMemberType)
{
    ULONG       cMembers        = 0;
    HRESULT     hr              = S_OK;
    HPEERENUM   hGrpMemberEnum  = NULL;

    hr = PeerGroupEnumMembers(phGroup, dwMemberType, NULL, &hGrpMemberEnum);
    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hGrpMemberEnum, &cMembers);
        if (FAILED(hr))
        {
            DisplayHrError(L"PeerGetItemCount for members FAILED.", hr);
        }

        PeerEndEnumeration(hGrpMemberEnum);
    }
    else
    {
        DisplayHrError(L"PeerGroupEnumMembers FAILED.", hr);
    }

    return cMembers;
}



//-----------------------------------------------------------------------------
// Function: AboutProc
//
// Purpose:  DialogProc for the About box
//
// Returns:  Returns TRUE if it processed the message, and FALSE if it did not.
//
LRESULT CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case IDCANCEL:
            case IDOK:
                {
                    EndDialog(hDlg, LOWORD(wParam));
                    break;
                }
            default:
                break;
        }
        break;
    }
    default:
        break;
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
// Function: EnumIdentities
//
// Purpose:  Enumerate through all identities and add them to the TreeView.
//
// Returns:  HRESULT
//

HRESULT EnumIdentities(HTREEITEM hRoot)
{
    HPEERENUM       hPeerEnum   = NULL;
    HRESULT         hr          = S_OK;
    ULONG           cIdentities = 0;
    PEER_NAME_PAIR  **NewPnp    = NULL;

    hr = PeerEnumIdentities(&hPeerEnum);
    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hPeerEnum, &cIdentities);
        if (SUCCEEDED(hr))
        {
            hr = PeerGetNextItem(hPeerEnum, &cIdentities, &NewPnp);
            if (SUCCEEDED(hr))
            {
                ULONG i;
                for (i = 0; i < cIdentities; i++)
                {
                    HTREEITEM hIdentityTreeNode = NULL;
                    PIDENTITY_NODE_DATA pIdNodeData = NULL;

                    hr = AllocIdentityNodeData(&pIdNodeData);
                    if (SUCCEEDED(hr))
                    {
                        hr = DeepCopyPeerNamePair(NewPnp[i], &pIdNodeData->pPeerNamePair);
                        if(SUCCEEDED(hr))
                        {

                            hIdentityTreeNode = InsertTreeViewItem(
                                    g_hTree,
                                    hRoot,
                                    NewPnp[i]->pwzFriendlyName,
                                    NODETYPE_IDENTITY,
                                    pIdNodeData);
                            if (NULL == hIdentityTreeNode)
                            {
                                hr = E_UNEXPECTED;
                                DisplayHrError(L"Insert of identity node in the TreeView FAILED!", hr);
                                FreePeerNamePair(pIdNodeData->pPeerNamePair);
                                FreeIdentityNodeData(pIdNodeData);
                            }
                            else
                            {
                                hr = EnumGroups(pIdNodeData->pPeerNamePair, &hIdentityTreeNode);
                                if (FAILED(hr))
                                {
                                    DisplayHrError(L"EnumGroups FAILED!!", hr);
                                }
                            }
                        }
                        else
                        {
                            DisplayHrError(L"Copy of Identity PEER_NAME_PAIR FAILED!", hr);
                            FreeIdentityNodeData(pIdNodeData);
                        }
                    }
                    else
                    {
                        DisplayHrError(L"Allocation of the IDENTITY_NODE_DATA FAILED.!", hr);
                    }
               }// end of for loop

               PeerFreeData(NewPnp);
            }// end of PeerGetNextItem SUCCESS
            else
            {
                DisplayHrError(L"PeerGetNextItem FAILED!", hr);
            }
        }// end of PeerGetItemCount SUCCESS
        else
        {
            DisplayHrError(L"PeerGetItemCount FAILED!", hr);
        }

        PeerEndEnumeration(hPeerEnum);
    }
    else
    {
        DisplayHrError(L"PeerEnumIdentities FAILED!", hr);
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function: EnumGroups
//
// Purpose:  Enumerate through all the groups of specified identity and add
// them to the TreeView.
//
// Returns:  HRESULT
//

HRESULT EnumGroups(PPEER_NAME_PAIR pIdPeerNamePair, HTREEITEM *hTreeNode)
{
    HPEERENUM       hPeerGroupEnum  = NULL;
    HRESULT         hr              = S_OK;
    ULONG           cGroups         = 0;
    PEER_NAME_PAIR  **NewPnp        = NULL;

    hr = PeerEnumGroups(pIdPeerNamePair->pwzPeerName, &hPeerGroupEnum);
    if (SUCCEEDED(hr))
    {
        hr = PeerGetItemCount(hPeerGroupEnum, &cGroups);
        if (SUCCEEDED(hr))
        {
            hr = PeerGetNextItem(hPeerGroupEnum, &cGroups, &NewPnp);
            if (SUCCEEDED(hr))
            {
                ULONG i;
                for (i = 0; i < cGroups; i++)
                {
                    HTREEITEM hGroupTreeNode = NULL;
                    PGROUP_NODE_DATA pGrpNodeData = NULL;

                    hr = AllocGroupNodeData(&pGrpNodeData);
                    if (SUCCEEDED(hr))
                    {
                        hr = DeepCopyPeerNamePair(NewPnp[i], &pGrpNodeData->pPeerNamePair);
                        if(SUCCEEDED(hr))
                        {
                            pGrpNodeData->hGroup = NULL;

                            hGroupTreeNode = InsertTreeViewItem(
                                    g_hTree,
                                    *hTreeNode,
                                    NewPnp[i]->pwzFriendlyName,
                                    NODETYPE_GROUP,
                                    pGrpNodeData);


                            if (NULL == hGroupTreeNode)
                            {
                                hr = E_UNEXPECTED;
                                DisplayHrError(L"Insert of group node in the TreeView FAILED!", hr);
                                FreePeerNamePair(pGrpNodeData->pPeerNamePair);
                                FreeGroupNodeData(pGrpNodeData);
                            }
                        }
                        else
                        {
                            DisplayHrError(L"Copy of Group PEER_NAME_PAIR FAILED!", hr);
                            FreeGroupNodeData(pGrpNodeData);
                        }
                    }
                    else
                    {
                        DisplayHrError(L"Allocation of the GROUP_NODE_DATA FAILED.!", hr);
                    }

               }// end of for loop

               PeerFreeData(NewPnp);
            }// end of PeerGetNextItem SUCCESS
            else
            {
                DisplayHrError(L"PeerGetNextItem FAILED!", hr);
            }
        }// end of PeerGetItemCount SUCCESS
        else
        {
            DisplayHrError(L"PeerGetItemCount FAILED!", hr);
        }

        PeerEndEnumeration(hPeerGroupEnum);
    }
    else
    {
        DisplayHrError(L"PeerEnumGroups FAILED!", hr);
    }

    return hr;
}


//-----------------------------------------------------------------------------
// Function: MsgErrHr
//
// Purpose:  Display a message for a function with an error code.
//
// Returns:  nothing
//
void MsgErrHr(PCWSTR pwzMsg, HRESULT hr, PCSTR pszFunction)
{
    WCHAR wzError[512];

    StringCchPrintf(wzError, celems(wzError), L"%s\r\n\r\nFunction: %S\r\nHRESULT=0x%08X", pwzMsg, pszFunction, hr);

    MessageBox(
            NULL,
            wzError,
            L"Group Browser Error",
            MB_OK | MB_ICONWARNING | MB_TASKMODAL);
}


//-----------------------------------------------------------------------------
// Fuction: PopulateCloudList
//
// Purpose: Populates the Create Graph UI with all the clouds in the machine.
//
// Returns:
//    S_OK   - Successfully retrieved all the available clouds.
//    E_UNEXPECTED - Failed to retrieve available clouds.
//
HRESULT PopulateCloudList(HWND hDlg)
{
    DWORD           querySize       = 1;
    int             retVal          = 0;
    HANDLE          handle          = 0;
    WSAQUERYSET     querySet        = {0};
    BLOB            Blob;
    PNRPCLOUDINFO   CloudInfo       = {0};
    WSADATA         wsaData;
    HRESULT         hr              = S_FALSE;
    HWND            cloudList       = NULL;
    LRESULT         insertedItem    = 0;
    int             itemCount       = 0;

    cloudList = GetDlgItem(hDlg, IDC_CLOUDLIST);
    if (cloudList == NULL)
    {
        DisplayHrError(L"Unable to get an handle to the Cloud List Dialog Item.", 0);
        return E_INVALIDARG;
    }

    // Initialize WSA.
    hr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (FAILED(hr))
    {
        DisplayHrError(L"Unable to initialize WSA.", hr);
        return E_UNEXPECTED;
    }

    // Set the First item in the Cloud List box to be "Choose Cloud..."
    // and set current selection to that.
    insertedItem = SendMessage(cloudList, CB_INSERTSTRING, (WPARAM)itemCount,
                           (LPARAM)c_wsChooseCloudCaption);
    SendMessage(cloudList, CB_SETITEMDATA, (WPARAM)insertedItem, 
                            (LPARAM)itemCount);
    SendMessage(cloudList, CB_SETCURSEL, (WPARAM)itemCount, (LPARAM)0);
    ++itemCount;

    // Begin Cloud Enumeration for all clouds in this machine.
    CloudInfo.dwSize = sizeof(PNRPCLOUDINFO);
    CloudInfo.Cloud.Scope = PNRP_SCOPE_ANY;

    Blob.cbSize = sizeof(PNRPCLOUDINFO);
    Blob.pBlobData = (LPBYTE)&CloudInfo;

    querySet.dwSize = sizeof(WSAQUERYSET);
    querySet.dwNameSpace = NS_PNRPCLOUD;
    querySet.lpServiceClassId = (LPGUID)(&SVCID_PNRPCLOUD);
    querySet.lpBlob = &Blob;

    retVal = WSALookupServiceBegin(&querySet, LUP_RETURN_ALL, &handle);
    if (retVal != 0)
    {
        retVal = WSAGetLastError();
        DisplayHrError(L"WSALookupServiceBegin FAILED!", retVal);
        return retVal;
    }

    while (TRUE)
    {
        WSAQUERYSET queryResult = {0};
        querySize = sizeof(WSAQUERYSET);
        
        // We purposefully give it no space the first time so
        // it will fail and tell us how much space we need
        retVal = WSALookupServiceNext(handle, 0, &querySize, &queryResult);
        retVal = WSAGetLastError();

        if (retVal != WSAEFAULT)
        {
            break;
        }
        else
        {
            WSAQUERYSET *pResults = NULL;
            retVal = WSAGetLastError();

            pResults = (WSAQUERYSET*) LocalAlloc(LPTR, querySize);
            if (pResults == NULL)
            {
                hr = E_OUTOFMEMORY;
                break;
            }
            
            retVal = WSALookupServiceNext(handle, LUP_RETURN_NAME, &querySize, 
                                            pResults);

            // Insert the current retrieved cloud name to the List.
            if (retVal == 0)
            {
                insertedItem = SendMessage(cloudList, 
                    CB_INSERTSTRING, (WPARAM)itemCount, 
                    (LPARAM)pResults->lpszServiceInstanceName);
                SendMessage(cloudList, CB_SETITEMDATA, insertedItem, 
                    (LPARAM)itemCount++);
            }
            LocalFree(pResults);
        }
    }
    
    // End Cloud enumeration and shutdown WSA.
    WSALookupServiceEnd(handle);
    WSACleanup();
    return hr;
}


//-----------------------------------------------------------------------------
// Function: AllocNodeInfo
//
// Purpose:  Allocate memory for the NODEINFO sturcture.
//
// Returns:  S_OK if no errors
//           E_OUTOFMEMORY if memory allocations fail
//
HRESULT AllocNodeInfo(PNODEINFO *ppNodeInfo)
{
    HRESULT hr = S_OK;
    *ppNodeInfo = (PNODEINFO)HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            sizeof(NODEINFO));

    if (NULL == ppNodeInfo)
    {
        hr = E_OUTOFMEMORY;
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function: FreeNodeInfo
//
// Purpose:  Release the memory allocated for the NODEINFO sturcture
//           using the AllocNodeInfo function.
//
// Returns:  S_OK if no errors
//           E_UNEXPECTED if memory free operation fail
//
HRESULT FreeNodeInfo(PNODEINFO pNodeInfo)
{
    HRESULT hr = S_OK;
    if (!HeapFree(GetProcessHeap(), 0, pNodeInfo))
    {
        hr = E_UNEXPECTED;
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: AllocIdentityNodeData
//
// Purpose:  Allocate memory for the IDENTITY_NODE_DATA sturcture.
//
// Returns:  S_OK if no errors
//           E_OUTOFMEMORY if memory allocations fail
//
HRESULT AllocIdentityNodeData(PIDENTITY_NODE_DATA *ppIdNodeData)
{
    HRESULT hr = S_OK;

    *ppIdNodeData = (PIDENTITY_NODE_DATA)HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            sizeof(IDENTITY_NODE_DATA));

    if (NULL == *ppIdNodeData)
    {
        DisplayHrError(L"HeapAlloc Failed!", GetLastError());
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function: FreeIdentityNodeData
//
// Purpose:  Release the memory allocated for the IDENTITY_NODE_DATA sturcture
//           using the AllocIdentityNodeData function.
//
// Returns:  S_OK if no errors
//           E_UNEXPECTED if memory free operation fail
//
HRESULT FreeIdentityNodeData(PIDENTITY_NODE_DATA pIdNodeData)
{
    HRESULT hr = S_OK;
    if (!HeapFree(GetProcessHeap(), 0, pIdNodeData))
    {
        hr = E_UNEXPECTED;
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: AllocGroupNodeData
//
// Purpose:  Allocate memory for the GROUP_NODE_DATA sturcture.
//
// Returns:  S_OK if no errors
//           E_OUTOFMEMORY if memory allocations fail
//
HRESULT AllocGroupNodeData(PGROUP_NODE_DATA *ppGrpNodeData)
{
    HRESULT hr = S_OK;

    *ppGrpNodeData = (PGROUP_NODE_DATA)HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            sizeof(GROUP_NODE_DATA));

    if (NULL == *ppGrpNodeData)
    {
        DisplayHrError(L"HeapAlloc Failed!", GetLastError());
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: FreeGroupNodeData
//
// Purpose:  Release the memory allocated for the GROUP_NODE_DATA sturcture
//           using the AllocGroupNodeData function.
//
// Returns:  S_OK if no errors
//           E_UNEXPECTED if memory free operation fail
//
HRESULT FreeGroupNodeData(PGROUP_NODE_DATA pGrpNodeData)
{
    HRESULT hr = S_OK;
    if (!HeapFree(GetProcessHeap(), 0, pGrpNodeData))
    {
        hr = E_UNEXPECTED;
    }

    return hr;
}

//-----------------------------------------------------------------------------
// Function: DeepCopyPeerNamePair
//
// Purpose:  Perform a deep copy of a PEER_NAME_PAIR structure.
//
// Returns:  S_OK if no errors
//           E_OUTOFMEMORY if memory allocations fail
//
HRESULT DeepCopyPeerNamePair(PEER_NAME_PAIR * pPNPOriginal, PPEER_NAME_PAIR *
pPNPCopy)
{
    HRESULT hr = S_OK;
    PEER_NAME_PAIR * pPNPTemp = NULL;

    // Allow memory for the new PEER_NAME_PAIR structure from the application
    // heap.
    if(NULL != (pPNPTemp = (PEER_NAME_PAIR*)HeapAlloc(GetProcessHeap(),
                                                HEAP_ZERO_MEMORY,
                                                sizeof(PEER_NAME_PAIR))))
    {
        pPNPTemp->dwSize = pPNPOriginal->dwSize;
        if(NULL != (pPNPTemp->pwzPeerName = HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            ((wcslen(pPNPOriginal->pwzPeerName) + 1) * sizeof(WCHAR)))))
        {
            StringCchCopy(pPNPTemp->pwzPeerName, wcslen(pPNPOriginal->pwzPeerName) + 1,
                          pPNPOriginal->pwzPeerName);
        }
        if(NULL != (pPNPTemp->pwzFriendlyName = HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            ((wcslen(pPNPOriginal->pwzFriendlyName) + 1) * sizeof(WCHAR)))))
        {
            StringCchCopy(pPNPTemp->pwzFriendlyName, wcslen(pPNPOriginal->pwzFriendlyName) + 1,
                          pPNPOriginal->pwzFriendlyName);
        }

        // If either of the string memory allocation failed, free up any
        //  memory allocated, and exit with a memory error.
        if((NULL == pPNPTemp->pwzPeerName) || (NULL == pPNPTemp->pwzFriendlyName))
        {
            if(NULL != pPNPTemp->pwzPeerName)
            {
                HeapFree(GetProcessHeap(), 0, pPNPTemp->pwzPeerName);
            }

            if(NULL != pPNPTemp->pwzFriendlyName)
            {
                HeapFree(GetProcessHeap(), 0, pPNPTemp->pwzFriendlyName);
            }

            HeapFree(GetProcessHeap(), 0, pPNPTemp);
            pPNPTemp = NULL;
            hr = E_OUTOFMEMORY;
        }
    }
    else
    {
        pPNPTemp = NULL;
        hr = E_OUTOFMEMORY;
    }
    *pPNPCopy = pPNPTemp;
    return hr;
}

//-----------------------------------------------------------------------------
// Function: AllocPeerNamePair
//
// Purpose:  Allocate the name pair from the specified friendly name and the
// peer name.
//
// Returns:  S_OK if no errors
//           E_OUTOFMEMORY if memory allocations fail
//
HRESULT AllocPeerNamePair(PCWSTR pwzFriendlyName, PCWSTR pwzPeerName,
    PPEER_NAME_PAIR *pPNPCopy)
{
    HRESULT hr = S_OK;
    PEER_NAME_PAIR * pPNPTemp = NULL;

    // Allow memory for the new PEER_NAME_PAIR structure from the application
    // heap.
    if(NULL != (pPNPTemp = (PEER_NAME_PAIR*)HeapAlloc(GetProcessHeap(),
                                                HEAP_ZERO_MEMORY,
                                                sizeof(PEER_NAME_PAIR))))
    {
        pPNPTemp->dwSize = sizeof(PEER_NAME_PAIR);
        if(NULL != (pPNPTemp->pwzPeerName = HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            ((wcslen(pwzPeerName) + 1) * sizeof(WCHAR)))))
        {
            StringCchCopy(pPNPTemp->pwzPeerName, wcslen(pwzPeerName) + 1, pwzPeerName);
        }

        if(NULL != (pPNPTemp->pwzFriendlyName = HeapAlloc(
            GetProcessHeap(),
            HEAP_ZERO_MEMORY,
            ((wcslen(pwzFriendlyName) + 1) * sizeof(WCHAR)))))
        {
            StringCchCopy(pPNPTemp->pwzFriendlyName, wcslen(pwzFriendlyName) + 1, pwzFriendlyName);
        }

        // If either of the string memory allocation failed, free up any
        //  memory allocated, and exit with a memory error.
        if((NULL == pPNPTemp->pwzPeerName) || (NULL == pPNPTemp->pwzFriendlyName))
        {
            if(NULL != pPNPTemp->pwzPeerName)
            {
                HeapFree(GetProcessHeap(), 0, pPNPTemp->pwzPeerName);
            }

            if(NULL != pPNPTemp->pwzFriendlyName)
            {
                HeapFree(GetProcessHeap(), 0, pPNPTemp->pwzFriendlyName);
            }

            HeapFree(GetProcessHeap(), 0, pPNPTemp);
            pPNPTemp = NULL;
            hr = E_OUTOFMEMORY;
        }
    }
    else
    {
        pPNPTemp = NULL;
        hr = E_OUTOFMEMORY;
    }
    *pPNPCopy = pPNPTemp;
    return hr;
}


//-----------------------------------------------------------------------------
// Function: FreePeerNamePair
//
// Purpose:  Frees the memory allocated in the deep copy of a PEER_NAME_PAIR
//           structure using the DeepCopyPeerNamePair function.
//
// Returns:  S_OK if no errors
//           E_UNEXPECTED if memory free operation fail
//
HRESULT FreePeerNamePair(PEER_NAME_PAIR * pPNP)
{
    HRESULT hr = S_OK;

    if(NULL != pPNP)
    {
        if(NULL != pPNP->pwzPeerName)
        {
            if(!HeapFree(GetProcessHeap(), 0, pPNP->pwzPeerName))
            {
                DisplayHrError(L"HeapFree Failed!", GetLastError());
            }
        }
        if(NULL != pPNP->pwzFriendlyName)
        {
            if(!HeapFree(GetProcessHeap(), 0, pPNP->pwzFriendlyName))
            {
                DisplayHrError(L"HeapFree Failed!", GetLastError());
            }
        }
        if(HeapFree(GetProcessHeap(), 0, pPNP))
        {
            hr = E_UNEXPECTED;
        }
    }
    return hr;
}

//-----------------------------------------------------------------------------
// Function: CleanupTree
//
// Purpose:  Frees all the data associated to each node in the tree view
//
// Returns:  Nothing.
//

void CleanupTree(HWND hwndTree)

{
    // This function frees all the data associated to each node in the tree view
    FreeNodeData(hwndTree);

    // This function will be called on the Refresh View command as well, so
    // the tree already may have some items in it.
    TreeView_DeleteAllItems(hwndTree);
    return;
}

