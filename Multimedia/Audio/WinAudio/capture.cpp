// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


//-----------------------------------------------------------
//
// capture.cpp
//
//   Capture a wave stream from an endpoint capture device
//   and play the stream on the endpoint rendering device.
//   The PlayCaptureStream function creates two streams --
//   one on the capture device, and one on the rendering
//   device. Then, as the data arrives in the capture buffer
//   from the capture device, the function simply copies the
//   data from the capture buffer to the rendering buffer so
//   that the rendering device can play it.
//
//-----------------------------------------------------------

#include <assert.h>
#include <wtypes.h>
#include <winerror.h>
#include <objbase.h>
#include <mmreg.h>
#include <ks.h>
#include <ksmedia.h>
#include <ksguid.h>
#include "player.h"
#include <stdio.h>

// MFTIME time units per second and per millisecond
#define MFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

 static int        _recChannelsPrioList[2];

void initData() 
{
    _recChannelsPrioList[0] = 2;    // stereo is prio 1
    _recChannelsPrioList[1] = 1;    // mono is prio 2
    //_recChannelsPrioList[2] = 4;    // mono is prio 2

}

WAVEFORMATEXTENSIBLE checkMircoPhoneIsOkEx(IAudioClient * _ptrClientIn, IMMDevice * _ptrDeviceIn, HWND parent)
 {
     initData();
     char msgbuf[1024];
     sprintf(msgbuf, "checkMircoPhoneIsOkEx Faile");

     HRESULT hr = S_OK;
     WAVEFORMATEX* pWfxIn = NULL;
     WAVEFORMATEXTENSIBLE Wfx = WAVEFORMATEXTENSIBLE();
     WAVEFORMATEX* pWfxClosestMatch = NULL;

     // Create COM object with IAudioClient interface.
     SAFE_RELEASE(_ptrClientIn);
     hr = _ptrDeviceIn->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
         (void**)&_ptrClientIn);
     EXIT_ON_ERROR(hr);

     // Retrieve the stream format that the audio engine uses for its internal
     // processing (mixing) of shared-mode streams.
     hr = _ptrClientIn->GetMixFormat(&pWfxIn);
     if (SUCCEEDED(hr)) {
        
     }
     const int freqs[6] = { 48000, 44100, 16000, 96000, 32000, 8000 };


     // Set wave format
     Wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
     Wfx.Format.wBitsPerSample = 16;
     Wfx.Format.cbSize = 22;
     Wfx.dwChannelMask = 0;
     Wfx.Samples.wValidBitsPerSample = Wfx.Format.wBitsPerSample;
     Wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;


     hr = S_FALSE;

     // Iterate over frequencies and channels, in order of priority
     for (unsigned int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++) {
         for (unsigned int chan = 0;
             chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]);
             chan++) {
             Wfx.Format.nChannels = _recChannelsPrioList[chan];
             Wfx.Format.nSamplesPerSec = freqs[freq];
             Wfx.Format.nBlockAlign =
                 Wfx.Format.nChannels * Wfx.Format.wBitsPerSample / 8;
             Wfx.Format.nAvgBytesPerSec =
                 Wfx.Format.nSamplesPerSec * Wfx.Format.nBlockAlign;
             // If the method succeeds and the audio endpoint device supports the
             // specified stream format, it returns S_OK. If the method succeeds and
             // provides a closest match to the specified format, it returns S_FALSE.
             hr = _ptrClientIn->IsFormatSupported(
                 AUDCLNT_SHAREMODE_SHARED, (WAVEFORMATEX*)&Wfx, &pWfxClosestMatch);
             if (hr == S_OK) {

                 sprintf(msgbuf, " checkMircoPhoneIsOkEx GetMixFormatEx OK\nWfx.nChannels=%d,Wfx.nSamplesPerSec=%d\nWfx.nBlockAlign=%d,Wfx.nAvgBytesPerSec=%d\nWfx.nAvgBytesPerSec=%d,Wfx.wFormatTag=%d,Wfx.cbSize=%d",
                     Wfx.Format.nChannels, Wfx.Format.nSamplesPerSec, Wfx.Format.nBlockAlign, Wfx.Format.nAvgBytesPerSec, Wfx.Format.wFormatTag, Wfx.Format.cbSize
                 );
                 OutputDebugStringA(msgbuf);
                 break;
             }
             else {
                 if (pWfxClosestMatch) {

                     sprintf(msgbuf, "checkMircoPhoneIsOkEx   IsFormatSupported is not OK\n pWfxClosestMatch.nChannels=%d,Wfx.nSamplesPerSec=%d\nWfx.nBlockAlign=%d,Wfx.nAvgBytesPerSec=%d\nWfx.nAvgBytesPerSec=%d,Wfx.wFormatTag=%d,Wfx.cbSize=%d",
                         pWfxClosestMatch->nChannels, pWfxClosestMatch->nSamplesPerSec, pWfxClosestMatch->nBlockAlign, pWfxClosestMatch->nAvgBytesPerSec, pWfxClosestMatch->wFormatTag, pWfxClosestMatch->cbSize
                     );
                     CoTaskMemFree(pWfxClosestMatch);
                     pWfxClosestMatch = NULL;
                 }
                 else {
                     sprintf(msgbuf, "checkMircoPhoneIsOkEx IsFormatSupported is not  supported\nWfx.Format.nChannels=%d,Wfx.Format.nSamplesPerSec=%d",
                         Wfx.Format.nChannels, Wfx.Format.nSamplesPerSec
                     );

                     MessageBoxA(parent, msgbuf, "checkMircoPhoneIsOkEx faile", NULL);
                 }
             }
         }
         if (hr == S_OK) {
             return Wfx;
             break;
         }
     }

 Exit:
     //强行使用第一个作为默认值
     Wfx.Format.nChannels = _recChannelsPrioList[0];
     Wfx.Format.nSamplesPerSec = freqs[0];
     Wfx.Format.nBlockAlign =
         Wfx.Format.nChannels * Wfx.Format.wBitsPerSample / 8;
     Wfx.Format.nAvgBytesPerSec =
         Wfx.Format.nSamplesPerSec * Wfx.Format.nBlockAlign;

     MessageBoxA(parent, msgbuf, "checkMircoPhoneIsOkEx INFO", NULL);
     return Wfx;
 }

void checkMircoPhoneIsOk(IAudioClient * _ptrClientIn, IMMDevice* _ptrDeviceIn,HWND parent)
{
    initData();

    char msgbuf[1024];
    sprintf(msgbuf, "Faile");

    HRESULT hr = S_OK;
    WAVEFORMATEX* pWfxIn = NULL;
    WAVEFORMATEX Wfx = WAVEFORMATEX();
    WAVEFORMATEX* pWfxClosestMatch = NULL;

    // Create COM object with IAudioClient interface.
    SAFE_RELEASE(_ptrClientIn);
    hr = _ptrDeviceIn->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        NULL,
        (void**)&_ptrClientIn);
    EXIT_ON_ERROR(hr);

    // Retrieve the stream format that the audio engine uses for its internal
    // processing (mixing) of shared-mode streams.
    hr = _ptrClientIn->GetMixFormat(&pWfxIn);
    if (SUCCEEDED(hr))
    {
        OutputDebugString(L"GetMixFormat OK");
    }

    // Set wave format old code ,add by ljm 2021-10-20
    Wfx.wFormatTag = WAVE_FORMAT_PCM;//test 希沃
    Wfx.wBitsPerSample = 16;
    Wfx.cbSize = 0;

    const int freqs[8] = { 48000, 44100, 16000, 96000, 32000,22050,11025, 8000 };
    hr = S_FALSE;

    // Iterate over frequencies and channels, in order of priority
    for (int freq = 0; freq < sizeof(freqs) / sizeof(freqs[0]); freq++)
    {
        for (int chan = 0; chan < sizeof(_recChannelsPrioList) / sizeof(_recChannelsPrioList[0]); chan++)
        {
            Wfx.nChannels = _recChannelsPrioList[chan];
            Wfx.nSamplesPerSec = freqs[freq];
            Wfx.nBlockAlign = Wfx.nChannels * Wfx.wBitsPerSample / 8;
            Wfx.nAvgBytesPerSec = Wfx.nSamplesPerSec * Wfx.nBlockAlign;
            // If the method succeeds and the audio endpoint device supports the specified stream format,
            // it returns S_OK. If the method succeeds and provides a closest match to the specified format,
            // it returns S_FALSE.
            hr = _ptrClientIn->IsFormatSupported(
                AUDCLNT_SHAREMODE_SHARED,
                &Wfx,
                &pWfxClosestMatch);
            if (hr == S_OK)
            {
                sprintf(msgbuf, "GetMixFormat OK\nWfx.nChannels=%d,Wfx.nSamplesPerSec=%d\nWfx.nBlockAlign=%d,Wfx.nAvgBytesPerSec=%d\nWfx.nAvgBytesPerSec=%d,Wfx.wFormatTag=%d,Wfx.cbSize=%d",
                    Wfx.nChannels, Wfx.nSamplesPerSec, Wfx.nBlockAlign, Wfx.nAvgBytesPerSec, Wfx.wFormatTag,Wfx.cbSize
                );
                OutputDebugStringA(msgbuf);
                break;
            }
            else
            {
                if (pWfxClosestMatch != NULL) {
                    sprintf(msgbuf, "GetMixFormat not OK\nWfx.nChannels=%d,Wfx.nSamplesPerSec=%d\nWfx.nBlockAlign=%d,Wfx.nAvgBytesPerSec=%d\nWfx.nAvgBytesPerSec=%d,Wfx.wFormatTag=%d,Wfx.cbSize=%d",
                        pWfxClosestMatch->nChannels, pWfxClosestMatch->nSamplesPerSec, pWfxClosestMatch->nBlockAlign, pWfxClosestMatch->nAvgBytesPerSec, pWfxClosestMatch->wFormatTag, pWfxClosestMatch->cbSize
                    );
                    MessageBoxA(parent, msgbuf, "match a format", NULL);
                }
            }
        }
        if (hr == S_OK)
            break;
    }

Exit:
    MessageBoxA(parent, msgbuf,"INFO",NULL);

}

//
// Audio capture and playback thread -- Launched by Player
//
DWORD WINAPI PlayCaptureStream(LPVOID pPlayerObject)
{
    HRESULT hr = S_OK;

    IAudioRenderClient *pRenderClient = NULL;
    IAudioCaptureClient *pCaptureClient = NULL;

    // Specify a sleep period of 50 milliseconds.
    DWORD sleepPeriod = 50;

    // Request a buffer duration of 100 milliseconds.
    REFERENCE_TIME bufferDuration = 2 * sleepPeriod * MFTIMES_PER_MILLISEC;

    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    Player *pPlayer = (Player*)pPlayerObject;
    assert(pPlayer != NULL);

    IAudioClient *pClientOut = pPlayer->m_pClientOut;
    assert(pClientOut != NULL);

    IAudioClient *pClientIn = pPlayer->m_pClientIn;
    assert(pClientIn != NULL);

    //add by ljm 测试麦克风支持的采样率和通道数2021-11-6
    //checkMircoPhoneIsOk(pClientIn,pPlayer->m_pDeviceIn,pPlayer->m_hDlg);
    WAVEFORMATEXTENSIBLE Wfx = checkMircoPhoneIsOkEx(pClientIn, pPlayer->m_pDeviceIn, pPlayer->m_hDlg);


    
    // Get the capture stream format. (Later on, remember to free
    // *pWfx by calling CoTaskMemFree.)
     WAVEFORMATEX* pWfxout = NULL;
    hr = pClientOut->GetMixFormat(&pWfxout);//用此方法获取会导致我本机AUDCLNT_E_UNSUPPORTED_FORMAT
    EXIT_ON_ERROR(hr)
    

    ULONG frameSize = Wfx.Format.nChannels * Wfx.Format.wBitsPerSample / 8;


    {
        // Create a rendering stream with the same format as capture stream.
        hr = pClientOut->Initialize(AUDCLNT_SHAREMODE_SHARED,  // shared mode
            0,                         // stream flags
            bufferDuration,            // buffer duration
            0,                         // periodicity
            (WAVEFORMATEX*)&Wfx,                      // wave format
            NULL);                     // session GUID
        HRESULT demo = AUDCLNT_E_NOT_INITIALIZED;
        if (hr == AUDCLNT_E_UNSUPPORTED_FORMAT) {
            MessageBoxA(pPlayer->m_hDlg, ("输出设备格式不正确"), ("格式不正确"), NULL);
        }
        EXIT_ON_ERROR(hr)

            hr = pClientOut->GetService(__uuidof(IAudioRenderClient),
                (void**)&pRenderClient);
        EXIT_ON_ERROR(hr)
    }

    // Create the capture stream.
    hr = pClientIn->Initialize(AUDCLNT_SHAREMODE_SHARED,  // shared mode
                               0,                         // stream flags
                               bufferDuration,            // buffer duration
                               0,                         // periodicity
        (WAVEFORMATEX*)&Wfx,                      // wave format
                               NULL);                     // session GUID
    EXIT_ON_ERROR(hr)

    hr = pClientIn->GetService(__uuidof(IAudioCaptureClient),
                               (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr)

    // Get lengths of allocated capture and rendering buffers.
    UINT32 bufferLengthIn = 0;
    hr = pClientOut->GetBufferSize(&bufferLengthIn);
    EXIT_ON_ERROR(hr)

    UINT32 bufferLengthOut = 0;
    hr = pClientOut->GetBufferSize(&bufferLengthOut);
    EXIT_ON_ERROR(hr)

    // Initial conditions: Before starting the stream, fill the
    // rendering buffer with silence.
    BYTE *pDataOut = NULL;
    hr = pRenderClient->GetBuffer(bufferLengthOut, &pDataOut);
    EXIT_ON_ERROR(hr)

    hr = pRenderClient->ReleaseBuffer(bufferLengthOut, AUDCLNT_BUFFERFLAGS_SILENT);
    EXIT_ON_ERROR(hr)

    // Start up the capture and rendering streams.
    hr = pClientIn->Start();
    EXIT_ON_ERROR(hr)

    hr = pClientOut->Start();
    EXIT_ON_ERROR(hr)

    // Each loop below copies one device period's worth of data
    // from the capture buffer to the rendering buffer.
    while (pPlayer->m_keepPlaying == TRUE)
    {
        // Sleep for one device period.
        Sleep(sleepPeriod);

        while (pPlayer->m_keepPlaying == TRUE)
        {
            // See how much space is available in render buffer.
            UINT32 padding = 0;
            hr = pClientOut->GetCurrentPadding(&padding);
            EXIT_ON_ERROR(hr)

            UINT32 available = bufferLengthOut - padding;

            UINT32 packetLength = 0;
            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)

            if (packetLength == 0)
            {
                // No capture packet is available right now.
                // Sleep for a while...
                break;
            }

            if (packetLength > available)
            {
                // Not enough space in render buffer to store
                // next capture packet. Sleep for a while...
                break;
            }

            // Get pointer to next data packet in capture buffer.
            BYTE *pDataIn = 0;
            UINT32 packetLength2 = 0;
            DWORD flags = 0;
            hr = pCaptureClient->GetBuffer(&pDataIn, &packetLength2,
                                           &flags, NULL, NULL);
            EXIT_ON_ERROR(hr)
            assert(packetLength == packetLength2);

            // If the silence flag is set on the capture buffer,
            // pass the flag to the render buffer.
            flags &= AUDCLNT_BUFFERFLAGS_SILENT;

            // Get pointer to next space in render buffer.
            hr = pRenderClient->GetBuffer(packetLength, &pDataOut);
            EXIT_ON_ERROR(hr)

            // Unless the silence flag is set, copy the data packet
            // from the capture buffer to the render buffer.
            if (flags == 0)
            {
                // Calculate the packet size in bytes.
                UINT32 packetSize = packetLength * frameSize;

                // This app has only two threads, and we know that the
                // other thread doesn't call memcpy, so *maybe* we can
                // get away with not using the multithreaded CRT lib.
                memcpy(pDataOut, pDataIn, packetSize);
            }

            hr = pCaptureClient->ReleaseBuffer(packetLength);
            EXIT_ON_ERROR(hr)

            hr = pRenderClient->ReleaseBuffer(packetLength, flags);
            EXIT_ON_ERROR(hr)
        }
    }

    // Stop the capture and playback streams.
    hr = pClientOut->Stop();
    EXIT_ON_ERROR(hr)

    hr = pClientIn->Stop();
    EXIT_ON_ERROR(hr)

Exit:
    int lasterror = GetLastError();
    SAFE_RELEASE(pPlayer->m_pClientIn)
    SAFE_RELEASE(pPlayer->m_pClientOut)
    SAFE_RELEASE(pRenderClient)
    SAFE_RELEASE(pCaptureClient)
    //CoTaskMemFree(pWfx);

    if (pPlayer->m_keepPlaying == TRUE)
    {
        // The stream is stopping, but not because the client
        // told us to stop it. Unless we send this notification,
        // the client won't know that the stream is stopping.
        if (pPlayer->m_pPlayerCallbacks != NULL)
        {
            pPlayer->m_pPlayerCallbacks->PlayerStopCallback();
        }
    }

    return(DWORD)hr;
}

