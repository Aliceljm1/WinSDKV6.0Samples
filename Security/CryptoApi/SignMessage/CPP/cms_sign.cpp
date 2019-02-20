// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

/****************************************************************

Title: Signing and Signature Verification

This example shows how to sign and verify a PKCS7 / CMS message.                
It illustrates how to obtain the OID for an algorithm from its name
and then use it in the signing parameters. This example assumes that
the user has a certificate with a private key in a CNG provider.

****************************************************************/

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define SHA256_ALGORITHM  L"SHA256"

//-------------------------------------------------------------------
//   Define the name of a certificate subject.
//   To use this program, the definition of SIGNER_NAME
//   must be changed to the name of the subject of
//   a certificate that has access to a private key. That certificate
//   must have either the CERT_KEY_PROV_INFO_PROP_ID, or 
//   CERT_KEY_CONTEXT_PROP_ID property set for the context to 
//   provide access to the private signature key.

#define SIGNER_NAME  L"Insert_signer_name_here"

//-------------------------------------------------------------------
//    Define the name of the store where the needed certificate
//    can be found. 

#define CERT_STORE_NAME  L"MY"

//-------------------------------------------------------------------
//   Declare local functions.
//   Local function definitions follow main.

void MyHandleError(char *s);            
PCCERT_CONTEXT MyGetCertificate (void);

void __cdecl main(void)
{
//-------------------------------------------------------------------
// Declare and initialize local variables. 
// This includes initializing a pointer to the message. 
// Usually, the message will exist somewhere and a pointer will
// be passed to the application.

//-------------------------------------------------------------------
// The message to be signed

BYTE* pbMessage = 
    (BYTE*)"Enter the message to be signed here";

//-------------------------------------------------------------------
// Size of message. Note that the length set is one more than the 
// length returned by the strlen function in order to include
// the NULL string termination character.

DWORD cbMessage = (DWORD) (strlen((char*) pbMessage)+1);    

//-------------------------------------------------------------------
// Pointer to a signer certificate. 

PCCERT_CONTEXT pSignerCert = NULL; 

CRYPT_SIGN_MESSAGE_PARA  SigParams;
DWORD cbSignedMessageBlob = 0;
BYTE  *pbSignedMessageBlob = NULL;
DWORD cbDecodedMessageBlob = 0;
BYTE  *pbDecodedMessageBlob = NULL;
CRYPT_VERIFY_MESSAGE_PARA VerifyParams;

//-------------------------------------------------------------------
// Create the MessageArray and the MessageSizeArray.

const BYTE* MessageArray[] = {pbMessage};
DWORD MessageSizeArray[1];
MessageSizeArray[0] = cbMessage;

//-------------------------------------------------------------------
//  Begin processing. Display the original message.

printf_s("Begin processing. \n");

printf_s(" The message to be signed is\n-> %s.\n",pbMessage);

//-------------------------------------------------------------------
// Use the helper function to get the signer certificate

pSignerCert = MyGetCertificate();

if(pSignerCert == NULL)
{
	MyHandleError("Signer certificate not found.");
	goto done;
}

//-------------------------------------------------------------------
// Obtain the OID corresponding to the algorithm name

PCCRYPT_OID_INFO pOIDInfo = NULL;

pOIDInfo = CryptFindOIDInfo(
  CRYPT_OID_INFO_NAME_KEY,		// The key type points to algorithm name
  SHA256_ALGORITHM,				// Algorithm Name
  CRYPT_HASH_ALG_OID_GROUP_ID   // Search group of hash algorithms according
                                // to the dwKeyType
);

//pOIDInfo is a pointer to a constant structure and should not be freed.

if(pOIDInfo == NULL)
{
    MyHandleError( "Could not find OID Info");
	goto done;
}
//-------------------------------------------------------------------
// Initialize the signature structure.
// Initialize the HashAlgorithm OID with the OID obtained in the
// earlier step
memset(&SigParams, 0, sizeof(CRYPT_SIGN_MESSAGE_PARA));
SigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
SigParams.dwMsgEncodingType = MY_ENCODING_TYPE;
SigParams.pSigningCert = pSignerCert;
SigParams.HashAlgorithm.pszObjId = (LPSTR) pOIDInfo->pszOID;
SigParams.HashAlgorithm.Parameters.cbData = NULL;
SigParams.cMsgCert = 1;
SigParams.rgpMsgCert = &pSignerCert;

//-------------------------------------------------------------------
// With two calls to CryptSignMessage, sign the message.
// First, get the size of the output signed BLOB.

if(CryptSignMessage(
    &SigParams,            // Signature parameters
    FALSE,                 // Not detached
    1,                     // Number of messages
    MessageArray,          // Messages to be signed
    MessageSizeArray,      // Size of messages
    NULL,                  // Buffer for signed message
    &cbSignedMessageBlob)) // Size of buffer
{
    printf_s("The size of the BLOB is %d.\n",cbSignedMessageBlob);
}
else
{
    MyHandleError("Getting signed BLOB size failed");
	goto done;
}

//-------------------------------------------------------------------
// Allocate memory for the signed BLOB.

pbSignedMessageBlob = (BYTE*)malloc(cbSignedMessageBlob);
if(! (pbSignedMessageBlob))
{
    MyHandleError("Memory allocation error while signing.");
	goto done;
}

//-------------------------------------------------------------------
// Get the SignedMessageBlob.

if(CryptSignMessage(
      &SigParams,            // Signature parameters
      FALSE,                 // Not detached
      1,                     // Number of messages
      MessageArray,          // Messages to be signed
      MessageSizeArray,      // Size of messages
      pbSignedMessageBlob,   // Buffer for signed message
      &cbSignedMessageBlob)) // Size of buffer
{
    printf_s("The message was signed successfully. \n");
}
else
{
    MyHandleError("Error getting signed BLOB");
	goto done;
}

//-------------------------------------------------------------------
// pbSignedMessageBlob points to the signed BLOB.

//-------------------------------------------------------------------
//  Verify the message signature. Usually, this
//  would be done in a separate program. This imitates the signature 
//  verification that would occur at the receiver

//-------------------------------------------------------------------
//  Initialize the VerifyParams data structure.
memset(&VerifyParams, 0, sizeof(CRYPT_VERIFY_MESSAGE_PARA));
VerifyParams.cbSize = sizeof(CRYPT_VERIFY_MESSAGE_PARA);
VerifyParams.dwMsgAndCertEncodingType = MY_ENCODING_TYPE;
VerifyParams.hCryptProv = 0;
VerifyParams.pfnGetSignerCertificate = NULL;
VerifyParams.pvGetArg = NULL;

//-------------------------------------------------------------------
//   With two calls to CryptVerifyMessageSignature, verify and decode
//   the signed message.
//   First, call CryptVerifyMessageSignature to get the length of the
//   buffer needed to hold the decoded message.

if(CryptVerifyMessageSignature(
    &VerifyParams,           // Verify parameters.
    0,                       // Signer index.
    pbSignedMessageBlob,     // Pointer to signed BLOB.
    cbSignedMessageBlob,     // Size of signed BLOB.
    NULL,                    // Buffer for decoded message.
    &cbDecodedMessageBlob,   // Size of buffer.
    NULL))                   // Pointer to signer certificate.
{
    printf_s("%d bytes need for the buffer.\n",cbDecodedMessageBlob);
}
else
{
    MyHandleError("Verification message failed");
	goto done;
}

//-------------------------------------------------------------------
//   Allocate memory for the buffer.
pbDecodedMessageBlob = (BYTE*)malloc(cbDecodedMessageBlob);
if(!(pbDecodedMessageBlob))
{
    MyHandleError("Memory allocation error allocating decode BLOB.");
	goto done;
}

//-------------------------------------------------------------------
//  Call CryptVerifyMessageSignature again to copy the message into
//  the buffer.

if(CryptVerifyMessageSignature(
    &VerifyParams,           // Verify parameters.
    0,                       // Signer index.
    pbSignedMessageBlob,     // Pointer to signed BLOB.
    cbSignedMessageBlob,     // Size of signed BLOB.
    pbDecodedMessageBlob,    // Buffer for decoded message.
    &cbDecodedMessageBlob,   // Size of buffer.
    NULL))                   // Pointer to signer certificate.
{
    printf_s("The verified message is \n-> %s \n",
        pbDecodedMessageBlob);
}
else
{
    MyHandleError("Verification message failed.");
	goto done;
}

//-------------------------------------------------------------------
// Clean up and free memory.
done:

if(pbSignedMessageBlob)
     free(pbSignedMessageBlob);
if(pbDecodedMessageBlob)
     free(pbDecodedMessageBlob);
if(pSignerCert)
     CertFreeCertificateContext(pSignerCert);

printf_s("End of Program\n");

} // End of main    

//-------------------------------------------------------------------
//Function to obtain the signer certificate
PCCERT_CONTEXT MyGetCertificate (void)
{
        HCERTSTORE hStoreHandle = NULL;
        PCCERT_CONTEXT pCert = NULL;
        //-------------------------------------------------------------------
        // Open a certificate store.
		
		hStoreHandle = CertOpenStore(
           CERT_STORE_PROV_SYSTEM,           // the store provider type
           0,                                // the encoding type is not needed
           NULL,                             // use the default HCRYPTPROV
           CERT_SYSTEM_STORE_CURRENT_USER,   // set the store location in a 
                                             // registry location
           CERT_STORE_NAME);				 // the store name
        if ( !(hStoreHandle))                
        {
             printf_s("The store could not be opened.\n");
			 goto done;
        }

        //-------------------------------------------------------------------
        // Get a pointer to the signer's certificate.
        // This certificate must have access to the signer's private key.

        pCert = CertFindCertificateInStore(
           hStoreHandle,
           MY_ENCODING_TYPE,                 // Use X509_ASN_ENCODING
           0,                                // No dwFlags needed
           CERT_FIND_SUBJECT_STR,            // Find a certificate with a
                                             // subject that matches the 
                                             // string in the next parameter
           SIGNER_NAME,                      // The Unicode string to be found
                                             // in a certificate's subject
           NULL);							 // NULL for the first call to the
                                             // function; In all subsequent
                                             // calls, it is the last pointer
                                             // returned by the function
		if(pCert)                            
        {
           printf_s("The signer's certificate was found.\n");
        }
        else
        {
           printf_s( "CertFindCertificateInStore failed.\n");
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
//  For most applications, replace this function with one 
//  that does more extensive error reporting.

void MyHandleError(char *s)
{
    DWORD dwErr = GetLastError();
    LPVOID lpMsgBuf;
    printf_s("An error occurred in running the program. \n");
    printf_s("%s\n",s);
    printf_s("Error number %x : ", dwErr);
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
    printf_s( "Program terminating. \n");
    LocalFree(lpMsgBuf);
 } // end MyHandleError
