//----------------------------------------------------------------------------------------------
// Main.cpp
//----------------------------------------------------------------------------------------------
#include "Stdafx.h"

#define AUDIO_CAPTURE_FILE		L"Capture.wav"
#define REFTIMES_PER_SEC		10000000
#define REFTIMES_PER_MILLISEC	10000
#define MAX_LOOP_BEFORE_STOP	8  //可以控制录制时长

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);


HRESULT RecordAudioStream();

void main()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if(SUCCEEDED(hr))
	{
		LOG_HRESULT(RecordAudioStream());

		CoUninitialize();
	}
	getchar();
}

static int        _recChannelsPrioList[3];

void initData()
{
	_recChannelsPrioList[0] = 2;    // stereo is prio 1
	_recChannelsPrioList[1] = 1;    // mono is prio 2
	_recChannelsPrioList[2] = 4;    // mono is prio 2

}

WAVEFORMATEXTENSIBLE checkMircoPhoneIsOkEx(IAudioClient* _ptrClientIn, IMMDevice* _ptrDeviceIn, HWND parent)
{
	initData();
	char msgbuf[1024];
	sprintf(msgbuf, "checkMircoPhoneIsOkEx Faile");

	HRESULT hr = S_OK;
	WAVEFORMATEX* pWfxIn = NULL;
	WAVEFORMATEXTENSIBLE Wfx = WAVEFORMATEXTENSIBLE();
	WAVEFORMATEX* pWfxClosestMatch = NULL;

	// Create COM object with IAudioClient interface.
	//SAFE_RELEASE(_ptrClientIn);
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



HRESULT RecordAudioStream()
{
	HRESULT hr = S_OK;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice* pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	CMFWaveWriter cMFWaveWriter;
	UINT32 uiFileLength = 0;
	BOOL bExtensibleFormat = FALSE;
	WAVEFORMATEXTENSIBLE wfx;
	try
	{
		IF_FAILED_THROW(CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator));
		IF_FAILED_THROW(pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice));
		IF_FAILED_THROW(pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient));

		//WAVEFORMATEX* pwfx = NULL;//可以直接尝试获取默认配置来init设备
		//IF_FAILED_THROW(pAudioClient->GetMixFormat(&pwfx));

		 wfx = checkMircoPhoneIsOkEx(pAudioClient, pDevice, NULL);
		 printf("start init \npwfx.nChannels=%d,nSamplesPerSec=%d  \n", wfx.Format.nChannels, wfx.Format.nSamplesPerSec);
		switch(wfx.Format.wFormatTag)
		{
			case WAVE_FORMAT_PCM:
				TRACE((L"WAVE_FORMAT_PCM"));
				break;

			case WAVE_FORMAT_IEEE_FLOAT:
				TRACE((L"WAVE_FORMAT_IEEE_FLOAT"));
				break;

			case WAVE_FORMAT_EXTENSIBLE:
				TRACE((L"WAVE_FORMAT_EXTENSIBLE"));
				bExtensibleFormat = TRUE;

				WAVEFORMATEXTENSIBLE* pWaveFormatExtensible = &wfx;// reinterpret_cast<WAVEFORMATEXTENSIBLE *>(pwfx);

				if(pWaveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_PCM)
				{
					TRACE((L"KSDATAFORMAT_SUBTYPE_PCM"));
				}
				else if(pWaveFormatExtensible->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)
				{
					TRACE((L"KSDATAFORMAT_SUBTYPE_IEEE_FLOAT"));
				}
				break;
		}

		IF_FAILED_THROW(pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_SHARED, 
			0, hnsRequestedDuration, 0,
			(WAVEFORMATEX*)&wfx, NULL));
		IF_FAILED_THROW(pAudioClient->GetBufferSize(&bufferFrameCount));
		IF_FAILED_THROW(pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient));

		IF_FAILED_THROW(cMFWaveWriter.Initialize(AUDIO_CAPTURE_FILE, bExtensibleFormat) ? S_OK : E_FAIL);

		hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / wfx.Format.nSamplesPerSec;

		IF_FAILED_THROW(pAudioClient->Start());

		BOOL bDone = FALSE;
		UINT32 packetLength = 0;
		UINT32 numFramesAvailable;
		BYTE* pData;
		DWORD flags;
		int iLoop = 0;
		printf("start recoding\npwfx.nChannels=%d,nSamplesPerSec=%d  \n", wfx.Format.nChannels, wfx.Format.nSamplesPerSec);
		while(bDone == FALSE)
		{
			Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

			IF_FAILED_THROW(pCaptureClient->GetNextPacketSize(&packetLength));

			while(packetLength != 0)
			{
				IF_FAILED_THROW(pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL));

				if(flags & AUDCLNT_BUFFERFLAGS_SILENT)
				{
					TRACE((L"AUDCLNT_BUFFERFLAGS_SILENT"));
					break;
				}

				if(flags & AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY)
				{
					TRACE((L"AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY"));
				}

				if(flags & AUDCLNT_BUFFERFLAGS_TIMESTAMP_ERROR)
				{
					TRACE((L"AUDCLNT_BUFFERFLAGS_TIMESTAMP_ERROR"));
				}

				//TRACE((L"numFramesAvailable : %u", numFramesAvailable));

				assert(packetLength == numFramesAvailable);

				IF_FAILED_THROW(cMFWaveWriter.WriteWaveData(pData, numFramesAvailable * wfx.Format.nBlockAlign) 
					? S_OK : E_FAIL);

				uiFileLength += numFramesAvailable;

				IF_FAILED_THROW(pCaptureClient->ReleaseBuffer(numFramesAvailable));

				IF_FAILED_THROW(pCaptureClient->GetNextPacketSize(&packetLength));
			}

			if(iLoop++ == MAX_LOOP_BEFORE_STOP)
				bDone = TRUE;
		}
	}
	catch(HRESULT hr){
		printf("recoding error=%d,hr=%x\n",GetLastError(), hr);
	}
	printf("recoding done\n");
	TRACE((L"uiFileLength : %u", uiFileLength));

	if(hr == S_OK)
		cMFWaveWriter.FinalizeHeader((WAVEFORMATEX*)&wfx, uiFileLength, bExtensibleFormat);

	if(pAudioClient)
	{
		LOG_HRESULT(pAudioClient->Stop());
		SAFE_RELEASE(pAudioClient);
	}

	//CoTaskMemFree(pwfx);
	SAFE_RELEASE(pCaptureClient);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);

	return hr;
}