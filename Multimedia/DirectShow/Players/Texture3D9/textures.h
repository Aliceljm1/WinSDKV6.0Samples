//-----------------------------------------------------------------------------
// File: Textures.h
//
// Desc: DirectShow sample code - header file for DirectShow/Direct3D8 video 
//       texturing
//       
// Copyright (c) Microsoft Corporation.  All rights reserved.
//-----------------------------------------------------------------------------


#include <d3dx9.h>
#include <d3d9.h>
#include <windows.h>
#include <mmsystem.h>
#include <d3d9types.h>

#include "smartptr.h"

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------
HRESULT InitDShowTextureRenderer();


BOOL GetClipFileName(LPTSTR szName);
void CheckMovieStatus(void);
void CleanupDShow(void);
void Msg(TCHAR *szFormat, ...);

HRESULT AddToROT(IUnknown *pUnkGraph); 
void RemoveFromROT(void);

//-----------------------------------------------------------------------------
// Direct3D global variables
//-----------------------------------------------------------------------------
extern LPDIRECT3D9             g_pD3D;       // Used to create the D3DDevice
extern LPDIRECT3DDEVICE9       g_pd3dDevice; // Our rendering device
extern LPDIRECT3DVERTEXBUFFER9 g_pVB;        // Buffer to hold vertices
extern LPDIRECT3DTEXTURE9      g_pTexture;   // Our texture

