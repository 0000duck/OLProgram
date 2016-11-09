

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Nov 09 10:38:03 2016
 */
/* Compiler settings for Resource\EliteSoftWare.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __EliteSoftWare_i_h__
#define __EliteSoftWare_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEliteSoftWare_FWD_DEFINED__
#define __IEliteSoftWare_FWD_DEFINED__
typedef interface IEliteSoftWare IEliteSoftWare;
#endif 	/* __IEliteSoftWare_FWD_DEFINED__ */


#ifndef __ISwDocument_FWD_DEFINED__
#define __ISwDocument_FWD_DEFINED__
typedef interface ISwDocument ISwDocument;
#endif 	/* __ISwDocument_FWD_DEFINED__ */


#ifndef __IDocView_FWD_DEFINED__
#define __IDocView_FWD_DEFINED__
typedef interface IDocView IDocView;
#endif 	/* __IDocView_FWD_DEFINED__ */


#ifndef __IBitmapHandler_FWD_DEFINED__
#define __IBitmapHandler_FWD_DEFINED__
typedef interface IBitmapHandler IBitmapHandler;
#endif 	/* __IBitmapHandler_FWD_DEFINED__ */


#ifndef __IPMPageHandler_FWD_DEFINED__
#define __IPMPageHandler_FWD_DEFINED__
typedef interface IPMPageHandler IPMPageHandler;
#endif 	/* __IPMPageHandler_FWD_DEFINED__ */


#ifndef __IUserPropertyManagerPage_FWD_DEFINED__
#define __IUserPropertyManagerPage_FWD_DEFINED__
typedef interface IUserPropertyManagerPage IUserPropertyManagerPage;
#endif 	/* __IUserPropertyManagerPage_FWD_DEFINED__ */


#ifndef __EliteSoftWare_FWD_DEFINED__
#define __EliteSoftWare_FWD_DEFINED__

#ifdef __cplusplus
typedef class EliteSoftWare EliteSoftWare;
#else
typedef struct EliteSoftWare EliteSoftWare;
#endif /* __cplusplus */

#endif 	/* __EliteSoftWare_FWD_DEFINED__ */


#ifndef __SwDocument_FWD_DEFINED__
#define __SwDocument_FWD_DEFINED__

#ifdef __cplusplus
typedef class SwDocument SwDocument;
#else
typedef struct SwDocument SwDocument;
#endif /* __cplusplus */

#endif 	/* __SwDocument_FWD_DEFINED__ */


#ifndef __DocView_FWD_DEFINED__
#define __DocView_FWD_DEFINED__

#ifdef __cplusplus
typedef class DocView DocView;
#else
typedef struct DocView DocView;
#endif /* __cplusplus */

#endif 	/* __DocView_FWD_DEFINED__ */


#ifndef __BitmapHandler_FWD_DEFINED__
#define __BitmapHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class BitmapHandler BitmapHandler;
#else
typedef struct BitmapHandler BitmapHandler;
#endif /* __cplusplus */

#endif 	/* __BitmapHandler_FWD_DEFINED__ */


#ifndef __PMPageHandler_FWD_DEFINED__
#define __PMPageHandler_FWD_DEFINED__

#ifdef __cplusplus
typedef class PMPageHandler PMPageHandler;
#else
typedef struct PMPageHandler PMPageHandler;
#endif /* __cplusplus */

#endif 	/* __PMPageHandler_FWD_DEFINED__ */


#ifndef __UserPropertyManagerPage_FWD_DEFINED__
#define __UserPropertyManagerPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class UserPropertyManagerPage UserPropertyManagerPage;
#else
typedef struct UserPropertyManagerPage UserPropertyManagerPage;
#endif /* __cplusplus */

#endif 	/* __UserPropertyManagerPage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IEliteSoftWare_INTERFACE_DEFINED__
#define __IEliteSoftWare_INTERFACE_DEFINED__

/* interface IEliteSoftWare */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEliteSoftWare;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3B367AB6-2571-485B-884D-1DD898FF4C30")
    IEliteSoftWare : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarCallbackPathExport( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarEnablePathExport( 
            /* [retval][out] */ long *status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GeneratePath( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableGeneratePath( 
            /* [retval][out] */ long *status) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarBuildTube( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ToolbarEnableBuildTube( 
            /* [retval][out] */ long *status) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEliteSoftWareVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEliteSoftWare * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEliteSoftWare * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEliteSoftWare * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEliteSoftWare * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEliteSoftWare * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEliteSoftWare * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEliteSoftWare * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarCallbackPathExport )( 
            IEliteSoftWare * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarEnablePathExport )( 
            IEliteSoftWare * This,
            /* [retval][out] */ long *status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GeneratePath )( 
            IEliteSoftWare * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableGeneratePath )( 
            IEliteSoftWare * This,
            /* [retval][out] */ long *status);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarBuildTube )( 
            IEliteSoftWare * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ToolbarEnableBuildTube )( 
            IEliteSoftWare * This,
            /* [retval][out] */ long *status);
        
        END_INTERFACE
    } IEliteSoftWareVtbl;

    interface IEliteSoftWare
    {
        CONST_VTBL struct IEliteSoftWareVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEliteSoftWare_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEliteSoftWare_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEliteSoftWare_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEliteSoftWare_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEliteSoftWare_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEliteSoftWare_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEliteSoftWare_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEliteSoftWare_ToolbarCallbackPathExport(This)	\
    ( (This)->lpVtbl -> ToolbarCallbackPathExport(This) ) 

#define IEliteSoftWare_ToolbarEnablePathExport(This,status)	\
    ( (This)->lpVtbl -> ToolbarEnablePathExport(This,status) ) 

#define IEliteSoftWare_GeneratePath(This)	\
    ( (This)->lpVtbl -> GeneratePath(This) ) 

#define IEliteSoftWare_EnableGeneratePath(This,status)	\
    ( (This)->lpVtbl -> EnableGeneratePath(This,status) ) 

#define IEliteSoftWare_ToolbarBuildTube(This)	\
    ( (This)->lpVtbl -> ToolbarBuildTube(This) ) 

#define IEliteSoftWare_ToolbarEnableBuildTube(This,status)	\
    ( (This)->lpVtbl -> ToolbarEnableBuildTube(This,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IEliteSoftWare_INTERFACE_DEFINED__ */


#ifndef __ISwDocument_INTERFACE_DEFINED__
#define __ISwDocument_INTERFACE_DEFINED__

/* interface ISwDocument */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISwDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("47D7EABD-4142-4CF8-A788-B0A8580B957B")
    ISwDocument : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISwDocumentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISwDocument * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISwDocument * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISwDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISwDocument * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISwDocument * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISwDocument * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISwDocument * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISwDocumentVtbl;

    interface ISwDocument
    {
        CONST_VTBL struct ISwDocumentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISwDocument_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISwDocument_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISwDocument_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISwDocument_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISwDocument_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISwDocument_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISwDocument_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISwDocument_INTERFACE_DEFINED__ */


#ifndef __IDocView_INTERFACE_DEFINED__
#define __IDocView_INTERFACE_DEFINED__

/* interface IDocView */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDocView;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3AF07648-E725-4DFA-9228-931E7B377EC7")
    IDocView : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDocViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDocView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDocView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDocView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDocView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDocView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDocView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDocView * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDocViewVtbl;

    interface IDocView
    {
        CONST_VTBL struct IDocViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDocView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDocView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDocView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDocView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDocView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDocView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDocView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDocView_INTERFACE_DEFINED__ */


#ifndef __IBitmapHandler_INTERFACE_DEFINED__
#define __IBitmapHandler_INTERFACE_DEFINED__

/* interface IBitmapHandler */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IBitmapHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9E7BF461-496C-4C45-B5C9-3056E50AD965")
    IBitmapHandler : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateBitmapFileFromResource( 
            /* [in] */ DWORD resID,
            /* [retval][out] */ BSTR *retval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Dispose( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBitmapHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBitmapHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBitmapHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBitmapHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBitmapHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBitmapHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBitmapHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBitmapHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateBitmapFileFromResource )( 
            IBitmapHandler * This,
            /* [in] */ DWORD resID,
            /* [retval][out] */ BSTR *retval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Dispose )( 
            IBitmapHandler * This);
        
        END_INTERFACE
    } IBitmapHandlerVtbl;

    interface IBitmapHandler
    {
        CONST_VTBL struct IBitmapHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBitmapHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBitmapHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBitmapHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBitmapHandler_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBitmapHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBitmapHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBitmapHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBitmapHandler_CreateBitmapFileFromResource(This,resID,retval)	\
    ( (This)->lpVtbl -> CreateBitmapFileFromResource(This,resID,retval) ) 

#define IBitmapHandler_Dispose(This)	\
    ( (This)->lpVtbl -> Dispose(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBitmapHandler_INTERFACE_DEFINED__ */


#ifndef __IPMPageHandler_INTERFACE_DEFINED__
#define __IPMPageHandler_INTERFACE_DEFINED__

/* interface IPMPageHandler */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPMPageHandler;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E30E1B35-F62B-415C-852D-2ECB2462B438")
    IPMPageHandler : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IPMPageHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPMPageHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPMPageHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPMPageHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPMPageHandler * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPMPageHandler * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPMPageHandler * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPMPageHandler * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IPMPageHandlerVtbl;

    interface IPMPageHandler
    {
        CONST_VTBL struct IPMPageHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPMPageHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPMPageHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPMPageHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPMPageHandler_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPMPageHandler_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPMPageHandler_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPMPageHandler_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPMPageHandler_INTERFACE_DEFINED__ */


#ifndef __IUserPropertyManagerPage_INTERFACE_DEFINED__
#define __IUserPropertyManagerPage_INTERFACE_DEFINED__

/* interface IUserPropertyManagerPage */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IUserPropertyManagerPage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BECAC6BB-5252-4158-8A51-41E567CA5946")
    IUserPropertyManagerPage : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IUserPropertyManagerPageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUserPropertyManagerPage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUserPropertyManagerPage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUserPropertyManagerPage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUserPropertyManagerPage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUserPropertyManagerPage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUserPropertyManagerPage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUserPropertyManagerPage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IUserPropertyManagerPageVtbl;

    interface IUserPropertyManagerPage
    {
        CONST_VTBL struct IUserPropertyManagerPageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUserPropertyManagerPage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUserPropertyManagerPage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUserPropertyManagerPage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUserPropertyManagerPage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUserPropertyManagerPage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUserPropertyManagerPage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUserPropertyManagerPage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUserPropertyManagerPage_INTERFACE_DEFINED__ */



#ifndef __EliteSoftWareLib_LIBRARY_DEFINED__
#define __EliteSoftWareLib_LIBRARY_DEFINED__

/* library EliteSoftWareLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_EliteSoftWareLib;

EXTERN_C const CLSID CLSID_EliteSoftWare;

#ifdef __cplusplus

class DECLSPEC_UUID("3BB8AFA7-E4D6-4BA6-BFBD-C39729E1F545")
EliteSoftWare;
#endif

EXTERN_C const CLSID CLSID_SwDocument;

#ifdef __cplusplus

class DECLSPEC_UUID("1A28FAE3-74B4-4757-AB24-F2C96B1332F7")
SwDocument;
#endif

EXTERN_C const CLSID CLSID_DocView;

#ifdef __cplusplus

class DECLSPEC_UUID("F861E05B-FBCB-42B2-971F-F388D08EB1DC")
DocView;
#endif

EXTERN_C const CLSID CLSID_BitmapHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("E10E72D0-04C5-4EA1-B841-8DA6152B2DF0")
BitmapHandler;
#endif

EXTERN_C const CLSID CLSID_PMPageHandler;

#ifdef __cplusplus

class DECLSPEC_UUID("7741B23C-DDB2-483F-A2F7-117FE59300B9")
PMPageHandler;
#endif

EXTERN_C const CLSID CLSID_UserPropertyManagerPage;

#ifdef __cplusplus

class DECLSPEC_UUID("F43C6FAC-004A-40F4-A66C-13C2D1DC1D1D")
UserPropertyManagerPage;
#endif
#endif /* __EliteSoftWareLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


