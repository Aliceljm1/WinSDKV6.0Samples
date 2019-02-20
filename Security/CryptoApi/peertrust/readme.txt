Copyright (c) Microsoft Corporation. All rights reserved.

Peer Trust

This sample code shows chain building for a certificate in a Trusted People store. It also shows how to create a self-signed certificate and add it to the Trusted People store.

The Trusted People store contains certificates which are explicitly trusted by the user. These incldue certificates for Outlook, Outlook Express and EFS on Microsoft Windows XP and later. This sample code illustrates how to build a chain for a certificate in this store and verify that chain complies with policy. 

A thing to note in this sample code is the use of the CERT_CHAIN_ENABLE_PEER_TRUST flag. Since peer trust implies explicit trust by the user, it is not mandatory to build a chain to trusted root. If this flag is enabled, this chain building is not done, thereby optimizing the validation process.

This sample code also lists the common chain building errors that might occur.

APIs:

This example illustrates the use of the following APIs,

1. CertStrToName: This converts a string to an encoded certificate name
2. CertCreateSelfSignCertificate: This function creates a self signed certificate signed using the new algorithm
3. CertOpenStore: This opens a certificate store.
4. CertFindCertificateInStore: This function selects the certificate from the store.
5. CertGetCertificateChain: This function builds a chain for the certificate
6. CertVerifyCertificateChainPolicy: This function verifies that the chain complies with policy

Usage Requirements:

To run this sample, compile and link it with crypt32.lib.


