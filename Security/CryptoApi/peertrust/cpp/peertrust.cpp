// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

/****************************************************************

Peer Trust 

This sample code shows chain building for a certificate 
in Trusted People store. It also shows how to create a 
self-signed certificate and add it to the Trusted People store.

****************************************************************/

#define UNICODE 1

//Following functions will use their UNICODE equivalents
//CertStrToName : CertStrToNameW

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>

//-------------------------------------------------------------------
//   Declare local functions.
//   Local function definitions follow main.

void MyHandleError(char *s);
void DisplayErrorStatus(DWORD dwErr);
void DisplayInfoStatus(DWORD dwErr);
BOOL MyCreateCertificate(void);
PCCERT_CONTEXT MyGetCertificate (void);

void __cdecl main(void)
{
    
    //---------------------------------------------------------------
	// Declare variables

	PCCERT_CONTEXT           pcTestCertContext = NULL;
    PCCERT_CHAIN_CONTEXT     pChainContext     = NULL;
	CERT_ENHKEY_USAGE        EnhkeyUsage       = {0};
	CERT_USAGE_MATCH         CertUsage         = {0};  
	CERT_CHAIN_PARA          ChainPara         = {0};
	CERT_CHAIN_POLICY_PARA   ChainPolicy       = {0};
	CERT_CHAIN_POLICY_STATUS PolicyStatus      = {0};
    DWORD                    dwFlags           = 0;

    //Set the CERT_CHAIN_ENABLE_PEER_TRUST flag

    dwFlags = CERT_CHAIN_ENABLE_PEER_TRUST; 
    // When this flag is set, end entity certificates in the
    // Trusted People store are trusted without doing any chain building
    // This optimizes the chain building process.

    //-------------------------------------------------------------------
    // Create the certificate, add it to the Trusted People Store and
    // obtain a pointer to it

    if (!MyCreateCertificate())
	{
		MyHandleError("Failed to create the certificate.");
		goto done;
	}
    
    pcTestCertContext = MyGetCertificate();

	if(pcTestCertContext  == NULL)
	{
		MyHandleError("Test Certificate not found.");
		goto done;
	}
	
	//---------------------------------------------------------
    // Initialize data structures for chain building.

    EnhkeyUsage.cUsageIdentifier = 0;
    EnhkeyUsage.rgpszUsageIdentifier=NULL;
    
	CertUsage.dwType = USAGE_MATCH_TYPE_AND;
    CertUsage.Usage  = EnhkeyUsage;

	memset(&ChainPara, 0, sizeof(CERT_CHAIN_PARA));
    ChainPara.cbSize = sizeof(CERT_CHAIN_PARA);
    ChainPara.RequestedUsage=CertUsage;

	memset(&ChainPolicy, 0, sizeof(CERT_CHAIN_POLICY_PARA));
    ChainPolicy.cbSize = sizeof(CERT_CHAIN_POLICY_PARA);

	memset(&PolicyStatus, 0, sizeof(CERT_CHAIN_POLICY_STATUS));
    PolicyStatus.cbSize = sizeof(CERT_CHAIN_POLICY_STATUS);
    PolicyStatus.lChainIndex = -1;
    PolicyStatus.lElementIndex = -1;

    //-------------------------------------------------------------------
    // Build a chain using CertGetCertificateChain
    
    if(CertGetCertificateChain(
        NULL,                  // use the default chain engine
        pcTestCertContext,     // pointer to the end certificate
        NULL,                  // use the default time
        NULL,                  // search no additional stores
        &ChainPara,            // use AND logic and enhanced key usage 
                               //  as indicated in the ChainPara 
                               //  data structure
        dwFlags,
        NULL,                  // currently reserved
        &pChainContext))       // return a pointer to the chain created
    {
        printf_s("The chain has been created. \n");
    }
    else
    {
        MyHandleError("The chain could not be created.");
		goto done;
    }

    //---------------------------------------------------------------
    // Display the error status and info status for the chain    

    printf_s("\nError status for the chain: %x\n",
        pChainContext->TrustStatus.dwErrorStatus);
    DisplayErrorStatus(pChainContext->TrustStatus.dwErrorStatus);
        
    printf_s("\nInfo status for the chain: %x\n",
        pChainContext->TrustStatus.dwInfoStatus);
    DisplayInfoStatus(pChainContext->TrustStatus.dwInfoStatus);

    //---------------------------------------------------------------
    // Verify that the chain complies with policy

    if(!CertVerifyCertificateChainPolicy(
        CERT_CHAIN_POLICY_BASE, // use the base policy
        pChainContext,          // pointer to the chain    
        &ChainPolicy,             
        &PolicyStatus))         // return a pointer to the policy status
    {
        MyHandleError("CertVerifyCertificateChainPolicy failed.");
		goto done;
    }

    if(PolicyStatus.dwError != S_OK)
        printf_s("CertVerifyCertificateChainPolicy: Chain Status Failure:"
            " 0x%x\n", PolicyStatus.dwError);
	else
	{
        printf_s("CertVerifyCertificateChainPolicy succeeded.\n");
    }
    
    
    //---------------------------------------------------------
    // Cleanup
done:
	if (pChainContext)
	{
		CertFreeCertificateChain(pChainContext);
		printf_s("The Original chain is free.\n");
	}
   
    if (NULL != pcTestCertContext) {
        CertFreeCertificateContext(pcTestCertContext);
    }
    printf_s("End of Program.\n");    
	
} // end main

//Function to Create certificate and add it to the store
BOOL MyCreateCertificate(void)
{
    //---------------------------------------------------------
    // Declare and initialize variables.
	BOOL fReturn = TRUE;
    CERT_NAME_BLOB           SubjectName             = { 0 };
    CRYPT_KEY_PROV_INFO      KeyProvInfo             = { 0 };
    HCERTSTORE               hStore                  = NULL;
	HCERTSTORE				 hMyStore				 = NULL;
    PCCERT_CONTEXT           pcCertContext           = NULL;
            
    if (!CertStrToName(
        CRYPT_ASN_ENCODING,        // Use X509_ASN_ENCODING
        L"CN=TEST_PEER_CERT",      // String to be encoded
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
        printf_s( "CertStrToName failed.\n");
		goto ErrorReturn;
    }

    SubjectName.pbData = (PBYTE)LocalAlloc(LPTR, SubjectName.cbData);
    if (NULL == SubjectName.pbData) { 
        printf_s("SubjectName out of memory.\n");
		goto ErrorReturn;
    }
    
    if (!CertStrToName(
            CRYPT_ASN_ENCODING,    // Use X509_ASN_ENCODING    
            L"CN=TEST_PEER_CERT",  // String to be encoded
            0,                     // Type of the string
            NULL,                  // Reserved for future use
            SubjectName.pbData,    // Pointer to a buffer that receives the
                                   //  encoded structure
            &SubjectName.cbData,   // Size, in bytes, required for the buffer
            NULL) )                // Additional error information about 
                                   //  an invalid input string
    {
        printf_s( "CertStrToName failed.\n");
		goto ErrorReturn;
    }
           
    memset( &KeyProvInfo, 0, sizeof( CRYPT_KEY_PROV_INFO ));

    KeyProvInfo.dwProvType        = PROV_RSA_FULL;
    KeyProvInfo.dwKeySpec         = AT_KEYEXCHANGE;
	KeyProvInfo.pwszContainerName = L"TestPeerTrustContainer";
	KeyProvInfo.dwFlags			  = 0;

    pcCertContext = CertCreateSelfSignCertificate(
                        NULL,           // Use information from pKeyProvInfo
                                        //  parameter to acquire the handle
                                        //  to cryptographic provider
                        &SubjectName,   // Pointer to a BLOB containing the
                                        //  distinguished name (DN) for 
                                        //    the certificate subject
                        0,              // No flags: use default behavior
                        &KeyProvInfo,   // Key provider information 
                        NULL,           // Use the default signature algorithm
                        NULL,           // Start time : Use the system current
                                        //  time by default
                        NULL,           // End time : Use start time plus
                                        //  one year by default
                        NULL            // Array of extensions
                        );
    if (NULL == pcCertContext) { 
        printf_s( "CertCreateSelfSignCertificate failed.\n");
		goto ErrorReturn;
    }

    hStore = CertOpenStore(
                   CERT_STORE_PROV_SYSTEM,         // the store provider type
                   0,                              // encoding type not needed
                   NULL,                           // use default HCRYPTPROV
                   CERT_SYSTEM_STORE_CURRENT_USER, // set the store location  
                                                   //  in a registry location
                   L"TrustedPeople");             // the store name 
    if (NULL == hStore) {
        printf_s( "CertOpenStore failed.\n");
		goto ErrorReturn;
    }

    if (!CertAddCertificateContextToStore(
            hStore,                    // Handle of the certificate store
            pcCertContext,             // Pointer to the CERT_CONTEXT structure
                                       //  to be added to the store
            CERT_STORE_ADD_NEW,        // Fail if a matching certificate or link 
                                       //  to a matching certificate exists.
            NULL)                      // Calling application does not require
                                       //  a copy of the added certificate
            ) 
    {
        printf_s( "CertAddCertificateContextToStore failed.\n");
		goto ErrorReturn;
    }

	//-------------------------------------------------------------------
    // Since this is a self-signed certificate, we also add it to the 
	// MY store. 
	// To trust a certificate of a different subject for peer trust, 
	// add it only to the Trusted People store.

	hMyStore = CertOpenStore(
                   CERT_STORE_PROV_SYSTEM,         // the store provider type
                   0,                              // encoding type not needed
                   NULL,                           // use default HCRYPTPROV
                   CERT_SYSTEM_STORE_CURRENT_USER, // set the store location  
                                                   //  in a registry location
                   L"MY");						   // the store name 
    if (NULL == hMyStore) {
        printf_s( "CertOpenStore for MY store failed.\n");
		goto ErrorReturn;
    }

    if (!CertAddCertificateContextToStore(
            hMyStore,                  // Handle of the certificate store
            pcCertContext,             // Pointer to the CERT_CONTEXT structure
                                       //  to be added to the store
            CERT_STORE_ADD_NEW,        // Fail if a matching certificate or link 
                                       //  to a matching certificate exists.
            NULL)                      // Calling application does not require
                                       //  a copy of the added certificate
            ) 
    {
        printf_s( "CertAddCertificateContextToStore for MY Store failed.\n");
		goto ErrorReturn;
    }

    //-------------------------------------------------------------------
    // Clean up and free memory.
CommonReturn:
	if (NULL != hStore) { 
        CertCloseStore( hStore, 0 );
    }
	if (NULL != hMyStore) { 
        CertCloseStore( hMyStore, 0 );
    }
    if (NULL != pcCertContext) {
        CertFreeCertificateContext(pcCertContext);
    }
	if (SubjectName.pbData)
		LocalFree(SubjectName.pbData);
	return fReturn;

ErrorReturn:
	fReturn = FALSE;
	goto CommonReturn;

}

//Function to obtain the certificate
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
                                             //  registry location
           L"TrustedPeople");                // the store name
		if ( !( hStoreHandle))               
        {
             printf_s("CertOpenStore failed.\n");
			 goto done;
        }

        //-------------------------------------------------------------------
        // Locate the certificate in the store
		pCert = CertFindCertificateInStore(
           hStoreHandle,
           MY_ENCODING_TYPE,                 // Use X509_ASN_ENCODING
           0,                                // No dwFlags needed
           CERT_FIND_SUBJECT_STR,            // Find a certificate with a
                                             //  subject that matches the 
                                             //  string in the next parameter
           L"TEST_PEER_CERT",                // The Unicode string to be found
                                             //  in a certificate's subject
           NULL);							 // NULL for the first call to the
                                             //  function; In all subsequent
                                             //  calls, it is the last pointer
                                             //  returned by the function
       
		if( pCert )                            
        {
           printf_s("The test certificate was found.\n");
        }
        else
        {
           printf_s("CertFindCertificateInStore failed.\n");
        }
done:
        if (NULL != hStoreHandle) { 
            CertCloseStore( hStoreHandle, 0 );
        }
    return pCert;
}

void DisplayInfoStatus(DWORD dwInfo)
{
    if (dwInfo == 0) 
        printf_s("No information status reported.\n");
    if (dwInfo & CERT_TRUST_HAS_EXACT_MATCH_ISSUER)
        printf_s("An exact match issuer certificate has been found for "
            "this certificate.\n");
    if (dwInfo & CERT_TRUST_IS_PEER_TRUSTED)
        printf_s("The certificate is peer trusted\n");
    if (dwInfo & CERT_TRUST_HAS_KEY_MATCH_ISSUER) 
        printf_s("A key match issuer certificate has been found for this "
            "certificate.\n");
    if (dwInfo & CERT_TRUST_HAS_NAME_MATCH_ISSUER)
        printf_s("A name match issuer certificate has been found for this "
            "certificate.\n");
    if (dwInfo & CERT_TRUST_IS_SELF_SIGNED)
        printf_s("This certificate is self-signed.\n");
    if (dwInfo & CERT_TRUST_IS_COMPLEX_CHAIN)
        printf_s("The certificate chain created is a complex chain.\n");
    if (dwInfo & CERT_TRUST_HAS_VALID_NAME_CONSTRAINTS) 
        printf_s("The certificate has valid name constraints.\n");
    if (dwInfo & CERT_TRUST_HAS_CRL_VALIDITY_EXTENDED)
        printf_s("The CRL validity period has been extended for this "
            "certificate.\n");
    if (dwInfo & CERT_TRUST_HAS_PREFERRED_ISSUER)
        printf_s("This certificate has preferred issuer\n");
}

void DisplayErrorStatus(DWORD dwErr)
{
    if (dwErr == CERT_TRUST_NO_ERROR)
        printf_s("No error found for this certificate or chain.\n");
    if (dwErr & CERT_TRUST_IS_NOT_TIME_VALID)
        printf_s("This certificate or one of the certificates in the "
            "certificate chain is not time-valid.\n");
    if (dwErr & CERT_TRUST_IS_NOT_TIME_NESTED)
        printf_s("Certificates in the chain are not properly "
            "time-nested.\n");
    if (dwErr & CERT_TRUST_IS_REVOKED)
        printf_s("Trust for this certificate or one of the certificates "
            "in the certificate chain has been revoked.\n");
    if (dwErr & CERT_TRUST_IS_NOT_SIGNATURE_VALID)
        printf_s("The certificate or one of the certificates in the "
            "certificate chain does not have a valid signature.\n");
    if (dwErr & CERT_TRUST_IS_NOT_VALID_FOR_USAGE)
        printf_s("The certificate or certificate chain is not valid "
            "in its proposed usage.\n");
    if (dwErr & CERT_TRUST_IS_UNTRUSTED_ROOT)
        printf_s("The certificate or certificate chain is based "
            "on an untrusted root.\n");
    if (dwErr & CERT_TRUST_REVOCATION_STATUS_UNKNOWN)
        printf_s("The revocation status of the certificate or one of the"
            "certificates in the certificate chain is unknown.\n");
    if (dwErr & CERT_TRUST_IS_CYCLIC)
        printf_s("One of the certificates in the chain was issued by a "
            "certification authority that the original certificate "
            "had certified.\n");
    if (dwErr & CERT_TRUST_IS_PARTIAL_CHAIN)
        printf_s("The certificate chain is not complete.\n");
    if (dwErr & CERT_TRUST_CTL_IS_NOT_TIME_VALID)
        printf_s("A CTL used to create this chain was not time-valid.\n");
    if (dwErr & CERT_TRUST_CTL_IS_NOT_SIGNATURE_VALID)
        printf_s("A CTL used to create this chain did not have a valid "
            "signature.\n");
    if (dwErr & CERT_TRUST_CTL_IS_NOT_VALID_FOR_USAGE) 
        printf_s("A CTL used to create this chain is not valid for this "
            "usage.\n");
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
    printf_s( "Error number %x : ", dwErr);
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
