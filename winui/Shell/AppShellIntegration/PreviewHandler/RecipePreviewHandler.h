// RecipePreviewHandler.h : Declaration of the CRecipePreviewHandler

#pragma once
#include "resource.h"       // main symbols

#include "PreviewHandlerSDKSample.h"

#include <shobjidl.h>   // IPreviewHandler, IShellItem, IInitializeWithItem, IParentAndItem

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CRecipePreviewHandler

class ATL_NO_VTABLE CRecipePreviewHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRecipePreviewHandler, &CLSID_RecipePreviewHandler>,
	public IObjectWithSiteImpl<CRecipePreviewHandler>,
	public IPreviewHandler,
    public IOleWindow,
    public IInitializeWithStream
{
public:
CRecipePreviewHandler::CRecipePreviewHandler() : _hwndParent(NULL),
                                                 _hwndPreview(NULL), 
                                                 _hinstEditLibrary(NULL),
                                                 _pFrame(NULL),
                                                 _pStyleSheetNode(NULL),
                                                 _pStream(NULL)
{
}

DECLARE_REGISTRY_RESOURCEID(IDR_RECIPEPREVIEWHANDLER)

DECLARE_NOT_AGGREGATABLE(CRecipePreviewHandler)

BEGIN_COM_MAP(CRecipePreviewHandler)
	COM_INTERFACE_ENTRY(IPreviewHandler)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY(IInitializeWithStream)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

    // IObjectWithSite override
    IFACEMETHODIMP SetSite(IUnknown *punkSite);

    // IPreviewHandler
    IFACEMETHODIMP SetWindow(HWND hwnd, const RECT *prc);
    IFACEMETHODIMP SetFocus();
    IFACEMETHODIMP QueryFocus(HWND *phwnd);
    IFACEMETHODIMP TranslateAccelerator(MSG *pmsg);
    IFACEMETHODIMP SetRect(const RECT *prc);
    IFACEMETHODIMP DoPreview();
    IFACEMETHODIMP Unload();

    // IOleWindow
    IFACEMETHODIMP GetWindow(HWND *phwnd);
    IFACEMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);
  
    // IInitializeWithStream
    IFACEMETHODIMP Initialize(IStream *pStream, DWORD grfMode);

private:

    HRESULT _CreatePreviewWindow(BSTR pszRtf);
    HRESULT _CreateStyleSheetNode();

    HWND                    _hwndParent;        // parent window that hosts the previewer window; do NOT DestroyWindow this
    RECT                    _rcParent;          // bounding rect of the parent window
    HWND                    _hwndPreview;       // the actual previewer window
    HINSTANCE               _hinstEditLibrary;  // the library that lets us create a richedit control
    IPreviewHandlerFrame    *_pFrame;           // the previewer frame
    IXMLDOMNode             *_pStyleSheetNode;  // an xml node representing the style-sheet to use for formatting
    IStream                 *_pStream;          // the stream for the file we are previewing

};

OBJECT_ENTRY_AUTO(__uuidof(RecipePreviewHandler), CRecipePreviewHandler)
