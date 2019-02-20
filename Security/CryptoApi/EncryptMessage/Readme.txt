Copyright (c) Microsoft Corporation. All rights reserved.

Encrypting Data and creating an enveloped message

This sample code shows how to encrypt and decrypt a PKCS7 (CMS) message using the CryptEncryptMessage and CryptDecryptMessage APIs.

The new part illustrated here is that this sample uses a ECC certificate for the recipient of the message.
The sample shows how to create a ECDH certificate that has a private key associated with it.
For the purposes of the sample, we create a self-signed ECDH certificate. Ideally you would use a certificate signed by a CA.

The sample code also illustrates the use of ECC public key algorithm.
The CertCreateSelfSignCertificate API which is used to create the certificate requires the key provider information for the ECC key. The sample shows how to create this ECC key using the new Cryptography API.

APIs:

This example illustrates the use of the following APIs

1. CryptEncryptMessage: This function encrypts and encodes a message
2. CryptDecryptMessage: This function decodes and decrypts a message
3. CertOpenStore: This function opens a certificate store by using a specified store provider type
4. CertStrToName: This converts a string to an encoded certificate name
5. CertCreateSelfSignCertificate: This function creates a self signed certificate
6. CertCloseStore: This function closes a certificate store handle

Usage Requirements:

To run this sample, compile and link it with crypt32.lib and ncrypt.lib.


