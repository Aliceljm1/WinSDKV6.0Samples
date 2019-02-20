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


' Simple template based machine enrollment.
' Add friendly name and description.

Option Explicit

' Constants for enroll context
Const ContextUser                       = 1
Const ContextMachine                    = 2
Const ContextAdministratorForceMachine  = 3

' Constants for enrollment status
Const Enrolled              = 1
Const EnrollPended          = 2

Dim TemplateName
Dim CertFriendlyName
Dim CertDescription
Dim X509Enrollment
Dim X509EnrollmentStatus
Dim EnrollmentStatus

' Parse the command line
If WScript.Arguments.Count = 3 Then
    TemplateName = WScript.Arguments.Item(0)
    CertFriendlyName = WScript.Arguments.Item(1)
    CertDescription = WScript.Arguments.Item(2)
Else
    PrintUsage
End If

' Create IX509Enrollment
Set X509Enrollment = CreateObject("X509Enrollment.CX509Enrollment")
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Initialize IX509Enrollment
Call X509Enrollment.InitializeFromTemplateName(ContextAdministratorForceMachine, TemplateName)
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Add certificate friendly name 
X509Enrollment.CertificateFriendlyName = CertFriendlyName
If (Err.Number <> 0) Then
    Wscript.Echo Err.Number & Err.Description
    Wscript.Quit(-1)
End If

' Add certificate description
X509Enrollment.CertificateDescription = CertDescription
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
    Wscript.Echo "Usage: enrollSimpleMachineCert.vbs <Template> <FriendlyName> <Description>"
    Wscript.Echo "Example: enrollSimpleMachineCert.vbs Machine ""Machine Cert"" ""Simple Machine Cert"""
    Wscript.Quit(-1)
End Sub ' PrintUsage


