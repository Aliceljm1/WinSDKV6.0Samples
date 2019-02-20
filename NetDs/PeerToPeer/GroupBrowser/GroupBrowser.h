/********************************************************************++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    GroupBrowser.h

Abstract:

   This is the main include file for standard system include files,
   or project specific include files.

--********************************************************************/

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

// Header Files

#include <p2p.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <objbase.h>
#include <initguid.h>
#include <pnrpns.h>
#include <strsafe.h>
#include "resource.h"


// ----------------------------------------------------------------------------
// Utility Macros.
// ----------------------------------------------------------------------------
#define celems(a)   (sizeof(a) / sizeof(a[0]))
#define DisplayHrError(pwzMsg, hr)   MsgErrHr(pwzMsg, hr, __FUNCTION__)

// ----------------------------------------------------------------------------
// Constants.
// ----------------------------------------------------------------------------
#define MAX_USERNAME     256      // Maximum number of characters in a user's name
#define MAX_GROUPNAME    256      // Maximum number of characters in a group name
#define MAX_IDENTITY     256      // Maximum number of characters in an identity
#define MAX_CLASSIFIER   50       // Maximum size of a classifier string
#define MAX_CLOUDNAME    50       // Maximum size of a cloud name
#define MAX_COMMENT      256      // Maximum size of a comment string
#define MAX_STRING       256      // Maximum size of string

#define SB_PART_STATUS      0   // status bar part number for "Listening"/"Connected"
#define SB_PART_MESSAGE     1   // status bar part number for messages


// ----------------------------------------------------------------------------
// Enumerations.
// ----------------------------------------------------------------------------
typedef enum {
    NODETYPE_ROOT,      // Root of the tree
    NODETYPE_IDENTITY,  // Identity type of node
    NODETYPE_GROUP,     // Group type of node 
} NODETYPE;


// ----------------------------------------------------------------------------
// Data structure definitions.
// ----------------------------------------------------------------------------

// Data structure that will be associated with the Group node.
typedef struct GroupNodeData {
    PPEER_NAME_PAIR pPeerNamePair;  // Peer name pair of the group.
    HGROUP hGroup;                  // Handle to the group.
} *PGROUP_NODE_DATA, GROUP_NODE_DATA;

// Data structure that will be associated with the Identity node.
typedef struct IdentityNodeData {
    PPEER_NAME_PAIR pPeerNamePair;  // Peer name pair of the identity.
} *PIDENTITY_NODE_DATA, IDENTITY_NODE_DATA;


// Node information associated with the TreeView item.
typedef struct NODEINFO {
    NODETYPE    nt;         // Node type
    LPVOID      pData;      // Pointer to the node's Data. For identity node 
                            // this will be PIDENTITY_NODE_DATA and for the 
                            // group this will be PGROUP_NODE_DATA.
} *PNODEINFO, NODEINFO;


// ----------------------------------------------------------------------------
// UI callback function declarations.
// ----------------------------------------------------------------------------
void InitializeUIComponents(HWND hwndDlg);
LRESULT CALLBACK MainDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateIdentityDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateGroupDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SaveIdentityInfoProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CreateInvitationProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);




// ----------------------------------------------------------------------------
// Operation handler function declarations.
// ----------------------------------------------------------------------------
HRESULT HandleCreateIdentity(HWND hDlg);
HRESULT HandleCreateGroup(HWND hDlg, HTREEITEM htItem);
HRESULT HandleSaveIdentityInfo(HWND hDlg, PNODEINFO pNodeInfo);
HRESULT HandleCreateInvitation(HWND hDlg, PNODEINFO pNodeInfo);
HRESULT DeleteIdentity(HTREEITEM htItem);
HRESULT DeleteGroup(HTREEITEM htItem);

// ----------------------------------------------------------------------------
// Functions to extract, insert or modify data from TreeView and ListView.
// ----------------------------------------------------------------------------
void HandleTreeViewMessages(HWND hwndDlg, WPARAM wParam, LPARAM lParam);
void HandleListViewMessages(HWND hwndDlg, WPARAM wParam, LPARAM lParam);
HTREEITEM InitializeTreeView(HWND hwndTree);
HTREEITEM InsertTreeViewItem(HWND hwndTree, HTREEITEM hParent, PCWSTR pwcName, NODETYPE nt, LPVOID pData);
PNODEINFO GetNodeInfo(HTREEITEM htItem);

void ResizeMainWindow(int dxMain, int dyMain);
void InitializeListView(HWND hwnd);
void SynchronizeListView(HTREEITEM htItem);
void ListIdentityProperties(HTREEITEM htItem, PNODEINFO pNodeInfo);
void ListGroupProperties(HTREEITEM htItem, PNODEINFO pNodeInfo);

// ----------------------------------------------------------------------------
// Popup menu related functions.
// ----------------------------------------------------------------------------
void DisplayTrackMenu(HTREEITEM htItem);
void DisplayListViewMenu(INT iItem);

void CreateRootNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo);
void CreateIdentityNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo);
void CreateGroupNodePopupMenu(HMENU hMenu, PNODEINFO pNodeInfo);
void ExecuteIdentityNodePopupMenuCommand(int iCmd, HTREEITEM htItem, PNODEINFO pNodeInfo);
void ExecuteGroupNodePopupMenuCommand(int iCmd, HTREEITEM htItem, PNODEINFO pNodeInfo);

// ----------------------------------------------------------------------------
// Helper and Utility function declarations.
// ----------------------------------------------------------------------------
void    BrowseHelper(HWND hDlg, int idEditbox, PCWSTR pwzFileType, PCWSTR pwzFileExtension, BOOL fOpen);
void    SetListItemContents(PCWSTR pwzTitleText, PWSTR pwzItemText, INT* lvitem);
void    SetMemberNameInListView(PPEER_MEMBER pMember, INT iItem);
HGROUP  OpenGroup(HTREEITEM htItem, PPEER_NAME_PAIR pGrpPeerNamePair);
HRESULT SaveIdentityInfo(PCWSTR pwzIdentity, PCWSTR pwzFile);
HRESULT CreateInvitation(PCWSTR wzIdentityInfoPath, PCWSTR wzInvitationPath, PNODEINFO pNodeInfo);
ULONG   GetMemberCount(HGROUP phGroup, DWORD dwMemberType);
BOOL    DoGroupsExist(PPEER_NAME_PAIR pIdPeerNamePair);
HRESULT EnumIdentities(HTREEITEM hRoot);
HRESULT EnumGroups(PPEER_NAME_PAIR pIdPeerNamePair, HTREEITEM *hTreeNode);
void    DisplayMsg(PCWSTR pwzMsg);
HRESULT PopulateCloudList(HWND hDlg);
void    DisplayError(PCWSTR pwzMsg);
void    MsgErrHr(PCWSTR pwzMsg, HRESULT hr, PCSTR pszFunction);
HRESULT AllocNodeInfo(PNODEINFO *ppNodeInfo);
HRESULT FreeNodeInfo(PNODEINFO pNodeInfo);
HRESULT FreeNodeData(HWND hwndTree);
HRESULT DeleteId(HTREEITEM hti_id);
HRESULT DeleteGroups(HTREEITEM hti_id);
void    CleanupTree(HWND hwndTree);
HRESULT AllocIdentityNodeData(PIDENTITY_NODE_DATA *ppIdNodeData);
HRESULT FreeIdentityNodeData(PIDENTITY_NODE_DATA pIdNodeData);
HRESULT AllocGroupNodeData(PGROUP_NODE_DATA *ppGrpNodeData);
HRESULT FreeGroupNodeData(PGROUP_NODE_DATA pGrpNodeData);

HRESULT AllocPeerNamePair(PCWSTR pwzFriendlyName, PCWSTR pwzPeerName, PPEER_NAME_PAIR *pPNPCopy);
HRESULT DeepCopyPeerNamePair(PEER_NAME_PAIR * pPNPOriginal, PPEER_NAME_PAIR * pPNPCopy);
HRESULT FreePeerNamePair(PEER_NAME_PAIR * pPNP);

