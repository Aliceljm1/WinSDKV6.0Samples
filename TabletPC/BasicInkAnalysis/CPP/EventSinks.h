// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Module:       
//      EventSinks.h
//
// Description:
//      This file contains the definitions of the event sink templates,
//      instantiated in the CBasicInkAnalysis class.
//
//      The event source interfaces used are: 
//      _IInkEvents, _IInkRecognitionEvent, _IInkCollectorEvents
//   
//--------------------------------------------------------------------------

#pragma once

// The DISPID's of the events 
#ifndef DISPID_CEStroke
    #define DISPID_CEStroke                     0x00000001
#endif
#ifndef DISPID_CEGesture
    #define DISPID_CEGesture                    0x0000000a
#endif
#ifndef DISPID_RERecognitionWithAlternates
    #define DISPID_RERecognitionWithAlternates  0x00000001
#endif

// IDispEventSimpleImpl requires a constant as a sink id; define it here
#define SINK_ID  1

/////////////////////////////////////////////////////////
//                                          
// IInkRecognitionEventsImpl
// 
// The template is derived from the ATL's IDispEventSimpleImpl 
// to implement a sink for the IInkRecognitionEvents, fired by 
// the InkRecoContext object
//
/////////////////////////////////////////////////////////

template <class T>
class ATL_NO_VTABLE IInkRecognitionEventsImpl :
	public IDispEventSimpleImpl<SINK_ID, IInkRecognitionEventsImpl<T>, 
                                &DIID__IInkRecognitionEvents>
{
public:
    // ATL structure with the type information for the event, 
    // handled in this template. (Initialized in the BasicInkAnalysis.cpp)
    static const _ATL_FUNC_INFO mc_AtlFuncInfo;

BEGIN_SINK_MAP(IInkRecognitionEventsImpl)
    SINK_ENTRY_INFO(SINK_ID, 
                    DIID__IInkRecognitionEvents, 
                    DISPID_RERecognitionWithAlternates,
                    RecognitionWithAlternates, 
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo))
END_SINK_MAP()

    HRESULT __stdcall RecognitionWithAlternates(IInkRecognitionResult* pIInkRecoResult, 
                                                VARIANT vCustomParam,
                                                InkRecognitionStatus RecognitionStatus)
    {
		T* pT = static_cast<T*>(this);
        return pT->OnRecognitionWithAlternates(pIInkRecoResult, 
                                               vCustomParam,
                                               RecognitionStatus);
    }
};


/////////////////////////////////////////////////////////
//                                          
// IInkCollectorEventsImpl
// 
// The template is derived from the ATL's IDispEventSimpleImpl 
// to implement a sink for the IInkCollectorEvents, fired by 
// the InkCollector object
// Since the IDispEventSimpleImpl doesn't require to supply 
// implementation code for every event, this template has a handler
// for the Gesture event only.
//
/////////////////////////////////////////////////////////

template <class T>
class ATL_NO_VTABLE IInkOverlayEventsImpl :
	public IDispEventSimpleImpl<SINK_ID, 
                                IInkOverlayEventsImpl<T>, 
                                &DIID__IInkOverlayEvents>
{
public:
    // ATL structures with the type information for each event, 
    // handled in this template.(Initialized in the BasicInkAnalysis.cpp)
    static const _ATL_FUNC_INFO mc_AtlFuncInfo[5];

BEGIN_SINK_MAP(IInkOverlayEventsImpl)
    SINK_ENTRY_INFO(SINK_ID, 
                    DIID__IInkOverlayEvents, 
                    DISPID_CEStroke, 
                    Stroke, 
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[0]))
    SINK_ENTRY_INFO(SINK_ID, 
                    DIID__IInkOverlayEvents, 
                    DISPID_CEGesture, 
                    Gesture, 
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[1]))
    SINK_ENTRY_INFO(SINK_ID, 
                    DIID__IInkOverlayEvents, 
                    DISPID_IOEStrokesDeleting, 
                    StrokesDeleting,
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[2]))
    SINK_ENTRY_INFO(SINK_ID,
                    DIID__IInkOverlayEvents,
                    DISPID_IOESelectionMoved,
                    SelectionMoved,
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[3]))
    SINK_ENTRY_INFO(SINK_ID,
                    DIID__IInkOverlayEvents,
                    DISPID_IOESelectionResized,
                    SelectionResized,
                    const_cast<_ATL_FUNC_INFO*>(&mc_AtlFuncInfo[4]))
END_SINK_MAP()

    HRESULT __stdcall Stroke(IInkCursor* pIInkCursor, IInkStrokeDisp* pInkStroke, 
                             VARIANT_BOOL* pbCancel)
    {
		T* pT = static_cast<T*>(this);
        return pT->OnStroke(pIInkCursor, pInkStroke, pbCancel);
    }
    
    HRESULT __stdcall Gesture(IInkCursor* pIInkCursor, IInkStrokes* pInkStrokes, 
                              VARIANT vGestures, VARIANT_BOOL* pbCancel)
    {
		T* pT = static_cast<T*>(this);
        return pT->OnGesture(pIInkCursor, pInkStrokes, vGestures, pbCancel);
    }
    HRESULT __stdcall StrokesDeleting(IInkStrokes *pInkStrokesToDelete)
    {
        T *pT = static_cast<T*>(this);
        return pT->OnStrokesDeleting(pInkStrokesToDelete);
    }

    HRESULT __stdcall SelectionMoved(IInkRectangle *pRect)
    {
        T *pT = static_cast<T*>(this);
        return pT->OnSelectionMoved(pRect);
    }

    HRESULT __stdcall SelectionResized(IInkRectangle *pRect)
    {
        T *pT = static_cast<T*>(this);
        return pT->OnSelectionResized(pRect);
    }
};

