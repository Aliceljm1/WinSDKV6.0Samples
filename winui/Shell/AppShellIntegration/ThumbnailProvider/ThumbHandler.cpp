#include <shlwapi.h>
#include <shlguid.h>
#include <Wincrypt.h>   // For CryptStringToBinary.
#include <thumbcache.h> // For IThumbnailProvider.
#include <gdiplus.h>    // For Gdiplus::Bitmap.
#import <msxml6.dll>    exclude("IStream", "ISequentialStream", "_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME") 
                                // for DOM intefaces from MSXML for parsing our file format (which is xml)

_COM_SMARTPTR_TYPEDEF(IShellItem,  IID_IShellItem);

// We need to implement either IInitializeWithItem or IInitializeWithStream alongside IThumbnailProvider.
// Explorer will call the Initialize methods of either of these interfaces and if
//    (1) IInitializeWithItem    is implemented, provides a IShellItem                  in the Initialize method or if
//    (2) IInitializeWithStream  is implemented, provides a pointer to the file stream  in the Initialize method.
// IT IS STRONGLY RECOMMENDED TO AVOID USING METHOD (1) AS IT REQUIRES OPTING OUT OF PROCESS.
class CRecipeThumbHandler : public IInitializeWithStream,
                            public IThumbnailProvider
{

public:

    CRecipeThumbHandler() : _cRef(1) {}
    
    virtual ~CRecipeThumbHandler()
    {
        if (_pStream != NULL)
        {
            _pStream->Release();
        }
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        static const QITAB qit[] = 
        {
            QITABENT(CRecipeThumbHandler, IInitializeWithStream),
            QITABENT(CRecipeThumbHandler, IThumbnailProvider),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    // IUnknown
    IFACEMETHODIMP_(ULONG) AddRef() 
    { 
        return InterlockedIncrement(&_cRef); 
    }

    // IUnknown
    IFACEMETHODIMP_(ULONG) Release()
    {
        ULONG cRef = InterlockedDecrement(&_cRef);
        if (!cRef)
        {
            delete this;
        }
        return cRef;
    }    

    // IInitializeWithStream.Initialize method
    HRESULT STDMETHODCALLTYPE Initialize(IStream *pStream, DWORD grfMode);
   
    // IThumbnailProvider.GetThumbnail method. Gets the thumbnail for this item.
    IFACEMETHODIMP GetThumbnail(UINT cx, HBITMAP *phbmp, WTS_ALPHATYPE *pdwAlpha);

protected:
    
    HRESULT _GetBase64EncodedImageString(UINT cx, BSTR *pbstrOut);
    HRESULT _GetStreamFromString(BSTR bstrImageName, IStream **ppStream);

private:

    long _cRef;
    IStream *_pStream;     // To store a reference to the stream provided during initialization.

};

HRESULT CRecipeThumbHandler_CreateInstance(REFIID riid, void **ppv)
{
    HRESULT hr;
    CRecipeThumbHandler *pNew = new CRecipeThumbHandler;
    if (pNew)
    {
        hr = pNew->QueryInterface(riid, ppv);
        pNew->Release();        
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE CRecipeThumbHandler::Initialize(IStream *pStream, DWORD)
{
    HRESULT hr = E_UNEXPECTED;

    if (_pStream != NULL)
    {
        // store the reference to the stream.
        _pStream = pStream;
        pStream->AddRef();

        hr = S_OK;
    }
    
    return hr;
}

const WCHAR c_szSmallXPath[]  = L"Recipe/Attachments/Picture[@Size='Small']";
const WCHAR c_szLargeXPath[]  = L"Recipe/Attachments/Picture[@Size='Large']";
const WCHAR c_szNoSizeXPath[] = L"Recipe/Attachments/Picture";

// Gets the base64-encoded string which represents the image.
HRESULT CRecipeThumbHandler::_GetBase64EncodedImageString(UINT cx, BSTR *pbstrOut)
{
    HRESULT hr = E_FAIL;

    try
    {
        MSXML2::IXMLDOMDocumentPtr spDomDoc;
        hr = spDomDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
        if (SUCCEEDED(hr))
        {
            hr = E_FAIL;

            if (VARIANT_TRUE == spDomDoc->load(static_cast<IUnknown *>(_pStream)))
            {
                MSXML2::IXMLDOMNodePtr spNodePicture;

                if (cx <= 48)
                {
                    spNodePicture = spDomDoc->selectSingleNode(c_szSmallXPath);
                }
                else
                {
                    spNodePicture = spDomDoc->selectSingleNode(c_szLargeXPath);
                }

                if (!spNodePicture)
                {
                    spNodePicture = spDomDoc->selectSingleNode(c_szNoSizeXPath);
                }

                if (spNodePicture)
                {
                    MSXML2::IXMLDOMElementPtr spElement;
                    if (SUCCEEDED(spNodePicture->QueryInterface(&spElement)))
                    {
                        *pbstrOut = SysAllocString(spElement->getAttribute(L"Source").bstrVal);

                        if (*pbstrOut == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                        }
                        else if (SysStringLen(*pbstrOut) == 0)
                        {
                            hr = E_UNEXPECTED;
                            SysFreeString(*pbstrOut);
                        }
                        else
                        {
                            hr = S_OK;
                        }
                    }
                }
            }
        }
    }
    catch (_com_error &e)
    {
        hr = e.Error();
    }

    return hr;
}

// Decodes the base64-encoded string to a stream.
HRESULT CRecipeThumbHandler::_GetStreamFromString(BSTR bstrBase64EncodedImageString, IStream **ppImageStream)
{
    HRESULT hr = E_FAIL;

    DWORD dwDecodedImageSize = 0;
    DWORD dwSkipChars        = 0;
    DWORD dwActualFormat     = 0;

    // Base64-decode the string
    BOOL fSuccess = CryptStringToBinaryW(bstrBase64EncodedImageString, NULL, CRYPT_STRING_BASE64, 
                                         NULL, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
    if (fSuccess)
    {
        BYTE *pbDecodedImage = (BYTE*)LocalAlloc(LPTR, dwDecodedImageSize);
        if (NULL != pbDecodedImage) 
        {
            fSuccess = CryptStringToBinaryW(bstrBase64EncodedImageString, wcslen(bstrBase64EncodedImageString), CRYPT_STRING_BASE64, 
                                            pbDecodedImage, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
            if (fSuccess)
            {
                *ppImageStream = SHCreateMemStream(pbDecodedImage, dwDecodedImageSize);
                if (*ppImageStream != NULL)
                {
                    hr = S_OK;
                }
            }

            LocalFree((HLOCAL)pbDecodedImage);
        }
    }

    return hr;
}

HRESULT CRecipeThumbHandler::GetThumbnail(UINT cx, HBITMAP *phbmp, WTS_ALPHATYPE *pdwAlpha)
{
    HRESULT hr = E_FAIL;

    BSTR bstrBase64EncodedImageString = NULL;
    hr = _GetBase64EncodedImageString(cx, &bstrBase64EncodedImageString);

    if (SUCCEEDED(hr))
    {
        IStreamPtr spImageStream ;
        hr = _GetStreamFromString(bstrBase64EncodedImageString, &spImageStream );

        if (SUCCEEDED(hr))
        {
            hr = E_FAIL;
            using namespace Gdiplus;
            ULONG_PTR token;
            GdiplusStartupInput input;
            if (Ok == GdiplusStartup(&token, &input, NULL))
            {
                // Create a bitmap object for the image file to be shown as the thumbnail.
                Bitmap *pBitmap = Bitmap::FromStream(spImageStream , false);

                if (pBitmap)
                {
                    Color color(32, 32, 32);
                    // Returns an HBITMAP object of the image file.
                    if (Ok == pBitmap->GetHBITMAP(color, phbmp))
                    {
                        *pdwAlpha = WTSAT_RGB;
                        hr = S_OK;
                    }

                    delete pBitmap;
                }

                GdiplusShutdown(token);
            }
        }

        SysFreeString(bstrBase64EncodedImageString);
    }

    return hr;
}
