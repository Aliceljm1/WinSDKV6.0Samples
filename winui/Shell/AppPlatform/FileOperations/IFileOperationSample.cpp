#include <windows.h>      // Standard include
#include <propsys.h>      // Needed for property manipulation
#include <propvarutil.h>  // Needed for property manipulation
#include <propkey.h>      // Needed for property manipulation
#include <Shellapi.h>     // Included for shell constants such as FO_* values
#include <shlobj.h>       // Required for necessary shell dependencies
#include <strsafe.h>      // Including StringCch* helpers

//
// CONSTANTS 
//
#define SZ_TESTFILE_NAME    L"TestFile"
#define SZ_TESTFILE_NEWNAME L"NewName" 
#define SZ_AUTHOR_NAME      L"John Doe"
#define SZ_TESTFILE_EXT     L"txt"
#define MAX_FILES_TO_CREATE 10


HRESULT CreateTestFile(__in LPCWSTR pszPathDir, __in LPCWSTR pszFileName);
HRESULT AddOperation(__in IFileOperation *pfo, __in LPCWSTR pszFrom, __in_opt LPCWSTR pszTo, __in_opt LPCWSTR pszNewName, UINT wFunc);

HRESULT CopySingleFile(__in LPCWSTR pszSrcDir);
HRESULT CopyMultipleFiles(__in LPCWSTR pszSrcDir);


//-----------------------------------------------------------------------
//
//  Function:  AddOperation
//
//  Synopsis:  Add a copy, move, rename, delete, new, or apply property 
//             operation to the IFileOperation interface.  Later, calling 
//             PerformOperations will execute the operations entered.
//
//  Arguments: pfo        - IFileOperation interface to use
//             pszFrom    - The path to the source item to add
//             pszTo      - Destination directory path for the operation 
//             pszNewName - The new name to apply to the item in the destination 
//             wFunc      - The type of operation to perform
// 
//  Returns:   S_OK if successful
//
//------------------------------------------------------------------------
HRESULT AddOperation(__in IFileOperation *pfo, __in LPCWSTR pszFrom, __in_opt LPCWSTR pszTo, __in_opt LPCWSTR pszNewName, UINT wFunc)
{
    HRESULT hr = E_INVALIDARG;
   
    if ((NULL != pfo) && (NULL != pszFrom))
    {
        //
        // Create an IShellItem from the supplied source path
        //
        IShellItem *psiFrom = NULL;
        hr = SHCreateItemFromParsingName(pszFrom, 
                                         NULL, 
                                         IID_PPV_ARGS(&psiFrom));
        if (SUCCEEDED(hr))
        {
            IShellItem *psiTo = NULL;

            //
            // We only need the destination folder in the case
            // of a copy or move.
            //
            if ((FO_COPY == wFunc) || (FO_MOVE == wFunc))
            {
                if (NULL != pszTo)
                {
                    //
                    // Create an IShellItem from the supplied 
                    // destination path
                    //
                    hr = SHCreateItemFromParsingName(pszTo, 
                                                     NULL, 
                                                     IID_PPV_ARGS(&psiTo));
                }
                else
                {
                    //
                    // Caller did not supply a valid destination path
                    //
                    hr = E_INVALIDARG;
                }
            }
            
            if (SUCCEEDED(hr))
            {
                switch(wFunc)
                {
                case FO_COPY:
                    hr = pfo->CopyItem(psiFrom, psiTo, pszNewName, NULL);
                    break;
                case FO_MOVE:
                    hr = pfo->MoveItem(psiFrom, psiTo, pszNewName, NULL);
                    break;
                case FO_RENAME:
                    hr = pfo->RenameItem(psiFrom, pszNewName, NULL);
                    break;
                case FO_DELETE:
                    hr = pfo->DeleteItem(psiFrom, NULL);
                    break;
                default:
                    hr = E_NOTIMPL;
                    break;
                }

                if (NULL != psiTo)
                {
                    psiTo->Release();
                }
            }
            
            psiFrom->Release();
        }
    }
    else
    {
        hr = E_INVALIDARG;
    }

    return hr;
}

//-----------------------------------------------------------------------
//  Function:  CopySingleFile
//
//  Synopsis:  This example creates a file under the specified folder
//             path and copies it to the same directory with a new name.
//
//  Arguments: pszSrcDir - path to a directory to create a test file and 
//                         copy to it.  This is used as both the source 
//                         and destination directory.
// 
//  Returns:   S_OK if successful
//
//-----------------------------------------------------------------------
HRESULT CopySingleFile(__in LPCWSTR pszSrcDir)
{
    HRESULT hr = E_INVALIDARG;

    if (NULL != pszSrcDir)
    {
        IFileOperation *pfo;

        //
        // Create the IFileOperation interface 
        //
        hr = CoCreateInstance(CLSID_FileOperation, 
                              NULL, 
                              CLSCTX_ALL, 
                              IID_PPV_ARGS(&pfo));
        if (SUCCEEDED(hr))
        {
            //
            // Set the operation flags.  Turn off  all UI
            // from being shown to the user during the 
            // operation.  This includes error, confirmation 
            // and progress dialogs.
            //
            hr = pfo->SetOperationFlags(FOF_NO_UI);
            if (SUCCEEDED(hr))
            {
                //
                // Create a file under the specified directory
                // to copy from.  We will copy the file to the same
                // directory with a different name.
                //
                WCHAR szSrcFileName[MAX_PATH] = {0};
                hr = StringCchPrintfW(szSrcFileName, 
                                      ARRAYSIZE(szSrcFileName), 
                                      L"%s.%s", 
                                      SZ_TESTFILE_NAME,
                                      SZ_TESTFILE_EXT);
                if (SUCCEEDED(hr))
                {
                    WCHAR szSrcPath[MAX_PATH] = {0};
                    hr = StringCchPrintfW(szSrcPath, 
                                          ARRAYSIZE(szSrcPath), 
                                          L"%s\\%s", 
                                          pszSrcDir, 
                                          szSrcFileName);
                    if (SUCCEEDED(hr))
                    {
                        hr = CreateTestFile(pszSrcDir, szSrcFileName); 
                        if (SUCCEEDED(hr))
                        {
                            WCHAR szNewName[MAX_PATH] = {0};
                            hr = StringCchPrintfW(szNewName,
                                                  ARRAYSIZE(szNewName),
                                                  L"%s.%s",
                                                  SZ_TESTFILE_NEWNAME,
                                                  SZ_TESTFILE_EXT);
                            
                            if (SUCCEEDED(hr))
                            {
                                hr = AddOperation(pfo, 
                                                  szSrcPath, 
                                                  pszSrcDir, 
                                                  szNewName, 
                                                  FO_COPY);
                                if (SUCCEEDED(hr))
                                {
                                    //
                                    // Perform the operation to copy the file
                                    //
                                    hr = pfo->PerformOperations();
                                    if (SUCCEEDED(hr))
                                    {
                                        //
                                        // Cleanup the created file
                                        //
                                        WCHAR szNewFileName[MAX_PATH] = {0};
                                        hr = StringCchPrintfW(szNewFileName,
                                                              ARRAYSIZE(szNewFileName),
                                                              L"%s\\%s", 
                                                              pszSrcDir,
                                                              szNewName);
                                        if (SUCCEEDED(hr))
                                        {
                                            DeleteFileW(szNewFileName);
                                        }
                                    }
                                }
                            }
                            //
                            // Cleanup the source file
                            //
                            DeleteFileW(szSrcPath);
                        }
                    }
                }
            }
            //
            // Release the IFileOperation interface
            //
            pfo->Release();
        }
    }
    
    return hr;
}

//-----------------------------------------------------------------------
//  Function:  CopyMultipleFiles
//
//  Synopsis:  This example creates multiple files under the specified folder
//             path and copies them to the same directory with a new name.
//
//  Arguments: pszSrcDir - path to a directory to create the test files and 
//                         copy to it.  This is used as both the source and 
//                         destination directory.
// 
//  Returns:   S_OK if successful
//
//-----------------------------------------------------------------------
HRESULT CopyMultipleFiles(__in LPCWSTR pszSrcDir)
{
    HRESULT hr = E_INVALIDARG;

    if (NULL != pszSrcDir)
    {
        IFileOperation *pfo;

        //
        // Create the IFileOperation interface 
        //
        hr = CoCreateInstance(CLSID_FileOperation, 
                              NULL, 
                              CLSCTX_ALL, 
                              IID_PPV_ARGS(&pfo));
        if (SUCCEEDED(hr))
        {
            //
            // Set the operation flags.  Turn off  all UI
            // from being shown to the user during the 
            // operation.  This includes error, confirmation 
            // and progress dialogs.
            //
            hr = pfo->SetOperationFlags(FOF_NO_UI);
            if (SUCCEEDED(hr))
            {
                //
                // Create multiple files.  We will add each file individually
                // to the IFileOperation interface before calling PerformOperations()
                //
                for (int i = 0; ((SUCCEEDED(hr)) && (i < MAX_FILES_TO_CREATE)); i++)
                {
                    WCHAR szSrcFileName[MAX_PATH] = {0};
                    hr = StringCchPrintfW(szSrcFileName, 
                                          ARRAYSIZE(szSrcFileName), 
                                          L"%s%d.%s", 
                                          SZ_TESTFILE_NAME,
                                          i,
                                          SZ_TESTFILE_EXT);
                    if (SUCCEEDED(hr))
                    {
                        hr = CreateTestFile(pszSrcDir, szSrcFileName); 
                        if (SUCCEEDED(hr))
                        {
                            //
                            // Create the new name to supply to the 
                            // destination files.
                            //
                            WCHAR szNewFileName[MAX_PATH] = {0};
                            hr = StringCchPrintfW(szNewFileName,
                                                  ARRAYSIZE(szNewFileName),
                                                  L"%s%d.%s",
                                                  SZ_TESTFILE_NEWNAME,
                                                  i,
                                                  SZ_TESTFILE_EXT);
                            if (SUCCEEDED(hr))
                            {
                                WCHAR szSrcFullPath[MAX_PATH] = {0};
                                hr = StringCchPrintfW(szSrcFullPath, 
                                                      ARRAYSIZE(szSrcFullPath), 
                                                      L"%s\\%s", 
                                                      pszSrcDir,
                                                      szSrcFileName);
                                if (SUCCEEDED(hr))
                                {
                                    hr = AddOperation(pfo, 
                                                      szSrcFullPath, 
                                                      pszSrcDir, 
                                                      szNewFileName, 
                                                      FO_COPY);
                                }
                            }
                        }
                    }
                }
                
                if (SUCCEEDED(hr))
                {
                    //
                    // Perform the operation to copy the files
                    //
                    hr = pfo->PerformOperations();
                    if (SUCCEEDED(hr))
                    {
                        for (int i = 0; ((SUCCEEDED(hr)) && (i < MAX_FILES_TO_CREATE)); i++)
                        {
                            //
                            // Cleanup the created files
                            //
                            WCHAR szNewFilePath[MAX_PATH] = {0};
                            hr = StringCchPrintfW(szNewFilePath,
                                                  ARRAYSIZE(szNewFilePath),
                                                  L"%s\\%s%d.%s", 
                                                  pszSrcDir,
                                                  SZ_TESTFILE_NEWNAME,
                                                  i,
                                                  SZ_TESTFILE_EXT);
                            if (SUCCEEDED(hr))
                            {
                                DeleteFileW(szNewFilePath);
                            }
                        }
                    }
                }

                // 
                // Cleanup the source files
                //
                HRESULT hrCleanup = S_OK;
                for (int i = 0; ((SUCCEEDED(hrCleanup)) && (i < MAX_FILES_TO_CREATE)); i++)
                {
                    WCHAR szSrcPath[MAX_PATH] = {0};
                    hrCleanup = StringCchPrintfW(szSrcPath, 
                                                 ARRAYSIZE(szSrcPath), 
                                                 L"%s\\%s%d.%s", 
                                                 pszSrcDir,
                                                 SZ_TESTFILE_NAME,
                                                 i,
                                                 SZ_TESTFILE_EXT);
                    if (SUCCEEDED(hrCleanup))
                    {
                        DeleteFileW(szSrcPath);
                    }
                }
            }
            
            //            
            // Release the IFileOperation interface
            //
            pfo->Release();
        }
    }
    
    return hr;
}


//-----------------------------------------------------------------------
//
//  Function:  CreateTestFile
//
//  Synopsis:  Create a normally attributed file with the supplied parent
//             directory and file name.  This function uses IFileOperation
//             to create the new file.
//
//  Arguments: pszPathDir  - Full path to the parent folder where the file
//                           is to be created.
//             pszFileName - The name of the file to create
// 
//  Returns:   S_OK if successful
//
//------------------------------------------------------------------------
HRESULT CreateTestFile(__in LPCWSTR pszPathDir, __in LPCWSTR pszFileName)
{
    HRESULT hr = E_INVALIDARG;

    if ((NULL != pszPathDir) && (NULL != pszFileName))
    {
        WCHAR szFullPath[MAX_PATH] = {0};
        hr = StringCchPrintfW(szFullPath, ARRAYSIZE(szFullPath), L"%s\\%s", pszPathDir, pszFileName);
        if (SUCCEEDED(hr))
        {
            HANDLE hFile = CreateFileW(szFullPath, 
                                       GENERIC_READ | GENERIC_WRITE, 
                                       0, NULL, CREATE_NEW, 
                                       FILE_ATTRIBUTE_NORMAL, 
                                       NULL);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                CloseHandle(hFile);
            }
            else
            {
                hr = E_FAIL;
            }
        }
    }
    
    return hr;
}


int __cdecl wmain(int /*argc*/, __in PWSTR /*argv[]*/)
{
    HRESULT hr = CoInitialize(NULL);

    if (SUCCEEDED(hr))
    {
        //
        // Get path to the current user's documents folder.
        // This will be used as an area to create and perform
        // operations on files.
        //
        LPWSTR pszFolderPath = NULL;
        
        //
        // Use the KnownFolder ID for the user's documents folder
        //
        hr = SHGetKnownFolderPath(FOLDERID_Documents, 
                                  KF_FLAG_DEFAULT_PATH, 
                                  NULL, 
                                  &pszFolderPath);
        if (SUCCEEDED(hr))
        {
            //
            // Use IFileOperation to copy a single file
            //
            hr = CopySingleFile(pszFolderPath);
        }

        if (SUCCEEDED(hr))
        {
            //
            // Use IFileOperation to setup multiple operations and perform them
            //
            hr = CopyMultipleFiles(pszFolderPath);
        }

        if (pszFolderPath)
        {
            CoTaskMemFree(pszFolderPath);
        }
        
        CoUninitialize();
    }

    return 0;
}

