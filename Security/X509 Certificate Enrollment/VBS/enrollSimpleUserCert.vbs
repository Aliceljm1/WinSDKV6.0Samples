'---------------------------------------------------------------------
'  This file is part of the Microsoft .NET Framework SDK Code Samples.
' 
'  Copyright (C) Microsoft Corporation.  All rights reserved.
' 
'This source code is intended only as a supplement to Microsoft
'Development Tools and/or on-line documentation.  See these other
'materials for detailed information regarding Microsoft code samples.
' 
'THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
'KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
'IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
'PARTICULAR PURPOSE.
'---------------------------------------------------------------------


' Simple template based user enrollment.
' Add Subject name and set key size.

Option Explicit

' Constants for enroll context
Const ContextUser                       = 1
Const ContextMachine                    = 2
Const ContextAdministratorForceMachine  = 3

' Constants for enrollment status
Const Enrolled              = 1
Const EnrollPended          = 2

' Constants for GetInnerRequest parameter
Const LevelInnerMost        = 0
Const LevelNext             = 1

' Constants for certificate name string type
Const XCN_CERT_NAME_STR_NONE            = 0

Dim TemplateName
Dim SubjectName
Dim KeyLength
Dim X509Enrollment
Dim Pkcs10Request
Dim PrivateKey
Dim X500DistinguishedName
Dim X509EnrollmentStatus
Dim EnrollmentStatus

' Parse the command line
If WScript.Arguments.Count = 3 Then
    TemplateName = WScript.Arguments.Item(0)
    SubjectName = WScript.Arguments.Item(1)
    KeyLength = WScript.Arguments.Item(2)
Else
    PrintUsage
End If

' Create IX509CertificateRequestPkcs10
Set Pkcs10Request = CreateObject("X509Enrollment.CX509CertificateRequestPkcs10")
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Initialize IX509CertificateRequestPkcs10
Call Pkcs10Request.InitializeFromTemplateName(ContextUser, TemplateName)
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Retrieve the private key
Set PrivateKey = Pkcs10Request.PrivateKey
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Set the key length
PrivateKey.Length = KeyLength
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Create IX500DistinguishedName
Set X500DistinguishedName = CreateObject("X509Enrollment.CX500DistinguishedName")
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Encode the subject name
Call X500DistinguishedName.Encode(SubjectName, XCN_CERT_NAME_STR_NONE)
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Add subject name into the request
Pkcs10Request.Subject = X500DistinguishedName  
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Create IX509Enrollment
Set X509Enrollment = CreateObject("X509Enrollment.CX509Enrollment")
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Initialize IX509Enrollment
Call X509Enrollment.InitializeFromRequest(Pkcs10Request)
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Enroll
Call X509Enrollment.Enroll()
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Get IX509EnrollmentStatus
Set X509EnrollmentStatus = X509Enrollment.Status
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Get enrollment status
EnrollmentStatus = X509EnrollmentStatus.Status
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Display result
If (EnrollmentStatus = Enrolled) Then
    Wscript.Echo "Cert Issued: " & X509EnrollmentStatus.ErrorText & "--" & X509EnrollmentStatus.Text
Elseif (EnrollmentStatus = EnrollPended) Then
    Wscript.Echo "Request is pending: " & X509EnrollmentStatus.ErrorText & "--" & X509EnrollmentStatus.Text
Else
    Wscript.Echo "Request Failed: " & X509EnrollmentStatus.ErrorText & "--" & X509EnrollmentStatus.Text
End If

Wscript.Quit(0)

' PrintUsage function
Sub PrintUsage
    Wscript.Echo "Usage: enrollSimpleUserCert.vbs <Template> <SubjectName> <KeyLength>"
    Wscript.Echo "Example: enrollSimpleUserCert.vbs User ""CN=foo,OU=bar,DC=com"" 1024"
    Wscript.Quit(-1)
End Sub ' PrintUsage


