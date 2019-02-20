// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

/****************************************************************

Title: Encrypting Data and creating an enveloped message

This example shows how to encrypt and decrypt a PKCS7 / CMS message.                
It illustrates use of the CryptEncryptMessage and CryptDecryptMessage API.
This example creates a ECDH certificate with a private key 
associated with it.

****************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <windows.h>
#include <wincrypt.h>

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

void MyHandleError(char *s);

BOOL MyDecryptMessage( 
       BYTE				*pbEncryptedBlob, 
       DWORD			cbEncryptedBlob,
       HCERTSTORE		hStoreHandle);

NCRYPT_KEY_HANDLE MyCreateKey(
	   IN LPWSTR         wszContainerName,							                  
	   IN LPCWSTR        pwszAlgid,
	   IN LPCWSTR        pwszProvName);

PCCERT_CONTEXT MyCreateECDHCertAndAddToStore(
	   IN HCERTSTORE	 hStoreHandle);

void __cdecl main()
{
//-------------------------------------------------------------------
// Declare and initialize variables. This includes getting a pointer 
// to the message to be encrypted. This code creates a message
// and gets a pointer to it. In reality, the message content 
// usually exists somewhere and a pointer to the message is 
// passed to the application. 

BYTE* pbContent = (BYTE*) "Enter message to be encrypted here";  // Message
DWORD cbContent = strlen((char *)pbContent)+1;           // Size of message
HCERTSTORE hStoreHandle = NULL;
PCCERT_CONTEXT pRecipientCert = NULL;
PCCERT_CONTEXT RecipientCertArray[1];
DWORD cbEncryptAlgSize = 0;
CRYPT_ALGORITHM_IDENTIFIER EncryptAlgorithm = {0};
CRYPT_ENCRYPT_MESSAGE_PARA EncryptParams = {0};
DWORD cbEncryptParamsSize = 0;
BYTE*    pbEncryptedBlob = NULL;
DWORD    cbEncryptedBlob = 0;
CHAR szStoreName [MAX_PATH]    = "CMS_ENCRYPT_SAMPLE_STORE";

//-------------------------------------------------------------------
//  Begin processing.
printf_s("The message to be encrypted is: %s.\n",pbContent);

//-------------------------------------------------------------------
// Create a temporary store in the memory
// Temporary store is required to store the certificate as we need to pass 
// the store handle as a parameter to the CryptDecryptMessage API for 
// decrypting the message

if(hStoreHandle  = CertOpenStore(
						CERT_STORE_PROV_MEMORY,  
						0,								
						NULL,							
						NULL,
						szStoreName)	 )
{
    printf_s("The store is open. \n");
}
else
{
    MyHandleError( "Error getting store handle.");
	goto done;
}

if ( pRecipientCert = MyCreateECDHCertAndAddToStore(hStoreHandle) )
{
	printf_s("ECDH certificate is created! \n");
}
else
{
	MyHandleError("Error Creating certificate." );
	goto done;
}

//-------------------------------------------------------------------
// Create a RecipientCertArray.

RecipientCertArray[0] = pRecipientCert;

//-------------------------------------------------------------------
// Initialize the algorithm identifier structure.

cbEncryptAlgSize = sizeof(EncryptAlgorithm);

//-------------------------------------------------------------------
// Initialize the structure to zero.

memset(&EncryptAlgorithm, 0, cbEncryptAlgSize);

//-------------------------------------------------------------------
// Set the necessary member.

EncryptAlgorithm.pszObjId = szOID_NIST_AES128_CBC ;  

//-------------------------------------------------------------------
// Initialize the CRYPT_ENCRYPT_MESSAGE_PARA structure. 

cbEncryptParamsSize = sizeof(EncryptParams);
memset(&EncryptParams, 0, cbEncryptParamsSize);
EncryptParams.cbSize =  cbEncryptParamsSize;
EncryptParams.dwMsgEncodingType = MY_ENCODING_TYPE;
EncryptParams.hCryptProv = NULL;
EncryptParams.ContentEncryptionAlgorithm = EncryptAlgorithm;

//-------------------------------------------------------------------
// Call CryptEncryptMessage.

if(CryptEncryptMessage(
          &EncryptParams,		// Pointer to encryption parameters
          1,					// Number of elements in the CertArray
          RecipientCertArray,	// Array of pointers to recipient certificates
          pbContent,			// Pointer to message to be encrypted
          cbContent,			// Size of message to be encrypted
          NULL,					// Pointer to encrypted content
          &cbEncryptedBlob))	// Size of encrypted content
{
    printf_s("The encrypted message is %d bytes. \n",cbEncryptedBlob);
}
else
{
    MyHandleError( "Getting EncryptedBlob size failed.");
	goto done;
}
//-------------------------------------------------------------------
// Allocate memory for the returned BLOB.

if(pbEncryptedBlob = (BYTE*)malloc(cbEncryptedBlob))
{
    printf_s("Memory has been allocated for the encrypted blob. \n");
}
else
{
    MyHandleError("Memory allocation error while encrypting.");
	goto done;
}
//-------------------------------------------------------------------
// Call CryptEncryptMessage again to encrypt the content.

if(CryptEncryptMessage(
          &EncryptParams,		// Pointer to encryption parameters
          1,					// Number of elements in the CertArray
          RecipientCertArray,	// Array of pointers to recipient certificates
          pbContent,			// Pointer to message to be encrypted
          cbContent,			// Size of message to be encrypted
          pbEncryptedBlob,		// Pointer to encrypted content
          &cbEncryptedBlob))	// Size of encrypted content
{
    printf_s( "Encryption succeeded. \n");
}
else
{
    MyHandleError("Encryption failed.");
	goto done;
}

//-------------------------------------------------------------------
// Call the function MyDecryptMessage, whose code follows main,
// to decrypt the message.

if(MyDecryptMessage( 
     pbEncryptedBlob, 
     cbEncryptedBlob,
     hStoreHandle))
{
   printf_s("Decryption succeeded. \n");
}
else
{
   MyHandleError("Decryption failed. \n");
   goto done;
}
//-------------------------------------------------------------------
// Clean up memory.

done:
	if(pbEncryptedBlob)
		free(pbEncryptedBlob);
	if(pRecipientCert)
		CertFreeCertificateContext(pRecipientCert);
	if(hStoreHandle){
		if(CertCloseStore(
				  hStoreHandle, 
				  CERT_CLOSE_STORE_CHECK_FLAG))
		{
		   printf_s("The store was closed successfully.\n");
		}
		else
		{
		   printf_s("Store closed after encryption -- \n"
			  "but not all certificates or CRLs were freed. \n");
		}
	}
} // End of main

//-------------------------------------------------------------------
//  Define the function MyDecryptMessage.

BOOL MyDecryptMessage( 
     BYTE *pbEncryptedBlob, 
     DWORD cbEncryptedBlob,
     HCERTSTORE hStoreHandle)

//-------------------------------------------------------------------
// Example function for decrypting an encrypted message using
// CryptDecryptMessage. Its parameters are pbEncryptedBlob,
// an encrypted message; cbEncryptedBlob, the length of that
// message; and hStoreHandle, the handle of an open certificate store.

{
	//-------------------------------------------------------------------
	// Declare and initialize local variables.

	DWORD cbDecryptedMessage = 0;
	HCERTSTORE CertStoreArray[] = {hStoreHandle};
	CRYPT_DECRYPT_MESSAGE_PARA  DecryptParams = {0};
	DWORD  DecryptParamsSize = sizeof(DecryptParams);
	BYTE*  pbDecryptedMessage = NULL;
	LPSTR  DecryptedString = NULL;
	BOOL   fReturn = TRUE;

	//-------------------------------------------------------------------
	// Get a pointer to the encrypted message, pbEncryptedBlob,
	// and its length, cbEncryptedBlob. In this sample program, these are
	// passed as parameters along with a CSP and an open store handle.

	//-------------------------------------------------------------------
	//   In this example, the handle to the store was passed in as a 
	//   parameter. 

	//-------------------------------------------------------------------
	//   Create a "CertStoreArray."
	//   In this sample program, this step was done in the declaration
	//   and initialization of local variables, because the store handle 
	//   was passed into the function as a parameter.

	//-------------------------------------------------------------------
	//   Initialize the CRYPT_DECRYPT_MESSAGE_PARA structure.

	memset(&DecryptParams, 0, DecryptParamsSize);
	DecryptParams.cbSize = DecryptParamsSize;
	DecryptParams.dwMsgAndCertEncodingType = MY_ENCODING_TYPE;
	DecryptParams.cCertStore = 1;
	DecryptParams.rghCertStore = CertStoreArray;

	//-------------------------------------------------------------------
	//  Decrypt the message data.
	//  Call CryptDecryptMessage to get the returned data size.

	if(CryptDecryptMessage(
			  &DecryptParams,		// Pointer to decryption parameters
			  pbEncryptedBlob,		// Pointer to encrypted content
			  cbEncryptedBlob,		// Size of encrypted content
			  NULL,					// Pointer to decrypted message
			  &cbDecryptedMessage,	// Size of decrypted message
			  NULL))				// Pointer to certificate that corresponds
									// to private exchange key for decryption
	{
		printf_s("The size for the decrypted message is: %d.\n",
			cbDecryptedMessage);
	}
	else
	{
		printf_s( "Error getting decrypted message size");
		goto ErrorReturn;
	}
	//-------------------------------------------------------------------
	// Allocate memory for the returned decrypted data.

	if(pbDecryptedMessage = (BYTE*)malloc(
		   cbDecryptedMessage))
	{
		printf_s("Memory has been allocated for the decrypted message. "
			"\n");
	}
	else
	{
		printf_s("Memory allocation error while decrypting");
		goto ErrorReturn;
	}
	//-------------------------------------------------------------------
	// Call CryptDecryptMessage to decrypt the data.

	if(CryptDecryptMessage(
			  &DecryptParams,		// Pointer to decryption parameters
			  pbEncryptedBlob,		// Pointer to encrypted content
			  cbEncryptedBlob,		// Size of encrypted content
			  pbDecryptedMessage,	// Pointer to decrypted message
			  &cbDecryptedMessage,	// Size of decrypted message
			  NULL))				// Pointer to certificate that corresponds
									// to private exchange key for decryption
	{
		DecryptedString = (LPSTR) pbDecryptedMessage;
		printf_s("The decrypted string is: %s\n",DecryptedString);
	}
	else
	{
		printf_s("Error decrypting the message \n");
		goto ErrorReturn;
	}

CommonReturn: 
	if (pbDecryptedMessage)
		free (pbDecryptedMessage);
	return fReturn;

ErrorReturn:
	fReturn = FALSE;
	goto CommonReturn;

}  // End of MyDecryptMessage

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

//+--------------------------------------------
//	Create an ECDH certificate context and add it to
//	the memory store to be used in message creation.
//	The structure of a X.509 v3 digital certificate is as follows:
//		Version 
//		Serial Number 
//		Algorithm ID 
//		Issuer Name
//		Validity 
//			Not Before 
//			Not After 
//		Subject Name
//		Subject Public Key Info 
//			Public Key Algorithm 
//			Subject Public Key 
//		Issuer Unique Identifier (Optional) 
//		Subject Unique Identifier (Optional) 
//		Extensions (Optional) 
//		Certificate Signature Algorithm 
//		Certificate Signature 

//---------------------------------------------
PCCERT_CONTEXT
MyCreateECDHCertAndAddToStore(
						IN HCERTSTORE hStoreHandle						
						)
{
	LPWSTR wszContainerName= L"CMS_ENCRYPT_SAMPLE_ECDH256";
	LPCSTR	szSubject= "CN=CMS_ENCRYPT_SAMPLE_CERT";

	LPCWSTR pwszCNGAlg = NULL;
	NCRYPT_KEY_HANDLE hCNGKey = NULL;

	// Certificate
	PCCERT_CONTEXT			pCertContext		 = NULL;
	
	// Private key info
	CRYPT_KEY_PROV_INFO		KeyProvInfo = {0};

	// Subject Name
	// For a self-signed certificate, subject and issuer are the same
	CERT_NAME_BLOB           SubjectName             = { 0 };


	CRYPT_ALGORITHM_IDENTIFIER signatureAlgorithm = { 0 };
    	
    if (!CertStrToName(
        CRYPT_ASN_ENCODING,        // Use X509_ASN_ENCODING
        szSubject,				   // String to be encoded
        0,                         // Type of the string
        NULL,                      // Reserved for future use
        NULL,                      // Pointer to a buffer that receives the
                                   //  encoded structure; NULL to obtain the
                                   //  required size of the buffer
                                   //  for memory allocation purposes
        &SubjectName.cbData,       // Size, in bytes, required for the buffer
        NULL) )                    // Additional error information about 
                                   //  an invalid input string
    {
        printf_s("CertStrToName failed");
		goto ErrorReturn;
    }

    SubjectName.pbData = (PBYTE)LocalAlloc(LPTR, SubjectName.cbData);
	if (NULL == SubjectName.pbData) { 
        printf_s("SubjectName out of memory");
		goto ErrorReturn;
    }
    
    if (!CertStrToName(
            CRYPT_ASN_ENCODING,    // Use X509_ASN_ENCODING    
            szSubject,			   // String to be encoded
            0,                     // Type of the string
            NULL,                  // Reserved for future use
            SubjectName.pbData,    // Pointer to a buffer that receives the
                                   //  encoded structure
            &SubjectName.cbData,   // Size, in bytes, required for the buffer
            NULL) )                // Additional error information about 
                                   //  an invalid input string
    {
        printf_s( "CertStrToName failed");
		goto ErrorReturn;
    }

	pwszCNGAlg =L"ECDH_P256";
	// Get a CERT_PUBLIC_KEY_INFO

	hCNGKey = MyCreateKey(
				wszContainerName,
				pwszCNGAlg,
				NULL);

	if (hCNGKey == NULL)
	{
		printf_s ("MyCreateKey failed.\n");
		goto ErrorReturn;
	}
	
	memset( &signatureAlgorithm, 0, sizeof( CRYPT_ALGORITHM_IDENTIFIER ));
	signatureAlgorithm.pszObjId = szOID_ECDSA_SHA256;

	// Tie this certificate to the Private key by setting a property
	memset(&KeyProvInfo, 0, sizeof(KeyProvInfo));
	KeyProvInfo.pwszContainerName	= wszContainerName;
	KeyProvInfo.pwszProvName = MS_KEY_STORAGE_PROVIDER;
	KeyProvInfo.dwProvType	    	= 0;
	KeyProvInfo.dwKeySpec			= 0;

    pCertContext = CertCreateSelfSignCertificate(
                    NULL,				  // Use information from pKeyProvInfo
										  //  parameter to acquire the handle
										  //  to cryptographic provider
                    &SubjectName,		  // Pointer to a BLOB containing the
										  //  distinguished name (DN) for 
										  //  the certificate subject
                    0,					  // No flags: use default behavior
                    &KeyProvInfo,		  // Key provider information 
                    &signatureAlgorithm,  // Use default signature algorithm
                    NULL,				  // Start time : Use the system 
										  //  current time by default
                    NULL,				  // End time : Use start time plus
										  //  one year by default
                    NULL				  // Array of extensions
                    );
    	if (NULL == pCertContext) { 
      		printf_s( "CertCreateSelfSignCertificate failed");
			goto ErrorReturn;
    	}
		else {	
			printf_s("Successfully Created Certificate.\n");
		}

	// Add it to the Store
	if (!CertAddCertificateContextToStore  (
							hStoreHandle,
							pCertContext,
							CERT_STORE_ADD_ALWAYS,
							NULL))	{
		printf_s("CertAddCertificateContextToStore for ECC ");
		goto ErrorReturn;
	} 

CommonReturn: 
	if (SubjectName.pbData)
		LocalFree(SubjectName.pbData);
	if (NULL != hCNGKey)
		NCryptFreeObject(hCNGKey);
	return pCertContext;

ErrorReturn:
	if (pCertContext)
		pCertContext = NULL;
	goto CommonReturn;
}

// Function to create an ECC key
// This function uses the NCrypt API to open the storage provider
//  and create the key
// It handles the errors which this API might throw
NCRYPT_KEY_HANDLE MyCreateKey(
                              IN LPWSTR         wszContainerName,                                                
                              IN LPCWSTR        pwszAlgid,
                              IN LPCWSTR        pwszProvName                             
                            )
{
      NCRYPT_PROV_HANDLE hProvider = NULL;
      NCRYPT_KEY_HANDLE hKey = NULL;
      SECURITY_STATUS Status;

      char szErr[100];

      Status = NCryptOpenStorageProvider(
            &hProvider,
            NULL != pwszProvName ? pwszProvName : MS_KEY_STORAGE_PROVIDER,
            0           // dwFlags
            );
      if (0 != Status) {
            sprintf_s(szErr, 100, "NCryptOpenStorageProvider failed"
				" 0x%x (%d) \n", Status, Status);
            goto ErrorReturn;
      }

      Status = NCryptOpenKey(
            hProvider,
            &hKey,
            wszContainerName,
            0,					// dwLegacyKeySpec
            0					// dwFlags
            );
      if (0 == Status) {
            Status = NCryptDeleteKey(
                  hKey,
                  0                   // dwFlags
                  );
            hKey = NULL;
            if (0 != Status) {
                  sprintf_s(szErr, 100, "NCryptDeleteKey failed"
					  " 0x%x (%d) \n", Status, Status);
                  goto ErrorReturn;
            }
      } 

      Status = NCryptCreatePersistedKey(
            hProvider,
            &hKey,
            pwszAlgid,
            wszContainerName,
            0,						// dwLegacyKeySpec
            0						// dwFlags
            );
      if (0 != Status) {
            sprintf_s(szErr, 100, "NCryptCreatePersistedKey failed"
				" 0x%x (%d) \n", Status, Status);
            goto ErrorReturn;
      }

      Status = NCryptFinalizeKey(
            hKey,
            0                   // dwFlags
            );

      if (0 != Status) {
            sprintf_s(szErr, 100, "NCryptFinalizeKey failed"
				" 0x%x (%d) \n", Status, Status);
            goto ErrorReturn;
      }

CommonReturn:
      if (NULL != hProvider) {
            Status = NCryptFreeObject(hProvider);
            if (0 != Status){
                  sprintf_s(szErr, 100, "NCryptFreeObject(hProvider) failed"
					  " 0x%x (%d) \n", Status, Status);
            }
      }

      return hKey;

ErrorReturn:
      if (NULL != hKey) {
            Status = NCryptFreeObject(hKey);
            if (0 != Status){
                  sprintf_s(szErr, 100, "NCryptFreeObject(hKey) failed"
					  " 0x%x (%d) \n", Status, Status);
            }
            hKey = NULL;
      }
      printf_s("MyCreateKey: %s", szErr ) ;
      goto CommonReturn;
}
