// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

// Implemented in ContentProperties.cpp
void DisplayStringProperty(
    IPortableDeviceValues*  pProperties,
    PROPERTYKEY             key,
    LPCWSTR                 wszKey);

// Implemented in ContentProperties.cpp
void DisplayGuidProperty(
    IPortableDeviceValues*  pProperties,
    PROPERTYKEY             key,
    LPCWSTR                 wszKey);

// Determines if a device supports a particular functional category.
BOOL SupportsCommand(
    IPortableDevice*  pDevice,
    const PROPERTYKEY keyCommand);

