// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

/****************************************************************

Title: Retrieval of Logotype or Biometric Information in a Certificate

This example shows how to use the new CertRetrieveLogoOrBiometricInfo API
to retrieve the issuer logo from a certificate that has a logotype extension
This is a helper API to support the two new X.509 extensions.

The BioMetricInfo extension (IETF RFC 3739) supports the addition of a 
signature or a pictorial representation of the human holder of the certificate
The Logotype extension (IETF RFC 3709) adds support for the addition of
organizational pictorial representations in certificates. 

****************************************************************/
#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>

//-------------------------------------------------------------------
//   Define the name of a certificate subject.
//   To use this program, the definition of SUBJECT_NAME
//   must be changed to the name of the subject of
//   a certificate that has logotype extension

#define SUBJECT_NAME L"Insert_subject_name_here"

//-------------------------------------------------------------------
//    Define the name of the store where the needed certificate
//    can be found. 

#define CERT_STORE_NAME  L"CA"

//-------------------------------------------------------------------
//    Define the maximum timeout in milliseconds for retrieving the 
//    logotype information. 

#define RETRIEVAL_TIMEOUT 10000
	
void MyHandleError(char *s);
PCCERT_CONTEXT MyGetCertificate (void);

void __cdecl main(void)
{
//-------------------------------------------------------------------
// Pointer to a certificate. 

PCCERT_CONTEXT pCertContext = NULL; 

//-------------------------------------------------------------------
// Declare and Initialize variables 
DWORD cbData = 0;
BYTE* pbData = NULL;
LPWSTR pwszMimeType = NULL;

//-------------------------------------------------------------------
// Use the helper function to get the certificate which has a logotype
// extension

pCertContext = MyGetCertificate();

if(pCertContext == NULL)
{
	MyHandleError("The certificate with logotype extension not found.");
	goto done;
}

//-------------------------------------------------------------------
// This API allocates memory for pbData and pwszMimeType which has to be freed 
// by calling CryptMemFree()
if(!CertRetrieveLogoOrBiometricInfo(
				pCertContext,
                CERT_RETRIEVE_ISSUER_LOGO,	  
                0,                            // Retrieval Flags
                RETRIEVAL_TIMEOUT,            // TimeOut in milliseconds
                0,                            // dwFlags : reserved
                NULL,                         // Reserved for future use
                &pbData,					   
                &cbData,
                &pwszMimeType
                ))
{
    MyHandleError("CertRetrieveLogoOrBiometricInfo failed.");
}
else
{
    printf_s("Successfully retrieved logo information \n");  
}
//-------------------------------------------------------------------
// Clean up. 
done:
	if(pwszMimeType)
		CryptMemFree(pwszMimeType);
	if(pbData)
		CryptMemFree(pbData);
	if(pCertContext)
		CertFreeCertificateContext(pCertContext);

} //end Main

//Function to obtain the certificate
PCCERT_CONTEXT MyGetCertificate (void)
{
        //---------------------------------------------------------
        // Declare and initialize variables.
        HCERTSTORE  hStoreHandle;         // The system store handle.
        PCCERT_CONTEXT  pCert = NULL;     // Set to NULL for the first call to
                                          // CertFindCertificateInStore.

        //-------------------------------------------------------------------
        // Open the certificate store to be searched.

        hStoreHandle = CertOpenStore(
           CERT_STORE_PROV_SYSTEM,          // the store provider type
           0,                               // the encoding type is not needed
           NULL,                            // use the default HCRYPTPROV
           CERT_SYSTEM_STORE_CURRENT_USER,  // set the store location in a 
                                            //  registry location
           CERT_STORE_NAME);                // the store name 

        if (NULL == hStoreHandle)
        {
           printf_s("Could not open the store.");
		   goto done;
        }
        else
        {
           printf_s("Opened the store.\n");
        }

        //-------------------------------------------------------------------
        // Get a certificate that has the specified Subject Name

        pCert = CertFindCertificateInStore(
               hStoreHandle,
			   CRYPT_ASN_ENCODING,          // Use X509_ASN_ENCODING
			   0,                         // No dwFlags needed
			   CERT_FIND_SUBJECT_STR,     // Find a certificate with a
										  //  subject that matches the 
										  //  string in the next parameter
			   SUBJECT_NAME,              // The Unicode string to be found
										  //  in a certificate's subject
			   NULL);                     // NULL for the first call to the
										  //  function; In all subsequent
										  //  calls, it is the last pointer
										  //  returned by the function
        if (NULL == pCert)
        {
            printf_s("Could not find the desired certificate.");
		}
        else
        {
            printf_s("The desired certificate was found. \n");  
        }
done:
        if(NULL != hStoreHandle)
        {
            CertCloseStore( hStoreHandle, 0);
        }        
    return pCert;
}


//-------------------------------------------------------------------
//  This example uses the function MyHandleError, a simple error
//  reporting function, to print an error message corresponding 
//  to the error that occured. 
//  Here we include two error messages specific to the 
//  CertRetrieveLogoOrBiometricInfo API
//  For most applications, replace this function with one 
//  that does more extensive error reporting.
 
void MyHandleError(char *s)
{
    DWORD dwErr = GetLastError(); 
    printf_s("An error occurred in running the program. \n");
    printf_s("%s\n",s);
    printf_s( "Error number 0x%x : ", dwErr);
    switch (dwErr) 
    {
        case CRYPT_E_NOT_FOUND:
            printf_s("This certificate does not have a logotype.\n");
            break;
        case CRYPT_E_HASH_VALUE:
            printf_s("The hash value is not correct.\n");
            break;
        default:
            LPVOID lpMsgBuf;
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,                                       // Location of message
                                                            //  definition ignored
                dwErr,                                      // Message identifier for
                                                            //  the requested message    
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Language identifier for
                                                            //  the requested message
                (LPTSTR) &lpMsgBuf,                         // Buffer that receives
                                                            //  the formatted message
                0,                                          // Size of output buffer
                                                            //  not needed as allocate
                                                            //  buffer flag is set
                NULL );                                     // Array of insert values
            printf_s( "%S\n",lpMsgBuf);
            LocalFree(lpMsgBuf);
    }
    printf_s( "Program terminating. \n");

} // end MyHandleError
