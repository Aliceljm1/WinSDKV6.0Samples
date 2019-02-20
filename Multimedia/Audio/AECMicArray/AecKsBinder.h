//-------------------------------------------------------------------------
// File: AecKsBinder.h
// 
// Desciption: Definition of audio devices binding functions 
//
// Copyright (c) 2004-2006, Microsoft Corporation. All rights reserved.
//---------------------------------------------------------------------------

#ifndef _AEC_KSBINDER_H_
#define _AEC_KSBINDER_H_

#include <atlbase.h>
#include <ATLComCli.h>
#include <audioclient.h>
#include <MMDeviceApi.h>
#include <AudioEngineEndPoint.h>
#include <DeviceTopology.h>
#include <EndpointVolume.h>

typedef struct 
{
    KSPROPERTY KsProperty;
    BOOLEAN bEndpointFlag;
    ULONG ulEntityId;
    union {
         ULONG ulEndpoint;
         ULONG ulInterface;
    };
    ULONG ulOffset;
} USBAUDIO_MEMORY_PROPERTY, *PUSBAUDIO_MEMORY_PROPERTY;

static const GUID USB_AUDIO_PROP_SET_GUID = 
     {0xC3FA16D7, 0x274E, 0x4f2b, 
     {0xA6, 0x3B, 0xD5, 0xE1, 0x09, 0x55, 0xFA, 0x27}};
const DWORD USBAUDIO_PROPERTY_GETSET_MEM = 0;

#define MAX_STR_LEN 512
typedef struct
{
    wchar_t szDeviceName[MAX_STR_LEN];
    wchar_t szDeviceID[MAX_STR_LEN];
    bool bIsMicArrayDevice;
} AUDIO_DEVICE_INFO, *PAUDIO_DEVICE_INFO;
    

HRESULT GetDeviceNum(EDataFlow eDataFlow, UINT &uDevCount);

__inline HRESULT GetRenderDeviceNum(UINT &uDevCount)
{ return GetDeviceNum(eRender, uDevCount); }

__inline HRESULT GetCaptureDeviceNum(UINT &uDevCount)
{ return GetDeviceNum(eCapture, uDevCount); }


HRESULT EnumDevice(
    EDataFlow eDataFlow, 
    UINT  uNumElements,
    __out_ecount_full(uNumElements) AUDIO_DEVICE_INFO *pDevicInfo);

__inline HRESULT EnumRenderDevice(UINT  uNumElements, AUDIO_DEVICE_INFO *pDevicInfo) 
    { return EnumDevice(eRender, uNumElements, pDevicInfo); }

__inline HRESULT EnumCaptureDevice(UINT  uNumElements, AUDIO_DEVICE_INFO *pDevicInfo) 
    { return EnumDevice(eCapture, uNumElements, pDevicInfo); }


HRESULT DeviceBindTo(
        EDataFlow eDataFlow,        // eCapture or eRender
        INT uDevIdx,                // Device Index. USE_DEFAULT_DEVICE - use default device. 
        __out IAudioClient **ppAudioClient,    // pointer pointer to IAudioClient interface
        __out IAudioEndpointVolume **ppEndpointVolume,
        __out WCHAR** ppszEndpointDeviceId   // Device ID. Need to be freed in caller with CoTaskMemoryFree
);

__inline HRESULT CaptureDeviceBindTo(
        INT uDevIdx, 
        __out IAudioClient **ppAudioClient,    // pointer pointer to IAudioClient interface
        __out IAudioEndpointVolume **ppEndpointVolume,
        __out WCHAR** ppszEndpointDeviceId)
{
    return DeviceBindTo(eCapture, uDevIdx, ppAudioClient, ppEndpointVolume, ppszEndpointDeviceId);
}

__inline HRESULT RenderDeviceBindTo(
        INT uDevIdx, 
        __out IAudioClient **ppAudioClient,    // pointer pointer to IAudioClient interface
        __out IAudioEndpointVolume **ppEndpointVolume,
        __out WCHAR** ppszEndpointDeviceId)
{
    return DeviceBindTo(eRender, uDevIdx, ppAudioClient, ppEndpointVolume, ppszEndpointDeviceId);
}

HRESULT DeviceIsMicArray(
         __in wchar_t szDeviceId[],
         __out bool &bIsMicArray);

HRESULT EndpointIsMicArray(
                      __in IMMDevice* pEndpoint,
                      __out bool & isMicrophoneArray);

HRESULT GetJackSubtypeForEndpoint(
                      __in  IMMDevice*  pEndpoint,  
                      __out GUID*       pgSubtype);

__checkReturn HRESULT GetInputJack(__in IMMDevice * pDevice, 
                                   __out CComPtr<IPart> & spPart);

HRESULT GetMicArrayGeometry(
                    __in wchar_t szDeviceId[],
                    __out KSAUDIO_MIC_ARRAY_GEOMETRY ** ppGeometry,
                    __out ULONG & cbSize);

#endif //_AEC_KSBINDER_H_

