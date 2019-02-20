Copyright (c) Microsoft Corporation. All rights reserved.

Signing and Signature Verification

This sample shows how to sign a message and verify the signature. 

This uses the CryptSignMessage and CryptVerifySignature APIs. The new part in this sample code is that it illustrates how to obtain the OID for an algorithm from its name and then use it in the signing parameters. 

The CryptSignMessage API requires the hash algorithm used for signing to be provided as one of the Signing Parameters. The only way to do this in Microsoft Windows Vista is to provide the Object Identifier (OID) for the algorithm. Given the name of the algorithm, the CryptFindOIDInfo API can be used to obtain the OID. This can then be used for signing as shown in the sample code.

APIs:

This example illustrates the use of the following APIs,

1. CertOpenStore: This opens a certificate store
2. CertFindCertificateInStore: This function selects the signer certificate from the store.
3. CryptFindOIDInfo: This function maps the algorithm identifier to the corresponding OID.
4. CryptSignMessage: This function creates a hash of the specified content, signs the hash,
	and then encodes both the original message content and the signed hash.
5. CryptVerifyMessageSignature: This function verifies a signed message's signature.

Usage Requirements:

This sample requires a signer certificate to sign the message. The certificate subject name should be inserted in the code in the "SIGNER_NAME" definition.
By default, it looks for the certificate for the signer in the Personal (MY) store as defined by the CERT_STORE_NAME.

To run this sample, compile and link it with crypt32.lib.