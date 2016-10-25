// SwAddin.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "OLProgram.h"


class COLProgramModule : public CAtlDllModuleT< COLProgramModule >
{
public :

	DECLARE_LIBID(LIBID_OLProgramLib)

};

COLProgramModule _AtlModule;

class COLProgramApp : public CWinApp
{
public:

// Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(COLProgramApp, CWinApp)
END_MESSAGE_MAP()

COLProgramApp theApp;

BOOL COLProgramApp::InitInstance()
{
    return CWinApp::InitInstance();
}

int COLProgramApp::ExitInstance()
{
    return CWinApp::ExitInstance();
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _AtlModule.GetLockCount()==0) ? S_OK : S_FALSE;
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

