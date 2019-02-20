

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0476 */
/* Compiler settings for taskschd.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __taskschd_h__
#define __taskschd_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITaskFolderCollection_FWD_DEFINED__
#define __ITaskFolderCollection_FWD_DEFINED__
typedef interface ITaskFolderCollection ITaskFolderCollection;
#endif 	/* __ITaskFolderCollection_FWD_DEFINED__ */


#ifndef __ITaskDefinition_FWD_DEFINED__
#define __ITaskDefinition_FWD_DEFINED__
typedef interface ITaskDefinition ITaskDefinition;
#endif 	/* __ITaskDefinition_FWD_DEFINED__ */


#ifndef __ITaskService_FWD_DEFINED__
#define __ITaskService_FWD_DEFINED__
typedef interface ITaskService ITaskService;
#endif 	/* __ITaskService_FWD_DEFINED__ */


#ifndef __ITaskNamedValuePair_FWD_DEFINED__
#define __ITaskNamedValuePair_FWD_DEFINED__
typedef interface ITaskNamedValuePair ITaskNamedValuePair;
#endif 	/* __ITaskNamedValuePair_FWD_DEFINED__ */


#ifndef __ITaskNamedValueCollection_FWD_DEFINED__
#define __ITaskNamedValueCollection_FWD_DEFINED__
typedef interface ITaskNamedValueCollection ITaskNamedValueCollection;
#endif 	/* __ITaskNamedValueCollection_FWD_DEFINED__ */


#ifndef __IRunningTask_FWD_DEFINED__
#define __IRunningTask_FWD_DEFINED__
typedef interface IRunningTask IRunningTask;
#endif 	/* __IRunningTask_FWD_DEFINED__ */


#ifndef __IRunningTaskCollection_FWD_DEFINED__
#define __IRunningTaskCollection_FWD_DEFINED__
typedef interface IRunningTaskCollection IRunningTaskCollection;
#endif 	/* __IRunningTaskCollection_FWD_DEFINED__ */


#ifndef __IRegisteredTask_FWD_DEFINED__
#define __IRegisteredTask_FWD_DEFINED__
typedef interface IRegisteredTask IRegisteredTask;
#endif 	/* __IRegisteredTask_FWD_DEFINED__ */


#ifndef __IRegisteredTaskCollection_FWD_DEFINED__
#define __IRegisteredTaskCollection_FWD_DEFINED__
typedef interface IRegisteredTaskCollection IRegisteredTaskCollection;
#endif 	/* __IRegisteredTaskCollection_FWD_DEFINED__ */


#ifndef __ITaskFolder_FWD_DEFINED__
#define __ITaskFolder_FWD_DEFINED__
typedef interface ITaskFolder ITaskFolder;
#endif 	/* __ITaskFolder_FWD_DEFINED__ */


#ifndef __ITaskVariables_FWD_DEFINED__
#define __ITaskVariables_FWD_DEFINED__
typedef interface ITaskVariables ITaskVariables;
#endif 	/* __ITaskVariables_FWD_DEFINED__ */


#ifndef __ITaskHandlerStatus_FWD_DEFINED__
#define __ITaskHandlerStatus_FWD_DEFINED__
typedef interface ITaskHandlerStatus ITaskHandlerStatus;
#endif 	/* __ITaskHandlerStatus_FWD_DEFINED__ */


#ifndef __ITaskHandler_FWD_DEFINED__
#define __ITaskHandler_FWD_DEFINED__
typedef interface ITaskHandler ITaskHandler;
#endif 	/* __ITaskHandler_FWD_DEFINED__ */


#ifndef __IIdleSettings_FWD_DEFINED__
#define __IIdleSettings_FWD_DEFINED__
typedef interface IIdleSettings IIdleSettings;
#endif 	/* __IIdleSettings_FWD_DEFINED__ */


#ifndef __IRepetitionPattern_FWD_DEFINED__
#define __IRepetitionPattern_FWD_DEFINED__
typedef interface IRepetitionPattern IRepetitionPattern;
#endif 	/* __IRepetitionPattern_FWD_DEFINED__ */


#ifndef __ITrigger_FWD_DEFINED__
#define __ITrigger_FWD_DEFINED__
typedef interface ITrigger ITrigger;
#endif 	/* __ITrigger_FWD_DEFINED__ */


#ifndef __IEventTrigger_FWD_DEFINED__
#define __IEventTrigger_FWD_DEFINED__
typedef interface IEventTrigger IEventTrigger;
#endif 	/* __IEventTrigger_FWD_DEFINED__ */


#ifndef __ITimeTrigger_FWD_DEFINED__
#define __ITimeTrigger_FWD_DEFINED__
typedef interface ITimeTrigger ITimeTrigger;
#endif 	/* __ITimeTrigger_FWD_DEFINED__ */


#ifndef __IDailyTrigger_FWD_DEFINED__
#define __IDailyTrigger_FWD_DEFINED__
typedef interface IDailyTrigger IDailyTrigger;
#endif 	/* __IDailyTrigger_FWD_DEFINED__ */


#ifndef __IWeeklyTrigger_FWD_DEFINED__
#define __IWeeklyTrigger_FWD_DEFINED__
typedef interface IWeeklyTrigger IWeeklyTrigger;
#endif 	/* __IWeeklyTrigger_FWD_DEFINED__ */


#ifndef __IMonthlyTrigger_FWD_DEFINED__
#define __IMonthlyTrigger_FWD_DEFINED__
typedef interface IMonthlyTrigger IMonthlyTrigger;
#endif 	/* __IMonthlyTrigger_FWD_DEFINED__ */


#ifndef __IMonthlyDOWTrigger_FWD_DEFINED__
#define __IMonthlyDOWTrigger_FWD_DEFINED__
typedef interface IMonthlyDOWTrigger IMonthlyDOWTrigger;
#endif 	/* __IMonthlyDOWTrigger_FWD_DEFINED__ */


#ifndef __IIdleTrigger_FWD_DEFINED__
#define __IIdleTrigger_FWD_DEFINED__
typedef interface IIdleTrigger IIdleTrigger;
#endif 	/* __IIdleTrigger_FWD_DEFINED__ */


#ifndef __ILogonTrigger_FWD_DEFINED__
#define __ILogonTrigger_FWD_DEFINED__
typedef interface ILogonTrigger ILogonTrigger;
#endif 	/* __ILogonTrigger_FWD_DEFINED__ */


#ifndef __ISessionStateChangeTrigger_FWD_DEFINED__
#define __ISessionStateChangeTrigger_FWD_DEFINED__
typedef interface ISessionStateChangeTrigger ISessionStateChangeTrigger;
#endif 	/* __ISessionStateChangeTrigger_FWD_DEFINED__ */


#ifndef __IBootTrigger_FWD_DEFINED__
#define __IBootTrigger_FWD_DEFINED__
typedef interface IBootTrigger IBootTrigger;
#endif 	/* __IBootTrigger_FWD_DEFINED__ */


#ifndef __IRegistrationTrigger_FWD_DEFINED__
#define __IRegistrationTrigger_FWD_DEFINED__
typedef interface IRegistrationTrigger IRegistrationTrigger;
#endif 	/* __IRegistrationTrigger_FWD_DEFINED__ */


#ifndef __IAction_FWD_DEFINED__
#define __IAction_FWD_DEFINED__
typedef interface IAction IAction;
#endif 	/* __IAction_FWD_DEFINED__ */


#ifndef __IExecAction_FWD_DEFINED__
#define __IExecAction_FWD_DEFINED__
typedef interface IExecAction IExecAction;
#endif 	/* __IExecAction_FWD_DEFINED__ */


#ifndef __IComHandlerAction_FWD_DEFINED__
#define __IComHandlerAction_FWD_DEFINED__
typedef interface IComHandlerAction IComHandlerAction;
#endif 	/* __IComHandlerAction_FWD_DEFINED__ */


#ifndef __IEmailAction_FWD_DEFINED__
#define __IEmailAction_FWD_DEFINED__
typedef interface IEmailAction IEmailAction;
#endif 	/* __IEmailAction_FWD_DEFINED__ */


#ifndef __ITriggerCollection_FWD_DEFINED__
#define __ITriggerCollection_FWD_DEFINED__
typedef interface ITriggerCollection ITriggerCollection;
#endif 	/* __ITriggerCollection_FWD_DEFINED__ */


#ifndef __IActionCollection_FWD_DEFINED__
#define __IActionCollection_FWD_DEFINED__
typedef interface IActionCollection IActionCollection;
#endif 	/* __IActionCollection_FWD_DEFINED__ */


#ifndef __IPrincipal_FWD_DEFINED__
#define __IPrincipal_FWD_DEFINED__
typedef interface IPrincipal IPrincipal;
#endif 	/* __IPrincipal_FWD_DEFINED__ */


#ifndef __IRegistrationInfo_FWD_DEFINED__
#define __IRegistrationInfo_FWD_DEFINED__
typedef interface IRegistrationInfo IRegistrationInfo;
#endif 	/* __IRegistrationInfo_FWD_DEFINED__ */


#ifndef __ITaskSettings_FWD_DEFINED__
#define __ITaskSettings_FWD_DEFINED__
typedef interface ITaskSettings ITaskSettings;
#endif 	/* __ITaskSettings_FWD_DEFINED__ */


#ifndef __TaskScheduler_FWD_DEFINED__
#define __TaskScheduler_FWD_DEFINED__

#ifdef __cplusplus
typedef class TaskScheduler TaskScheduler;
#else
typedef struct TaskScheduler TaskScheduler;
#endif /* __cplusplus */

#endif 	/* __TaskScheduler_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_taskschd_0000_0000 */
/* [local] */ 

/*******************************************************************************/
/*                                                                             */
/*    Copyright (C) 2004 Microsoft Corporation.  All rights reserved.          */
/*                                                                             */
/*    IDL source for Task Scheduler API                                        */
/*                                                                             */
/*******************************************************************************/


extern RPC_IF_HANDLE __MIDL_itf_taskschd_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_taskschd_0000_0000_v0_0_s_ifspec;


#ifndef __TaskScheduler_LIBRARY_DEFINED__
#define __TaskScheduler_LIBRARY_DEFINED__

/* library TaskScheduler */
/* [version][uuid] */ 

typedef 
enum _TASK_RUN_FLAGS
    {	TASK_RUN_NO_FLAGS	= 0,
	TASK_RUN_AS_SELF	= 0x1,
	TASK_RUN_IGNORE_CONSTRAINTS	= 0x2,
	TASK_RUN_USE_SESSION_ID	= 0x4,
	TASK_RUN_USER_SID	= 0x8
    } 	TASK_RUN_FLAGS;

typedef 
enum _TASK_ENUM_FLAGS
    {	TASK_ENUM_HIDDEN	= 0x1
    } 	TASK_ENUM_FLAGS;

typedef 
enum _TASK_LOGON_TYPE
    {	TASK_LOGON_NONE	= 0,
	TASK_LOGON_PASSWORD	= 1,
	TASK_LOGON_S4U	= 2,
	TASK_LOGON_INTERACTIVE_TOKEN	= 3,
	TASK_LOGON_GROUP	= 4,
	TASK_LOGON_SERVICE_ACCOUNT	= 5,
	TASK_LOGON_INTERACTIVE_TOKEN_OR_PASSWORD	= 6
    } 	TASK_LOGON_TYPE;

typedef 
enum _TASK_RUNLEVEL
    {	TASK_RUNLEVEL_LUA	= 0,
	TASK_RUNLEVEL_HIGHEST	= 1
    } 	TASK_RUNLEVEL_TYPE;

typedef 
enum _TASK_STATE
    {	TASK_STATE_UNKNOWN	= 0,
	TASK_STATE_DISABLED	= 1,
	TASK_STATE_QUEUED	= 2,
	TASK_STATE_READY	= 3,
	TASK_STATE_RUNNING	= 4
    } 	TASK_STATE;

typedef 
enum _TASK_CREATION
    {	TASK_VALIDATE_ONLY	= 0x1,
	TASK_CREATE	= 0x2,
	TASK_UPDATE	= 0x4,
	TASK_CREATE_OR_UPDATE	= ( TASK_CREATE | TASK_UPDATE ) ,
	TASK_DISABLE	= 0x8,
	TASK_DONT_ADD_PRINCIPAL_ACE	= 0x10
    } 	TASK_CREATION;

typedef 
enum _TASK_TRIGGER_TYPE2
    {	TASK_TRIGGER_EVENT	= 0,
	TASK_TRIGGER_TIME	= 1,
	TASK_TRIGGER_DAILY	= 2,
	TASK_TRIGGER_WEEKLY	= 3,
	TASK_TRIGGER_MONTHLY	= 4,
	TASK_TRIGGER_MONTHLYDOW	= 5,
	TASK_TRIGGER_IDLE	= 6,
	TASK_TRIGGER_REGISTRATION	= 7,
	TASK_TRIGGER_BOOT	= 8,
	TASK_TRIGGER_LOGON	= 9,
	TASK_TRIGGER_SESSION_STATE_CHANGE	= 11
    } 	TASK_TRIGGER_TYPE2;

typedef 
enum _TASK_SESSION_STATE_CHANGE_TYPE
    {	TASK_CONSOLE_CONNECT	= 1,
	TASK_CONSOLE_DISCONNECT	= 2,
	TASK_REMOTE_CONNECT	= 3,
	TASK_REMOTE_DISCONNECT	= 4,
	TASK_SESSION_LOCK	= 7,
	TASK_SESSION_UNLOCK	= 8
    } 	TASK_SESSION_STATE_CHANGE_TYPE;

typedef 
enum _TASK_ACTION_TYPE
    {	TASK_ACTION_EXEC	= 0,
	TASK_ACTION_COM_HANDLER	= 5,
	TASK_ACTION_SEND_EMAIL	= 6
    } 	TASK_ACTION_TYPE;

typedef 
enum _TASK_INSTANCES_POLICY
    {	TASK_INSTANCES_PARALLEL	= 0,
	TASK_INSTANCES_QUEUE	= 1,
	TASK_INSTANCES_IGNORE_NEW	= 2,
	TASK_INSTANCES_STOP_EXISTING	= 3
    } 	TASK_INSTANCES_POLICY;

typedef 
enum _TASK_COMPATIBILITY
    {	TASK_COMPATIBILITY_AT	= 0,
	TASK_COMPATIBILITY_V1	= 1,
	TASK_COMPATIBILITY_V2	= 2
    } 	TASK_COMPATIBILITY;





EXTERN_C const IID LIBID_TaskScheduler;

#ifndef __ITaskFolderCollection_INTERFACE_DEFINED__
#define __ITaskFolderCollection_INTERFACE_DEFINED__

/* interface ITaskFolderCollection */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ITaskFolderCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("79184a66-8664-423f-97f1-637356a5d812")
    ITaskFolderCollection : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ __RPC__out LONG *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskFolderCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskFolderCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskFolderCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskFolderCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskFolderCollection * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskFolderCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskFolderCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskFolderCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ITaskFolderCollection * This,
            /* [retval][out] */ __RPC__out LONG *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ITaskFolderCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ITaskFolderCollection * This,
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum);
        
        END_INTERFACE
    } ITaskFolderCollectionVtbl;

    interface ITaskFolderCollection
    {
        CONST_VTBL struct ITaskFolderCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskFolderCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskFolderCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskFolderCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskFolderCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskFolderCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskFolderCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskFolderCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskFolderCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define ITaskFolderCollection_get_Item(This,index,ppFolder)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppFolder) ) 

#define ITaskFolderCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskFolderCollection_INTERFACE_DEFINED__ */


#ifndef __ITaskDefinition_INTERFACE_DEFINED__
#define __ITaskDefinition_INTERFACE_DEFINED__

/* interface ITaskDefinition */
/* [helpstring][nonextensible][oleautomation][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITaskDefinition;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f5bc8fc5-536d-4f77-b852-fbc1356fdeb6")
    ITaskDefinition : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RegistrationInfo( 
            /* [retval][out] */ IRegistrationInfo **ppRegistrationInfo) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RegistrationInfo( 
            /* [in] */ IRegistrationInfo *pRegistrationInfo) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Triggers( 
            /* [retval][out] */ ITriggerCollection **ppTriggers) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Triggers( 
            /* [in] */ ITriggerCollection *pTriggers) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Settings( 
            /* [retval][out] */ ITaskSettings **ppSettings) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Settings( 
            /* [in] */ ITaskSettings *pSettings) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ BSTR *pData) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Data( 
            /* [in] */ BSTR data) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Principal( 
            /* [retval][out] */ IPrincipal **ppPrincipal) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Principal( 
            /* [in] */ IPrincipal *pPrincipal) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Actions( 
            /* [retval][out] */ IActionCollection **ppActions) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Actions( 
            /* [in] */ IActionCollection *pActions) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_XmlText( 
            /* [retval][out] */ BSTR *pXml) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_XmlText( 
            /* [in] */ BSTR xml) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskDefinitionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskDefinition * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskDefinition * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskDefinition * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskDefinition * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskDefinition * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskDefinition * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskDefinition * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RegistrationInfo )( 
            ITaskDefinition * This,
            /* [retval][out] */ IRegistrationInfo **ppRegistrationInfo);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RegistrationInfo )( 
            ITaskDefinition * This,
            /* [in] */ IRegistrationInfo *pRegistrationInfo);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Triggers )( 
            ITaskDefinition * This,
            /* [retval][out] */ ITriggerCollection **ppTriggers);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Triggers )( 
            ITaskDefinition * This,
            /* [in] */ ITriggerCollection *pTriggers);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Settings )( 
            ITaskDefinition * This,
            /* [retval][out] */ ITaskSettings **ppSettings);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Settings )( 
            ITaskDefinition * This,
            /* [in] */ ITaskSettings *pSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            ITaskDefinition * This,
            /* [retval][out] */ BSTR *pData);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Data )( 
            ITaskDefinition * This,
            /* [in] */ BSTR data);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Principal )( 
            ITaskDefinition * This,
            /* [retval][out] */ IPrincipal **ppPrincipal);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Principal )( 
            ITaskDefinition * This,
            /* [in] */ IPrincipal *pPrincipal);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Actions )( 
            ITaskDefinition * This,
            /* [retval][out] */ IActionCollection **ppActions);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Actions )( 
            ITaskDefinition * This,
            /* [in] */ IActionCollection *pActions);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_XmlText )( 
            ITaskDefinition * This,
            /* [retval][out] */ BSTR *pXml);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_XmlText )( 
            ITaskDefinition * This,
            /* [in] */ BSTR xml);
        
        END_INTERFACE
    } ITaskDefinitionVtbl;

    interface ITaskDefinition
    {
        CONST_VTBL struct ITaskDefinitionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskDefinition_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskDefinition_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskDefinition_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskDefinition_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskDefinition_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskDefinition_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskDefinition_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskDefinition_get_RegistrationInfo(This,ppRegistrationInfo)	\
    ( (This)->lpVtbl -> get_RegistrationInfo(This,ppRegistrationInfo) ) 

#define ITaskDefinition_put_RegistrationInfo(This,pRegistrationInfo)	\
    ( (This)->lpVtbl -> put_RegistrationInfo(This,pRegistrationInfo) ) 

#define ITaskDefinition_get_Triggers(This,ppTriggers)	\
    ( (This)->lpVtbl -> get_Triggers(This,ppTriggers) ) 

#define ITaskDefinition_put_Triggers(This,pTriggers)	\
    ( (This)->lpVtbl -> put_Triggers(This,pTriggers) ) 

#define ITaskDefinition_get_Settings(This,ppSettings)	\
    ( (This)->lpVtbl -> get_Settings(This,ppSettings) ) 

#define ITaskDefinition_put_Settings(This,pSettings)	\
    ( (This)->lpVtbl -> put_Settings(This,pSettings) ) 

#define ITaskDefinition_get_Data(This,pData)	\
    ( (This)->lpVtbl -> get_Data(This,pData) ) 

#define ITaskDefinition_put_Data(This,data)	\
    ( (This)->lpVtbl -> put_Data(This,data) ) 

#define ITaskDefinition_get_Principal(This,ppPrincipal)	\
    ( (This)->lpVtbl -> get_Principal(This,ppPrincipal) ) 

#define ITaskDefinition_put_Principal(This,pPrincipal)	\
    ( (This)->lpVtbl -> put_Principal(This,pPrincipal) ) 

#define ITaskDefinition_get_Actions(This,ppActions)	\
    ( (This)->lpVtbl -> get_Actions(This,ppActions) ) 

#define ITaskDefinition_put_Actions(This,pActions)	\
    ( (This)->lpVtbl -> put_Actions(This,pActions) ) 

#define ITaskDefinition_get_XmlText(This,pXml)	\
    ( (This)->lpVtbl -> get_XmlText(This,pXml) ) 

#define ITaskDefinition_put_XmlText(This,xml)	\
    ( (This)->lpVtbl -> put_XmlText(This,xml) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskDefinition_INTERFACE_DEFINED__ */


#ifndef __ITaskService_INTERFACE_DEFINED__
#define __ITaskService_INTERFACE_DEFINED__

/* interface ITaskService */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ITaskService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2faba4c7-4da9-4013-9697-20cc3fd40f85")
    ITaskService : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFolder( 
            /* [in] */ __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRunningTasks( 
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTaskCollection **ppRunningTasks) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE NewTask( 
            /* [in] */ DWORD flags,
            /* [retval][out] */ __RPC__deref_out_opt ITaskDefinition **ppDefinition) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [optional][in] */ VARIANT serverName,
            /* [optional][in] */ VARIANT user,
            /* [optional][in] */ VARIANT domain,
            /* [optional][in] */ VARIANT password) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Connected( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pConnected) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_TargetServer( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pServer) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectedUser( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pUser) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectedDomain( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pDomain) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HighestVersion( 
            /* [retval][out] */ __RPC__out DWORD *pVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskService * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskService * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskService * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFolder )( 
            ITaskService * This,
            /* [in] */ __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRunningTasks )( 
            ITaskService * This,
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTaskCollection **ppRunningTasks);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *NewTask )( 
            ITaskService * This,
            /* [in] */ DWORD flags,
            /* [retval][out] */ __RPC__deref_out_opt ITaskDefinition **ppDefinition);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            ITaskService * This,
            /* [optional][in] */ VARIANT serverName,
            /* [optional][in] */ VARIANT user,
            /* [optional][in] */ VARIANT domain,
            /* [optional][in] */ VARIANT password);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            ITaskService * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pConnected);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TargetServer )( 
            ITaskService * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pServer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectedUser )( 
            ITaskService * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pUser);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectedDomain )( 
            ITaskService * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pDomain);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HighestVersion )( 
            ITaskService * This,
            /* [retval][out] */ __RPC__out DWORD *pVersion);
        
        END_INTERFACE
    } ITaskServiceVtbl;

    interface ITaskService
    {
        CONST_VTBL struct ITaskServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskService_GetFolder(This,path,ppFolder)	\
    ( (This)->lpVtbl -> GetFolder(This,path,ppFolder) ) 

#define ITaskService_GetRunningTasks(This,flags,ppRunningTasks)	\
    ( (This)->lpVtbl -> GetRunningTasks(This,flags,ppRunningTasks) ) 

#define ITaskService_NewTask(This,flags,ppDefinition)	\
    ( (This)->lpVtbl -> NewTask(This,flags,ppDefinition) ) 

#define ITaskService_Connect(This,serverName,user,domain,password)	\
    ( (This)->lpVtbl -> Connect(This,serverName,user,domain,password) ) 

#define ITaskService_get_Connected(This,pConnected)	\
    ( (This)->lpVtbl -> get_Connected(This,pConnected) ) 

#define ITaskService_get_TargetServer(This,pServer)	\
    ( (This)->lpVtbl -> get_TargetServer(This,pServer) ) 

#define ITaskService_get_ConnectedUser(This,pUser)	\
    ( (This)->lpVtbl -> get_ConnectedUser(This,pUser) ) 

#define ITaskService_get_ConnectedDomain(This,pDomain)	\
    ( (This)->lpVtbl -> get_ConnectedDomain(This,pDomain) ) 

#define ITaskService_get_HighestVersion(This,pVersion)	\
    ( (This)->lpVtbl -> get_HighestVersion(This,pVersion) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskService_INTERFACE_DEFINED__ */


#ifndef __ITaskNamedValuePair_INTERFACE_DEFINED__
#define __ITaskNamedValuePair_INTERFACE_DEFINED__

/* interface ITaskNamedValuePair */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITaskNamedValuePair;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("39038068-2B46-4afd-8662-7BB6F868D221")
    ITaskNamedValuePair : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ BSTR *pValue) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ BSTR value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskNamedValuePairVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskNamedValuePair * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskNamedValuePair * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskNamedValuePair * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskNamedValuePair * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskNamedValuePair * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskNamedValuePair * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskNamedValuePair * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ITaskNamedValuePair * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            ITaskNamedValuePair * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            ITaskNamedValuePair * This,
            /* [retval][out] */ BSTR *pValue);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            ITaskNamedValuePair * This,
            /* [in] */ BSTR value);
        
        END_INTERFACE
    } ITaskNamedValuePairVtbl;

    interface ITaskNamedValuePair
    {
        CONST_VTBL struct ITaskNamedValuePairVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskNamedValuePair_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskNamedValuePair_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskNamedValuePair_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskNamedValuePair_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskNamedValuePair_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskNamedValuePair_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskNamedValuePair_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskNamedValuePair_get_Name(This,pName)	\
    ( (This)->lpVtbl -> get_Name(This,pName) ) 

#define ITaskNamedValuePair_put_Name(This,name)	\
    ( (This)->lpVtbl -> put_Name(This,name) ) 

#define ITaskNamedValuePair_get_Value(This,pValue)	\
    ( (This)->lpVtbl -> get_Value(This,pValue) ) 

#define ITaskNamedValuePair_put_Value(This,value)	\
    ( (This)->lpVtbl -> put_Value(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskNamedValuePair_INTERFACE_DEFINED__ */


#ifndef __ITaskNamedValueCollection_INTERFACE_DEFINED__
#define __ITaskNamedValueCollection_INTERFACE_DEFINED__

/* interface ITaskNamedValueCollection */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITaskNamedValueCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B4EF826B-63C3-46e4-A504-EF69E4F7EA4D")
    ITaskNamedValueCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ LONG index,
            /* [retval][out] */ ITaskNamedValuePair **ppPair) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ BSTR name,
            /* [in] */ BSTR value,
            /* [retval][out] */ ITaskNamedValuePair **ppPair) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ LONG index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskNamedValueCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskNamedValueCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskNamedValueCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskNamedValueCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskNamedValueCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskNamedValueCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskNamedValueCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskNamedValueCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ITaskNamedValueCollection * This,
            /* [retval][out] */ long *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ITaskNamedValueCollection * This,
            /* [in] */ LONG index,
            /* [retval][out] */ ITaskNamedValuePair **ppPair);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ITaskNamedValueCollection * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            ITaskNamedValueCollection * This,
            /* [in] */ BSTR name,
            /* [in] */ BSTR value,
            /* [retval][out] */ ITaskNamedValuePair **ppPair);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            ITaskNamedValueCollection * This,
            /* [in] */ LONG index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            ITaskNamedValueCollection * This);
        
        END_INTERFACE
    } ITaskNamedValueCollectionVtbl;

    interface ITaskNamedValueCollection
    {
        CONST_VTBL struct ITaskNamedValueCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskNamedValueCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskNamedValueCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskNamedValueCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskNamedValueCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskNamedValueCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskNamedValueCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskNamedValueCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskNamedValueCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define ITaskNamedValueCollection_get_Item(This,index,ppPair)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppPair) ) 

#define ITaskNamedValueCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#define ITaskNamedValueCollection_Create(This,name,value,ppPair)	\
    ( (This)->lpVtbl -> Create(This,name,value,ppPair) ) 

#define ITaskNamedValueCollection_Remove(This,index)	\
    ( (This)->lpVtbl -> Remove(This,index) ) 

#define ITaskNamedValueCollection_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskNamedValueCollection_INTERFACE_DEFINED__ */


#ifndef __IRunningTask_INTERFACE_DEFINED__
#define __IRunningTask_INTERFACE_DEFINED__

/* interface IRunningTask */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IRunningTask;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("653758fb-7b9a-4f1e-a471-beeb8e9b834e")
    IRunningTask : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InstanceGuid( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pGuid) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ __RPC__out TASK_STATE *pState) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentAction( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRunningTaskVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRunningTask * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRunningTask * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRunningTask * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRunningTask * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRunningTask * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRunningTask * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRunningTask * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IRunningTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InstanceGuid )( 
            IRunningTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pGuid);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IRunningTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_State )( 
            IRunningTask * This,
            /* [retval][out] */ __RPC__out TASK_STATE *pState);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentAction )( 
            IRunningTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IRunningTask * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IRunningTask * This);
        
        END_INTERFACE
    } IRunningTaskVtbl;

    interface IRunningTask
    {
        CONST_VTBL struct IRunningTaskVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRunningTask_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRunningTask_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRunningTask_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRunningTask_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRunningTask_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRunningTask_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRunningTask_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRunningTask_get_Name(This,pName)	\
    ( (This)->lpVtbl -> get_Name(This,pName) ) 

#define IRunningTask_get_InstanceGuid(This,pGuid)	\
    ( (This)->lpVtbl -> get_InstanceGuid(This,pGuid) ) 

#define IRunningTask_get_Path(This,pPath)	\
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define IRunningTask_get_State(This,pState)	\
    ( (This)->lpVtbl -> get_State(This,pState) ) 

#define IRunningTask_get_CurrentAction(This,pName)	\
    ( (This)->lpVtbl -> get_CurrentAction(This,pName) ) 

#define IRunningTask_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IRunningTask_Refresh(This)	\
    ( (This)->lpVtbl -> Refresh(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRunningTask_INTERFACE_DEFINED__ */


#ifndef __IRunningTaskCollection_INTERFACE_DEFINED__
#define __IRunningTaskCollection_INTERFACE_DEFINED__

/* interface IRunningTaskCollection */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IRunningTaskCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6a67614b-6828-4fec-aa54-6d52e8f1f2db")
    IRunningTaskCollection : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ __RPC__out LONG *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRunningTaskCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRunningTaskCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRunningTaskCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRunningTaskCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRunningTaskCollection * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRunningTaskCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRunningTaskCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRunningTaskCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IRunningTaskCollection * This,
            /* [retval][out] */ __RPC__out LONG *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IRunningTaskCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IRunningTaskCollection * This,
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum);
        
        END_INTERFACE
    } IRunningTaskCollectionVtbl;

    interface IRunningTaskCollection
    {
        CONST_VTBL struct IRunningTaskCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRunningTaskCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRunningTaskCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRunningTaskCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRunningTaskCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRunningTaskCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRunningTaskCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRunningTaskCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRunningTaskCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IRunningTaskCollection_get_Item(This,index,ppRunningTask)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppRunningTask) ) 

#define IRunningTaskCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRunningTaskCollection_INTERFACE_DEFINED__ */


#ifndef __IRegisteredTask_INTERFACE_DEFINED__
#define __IRegisteredTask_INTERFACE_DEFINED__

/* interface IRegisteredTask */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IRegisteredTask;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9c86f320-dee3-4dd1-b972-a303f26b061e")
    IRegisteredTask : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ __RPC__out TASK_STATE *pState) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pEnabled) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            VARIANT_BOOL enabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ VARIANT params,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RunEx( 
            /* [in] */ VARIANT params,
            /* [in] */ LONG flags,
            /* [in] */ LONG sessionID,
            /* [in] */ __RPC__in BSTR user,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetInstances( 
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTaskCollection **ppRunningTasks) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LastRunTime( 
            /* [retval][out] */ __RPC__out DATE *pLastRunTime) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LastTaskResult( 
            /* [retval][out] */ __RPC__out HRESULT *pLastTaskResult) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LastActionResult( 
            /* [retval][out] */ __RPC__out LONG *pLastActionResult) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_NumberOfMissedRuns( 
            /* [retval][out] */ __RPC__out LONG *pNumberOfMissedRuns) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_NextRunTime( 
            /* [retval][out] */ __RPC__out DATE *pNextRunTime) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Definition( 
            /* [retval][out] */ __RPC__deref_out_opt ITaskDefinition **ppDefinition) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Xml( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pXml) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSecurityDescriptor( 
            LONG securityInformation,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pSddl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSecurityDescriptor( 
            /* [in] */ __RPC__in BSTR sddl,
            /* [in] */ LONG flags) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( 
            LONG flags) = 0;
        
        virtual /* [hidden][restricted] */ HRESULT STDMETHODCALLTYPE GetRunTimes( 
            /* [in] */ __RPC__in const LPSYSTEMTIME pstStart,
            /* [in] */ __RPC__in const LPSYSTEMTIME pstEnd,
            /* [out] */ __RPC__out DWORD *pCount,
            /* [out] */ __RPC__deref_out_opt LPSYSTEMTIME *pRunTimes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRegisteredTaskVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRegisteredTask * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRegisteredTask * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRegisteredTask * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRegisteredTask * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRegisteredTask * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRegisteredTask * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRegisteredTask * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_State )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out TASK_STATE *pState);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out VARIANT_BOOL *pEnabled);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IRegisteredTask * This,
            VARIANT_BOOL enabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IRegisteredTask * This,
            /* [in] */ VARIANT params,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RunEx )( 
            IRegisteredTask * This,
            /* [in] */ VARIANT params,
            /* [in] */ LONG flags,
            /* [in] */ LONG sessionID,
            /* [in] */ __RPC__in BSTR user,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTask **ppRunningTask);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetInstances )( 
            IRegisteredTask * This,
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRunningTaskCollection **ppRunningTasks);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastRunTime )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out DATE *pLastRunTime);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastTaskResult )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out HRESULT *pLastTaskResult);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastActionResult )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out LONG *pLastActionResult);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfMissedRuns )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out LONG *pNumberOfMissedRuns);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NextRunTime )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__out DATE *pNextRunTime);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Definition )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__deref_out_opt ITaskDefinition **ppDefinition);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Xml )( 
            IRegisteredTask * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pXml);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSecurityDescriptor )( 
            IRegisteredTask * This,
            LONG securityInformation,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pSddl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSecurityDescriptor )( 
            IRegisteredTask * This,
            /* [in] */ __RPC__in BSTR sddl,
            /* [in] */ LONG flags);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IRegisteredTask * This,
            LONG flags);
        
        /* [hidden][restricted] */ HRESULT ( STDMETHODCALLTYPE *GetRunTimes )( 
            IRegisteredTask * This,
            /* [in] */ __RPC__in const LPSYSTEMTIME pstStart,
            /* [in] */ __RPC__in const LPSYSTEMTIME pstEnd,
            /* [out] */ __RPC__out DWORD *pCount,
            /* [out] */ __RPC__deref_out_opt LPSYSTEMTIME *pRunTimes);
        
        END_INTERFACE
    } IRegisteredTaskVtbl;

    interface IRegisteredTask
    {
        CONST_VTBL struct IRegisteredTaskVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRegisteredTask_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRegisteredTask_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRegisteredTask_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRegisteredTask_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRegisteredTask_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRegisteredTask_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRegisteredTask_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRegisteredTask_get_Name(This,pName)	\
    ( (This)->lpVtbl -> get_Name(This,pName) ) 

#define IRegisteredTask_get_Path(This,pPath)	\
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define IRegisteredTask_get_State(This,pState)	\
    ( (This)->lpVtbl -> get_State(This,pState) ) 

#define IRegisteredTask_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IRegisteredTask_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 

#define IRegisteredTask_Run(This,params,ppRunningTask)	\
    ( (This)->lpVtbl -> Run(This,params,ppRunningTask) ) 

#define IRegisteredTask_RunEx(This,params,flags,sessionID,user,ppRunningTask)	\
    ( (This)->lpVtbl -> RunEx(This,params,flags,sessionID,user,ppRunningTask) ) 

#define IRegisteredTask_GetInstances(This,flags,ppRunningTasks)	\
    ( (This)->lpVtbl -> GetInstances(This,flags,ppRunningTasks) ) 

#define IRegisteredTask_get_LastRunTime(This,pLastRunTime)	\
    ( (This)->lpVtbl -> get_LastRunTime(This,pLastRunTime) ) 

#define IRegisteredTask_get_LastTaskResult(This,pLastTaskResult)	\
    ( (This)->lpVtbl -> get_LastTaskResult(This,pLastTaskResult) ) 

#define IRegisteredTask_get_LastActionResult(This,pLastActionResult)	\
    ( (This)->lpVtbl -> get_LastActionResult(This,pLastActionResult) ) 

#define IRegisteredTask_get_NumberOfMissedRuns(This,pNumberOfMissedRuns)	\
    ( (This)->lpVtbl -> get_NumberOfMissedRuns(This,pNumberOfMissedRuns) ) 

#define IRegisteredTask_get_NextRunTime(This,pNextRunTime)	\
    ( (This)->lpVtbl -> get_NextRunTime(This,pNextRunTime) ) 

#define IRegisteredTask_get_Definition(This,ppDefinition)	\
    ( (This)->lpVtbl -> get_Definition(This,ppDefinition) ) 

#define IRegisteredTask_get_Xml(This,pXml)	\
    ( (This)->lpVtbl -> get_Xml(This,pXml) ) 

#define IRegisteredTask_GetSecurityDescriptor(This,securityInformation,pSddl)	\
    ( (This)->lpVtbl -> GetSecurityDescriptor(This,securityInformation,pSddl) ) 

#define IRegisteredTask_SetSecurityDescriptor(This,sddl,flags)	\
    ( (This)->lpVtbl -> SetSecurityDescriptor(This,sddl,flags) ) 

#define IRegisteredTask_Stop(This,flags)	\
    ( (This)->lpVtbl -> Stop(This,flags) ) 

#define IRegisteredTask_GetRunTimes(This,pstStart,pstEnd,pCount,pRunTimes)	\
    ( (This)->lpVtbl -> GetRunTimes(This,pstStart,pstEnd,pCount,pRunTimes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRegisteredTask_INTERFACE_DEFINED__ */


#ifndef __IRegisteredTaskCollection_INTERFACE_DEFINED__
#define __IRegisteredTaskCollection_INTERFACE_DEFINED__

/* interface IRegisteredTaskCollection */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_IRegisteredTaskCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("86627eb4-42a7-41e4-a4d9-ac33a72f2d52")
    IRegisteredTaskCollection : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ __RPC__out LONG *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppRegisteredTask) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRegisteredTaskCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRegisteredTaskCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRegisteredTaskCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRegisteredTaskCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRegisteredTaskCollection * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRegisteredTaskCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRegisteredTaskCollection * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRegisteredTaskCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IRegisteredTaskCollection * This,
            /* [retval][out] */ __RPC__out LONG *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IRegisteredTaskCollection * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppRegisteredTask);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IRegisteredTaskCollection * This,
            /* [retval][out] */ __RPC__deref_out_opt IUnknown **ppEnum);
        
        END_INTERFACE
    } IRegisteredTaskCollectionVtbl;

    interface IRegisteredTaskCollection
    {
        CONST_VTBL struct IRegisteredTaskCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRegisteredTaskCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRegisteredTaskCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRegisteredTaskCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRegisteredTaskCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRegisteredTaskCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRegisteredTaskCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRegisteredTaskCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRegisteredTaskCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IRegisteredTaskCollection_get_Item(This,index,ppRegisteredTask)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppRegisteredTask) ) 

#define IRegisteredTaskCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRegisteredTaskCollection_INTERFACE_DEFINED__ */


#ifndef __ITaskFolder_INTERFACE_DEFINED__
#define __ITaskFolder_INTERFACE_DEFINED__

/* interface ITaskFolder */
/* [nonextensible][oleautomation][dual][uuid][object] */ 


EXTERN_C const IID IID_ITaskFolder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8cfac062-a080-4c15-9a88-aa7c2af80dfc")
    ITaskFolder : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFolder( 
            __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFolders( 
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolderCollection **ppFolders) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateFolder( 
            /* [in] */ __RPC__in BSTR folderName,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeleteFolder( 
            __RPC__in BSTR folderName,
            /* [in] */ LONG flags) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTask( 
            __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTasks( 
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTaskCollection **ppTasks) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DeleteTask( 
            /* [in] */ __RPC__in BSTR name,
            /* [in] */ LONG flags) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterTask( 
            /* [in] */ __RPC__in BSTR path,
            /* [in] */ __RPC__in BSTR xmlText,
            /* [in] */ LONG flags,
            /* [in] */ VARIANT userId,
            /* [in] */ VARIANT password,
            /* [in] */ TASK_LOGON_TYPE logonType,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterTaskDefinition( 
            /* [in] */ __RPC__in BSTR path,
            /* [in] */ __RPC__in_opt ITaskDefinition *pDefinition,
            /* [in] */ LONG flags,
            /* [in] */ VARIANT userId,
            /* [in] */ VARIANT password,
            /* [in] */ TASK_LOGON_TYPE logonType,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSecurityDescriptor( 
            LONG securityInformation,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pSddl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSecurityDescriptor( 
            /* [in] */ __RPC__in BSTR sddl,
            /* [in] */ LONG flags) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskFolderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskFolder * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskFolder * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskFolder * This,
            /* [out] */ __RPC__out UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskFolder * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskFolder * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ITaskFolder * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pName);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            ITaskFolder * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFolder )( 
            ITaskFolder * This,
            __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFolders )( 
            ITaskFolder * This,
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolderCollection **ppFolders);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateFolder )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in BSTR folderName,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt ITaskFolder **ppFolder);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteFolder )( 
            ITaskFolder * This,
            __RPC__in BSTR folderName,
            /* [in] */ LONG flags);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTask )( 
            ITaskFolder * This,
            __RPC__in BSTR path,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTasks )( 
            ITaskFolder * This,
            LONG flags,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTaskCollection **ppTasks);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DeleteTask )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in BSTR name,
            /* [in] */ LONG flags);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterTask )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in BSTR path,
            /* [in] */ __RPC__in BSTR xmlText,
            /* [in] */ LONG flags,
            /* [in] */ VARIANT userId,
            /* [in] */ VARIANT password,
            /* [in] */ TASK_LOGON_TYPE logonType,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterTaskDefinition )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in BSTR path,
            /* [in] */ __RPC__in_opt ITaskDefinition *pDefinition,
            /* [in] */ LONG flags,
            /* [in] */ VARIANT userId,
            /* [in] */ VARIANT password,
            /* [in] */ TASK_LOGON_TYPE logonType,
            /* [optional][in] */ VARIANT sddl,
            /* [retval][out] */ __RPC__deref_out_opt IRegisteredTask **ppTask);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSecurityDescriptor )( 
            ITaskFolder * This,
            LONG securityInformation,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pSddl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSecurityDescriptor )( 
            ITaskFolder * This,
            /* [in] */ __RPC__in BSTR sddl,
            /* [in] */ LONG flags);
        
        END_INTERFACE
    } ITaskFolderVtbl;

    interface ITaskFolder
    {
        CONST_VTBL struct ITaskFolderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskFolder_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskFolder_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskFolder_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskFolder_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskFolder_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskFolder_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskFolder_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskFolder_get_Name(This,pName)	\
    ( (This)->lpVtbl -> get_Name(This,pName) ) 

#define ITaskFolder_get_Path(This,pPath)	\
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define ITaskFolder_GetFolder(This,path,ppFolder)	\
    ( (This)->lpVtbl -> GetFolder(This,path,ppFolder) ) 

#define ITaskFolder_GetFolders(This,flags,ppFolders)	\
    ( (This)->lpVtbl -> GetFolders(This,flags,ppFolders) ) 

#define ITaskFolder_CreateFolder(This,folderName,sddl,ppFolder)	\
    ( (This)->lpVtbl -> CreateFolder(This,folderName,sddl,ppFolder) ) 

#define ITaskFolder_DeleteFolder(This,folderName,flags)	\
    ( (This)->lpVtbl -> DeleteFolder(This,folderName,flags) ) 

#define ITaskFolder_GetTask(This,path,ppTask)	\
    ( (This)->lpVtbl -> GetTask(This,path,ppTask) ) 

#define ITaskFolder_GetTasks(This,flags,ppTasks)	\
    ( (This)->lpVtbl -> GetTasks(This,flags,ppTasks) ) 

#define ITaskFolder_DeleteTask(This,name,flags)	\
    ( (This)->lpVtbl -> DeleteTask(This,name,flags) ) 

#define ITaskFolder_RegisterTask(This,path,xmlText,flags,userId,password,logonType,sddl,ppTask)	\
    ( (This)->lpVtbl -> RegisterTask(This,path,xmlText,flags,userId,password,logonType,sddl,ppTask) ) 

#define ITaskFolder_RegisterTaskDefinition(This,path,pDefinition,flags,userId,password,logonType,sddl,ppTask)	\
    ( (This)->lpVtbl -> RegisterTaskDefinition(This,path,pDefinition,flags,userId,password,logonType,sddl,ppTask) ) 

#define ITaskFolder_GetSecurityDescriptor(This,securityInformation,pSddl)	\
    ( (This)->lpVtbl -> GetSecurityDescriptor(This,securityInformation,pSddl) ) 

#define ITaskFolder_SetSecurityDescriptor(This,sddl,flags)	\
    ( (This)->lpVtbl -> SetSecurityDescriptor(This,sddl,flags) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskFolder_INTERFACE_DEFINED__ */


#ifndef __ITaskVariables_INTERFACE_DEFINED__
#define __ITaskVariables_INTERFACE_DEFINED__

/* interface ITaskVariables */
/* [uuid][object] */ 


EXTERN_C const IID IID_ITaskVariables;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3e4c9351-d966-4b8b-bb87-ceba68bb0107")
    ITaskVariables : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetInput( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pInput) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetOutput( 
            /* [in] */ __RPC__in BSTR input) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContext( 
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pContext) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskVariablesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskVariables * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskVariables * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskVariables * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetInput )( 
            ITaskVariables * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pInput);
        
        HRESULT ( STDMETHODCALLTYPE *SetOutput )( 
            ITaskVariables * This,
            /* [in] */ __RPC__in BSTR input);
        
        HRESULT ( STDMETHODCALLTYPE *GetContext )( 
            ITaskVariables * This,
            /* [retval][out] */ __RPC__deref_out_opt BSTR *pContext);
        
        END_INTERFACE
    } ITaskVariablesVtbl;

    interface ITaskVariables
    {
        CONST_VTBL struct ITaskVariablesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskVariables_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskVariables_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskVariables_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskVariables_GetInput(This,pInput)	\
    ( (This)->lpVtbl -> GetInput(This,pInput) ) 

#define ITaskVariables_SetOutput(This,input)	\
    ( (This)->lpVtbl -> SetOutput(This,input) ) 

#define ITaskVariables_GetContext(This,pContext)	\
    ( (This)->lpVtbl -> GetContext(This,pContext) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskVariables_INTERFACE_DEFINED__ */


#ifndef __ITaskHandlerStatus_INTERFACE_DEFINED__
#define __ITaskHandlerStatus_INTERFACE_DEFINED__

/* interface ITaskHandlerStatus */
/* [uuid][object] */ 


EXTERN_C const IID IID_ITaskHandlerStatus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("eaec7a8f-27a0-4ddc-8675-14726a01a38a")
    ITaskHandlerStatus : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE UpdateStatus( 
            /* [in] */ short percentComplete,
            /* [in] */ __RPC__in BSTR statusMessage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TaskCompleted( 
            /* [in] */ HRESULT taskErrCode) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskHandlerStatusVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskHandlerStatus * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskHandlerStatus * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskHandlerStatus * This);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateStatus )( 
            ITaskHandlerStatus * This,
            /* [in] */ short percentComplete,
            /* [in] */ __RPC__in BSTR statusMessage);
        
        HRESULT ( STDMETHODCALLTYPE *TaskCompleted )( 
            ITaskHandlerStatus * This,
            /* [in] */ HRESULT taskErrCode);
        
        END_INTERFACE
    } ITaskHandlerStatusVtbl;

    interface ITaskHandlerStatus
    {
        CONST_VTBL struct ITaskHandlerStatusVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskHandlerStatus_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskHandlerStatus_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskHandlerStatus_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskHandlerStatus_UpdateStatus(This,percentComplete,statusMessage)	\
    ( (This)->lpVtbl -> UpdateStatus(This,percentComplete,statusMessage) ) 

#define ITaskHandlerStatus_TaskCompleted(This,taskErrCode)	\
    ( (This)->lpVtbl -> TaskCompleted(This,taskErrCode) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskHandlerStatus_INTERFACE_DEFINED__ */


#ifndef __ITaskHandler_INTERFACE_DEFINED__
#define __ITaskHandler_INTERFACE_DEFINED__

/* interface ITaskHandler */
/* [uuid][object] */ 


EXTERN_C const IID IID_ITaskHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("839d7762-5121-4009-9234-4f0d19394f04")
    ITaskHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [in] */ __RPC__in_opt IUnknown *pHandlerServices,
            /* [in] */ __RPC__in BSTR data) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( 
            /* [out] */ __RPC__out HRESULT *pRetCode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Resume( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskHandler * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            ITaskHandler * This,
            /* [in] */ __RPC__in_opt IUnknown *pHandlerServices,
            /* [in] */ __RPC__in BSTR data);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            ITaskHandler * This,
            /* [out] */ __RPC__out HRESULT *pRetCode);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            ITaskHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Resume )( 
            ITaskHandler * This);
        
        END_INTERFACE
    } ITaskHandlerVtbl;

    interface ITaskHandler
    {
        CONST_VTBL struct ITaskHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskHandler_Start(This,pHandlerServices,data)	\
    ( (This)->lpVtbl -> Start(This,pHandlerServices,data) ) 

#define ITaskHandler_Stop(This,pRetCode)	\
    ( (This)->lpVtbl -> Stop(This,pRetCode) ) 

#define ITaskHandler_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define ITaskHandler_Resume(This)	\
    ( (This)->lpVtbl -> Resume(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskHandler_INTERFACE_DEFINED__ */


#ifndef __IIdleSettings_INTERFACE_DEFINED__
#define __IIdleSettings_INTERFACE_DEFINED__

/* interface IIdleSettings */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IIdleSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84594461-0053-4342-A8FD-088FABF11F32")
    IIdleSettings : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IdleDuration( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_IdleDuration( 
            /* [in] */ BSTR delay) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WaitTimeout( 
            /* [retval][out] */ BSTR *pTimeout) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WaitTimeout( 
            /* [in] */ BSTR timeout) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StopOnIdleEnd( 
            /* [retval][out] */ VARIANT_BOOL *pStop) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StopOnIdleEnd( 
            /* [in] */ VARIANT_BOOL stop) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RestartOnIdle( 
            /* [retval][out] */ VARIANT_BOOL *pRestart) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RestartOnIdle( 
            /* [in] */ VARIANT_BOOL restart) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIdleSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIdleSettings * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIdleSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIdleSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIdleSettings * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIdleSettings * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIdleSettings * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIdleSettings * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleDuration )( 
            IIdleSettings * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IdleDuration )( 
            IIdleSettings * This,
            /* [in] */ BSTR delay);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_WaitTimeout )( 
            IIdleSettings * This,
            /* [retval][out] */ BSTR *pTimeout);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_WaitTimeout )( 
            IIdleSettings * This,
            /* [in] */ BSTR timeout);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StopOnIdleEnd )( 
            IIdleSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pStop);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StopOnIdleEnd )( 
            IIdleSettings * This,
            /* [in] */ VARIANT_BOOL stop);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RestartOnIdle )( 
            IIdleSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pRestart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RestartOnIdle )( 
            IIdleSettings * This,
            /* [in] */ VARIANT_BOOL restart);
        
        END_INTERFACE
    } IIdleSettingsVtbl;

    interface IIdleSettings
    {
        CONST_VTBL struct IIdleSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIdleSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIdleSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIdleSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIdleSettings_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIdleSettings_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIdleSettings_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIdleSettings_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IIdleSettings_get_IdleDuration(This,pDelay)	\
    ( (This)->lpVtbl -> get_IdleDuration(This,pDelay) ) 

#define IIdleSettings_put_IdleDuration(This,delay)	\
    ( (This)->lpVtbl -> put_IdleDuration(This,delay) ) 

#define IIdleSettings_get_WaitTimeout(This,pTimeout)	\
    ( (This)->lpVtbl -> get_WaitTimeout(This,pTimeout) ) 

#define IIdleSettings_put_WaitTimeout(This,timeout)	\
    ( (This)->lpVtbl -> put_WaitTimeout(This,timeout) ) 

#define IIdleSettings_get_StopOnIdleEnd(This,pStop)	\
    ( (This)->lpVtbl -> get_StopOnIdleEnd(This,pStop) ) 

#define IIdleSettings_put_StopOnIdleEnd(This,stop)	\
    ( (This)->lpVtbl -> put_StopOnIdleEnd(This,stop) ) 

#define IIdleSettings_get_RestartOnIdle(This,pRestart)	\
    ( (This)->lpVtbl -> get_RestartOnIdle(This,pRestart) ) 

#define IIdleSettings_put_RestartOnIdle(This,restart)	\
    ( (This)->lpVtbl -> put_RestartOnIdle(This,restart) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIdleSettings_INTERFACE_DEFINED__ */


#ifndef __IRepetitionPattern_INTERFACE_DEFINED__
#define __IRepetitionPattern_INTERFACE_DEFINED__

/* interface IRepetitionPattern */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IRepetitionPattern;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7FB9ACF1-26BE-400e-85B5-294B9C75DFD6")
    IRepetitionPattern : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Interval( 
            /* [retval][out] */ BSTR *pInterval) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Interval( 
            /* [in] */ BSTR interval) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Duration( 
            /* [retval][out] */ BSTR *pDuration) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Duration( 
            /* [in] */ BSTR duration) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StopAtDurationEnd( 
            /* [retval][out] */ VARIANT_BOOL *pStop) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StopAtDurationEnd( 
            /* [in] */ VARIANT_BOOL stop) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRepetitionPatternVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRepetitionPattern * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRepetitionPattern * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRepetitionPattern * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRepetitionPattern * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRepetitionPattern * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRepetitionPattern * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRepetitionPattern * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Interval )( 
            IRepetitionPattern * This,
            /* [retval][out] */ BSTR *pInterval);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Interval )( 
            IRepetitionPattern * This,
            /* [in] */ BSTR interval);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Duration )( 
            IRepetitionPattern * This,
            /* [retval][out] */ BSTR *pDuration);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Duration )( 
            IRepetitionPattern * This,
            /* [in] */ BSTR duration);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StopAtDurationEnd )( 
            IRepetitionPattern * This,
            /* [retval][out] */ VARIANT_BOOL *pStop);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StopAtDurationEnd )( 
            IRepetitionPattern * This,
            /* [in] */ VARIANT_BOOL stop);
        
        END_INTERFACE
    } IRepetitionPatternVtbl;

    interface IRepetitionPattern
    {
        CONST_VTBL struct IRepetitionPatternVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRepetitionPattern_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRepetitionPattern_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRepetitionPattern_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRepetitionPattern_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRepetitionPattern_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRepetitionPattern_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRepetitionPattern_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRepetitionPattern_get_Interval(This,pInterval)	\
    ( (This)->lpVtbl -> get_Interval(This,pInterval) ) 

#define IRepetitionPattern_put_Interval(This,interval)	\
    ( (This)->lpVtbl -> put_Interval(This,interval) ) 

#define IRepetitionPattern_get_Duration(This,pDuration)	\
    ( (This)->lpVtbl -> get_Duration(This,pDuration) ) 

#define IRepetitionPattern_put_Duration(This,duration)	\
    ( (This)->lpVtbl -> put_Duration(This,duration) ) 

#define IRepetitionPattern_get_StopAtDurationEnd(This,pStop)	\
    ( (This)->lpVtbl -> get_StopAtDurationEnd(This,pStop) ) 

#define IRepetitionPattern_put_StopAtDurationEnd(This,stop)	\
    ( (This)->lpVtbl -> put_StopAtDurationEnd(This,stop) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRepetitionPattern_INTERFACE_DEFINED__ */


#ifndef __ITrigger_INTERFACE_DEFINED__
#define __ITrigger_INTERFACE_DEFINED__

/* interface ITrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("09941815-ea89-4b5b-89e0-2a773801fac3")
    ITrigger : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ BSTR *pId) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Id( 
            /* [in] */ BSTR id) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Repetition( 
            /* [retval][out] */ IRepetitionPattern **ppRepeat) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Repetition( 
            /* [in] */ IRepetitionPattern *pRepeat) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ExecutionTimeLimit( 
            /* [retval][out] */ BSTR *pTimeLimit) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ExecutionTimeLimit( 
            /* [in] */ BSTR timelimit) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StartBoundary( 
            /* [retval][out] */ BSTR *pStart) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StartBoundary( 
            /* [in] */ BSTR start) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_EndBoundary( 
            /* [retval][out] */ BSTR *pEnd) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_EndBoundary( 
            /* [in] */ BSTR end) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL enabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ITrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            ITrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            ITrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            ITrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            ITrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            ITrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            ITrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            ITrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            ITrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            ITrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            ITrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ITrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ITrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        END_INTERFACE
    } ITriggerVtbl;

    interface ITrigger
    {
        CONST_VTBL struct ITriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define ITrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define ITrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define ITrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define ITrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define ITrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define ITrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define ITrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define ITrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define ITrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define ITrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define ITrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define ITrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITrigger_INTERFACE_DEFINED__ */


#ifndef __IEventTrigger_INTERFACE_DEFINED__
#define __IEventTrigger_INTERFACE_DEFINED__

/* interface IEventTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IEventTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d45b0167-9653-4eef-b94f-0732ca7af251")
    IEventTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Subscription( 
            /* [retval][out] */ BSTR *pQuery) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Subscription( 
            /* [in] */ BSTR query) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Delay( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Delay( 
            /* [in] */ BSTR delay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEventTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEventTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEventTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEventTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEventTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEventTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEventTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEventTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IEventTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IEventTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IEventTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IEventTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IEventTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IEventTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IEventTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IEventTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IEventTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Subscription )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pQuery);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Subscription )( 
            IEventTrigger * This,
            /* [in] */ BSTR query);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Delay )( 
            IEventTrigger * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Delay )( 
            IEventTrigger * This,
            /* [in] */ BSTR delay);
        
        END_INTERFACE
    } IEventTriggerVtbl;

    interface IEventTrigger
    {
        CONST_VTBL struct IEventTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEventTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEventTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEventTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEventTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEventTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEventTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEventTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEventTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IEventTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IEventTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IEventTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IEventTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IEventTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IEventTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IEventTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IEventTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IEventTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IEventTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IEventTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IEventTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IEventTrigger_get_Subscription(This,pQuery)	\
    ( (This)->lpVtbl -> get_Subscription(This,pQuery) ) 

#define IEventTrigger_put_Subscription(This,query)	\
    ( (This)->lpVtbl -> put_Subscription(This,query) ) 

#define IEventTrigger_get_Delay(This,pDelay)	\
    ( (This)->lpVtbl -> get_Delay(This,pDelay) ) 

#define IEventTrigger_put_Delay(This,delay)	\
    ( (This)->lpVtbl -> put_Delay(This,delay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEventTrigger_INTERFACE_DEFINED__ */


#ifndef __ITimeTrigger_INTERFACE_DEFINED__
#define __ITimeTrigger_INTERFACE_DEFINED__

/* interface ITimeTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITimeTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b45747e0-eba7-4276-9f29-85c5bb300006")
    ITimeTrigger : public ITrigger
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITimeTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITimeTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITimeTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITimeTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITimeTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITimeTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITimeTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITimeTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ITimeTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            ITimeTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            ITimeTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            ITimeTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            ITimeTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            ITimeTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            ITimeTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            ITimeTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            ITimeTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            ITimeTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            ITimeTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ITimeTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ITimeTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        END_INTERFACE
    } ITimeTriggerVtbl;

    interface ITimeTrigger
    {
        CONST_VTBL struct ITimeTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITimeTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITimeTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITimeTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITimeTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITimeTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITimeTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITimeTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITimeTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define ITimeTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define ITimeTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define ITimeTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define ITimeTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define ITimeTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define ITimeTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define ITimeTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define ITimeTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define ITimeTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define ITimeTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define ITimeTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define ITimeTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITimeTrigger_INTERFACE_DEFINED__ */


#ifndef __IDailyTrigger_INTERFACE_DEFINED__
#define __IDailyTrigger_INTERFACE_DEFINED__

/* interface IDailyTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IDailyTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("126c5cd8-b288-41d5-8dbf-e491446adc5c")
    IDailyTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DaysInterval( 
            /* [retval][out] */ short *pDays) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DaysInterval( 
            /* [in] */ short days) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDailyTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDailyTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDailyTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDailyTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDailyTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDailyTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDailyTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDailyTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IDailyTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IDailyTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IDailyTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IDailyTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IDailyTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IDailyTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IDailyTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IDailyTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IDailyTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IDailyTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IDailyTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDailyTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDailyTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DaysInterval )( 
            IDailyTrigger * This,
            /* [retval][out] */ short *pDays);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DaysInterval )( 
            IDailyTrigger * This,
            /* [in] */ short days);
        
        END_INTERFACE
    } IDailyTriggerVtbl;

    interface IDailyTrigger
    {
        CONST_VTBL struct IDailyTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDailyTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDailyTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDailyTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDailyTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDailyTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDailyTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDailyTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDailyTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IDailyTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IDailyTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IDailyTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IDailyTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IDailyTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IDailyTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IDailyTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IDailyTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IDailyTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IDailyTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IDailyTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IDailyTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IDailyTrigger_get_DaysInterval(This,pDays)	\
    ( (This)->lpVtbl -> get_DaysInterval(This,pDays) ) 

#define IDailyTrigger_put_DaysInterval(This,days)	\
    ( (This)->lpVtbl -> put_DaysInterval(This,days) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDailyTrigger_INTERFACE_DEFINED__ */


#ifndef __IWeeklyTrigger_INTERFACE_DEFINED__
#define __IWeeklyTrigger_INTERFACE_DEFINED__

/* interface IWeeklyTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IWeeklyTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5038fc98-82ff-436d-8728-a512a57c9dc1")
    IWeeklyTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DaysOfWeek( 
            /* [retval][out] */ short *pDays) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DaysOfWeek( 
            /* [in] */ short days) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WeeksInterval( 
            /* [retval][out] */ short *pWeeks) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WeeksInterval( 
            /* [in] */ short weeks) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWeeklyTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWeeklyTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWeeklyTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWeeklyTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWeeklyTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWeeklyTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWeeklyTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWeeklyTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IWeeklyTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IWeeklyTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IWeeklyTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IWeeklyTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IWeeklyTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IWeeklyTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DaysOfWeek )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ short *pDays);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DaysOfWeek )( 
            IWeeklyTrigger * This,
            /* [in] */ short days);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_WeeksInterval )( 
            IWeeklyTrigger * This,
            /* [retval][out] */ short *pWeeks);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_WeeksInterval )( 
            IWeeklyTrigger * This,
            /* [in] */ short weeks);
        
        END_INTERFACE
    } IWeeklyTriggerVtbl;

    interface IWeeklyTrigger
    {
        CONST_VTBL struct IWeeklyTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWeeklyTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWeeklyTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWeeklyTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWeeklyTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWeeklyTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWeeklyTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWeeklyTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWeeklyTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IWeeklyTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IWeeklyTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IWeeklyTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IWeeklyTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IWeeklyTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IWeeklyTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IWeeklyTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IWeeklyTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IWeeklyTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IWeeklyTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IWeeklyTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IWeeklyTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IWeeklyTrigger_get_DaysOfWeek(This,pDays)	\
    ( (This)->lpVtbl -> get_DaysOfWeek(This,pDays) ) 

#define IWeeklyTrigger_put_DaysOfWeek(This,days)	\
    ( (This)->lpVtbl -> put_DaysOfWeek(This,days) ) 

#define IWeeklyTrigger_get_WeeksInterval(This,pWeeks)	\
    ( (This)->lpVtbl -> get_WeeksInterval(This,pWeeks) ) 

#define IWeeklyTrigger_put_WeeksInterval(This,weeks)	\
    ( (This)->lpVtbl -> put_WeeksInterval(This,weeks) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWeeklyTrigger_INTERFACE_DEFINED__ */


#ifndef __IMonthlyTrigger_INTERFACE_DEFINED__
#define __IMonthlyTrigger_INTERFACE_DEFINED__

/* interface IMonthlyTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IMonthlyTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("97c45ef1-6b02-4a1a-9c0e-1ebfba1500ac")
    IMonthlyTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DaysOfMonth( 
            /* [retval][out] */ long *pDays) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DaysOfMonth( 
            /* [in] */ long days) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MonthsOfYear( 
            /* [retval][out] */ short *pMonths) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_MonthsOfYear( 
            /* [in] */ short months) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RunOnLastDayOfMonth( 
            /* [retval][out] */ VARIANT_BOOL *pLastDay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RunOnLastDayOfMonth( 
            /* [in] */ VARIANT_BOOL lastDay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMonthlyTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMonthlyTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMonthlyTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMonthlyTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMonthlyTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMonthlyTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMonthlyTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMonthlyTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IMonthlyTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IMonthlyTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IMonthlyTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IMonthlyTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IMonthlyTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IMonthlyTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DaysOfMonth )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ long *pDays);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DaysOfMonth )( 
            IMonthlyTrigger * This,
            /* [in] */ long days);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MonthsOfYear )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ short *pMonths);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MonthsOfYear )( 
            IMonthlyTrigger * This,
            /* [in] */ short months);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RunOnLastDayOfMonth )( 
            IMonthlyTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pLastDay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RunOnLastDayOfMonth )( 
            IMonthlyTrigger * This,
            /* [in] */ VARIANT_BOOL lastDay);
        
        END_INTERFACE
    } IMonthlyTriggerVtbl;

    interface IMonthlyTrigger
    {
        CONST_VTBL struct IMonthlyTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMonthlyTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMonthlyTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMonthlyTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMonthlyTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMonthlyTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMonthlyTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMonthlyTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMonthlyTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IMonthlyTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IMonthlyTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IMonthlyTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IMonthlyTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IMonthlyTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IMonthlyTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IMonthlyTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IMonthlyTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IMonthlyTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IMonthlyTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IMonthlyTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IMonthlyTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IMonthlyTrigger_get_DaysOfMonth(This,pDays)	\
    ( (This)->lpVtbl -> get_DaysOfMonth(This,pDays) ) 

#define IMonthlyTrigger_put_DaysOfMonth(This,days)	\
    ( (This)->lpVtbl -> put_DaysOfMonth(This,days) ) 

#define IMonthlyTrigger_get_MonthsOfYear(This,pMonths)	\
    ( (This)->lpVtbl -> get_MonthsOfYear(This,pMonths) ) 

#define IMonthlyTrigger_put_MonthsOfYear(This,months)	\
    ( (This)->lpVtbl -> put_MonthsOfYear(This,months) ) 

#define IMonthlyTrigger_get_RunOnLastDayOfMonth(This,pLastDay)	\
    ( (This)->lpVtbl -> get_RunOnLastDayOfMonth(This,pLastDay) ) 

#define IMonthlyTrigger_put_RunOnLastDayOfMonth(This,lastDay)	\
    ( (This)->lpVtbl -> put_RunOnLastDayOfMonth(This,lastDay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMonthlyTrigger_INTERFACE_DEFINED__ */


#ifndef __IMonthlyDOWTrigger_INTERFACE_DEFINED__
#define __IMonthlyDOWTrigger_INTERFACE_DEFINED__

/* interface IMonthlyDOWTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IMonthlyDOWTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("77d025a3-90fa-43aa-b52e-cda5499b946a")
    IMonthlyDOWTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DaysOfWeek( 
            /* [retval][out] */ short *pDays) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DaysOfWeek( 
            /* [in] */ short days) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WeeksOfMonth( 
            /* [retval][out] */ short *pWeeks) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WeeksOfMonth( 
            /* [in] */ short weeks) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MonthsOfYear( 
            /* [retval][out] */ short *pMonths) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_MonthsOfYear( 
            /* [in] */ short months) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RunOnLastWeekOfMonth( 
            /* [retval][out] */ VARIANT_BOOL *pLastWeek) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RunOnLastWeekOfMonth( 
            /* [in] */ VARIANT_BOOL lastWeek) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMonthlyDOWTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMonthlyDOWTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMonthlyDOWTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMonthlyDOWTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DaysOfWeek )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ short *pDays);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DaysOfWeek )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ short days);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_WeeksOfMonth )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ short *pWeeks);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_WeeksOfMonth )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ short weeks);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MonthsOfYear )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ short *pMonths);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MonthsOfYear )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ short months);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RunOnLastWeekOfMonth )( 
            IMonthlyDOWTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pLastWeek);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RunOnLastWeekOfMonth )( 
            IMonthlyDOWTrigger * This,
            /* [in] */ VARIANT_BOOL lastWeek);
        
        END_INTERFACE
    } IMonthlyDOWTriggerVtbl;

    interface IMonthlyDOWTrigger
    {
        CONST_VTBL struct IMonthlyDOWTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMonthlyDOWTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMonthlyDOWTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMonthlyDOWTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMonthlyDOWTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMonthlyDOWTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMonthlyDOWTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMonthlyDOWTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMonthlyDOWTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IMonthlyDOWTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IMonthlyDOWTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IMonthlyDOWTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IMonthlyDOWTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IMonthlyDOWTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IMonthlyDOWTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IMonthlyDOWTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IMonthlyDOWTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IMonthlyDOWTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IMonthlyDOWTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IMonthlyDOWTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IMonthlyDOWTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IMonthlyDOWTrigger_get_DaysOfWeek(This,pDays)	\
    ( (This)->lpVtbl -> get_DaysOfWeek(This,pDays) ) 

#define IMonthlyDOWTrigger_put_DaysOfWeek(This,days)	\
    ( (This)->lpVtbl -> put_DaysOfWeek(This,days) ) 

#define IMonthlyDOWTrigger_get_WeeksOfMonth(This,pWeeks)	\
    ( (This)->lpVtbl -> get_WeeksOfMonth(This,pWeeks) ) 

#define IMonthlyDOWTrigger_put_WeeksOfMonth(This,weeks)	\
    ( (This)->lpVtbl -> put_WeeksOfMonth(This,weeks) ) 

#define IMonthlyDOWTrigger_get_MonthsOfYear(This,pMonths)	\
    ( (This)->lpVtbl -> get_MonthsOfYear(This,pMonths) ) 

#define IMonthlyDOWTrigger_put_MonthsOfYear(This,months)	\
    ( (This)->lpVtbl -> put_MonthsOfYear(This,months) ) 

#define IMonthlyDOWTrigger_get_RunOnLastWeekOfMonth(This,pLastWeek)	\
    ( (This)->lpVtbl -> get_RunOnLastWeekOfMonth(This,pLastWeek) ) 

#define IMonthlyDOWTrigger_put_RunOnLastWeekOfMonth(This,lastWeek)	\
    ( (This)->lpVtbl -> put_RunOnLastWeekOfMonth(This,lastWeek) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMonthlyDOWTrigger_INTERFACE_DEFINED__ */


#ifndef __IIdleTrigger_INTERFACE_DEFINED__
#define __IIdleTrigger_INTERFACE_DEFINED__

/* interface IIdleTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IIdleTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d537d2b0-9fb3-4d34-9739-1ff5ce7b1ef3")
    IIdleTrigger : public ITrigger
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IIdleTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIdleTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIdleTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIdleTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIdleTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIdleTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIdleTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIdleTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IIdleTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IIdleTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IIdleTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IIdleTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IIdleTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IIdleTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IIdleTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IIdleTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IIdleTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IIdleTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IIdleTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IIdleTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IIdleTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        END_INTERFACE
    } IIdleTriggerVtbl;

    interface IIdleTrigger
    {
        CONST_VTBL struct IIdleTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIdleTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIdleTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIdleTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIdleTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIdleTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIdleTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIdleTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IIdleTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IIdleTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IIdleTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IIdleTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IIdleTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IIdleTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IIdleTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IIdleTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IIdleTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IIdleTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IIdleTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IIdleTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IIdleTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIdleTrigger_INTERFACE_DEFINED__ */


#ifndef __ILogonTrigger_INTERFACE_DEFINED__
#define __ILogonTrigger_INTERFACE_DEFINED__

/* interface ILogonTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ILogonTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("72DADE38-FAE4-4b3e-BAF4-5D009AF02B1C")
    ILogonTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Delay( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Delay( 
            /* [in] */ BSTR delay) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_UserId( 
            /* [retval][out] */ BSTR *pUser) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_UserId( 
            /* [in] */ BSTR user) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILogonTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogonTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogonTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogonTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogonTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogonTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogonTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogonTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ILogonTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            ILogonTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            ILogonTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            ILogonTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            ILogonTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            ILogonTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            ILogonTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ILogonTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ILogonTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Delay )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Delay )( 
            ILogonTrigger * This,
            /* [in] */ BSTR delay);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_UserId )( 
            ILogonTrigger * This,
            /* [retval][out] */ BSTR *pUser);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_UserId )( 
            ILogonTrigger * This,
            /* [in] */ BSTR user);
        
        END_INTERFACE
    } ILogonTriggerVtbl;

    interface ILogonTrigger
    {
        CONST_VTBL struct ILogonTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogonTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogonTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogonTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogonTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogonTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogonTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogonTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogonTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define ILogonTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define ILogonTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define ILogonTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define ILogonTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define ILogonTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define ILogonTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define ILogonTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define ILogonTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define ILogonTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define ILogonTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define ILogonTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define ILogonTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define ILogonTrigger_get_Delay(This,pDelay)	\
    ( (This)->lpVtbl -> get_Delay(This,pDelay) ) 

#define ILogonTrigger_put_Delay(This,delay)	\
    ( (This)->lpVtbl -> put_Delay(This,delay) ) 

#define ILogonTrigger_get_UserId(This,pUser)	\
    ( (This)->lpVtbl -> get_UserId(This,pUser) ) 

#define ILogonTrigger_put_UserId(This,user)	\
    ( (This)->lpVtbl -> put_UserId(This,user) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogonTrigger_INTERFACE_DEFINED__ */


#ifndef __ISessionStateChangeTrigger_INTERFACE_DEFINED__
#define __ISessionStateChangeTrigger_INTERFACE_DEFINED__

/* interface ISessionStateChangeTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ISessionStateChangeTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("754DA71B-4385-4475-9DD9-598294FA3641")
    ISessionStateChangeTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Delay( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Delay( 
            /* [in] */ BSTR delay) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_UserId( 
            /* [retval][out] */ BSTR *pUser) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_UserId( 
            /* [in] */ BSTR user) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StateChange( 
            /* [retval][out] */ TASK_SESSION_STATE_CHANGE_TYPE *pType) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StateChange( 
            /* [in] */ TASK_SESSION_STATE_CHANGE_TYPE type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISessionStateChangeTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISessionStateChangeTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISessionStateChangeTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISessionStateChangeTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Delay )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Delay )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR delay);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_UserId )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ BSTR *pUser);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_UserId )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ BSTR user);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StateChange )( 
            ISessionStateChangeTrigger * This,
            /* [retval][out] */ TASK_SESSION_STATE_CHANGE_TYPE *pType);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StateChange )( 
            ISessionStateChangeTrigger * This,
            /* [in] */ TASK_SESSION_STATE_CHANGE_TYPE type);
        
        END_INTERFACE
    } ISessionStateChangeTriggerVtbl;

    interface ISessionStateChangeTrigger
    {
        CONST_VTBL struct ISessionStateChangeTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISessionStateChangeTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISessionStateChangeTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISessionStateChangeTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISessionStateChangeTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISessionStateChangeTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISessionStateChangeTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISessionStateChangeTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISessionStateChangeTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define ISessionStateChangeTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define ISessionStateChangeTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define ISessionStateChangeTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define ISessionStateChangeTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define ISessionStateChangeTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define ISessionStateChangeTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define ISessionStateChangeTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define ISessionStateChangeTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define ISessionStateChangeTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define ISessionStateChangeTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define ISessionStateChangeTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define ISessionStateChangeTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define ISessionStateChangeTrigger_get_Delay(This,pDelay)	\
    ( (This)->lpVtbl -> get_Delay(This,pDelay) ) 

#define ISessionStateChangeTrigger_put_Delay(This,delay)	\
    ( (This)->lpVtbl -> put_Delay(This,delay) ) 

#define ISessionStateChangeTrigger_get_UserId(This,pUser)	\
    ( (This)->lpVtbl -> get_UserId(This,pUser) ) 

#define ISessionStateChangeTrigger_put_UserId(This,user)	\
    ( (This)->lpVtbl -> put_UserId(This,user) ) 

#define ISessionStateChangeTrigger_get_StateChange(This,pType)	\
    ( (This)->lpVtbl -> get_StateChange(This,pType) ) 

#define ISessionStateChangeTrigger_put_StateChange(This,type)	\
    ( (This)->lpVtbl -> put_StateChange(This,type) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISessionStateChangeTrigger_INTERFACE_DEFINED__ */


#ifndef __IBootTrigger_INTERFACE_DEFINED__
#define __IBootTrigger_INTERFACE_DEFINED__

/* interface IBootTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IBootTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2A9C35DA-D357-41f4-BBC1-207AC1B1F3CB")
    IBootTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Delay( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Delay( 
            /* [in] */ BSTR delay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBootTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBootTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBootTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBootTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBootTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBootTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBootTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBootTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IBootTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IBootTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IBootTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IBootTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IBootTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IBootTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IBootTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IBootTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IBootTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IBootTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IBootTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IBootTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IBootTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Delay )( 
            IBootTrigger * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Delay )( 
            IBootTrigger * This,
            /* [in] */ BSTR delay);
        
        END_INTERFACE
    } IBootTriggerVtbl;

    interface IBootTrigger
    {
        CONST_VTBL struct IBootTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBootTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBootTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBootTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBootTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBootTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBootTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBootTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBootTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IBootTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IBootTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IBootTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IBootTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IBootTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IBootTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IBootTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IBootTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IBootTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IBootTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IBootTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IBootTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IBootTrigger_get_Delay(This,pDelay)	\
    ( (This)->lpVtbl -> get_Delay(This,pDelay) ) 

#define IBootTrigger_put_Delay(This,delay)	\
    ( (This)->lpVtbl -> put_Delay(This,delay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBootTrigger_INTERFACE_DEFINED__ */


#ifndef __IRegistrationTrigger_INTERFACE_DEFINED__
#define __IRegistrationTrigger_INTERFACE_DEFINED__

/* interface IRegistrationTrigger */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IRegistrationTrigger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4c8fec3a-c218-4e0c-b23d-629024db91a2")
    IRegistrationTrigger : public ITrigger
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Delay( 
            /* [retval][out] */ BSTR *pDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Delay( 
            /* [in] */ BSTR delay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRegistrationTriggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRegistrationTrigger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRegistrationTrigger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRegistrationTrigger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRegistrationTrigger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRegistrationTrigger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRegistrationTrigger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRegistrationTrigger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ TASK_TRIGGER_TYPE2 *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ BSTR *pId);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IRegistrationTrigger * This,
            /* [in] */ BSTR id);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Repetition )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ IRepetitionPattern **ppRepeat);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Repetition )( 
            IRegistrationTrigger * This,
            /* [in] */ IRepetitionPattern *pRepeat);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ BSTR *pTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            IRegistrationTrigger * This,
            /* [in] */ BSTR timelimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartBoundary )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ BSTR *pStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartBoundary )( 
            IRegistrationTrigger * This,
            /* [in] */ BSTR start);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_EndBoundary )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ BSTR *pEnd);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_EndBoundary )( 
            IRegistrationTrigger * This,
            /* [in] */ BSTR end);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IRegistrationTrigger * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Delay )( 
            IRegistrationTrigger * This,
            /* [retval][out] */ BSTR *pDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Delay )( 
            IRegistrationTrigger * This,
            /* [in] */ BSTR delay);
        
        END_INTERFACE
    } IRegistrationTriggerVtbl;

    interface IRegistrationTrigger
    {
        CONST_VTBL struct IRegistrationTriggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRegistrationTrigger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRegistrationTrigger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRegistrationTrigger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRegistrationTrigger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRegistrationTrigger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRegistrationTrigger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRegistrationTrigger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRegistrationTrigger_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#define IRegistrationTrigger_get_Id(This,pId)	\
    ( (This)->lpVtbl -> get_Id(This,pId) ) 

#define IRegistrationTrigger_put_Id(This,id)	\
    ( (This)->lpVtbl -> put_Id(This,id) ) 

#define IRegistrationTrigger_get_Repetition(This,ppRepeat)	\
    ( (This)->lpVtbl -> get_Repetition(This,ppRepeat) ) 

#define IRegistrationTrigger_put_Repetition(This,pRepeat)	\
    ( (This)->lpVtbl -> put_Repetition(This,pRepeat) ) 

#define IRegistrationTrigger_get_ExecutionTimeLimit(This,pTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pTimeLimit) ) 

#define IRegistrationTrigger_put_ExecutionTimeLimit(This,timelimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,timelimit) ) 

#define IRegistrationTrigger_get_StartBoundary(This,pStart)	\
    ( (This)->lpVtbl -> get_StartBoundary(This,pStart) ) 

#define IRegistrationTrigger_put_StartBoundary(This,start)	\
    ( (This)->lpVtbl -> put_StartBoundary(This,start) ) 

#define IRegistrationTrigger_get_EndBoundary(This,pEnd)	\
    ( (This)->lpVtbl -> get_EndBoundary(This,pEnd) ) 

#define IRegistrationTrigger_put_EndBoundary(This,end)	\
    ( (This)->lpVtbl -> put_EndBoundary(This,end) ) 

#define IRegistrationTrigger_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define IRegistrationTrigger_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 


#define IRegistrationTrigger_get_Delay(This,pDelay)	\
    ( (This)->lpVtbl -> get_Delay(This,pDelay) ) 

#define IRegistrationTrigger_put_Delay(This,delay)	\
    ( (This)->lpVtbl -> put_Delay(This,delay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRegistrationTrigger_INTERFACE_DEFINED__ */


#ifndef __IAction_INTERFACE_DEFINED__
#define __IAction_INTERFACE_DEFINED__

/* interface IAction */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IAction;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BAE54997-48B1-4cbe-9965-D6BE263EBEA4")
    IAction : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ BSTR *pName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Id( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ TASK_ACTION_TYPE *pType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAction * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAction * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAction * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAction * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAction * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAction * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAction * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IAction * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IAction * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IAction * This,
            /* [retval][out] */ TASK_ACTION_TYPE *pType);
        
        END_INTERFACE
    } IActionVtbl;

    interface IAction
    {
        CONST_VTBL struct IActionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAction_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAction_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAction_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAction_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAction_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAction_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAction_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAction_get_Id(This,pName)	\
    ( (This)->lpVtbl -> get_Id(This,pName) ) 

#define IAction_put_Id(This,name)	\
    ( (This)->lpVtbl -> put_Id(This,name) ) 

#define IAction_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAction_INTERFACE_DEFINED__ */


#ifndef __IExecAction_INTERFACE_DEFINED__
#define __IExecAction_INTERFACE_DEFINED__

/* interface IExecAction */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IExecAction;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4c3d624d-fd6b-49a3-b9b7-09cb3cd3f047")
    IExecAction : public IAction
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR *pPath) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Path( 
            /* [in] */ BSTR path) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Arguments( 
            /* [retval][out] */ BSTR *pArgument) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Arguments( 
            /* [in] */ BSTR argument) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WorkingDirectory( 
            /* [retval][out] */ BSTR *pWorkingDirectory) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WorkingDirectory( 
            /* [in] */ BSTR workingDirectory) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IExecActionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IExecAction * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IExecAction * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IExecAction * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IExecAction * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IExecAction * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IExecAction * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IExecAction * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IExecAction * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IExecAction * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IExecAction * This,
            /* [retval][out] */ TASK_ACTION_TYPE *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IExecAction * This,
            /* [retval][out] */ BSTR *pPath);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Path )( 
            IExecAction * This,
            /* [in] */ BSTR path);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Arguments )( 
            IExecAction * This,
            /* [retval][out] */ BSTR *pArgument);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Arguments )( 
            IExecAction * This,
            /* [in] */ BSTR argument);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_WorkingDirectory )( 
            IExecAction * This,
            /* [retval][out] */ BSTR *pWorkingDirectory);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_WorkingDirectory )( 
            IExecAction * This,
            /* [in] */ BSTR workingDirectory);
        
        END_INTERFACE
    } IExecActionVtbl;

    interface IExecAction
    {
        CONST_VTBL struct IExecActionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IExecAction_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IExecAction_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IExecAction_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IExecAction_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IExecAction_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IExecAction_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IExecAction_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IExecAction_get_Id(This,pName)	\
    ( (This)->lpVtbl -> get_Id(This,pName) ) 

#define IExecAction_put_Id(This,name)	\
    ( (This)->lpVtbl -> put_Id(This,name) ) 

#define IExecAction_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 


#define IExecAction_get_Path(This,pPath)	\
    ( (This)->lpVtbl -> get_Path(This,pPath) ) 

#define IExecAction_put_Path(This,path)	\
    ( (This)->lpVtbl -> put_Path(This,path) ) 

#define IExecAction_get_Arguments(This,pArgument)	\
    ( (This)->lpVtbl -> get_Arguments(This,pArgument) ) 

#define IExecAction_put_Arguments(This,argument)	\
    ( (This)->lpVtbl -> put_Arguments(This,argument) ) 

#define IExecAction_get_WorkingDirectory(This,pWorkingDirectory)	\
    ( (This)->lpVtbl -> get_WorkingDirectory(This,pWorkingDirectory) ) 

#define IExecAction_put_WorkingDirectory(This,workingDirectory)	\
    ( (This)->lpVtbl -> put_WorkingDirectory(This,workingDirectory) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IExecAction_INTERFACE_DEFINED__ */


#ifndef __IComHandlerAction_INTERFACE_DEFINED__
#define __IComHandlerAction_INTERFACE_DEFINED__

/* interface IComHandlerAction */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IComHandlerAction;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D2FD252-75C5-4f66-90BA-2A7D8CC3039F")
    IComHandlerAction : public IAction
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ClassId( 
            /* [retval][out] */ BSTR *pClsid) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ClassId( 
            /* [in] */ BSTR clsid) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Data( 
            /* [retval][out] */ BSTR *pData) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Data( 
            /* [in] */ BSTR data) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IComHandlerActionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IComHandlerAction * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IComHandlerAction * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IComHandlerAction * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IComHandlerAction * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IComHandlerAction * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IComHandlerAction * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IComHandlerAction * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IComHandlerAction * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IComHandlerAction * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IComHandlerAction * This,
            /* [retval][out] */ TASK_ACTION_TYPE *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ClassId )( 
            IComHandlerAction * This,
            /* [retval][out] */ BSTR *pClsid);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ClassId )( 
            IComHandlerAction * This,
            /* [in] */ BSTR clsid);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Data )( 
            IComHandlerAction * This,
            /* [retval][out] */ BSTR *pData);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Data )( 
            IComHandlerAction * This,
            /* [in] */ BSTR data);
        
        END_INTERFACE
    } IComHandlerActionVtbl;

    interface IComHandlerAction
    {
        CONST_VTBL struct IComHandlerActionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IComHandlerAction_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IComHandlerAction_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IComHandlerAction_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IComHandlerAction_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IComHandlerAction_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IComHandlerAction_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IComHandlerAction_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IComHandlerAction_get_Id(This,pName)	\
    ( (This)->lpVtbl -> get_Id(This,pName) ) 

#define IComHandlerAction_put_Id(This,name)	\
    ( (This)->lpVtbl -> put_Id(This,name) ) 

#define IComHandlerAction_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 


#define IComHandlerAction_get_ClassId(This,pClsid)	\
    ( (This)->lpVtbl -> get_ClassId(This,pClsid) ) 

#define IComHandlerAction_put_ClassId(This,clsid)	\
    ( (This)->lpVtbl -> put_ClassId(This,clsid) ) 

#define IComHandlerAction_get_Data(This,pData)	\
    ( (This)->lpVtbl -> get_Data(This,pData) ) 

#define IComHandlerAction_put_Data(This,data)	\
    ( (This)->lpVtbl -> put_Data(This,data) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IComHandlerAction_INTERFACE_DEFINED__ */


#ifndef __IEmailAction_INTERFACE_DEFINED__
#define __IEmailAction_INTERFACE_DEFINED__

/* interface IEmailAction */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IEmailAction;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("10F62C64-7E16-4314-A0C2-0C3683F99D40")
    IEmailAction : public IAction
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Server( 
            /* [retval][out] */ BSTR *pServer) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Server( 
            /* [in] */ BSTR server) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Subject( 
            /* [retval][out] */ BSTR *pSubject) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Subject( 
            /* [in] */ BSTR subject) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_To( 
            /* [retval][out] */ BSTR *pTo) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_To( 
            /* [in] */ BSTR to) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Cc( 
            /* [retval][out] */ BSTR *pCc) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Cc( 
            /* [in] */ BSTR cc) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Bcc( 
            /* [retval][out] */ BSTR *pBcc) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Bcc( 
            /* [in] */ BSTR bcc) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ReplyTo( 
            /* [retval][out] */ BSTR *pReplyTo) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ReplyTo( 
            /* [in] */ BSTR replyTo) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_From( 
            /* [retval][out] */ BSTR *pFrom) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_From( 
            /* [in] */ BSTR from) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_HeaderFields( 
            /* [retval][out] */ ITaskNamedValueCollection **ppHeaderFields) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_HeaderFields( 
            /* [in] */ ITaskNamedValueCollection *pHeaderFields) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Body( 
            /* [retval][out] */ BSTR *pBody) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Body( 
            /* [in] */ BSTR body) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Attachments( 
            /* [retval][out] */ SAFEARRAY * *pAttachements) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Attachments( 
            /* [max_is][in] */ SAFEARRAY * pAttachements) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEmailActionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEmailAction * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEmailAction * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEmailAction * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEmailAction * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEmailAction * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEmailAction * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEmailAction * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IEmailAction * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IEmailAction * This,
            /* [retval][out] */ TASK_ACTION_TYPE *pType);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Server )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pServer);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Server )( 
            IEmailAction * This,
            /* [in] */ BSTR server);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Subject )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pSubject);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Subject )( 
            IEmailAction * This,
            /* [in] */ BSTR subject);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_To )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pTo);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_To )( 
            IEmailAction * This,
            /* [in] */ BSTR to);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Cc )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pCc);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Cc )( 
            IEmailAction * This,
            /* [in] */ BSTR cc);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Bcc )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pBcc);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Bcc )( 
            IEmailAction * This,
            /* [in] */ BSTR bcc);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ReplyTo )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pReplyTo);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ReplyTo )( 
            IEmailAction * This,
            /* [in] */ BSTR replyTo);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_From )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pFrom);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_From )( 
            IEmailAction * This,
            /* [in] */ BSTR from);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HeaderFields )( 
            IEmailAction * This,
            /* [retval][out] */ ITaskNamedValueCollection **ppHeaderFields);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HeaderFields )( 
            IEmailAction * This,
            /* [in] */ ITaskNamedValueCollection *pHeaderFields);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Body )( 
            IEmailAction * This,
            /* [retval][out] */ BSTR *pBody);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Body )( 
            IEmailAction * This,
            /* [in] */ BSTR body);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Attachments )( 
            IEmailAction * This,
            /* [retval][out] */ SAFEARRAY * *pAttachements);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Attachments )( 
            IEmailAction * This,
            /* [max_is][in] */ SAFEARRAY * pAttachements);
        
        END_INTERFACE
    } IEmailActionVtbl;

    interface IEmailAction
    {
        CONST_VTBL struct IEmailActionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEmailAction_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEmailAction_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEmailAction_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEmailAction_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEmailAction_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEmailAction_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEmailAction_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEmailAction_get_Id(This,pName)	\
    ( (This)->lpVtbl -> get_Id(This,pName) ) 

#define IEmailAction_put_Id(This,name)	\
    ( (This)->lpVtbl -> put_Id(This,name) ) 

#define IEmailAction_get_Type(This,pType)	\
    ( (This)->lpVtbl -> get_Type(This,pType) ) 


#define IEmailAction_get_Server(This,pServer)	\
    ( (This)->lpVtbl -> get_Server(This,pServer) ) 

#define IEmailAction_put_Server(This,server)	\
    ( (This)->lpVtbl -> put_Server(This,server) ) 

#define IEmailAction_get_Subject(This,pSubject)	\
    ( (This)->lpVtbl -> get_Subject(This,pSubject) ) 

#define IEmailAction_put_Subject(This,subject)	\
    ( (This)->lpVtbl -> put_Subject(This,subject) ) 

#define IEmailAction_get_To(This,pTo)	\
    ( (This)->lpVtbl -> get_To(This,pTo) ) 

#define IEmailAction_put_To(This,to)	\
    ( (This)->lpVtbl -> put_To(This,to) ) 

#define IEmailAction_get_Cc(This,pCc)	\
    ( (This)->lpVtbl -> get_Cc(This,pCc) ) 

#define IEmailAction_put_Cc(This,cc)	\
    ( (This)->lpVtbl -> put_Cc(This,cc) ) 

#define IEmailAction_get_Bcc(This,pBcc)	\
    ( (This)->lpVtbl -> get_Bcc(This,pBcc) ) 

#define IEmailAction_put_Bcc(This,bcc)	\
    ( (This)->lpVtbl -> put_Bcc(This,bcc) ) 

#define IEmailAction_get_ReplyTo(This,pReplyTo)	\
    ( (This)->lpVtbl -> get_ReplyTo(This,pReplyTo) ) 

#define IEmailAction_put_ReplyTo(This,replyTo)	\
    ( (This)->lpVtbl -> put_ReplyTo(This,replyTo) ) 

#define IEmailAction_get_From(This,pFrom)	\
    ( (This)->lpVtbl -> get_From(This,pFrom) ) 

#define IEmailAction_put_From(This,from)	\
    ( (This)->lpVtbl -> put_From(This,from) ) 

#define IEmailAction_get_HeaderFields(This,ppHeaderFields)	\
    ( (This)->lpVtbl -> get_HeaderFields(This,ppHeaderFields) ) 

#define IEmailAction_put_HeaderFields(This,pHeaderFields)	\
    ( (This)->lpVtbl -> put_HeaderFields(This,pHeaderFields) ) 

#define IEmailAction_get_Body(This,pBody)	\
    ( (This)->lpVtbl -> get_Body(This,pBody) ) 

#define IEmailAction_put_Body(This,body)	\
    ( (This)->lpVtbl -> put_Body(This,body) ) 

#define IEmailAction_get_Attachments(This,pAttachements)	\
    ( (This)->lpVtbl -> get_Attachments(This,pAttachements) ) 

#define IEmailAction_put_Attachments(This,pAttachements)	\
    ( (This)->lpVtbl -> put_Attachments(This,pAttachements) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEmailAction_INTERFACE_DEFINED__ */


#ifndef __ITriggerCollection_INTERFACE_DEFINED__
#define __ITriggerCollection_INTERFACE_DEFINED__

/* interface ITriggerCollection */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITriggerCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85df5081-1b24-4f32-878a-d9d14df4cb77")
    ITriggerCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long index,
            /* [retval][out] */ ITrigger **ppTrigger) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ TASK_TRIGGER_TYPE2 type,
            /* [retval][out] */ ITrigger **ppTrigger) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITriggerCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITriggerCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITriggerCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITriggerCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITriggerCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITriggerCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITriggerCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITriggerCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ITriggerCollection * This,
            /* [retval][out] */ long *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ITriggerCollection * This,
            /* [in] */ long index,
            /* [retval][out] */ ITrigger **ppTrigger);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ITriggerCollection * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            ITriggerCollection * This,
            /* [in] */ TASK_TRIGGER_TYPE2 type,
            /* [retval][out] */ ITrigger **ppTrigger);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            ITriggerCollection * This,
            /* [in] */ VARIANT index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            ITriggerCollection * This);
        
        END_INTERFACE
    } ITriggerCollectionVtbl;

    interface ITriggerCollection
    {
        CONST_VTBL struct ITriggerCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITriggerCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITriggerCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITriggerCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITriggerCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITriggerCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITriggerCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITriggerCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITriggerCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define ITriggerCollection_get_Item(This,index,ppTrigger)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppTrigger) ) 

#define ITriggerCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#define ITriggerCollection_Create(This,type,ppTrigger)	\
    ( (This)->lpVtbl -> Create(This,type,ppTrigger) ) 

#define ITriggerCollection_Remove(This,index)	\
    ( (This)->lpVtbl -> Remove(This,index) ) 

#define ITriggerCollection_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITriggerCollection_INTERFACE_DEFINED__ */


#ifndef __IActionCollection_INTERFACE_DEFINED__
#define __IActionCollection_INTERFACE_DEFINED__

/* interface IActionCollection */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IActionCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("02820E19-7B98-4ed2-B2E8-FDCCCEFF619B")
    IActionCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long index,
            /* [retval][out] */ IAction **ppAction) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppEnum) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_XmlText( 
            /* [retval][out] */ BSTR *pText) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_XmlText( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ TASK_ACTION_TYPE type,
            /* [retval][out] */ IAction **ppAction) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Context( 
            /* [retval][out] */ BSTR *pContext) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Context( 
            /* [in] */ BSTR context) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActionCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActionCollection * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActionCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActionCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IActionCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IActionCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IActionCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IActionCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IActionCollection * This,
            /* [retval][out] */ long *pCount);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IActionCollection * This,
            /* [in] */ long index,
            /* [retval][out] */ IAction **ppAction);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IActionCollection * This,
            /* [retval][out] */ IUnknown **ppEnum);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_XmlText )( 
            IActionCollection * This,
            /* [retval][out] */ BSTR *pText);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_XmlText )( 
            IActionCollection * This,
            /* [in] */ BSTR text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Create )( 
            IActionCollection * This,
            /* [in] */ TASK_ACTION_TYPE type,
            /* [retval][out] */ IAction **ppAction);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IActionCollection * This,
            /* [in] */ VARIANT index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IActionCollection * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Context )( 
            IActionCollection * This,
            /* [retval][out] */ BSTR *pContext);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Context )( 
            IActionCollection * This,
            /* [in] */ BSTR context);
        
        END_INTERFACE
    } IActionCollectionVtbl;

    interface IActionCollection
    {
        CONST_VTBL struct IActionCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActionCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IActionCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IActionCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IActionCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IActionCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IActionCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IActionCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IActionCollection_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IActionCollection_get_Item(This,index,ppAction)	\
    ( (This)->lpVtbl -> get_Item(This,index,ppAction) ) 

#define IActionCollection_get__NewEnum(This,ppEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppEnum) ) 

#define IActionCollection_get_XmlText(This,pText)	\
    ( (This)->lpVtbl -> get_XmlText(This,pText) ) 

#define IActionCollection_put_XmlText(This,text)	\
    ( (This)->lpVtbl -> put_XmlText(This,text) ) 

#define IActionCollection_Create(This,type,ppAction)	\
    ( (This)->lpVtbl -> Create(This,type,ppAction) ) 

#define IActionCollection_Remove(This,index)	\
    ( (This)->lpVtbl -> Remove(This,index) ) 

#define IActionCollection_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IActionCollection_get_Context(This,pContext)	\
    ( (This)->lpVtbl -> get_Context(This,pContext) ) 

#define IActionCollection_put_Context(This,context)	\
    ( (This)->lpVtbl -> put_Context(This,context) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IActionCollection_INTERFACE_DEFINED__ */


#ifndef __IPrincipal_INTERFACE_DEFINED__
#define __IPrincipal_INTERFACE_DEFINED__

/* interface IPrincipal */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IPrincipal;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D98D51E5-C9B4-496a-A9C1-18980261CF0F")
    IPrincipal : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Id( 
            /* [retval][out] */ BSTR *pName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Id( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *pName) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DisplayName( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_UserId( 
            /* [retval][out] */ BSTR *pUser) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_UserId( 
            /* [in] */ BSTR user) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_LogonType( 
            /* [retval][out] */ TASK_LOGON_TYPE *pLogon) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_LogonType( 
            /* [in] */ TASK_LOGON_TYPE logon) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_GroupId( 
            /* [retval][out] */ BSTR *pGroup) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_GroupId( 
            /* [in] */ BSTR group) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RunLevel( 
            /* [retval][out] */ TASK_RUNLEVEL_TYPE *pRunLevel) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RunLevel( 
            /* [in] */ TASK_RUNLEVEL_TYPE runLevel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPrincipalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPrincipal * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPrincipal * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPrincipal * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPrincipal * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPrincipal * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPrincipal * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPrincipal * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Id )( 
            IPrincipal * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Id )( 
            IPrincipal * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IPrincipal * This,
            /* [retval][out] */ BSTR *pName);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DisplayName )( 
            IPrincipal * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_UserId )( 
            IPrincipal * This,
            /* [retval][out] */ BSTR *pUser);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_UserId )( 
            IPrincipal * This,
            /* [in] */ BSTR user);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LogonType )( 
            IPrincipal * This,
            /* [retval][out] */ TASK_LOGON_TYPE *pLogon);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_LogonType )( 
            IPrincipal * This,
            /* [in] */ TASK_LOGON_TYPE logon);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_GroupId )( 
            IPrincipal * This,
            /* [retval][out] */ BSTR *pGroup);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_GroupId )( 
            IPrincipal * This,
            /* [in] */ BSTR group);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RunLevel )( 
            IPrincipal * This,
            /* [retval][out] */ TASK_RUNLEVEL_TYPE *pRunLevel);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RunLevel )( 
            IPrincipal * This,
            /* [in] */ TASK_RUNLEVEL_TYPE runLevel);
        
        END_INTERFACE
    } IPrincipalVtbl;

    interface IPrincipal
    {
        CONST_VTBL struct IPrincipalVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPrincipal_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPrincipal_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPrincipal_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPrincipal_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPrincipal_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPrincipal_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPrincipal_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPrincipal_get_Id(This,pName)	\
    ( (This)->lpVtbl -> get_Id(This,pName) ) 

#define IPrincipal_put_Id(This,name)	\
    ( (This)->lpVtbl -> put_Id(This,name) ) 

#define IPrincipal_get_DisplayName(This,pName)	\
    ( (This)->lpVtbl -> get_DisplayName(This,pName) ) 

#define IPrincipal_put_DisplayName(This,name)	\
    ( (This)->lpVtbl -> put_DisplayName(This,name) ) 

#define IPrincipal_get_UserId(This,pUser)	\
    ( (This)->lpVtbl -> get_UserId(This,pUser) ) 

#define IPrincipal_put_UserId(This,user)	\
    ( (This)->lpVtbl -> put_UserId(This,user) ) 

#define IPrincipal_get_LogonType(This,pLogon)	\
    ( (This)->lpVtbl -> get_LogonType(This,pLogon) ) 

#define IPrincipal_put_LogonType(This,logon)	\
    ( (This)->lpVtbl -> put_LogonType(This,logon) ) 

#define IPrincipal_get_GroupId(This,pGroup)	\
    ( (This)->lpVtbl -> get_GroupId(This,pGroup) ) 

#define IPrincipal_put_GroupId(This,group)	\
    ( (This)->lpVtbl -> put_GroupId(This,group) ) 

#define IPrincipal_get_RunLevel(This,pRunLevel)	\
    ( (This)->lpVtbl -> get_RunLevel(This,pRunLevel) ) 

#define IPrincipal_put_RunLevel(This,runLevel)	\
    ( (This)->lpVtbl -> put_RunLevel(This,runLevel) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPrincipal_INTERFACE_DEFINED__ */


#ifndef __IRegistrationInfo_INTERFACE_DEFINED__
#define __IRegistrationInfo_INTERFACE_DEFINED__

/* interface IRegistrationInfo */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_IRegistrationInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("416D8B73-CB41-4ea1-805C-9BE9A5AC4A74")
    IRegistrationInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pDescription) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR description) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Author( 
            /* [retval][out] */ BSTR *pAuthor) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Author( 
            /* [in] */ BSTR author) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVersion) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Version( 
            /* [in] */ BSTR version) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Date( 
            /* [retval][out] */ BSTR *pDate) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Date( 
            /* [in] */ BSTR date) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Documentation( 
            /* [retval][out] */ BSTR *pDocumentation) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Documentation( 
            /* [in] */ BSTR documentation) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_XmlText( 
            /* [retval][out] */ BSTR *pText) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_XmlText( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_URI( 
            /* [retval][out] */ BSTR *pUri) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_URI( 
            /* [in] */ BSTR uri) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_SecurityDescriptor( 
            /* [retval][out] */ VARIANT *pSddl) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_SecurityDescriptor( 
            /* [in] */ VARIANT sddl) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Source( 
            /* [retval][out] */ BSTR *pSource) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Source( 
            /* [in] */ BSTR source) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRegistrationInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRegistrationInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRegistrationInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRegistrationInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRegistrationInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRegistrationInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRegistrationInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRegistrationInfo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pDescription);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR description);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Author )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pAuthor);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Author )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR author);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pVersion);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Version )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR version);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Date )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pDate);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Date )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR date);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Documentation )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pDocumentation);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Documentation )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR documentation);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_XmlText )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pText);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_XmlText )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR text);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_URI )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pUri);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_URI )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR uri);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SecurityDescriptor )( 
            IRegistrationInfo * This,
            /* [retval][out] */ VARIANT *pSddl);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SecurityDescriptor )( 
            IRegistrationInfo * This,
            /* [in] */ VARIANT sddl);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Source )( 
            IRegistrationInfo * This,
            /* [retval][out] */ BSTR *pSource);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Source )( 
            IRegistrationInfo * This,
            /* [in] */ BSTR source);
        
        END_INTERFACE
    } IRegistrationInfoVtbl;

    interface IRegistrationInfo
    {
        CONST_VTBL struct IRegistrationInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRegistrationInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRegistrationInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRegistrationInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRegistrationInfo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRegistrationInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRegistrationInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRegistrationInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRegistrationInfo_get_Description(This,pDescription)	\
    ( (This)->lpVtbl -> get_Description(This,pDescription) ) 

#define IRegistrationInfo_put_Description(This,description)	\
    ( (This)->lpVtbl -> put_Description(This,description) ) 

#define IRegistrationInfo_get_Author(This,pAuthor)	\
    ( (This)->lpVtbl -> get_Author(This,pAuthor) ) 

#define IRegistrationInfo_put_Author(This,author)	\
    ( (This)->lpVtbl -> put_Author(This,author) ) 

#define IRegistrationInfo_get_Version(This,pVersion)	\
    ( (This)->lpVtbl -> get_Version(This,pVersion) ) 

#define IRegistrationInfo_put_Version(This,version)	\
    ( (This)->lpVtbl -> put_Version(This,version) ) 

#define IRegistrationInfo_get_Date(This,pDate)	\
    ( (This)->lpVtbl -> get_Date(This,pDate) ) 

#define IRegistrationInfo_put_Date(This,date)	\
    ( (This)->lpVtbl -> put_Date(This,date) ) 

#define IRegistrationInfo_get_Documentation(This,pDocumentation)	\
    ( (This)->lpVtbl -> get_Documentation(This,pDocumentation) ) 

#define IRegistrationInfo_put_Documentation(This,documentation)	\
    ( (This)->lpVtbl -> put_Documentation(This,documentation) ) 

#define IRegistrationInfo_get_XmlText(This,pText)	\
    ( (This)->lpVtbl -> get_XmlText(This,pText) ) 

#define IRegistrationInfo_put_XmlText(This,text)	\
    ( (This)->lpVtbl -> put_XmlText(This,text) ) 

#define IRegistrationInfo_get_URI(This,pUri)	\
    ( (This)->lpVtbl -> get_URI(This,pUri) ) 

#define IRegistrationInfo_put_URI(This,uri)	\
    ( (This)->lpVtbl -> put_URI(This,uri) ) 

#define IRegistrationInfo_get_SecurityDescriptor(This,pSddl)	\
    ( (This)->lpVtbl -> get_SecurityDescriptor(This,pSddl) ) 

#define IRegistrationInfo_put_SecurityDescriptor(This,sddl)	\
    ( (This)->lpVtbl -> put_SecurityDescriptor(This,sddl) ) 

#define IRegistrationInfo_get_Source(This,pSource)	\
    ( (This)->lpVtbl -> get_Source(This,pSource) ) 

#define IRegistrationInfo_put_Source(This,source)	\
    ( (This)->lpVtbl -> put_Source(This,source) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRegistrationInfo_INTERFACE_DEFINED__ */


#ifndef __ITaskSettings_INTERFACE_DEFINED__
#define __ITaskSettings_INTERFACE_DEFINED__

/* interface ITaskSettings */
/* [helpstring][nonextensible][dual][uuid][object][local] */ 


EXTERN_C const IID IID_ITaskSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8FD4711D-2D02-4c8c-87E3-EFF699DE127E")
    ITaskSettings : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_AllowDemandStart( 
            /* [retval][out] */ VARIANT_BOOL *pAllowDemandStart) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AllowDemandStart( 
            /* [in] */ VARIANT_BOOL allowDemandStart) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RestartInterval( 
            /* [retval][out] */ BSTR *pRestartInterval) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RestartInterval( 
            /* [in] */ BSTR restartInterval) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RestartCount( 
            /* [retval][out] */ int *pRestartCount) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RestartCount( 
            /* [in] */ int restartCount) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_MultipleInstances( 
            /* [retval][out] */ TASK_INSTANCES_POLICY *pPolicy) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_MultipleInstances( 
            /* [in] */ TASK_INSTANCES_POLICY policy) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StopIfGoingOnBatteries( 
            /* [retval][out] */ VARIANT_BOOL *pStopIfOnBatteries) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StopIfGoingOnBatteries( 
            /* [in] */ VARIANT_BOOL stopIfOnBatteries) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DisallowStartIfOnBatteries( 
            /* [retval][out] */ VARIANT_BOOL *pDisallowStart) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DisallowStartIfOnBatteries( 
            /* [in] */ VARIANT_BOOL disallowStart) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_AllowHardTerminate( 
            /* [retval][out] */ VARIANT_BOOL *pAllowHardTerminate) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AllowHardTerminate( 
            /* [in] */ VARIANT_BOOL allowHardTerminate) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_StartWhenAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pStartWhenAvailable) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_StartWhenAvailable( 
            /* [in] */ VARIANT_BOOL startWhenAvailable) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_XmlText( 
            /* [retval][out] */ BSTR *pText) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_XmlText( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RunOnlyIfNetworkAvailable( 
            /* [retval][out] */ VARIANT_BOOL *pRunOnlyIfNetworkAvailable) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RunOnlyIfNetworkAvailable( 
            /* [in] */ VARIANT_BOOL runOnlyIfNetworkAvailable) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_ExecutionTimeLimit( 
            /* [retval][out] */ BSTR *pExecutionTimeLimit) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ExecutionTimeLimit( 
            /* [in] */ BSTR executionTimeLimit) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pEnabled) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL enabled) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_DeleteExpiredTaskAfter( 
            /* [retval][out] */ BSTR *pExpirationDelay) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_DeleteExpiredTaskAfter( 
            /* [in] */ BSTR expirationDelay) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Priority( 
            /* [retval][out] */ int *pPriority) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Priority( 
            /* [in] */ int priority) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Compatibility( 
            /* [retval][out] */ TASK_COMPATIBILITY *pCompatLevel) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Compatibility( 
            /* [in] */ TASK_COMPATIBILITY compatLevel) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Hidden( 
            /* [retval][out] */ VARIANT_BOOL *pHidden) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Hidden( 
            /* [in] */ VARIANT_BOOL hidden) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IdleSettings( 
            /* [retval][out] */ IIdleSettings **ppIdleSettings) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_IdleSettings( 
            /* [in] */ IIdleSettings *pIdleSettings) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_RunOnlyIfIdle( 
            /* [retval][out] */ VARIANT_BOOL *pRunOnlyIfIdle) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_RunOnlyIfIdle( 
            /* [in] */ VARIANT_BOOL runOnlyIfIdle) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_WakeToRun( 
            /* [retval][out] */ VARIANT_BOOL *pWake) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WakeToRun( 
            /* [in] */ VARIANT_BOOL wake) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITaskSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITaskSettings * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITaskSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITaskSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITaskSettings * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITaskSettings * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITaskSettings * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITaskSettings * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AllowDemandStart )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pAllowDemandStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AllowDemandStart )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL allowDemandStart);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RestartInterval )( 
            ITaskSettings * This,
            /* [retval][out] */ BSTR *pRestartInterval);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RestartInterval )( 
            ITaskSettings * This,
            /* [in] */ BSTR restartInterval);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RestartCount )( 
            ITaskSettings * This,
            /* [retval][out] */ int *pRestartCount);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RestartCount )( 
            ITaskSettings * This,
            /* [in] */ int restartCount);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MultipleInstances )( 
            ITaskSettings * This,
            /* [retval][out] */ TASK_INSTANCES_POLICY *pPolicy);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MultipleInstances )( 
            ITaskSettings * This,
            /* [in] */ TASK_INSTANCES_POLICY policy);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StopIfGoingOnBatteries )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pStopIfOnBatteries);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StopIfGoingOnBatteries )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL stopIfOnBatteries);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DisallowStartIfOnBatteries )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pDisallowStart);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DisallowStartIfOnBatteries )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL disallowStart);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AllowHardTerminate )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pAllowHardTerminate);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AllowHardTerminate )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL allowHardTerminate);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_StartWhenAvailable )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pStartWhenAvailable);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_StartWhenAvailable )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL startWhenAvailable);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_XmlText )( 
            ITaskSettings * This,
            /* [retval][out] */ BSTR *pText);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_XmlText )( 
            ITaskSettings * This,
            /* [in] */ BSTR text);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RunOnlyIfNetworkAvailable )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pRunOnlyIfNetworkAvailable);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RunOnlyIfNetworkAvailable )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL runOnlyIfNetworkAvailable);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ExecutionTimeLimit )( 
            ITaskSettings * This,
            /* [retval][out] */ BSTR *pExecutionTimeLimit);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ExecutionTimeLimit )( 
            ITaskSettings * This,
            /* [in] */ BSTR executionTimeLimit);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pEnabled);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL enabled);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DeleteExpiredTaskAfter )( 
            ITaskSettings * This,
            /* [retval][out] */ BSTR *pExpirationDelay);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DeleteExpiredTaskAfter )( 
            ITaskSettings * This,
            /* [in] */ BSTR expirationDelay);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Priority )( 
            ITaskSettings * This,
            /* [retval][out] */ int *pPriority);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Priority )( 
            ITaskSettings * This,
            /* [in] */ int priority);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Compatibility )( 
            ITaskSettings * This,
            /* [retval][out] */ TASK_COMPATIBILITY *pCompatLevel);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Compatibility )( 
            ITaskSettings * This,
            /* [in] */ TASK_COMPATIBILITY compatLevel);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Hidden )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pHidden);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Hidden )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL hidden);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleSettings )( 
            ITaskSettings * This,
            /* [retval][out] */ IIdleSettings **ppIdleSettings);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IdleSettings )( 
            ITaskSettings * This,
            /* [in] */ IIdleSettings *pIdleSettings);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_RunOnlyIfIdle )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pRunOnlyIfIdle);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RunOnlyIfIdle )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL runOnlyIfIdle);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_WakeToRun )( 
            ITaskSettings * This,
            /* [retval][out] */ VARIANT_BOOL *pWake);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_WakeToRun )( 
            ITaskSettings * This,
            /* [in] */ VARIANT_BOOL wake);
        
        END_INTERFACE
    } ITaskSettingsVtbl;

    interface ITaskSettings
    {
        CONST_VTBL struct ITaskSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITaskSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITaskSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITaskSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITaskSettings_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITaskSettings_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITaskSettings_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITaskSettings_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITaskSettings_get_AllowDemandStart(This,pAllowDemandStart)	\
    ( (This)->lpVtbl -> get_AllowDemandStart(This,pAllowDemandStart) ) 

#define ITaskSettings_put_AllowDemandStart(This,allowDemandStart)	\
    ( (This)->lpVtbl -> put_AllowDemandStart(This,allowDemandStart) ) 

#define ITaskSettings_get_RestartInterval(This,pRestartInterval)	\
    ( (This)->lpVtbl -> get_RestartInterval(This,pRestartInterval) ) 

#define ITaskSettings_put_RestartInterval(This,restartInterval)	\
    ( (This)->lpVtbl -> put_RestartInterval(This,restartInterval) ) 

#define ITaskSettings_get_RestartCount(This,pRestartCount)	\
    ( (This)->lpVtbl -> get_RestartCount(This,pRestartCount) ) 

#define ITaskSettings_put_RestartCount(This,restartCount)	\
    ( (This)->lpVtbl -> put_RestartCount(This,restartCount) ) 

#define ITaskSettings_get_MultipleInstances(This,pPolicy)	\
    ( (This)->lpVtbl -> get_MultipleInstances(This,pPolicy) ) 

#define ITaskSettings_put_MultipleInstances(This,policy)	\
    ( (This)->lpVtbl -> put_MultipleInstances(This,policy) ) 

#define ITaskSettings_get_StopIfGoingOnBatteries(This,pStopIfOnBatteries)	\
    ( (This)->lpVtbl -> get_StopIfGoingOnBatteries(This,pStopIfOnBatteries) ) 

#define ITaskSettings_put_StopIfGoingOnBatteries(This,stopIfOnBatteries)	\
    ( (This)->lpVtbl -> put_StopIfGoingOnBatteries(This,stopIfOnBatteries) ) 

#define ITaskSettings_get_DisallowStartIfOnBatteries(This,pDisallowStart)	\
    ( (This)->lpVtbl -> get_DisallowStartIfOnBatteries(This,pDisallowStart) ) 

#define ITaskSettings_put_DisallowStartIfOnBatteries(This,disallowStart)	\
    ( (This)->lpVtbl -> put_DisallowStartIfOnBatteries(This,disallowStart) ) 

#define ITaskSettings_get_AllowHardTerminate(This,pAllowHardTerminate)	\
    ( (This)->lpVtbl -> get_AllowHardTerminate(This,pAllowHardTerminate) ) 

#define ITaskSettings_put_AllowHardTerminate(This,allowHardTerminate)	\
    ( (This)->lpVtbl -> put_AllowHardTerminate(This,allowHardTerminate) ) 

#define ITaskSettings_get_StartWhenAvailable(This,pStartWhenAvailable)	\
    ( (This)->lpVtbl -> get_StartWhenAvailable(This,pStartWhenAvailable) ) 

#define ITaskSettings_put_StartWhenAvailable(This,startWhenAvailable)	\
    ( (This)->lpVtbl -> put_StartWhenAvailable(This,startWhenAvailable) ) 

#define ITaskSettings_get_XmlText(This,pText)	\
    ( (This)->lpVtbl -> get_XmlText(This,pText) ) 

#define ITaskSettings_put_XmlText(This,text)	\
    ( (This)->lpVtbl -> put_XmlText(This,text) ) 

#define ITaskSettings_get_RunOnlyIfNetworkAvailable(This,pRunOnlyIfNetworkAvailable)	\
    ( (This)->lpVtbl -> get_RunOnlyIfNetworkAvailable(This,pRunOnlyIfNetworkAvailable) ) 

#define ITaskSettings_put_RunOnlyIfNetworkAvailable(This,runOnlyIfNetworkAvailable)	\
    ( (This)->lpVtbl -> put_RunOnlyIfNetworkAvailable(This,runOnlyIfNetworkAvailable) ) 

#define ITaskSettings_get_ExecutionTimeLimit(This,pExecutionTimeLimit)	\
    ( (This)->lpVtbl -> get_ExecutionTimeLimit(This,pExecutionTimeLimit) ) 

#define ITaskSettings_put_ExecutionTimeLimit(This,executionTimeLimit)	\
    ( (This)->lpVtbl -> put_ExecutionTimeLimit(This,executionTimeLimit) ) 

#define ITaskSettings_get_Enabled(This,pEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,pEnabled) ) 

#define ITaskSettings_put_Enabled(This,enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,enabled) ) 

#define ITaskSettings_get_DeleteExpiredTaskAfter(This,pExpirationDelay)	\
    ( (This)->lpVtbl -> get_DeleteExpiredTaskAfter(This,pExpirationDelay) ) 

#define ITaskSettings_put_DeleteExpiredTaskAfter(This,expirationDelay)	\
    ( (This)->lpVtbl -> put_DeleteExpiredTaskAfter(This,expirationDelay) ) 

#define ITaskSettings_get_Priority(This,pPriority)	\
    ( (This)->lpVtbl -> get_Priority(This,pPriority) ) 

#define ITaskSettings_put_Priority(This,priority)	\
    ( (This)->lpVtbl -> put_Priority(This,priority) ) 

#define ITaskSettings_get_Compatibility(This,pCompatLevel)	\
    ( (This)->lpVtbl -> get_Compatibility(This,pCompatLevel) ) 

#define ITaskSettings_put_Compatibility(This,compatLevel)	\
    ( (This)->lpVtbl -> put_Compatibility(This,compatLevel) ) 

#define ITaskSettings_get_Hidden(This,pHidden)	\
    ( (This)->lpVtbl -> get_Hidden(This,pHidden) ) 

#define ITaskSettings_put_Hidden(This,hidden)	\
    ( (This)->lpVtbl -> put_Hidden(This,hidden) ) 

#define ITaskSettings_get_IdleSettings(This,ppIdleSettings)	\
    ( (This)->lpVtbl -> get_IdleSettings(This,ppIdleSettings) ) 

#define ITaskSettings_put_IdleSettings(This,pIdleSettings)	\
    ( (This)->lpVtbl -> put_IdleSettings(This,pIdleSettings) ) 

#define ITaskSettings_get_RunOnlyIfIdle(This,pRunOnlyIfIdle)	\
    ( (This)->lpVtbl -> get_RunOnlyIfIdle(This,pRunOnlyIfIdle) ) 

#define ITaskSettings_put_RunOnlyIfIdle(This,runOnlyIfIdle)	\
    ( (This)->lpVtbl -> put_RunOnlyIfIdle(This,runOnlyIfIdle) ) 

#define ITaskSettings_get_WakeToRun(This,pWake)	\
    ( (This)->lpVtbl -> get_WakeToRun(This,pWake) ) 

#define ITaskSettings_put_WakeToRun(This,wake)	\
    ( (This)->lpVtbl -> put_WakeToRun(This,wake) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITaskSettings_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TaskScheduler;

#ifdef __cplusplus

class DECLSPEC_UUID("0f87369f-a4e5-4cfc-bd3e-73e6154572dd")
TaskScheduler;
#endif
#endif /* __TaskScheduler_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

