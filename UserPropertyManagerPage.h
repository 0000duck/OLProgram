// UserPropertyManagerPage.h : Declaration of the CUserPropertyManagerPage

#pragma once
#include "EliteSoftWare_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
class CEliteSoftWare;
class CPMPageHandler;

//Control IDs
#define GROUP1		1
#define GROUP2		2
#define GROUP3		3
#define GROUP4		4

// CUserPropertyManagerPage

class ATL_NO_VTABLE CUserPropertyManagerPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUserPropertyManagerPage, &CLSID_UserPropertyManagerPage>,
	public IDispatchImpl<IUserPropertyManagerPage, &IID_IUserPropertyManagerPage, &LIBID_EliteSoftWareLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:

private:
	CEliteSoftWare *userAddin;
	CComPtr<ISldWorks> iSwApp;
	CComPtr<IPropertyManagerPage2> swPropertyPage;
	CComObject<CPMPageHandler> *handler;

protected: 
	//Generic Control
	IPropertyManagerPageControl *control;

	// Groups
	IPropertyManagerPageGroup *group1;
	IPropertyManagerPageGroup *group2;
	IPropertyManagerPageGroup *group3;
	IPropertyManagerPageGroup *group4;

	// In Group1
	IPropertyManagerPageSelectionbox *selection_BaseTubeFace;

	// In Group2
	IPropertyManagerPageSelectionbox *selection_HoleEdges;

	// In Group3
	IPropertyManagerPageButton       *button_SetParam;

	// In Group4
	IPropertyManagerPageButton       *button_CalculPath;
	IPropertyManagerPageButton       *button_ClearPath;
public:
	CUserPropertyManagerPage()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_USERPROPERTYMANAGERPAGE)

DECLARE_NOT_AGGREGATABLE(CUserPropertyManagerPage)

BEGIN_COM_MAP(CUserPropertyManagerPage)
	COM_INTERFACE_ENTRY(IUserPropertyManagerPage)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IUserPropertyManagerPage
public:
	void Init(CEliteSoftWare *app);
	void CreatePropertyManagerPage();
	void Destroy();
	void Show();
	void AddControls();
};

OBJECT_ENTRY_AUTO(__uuidof(UserPropertyManagerPage), CUserPropertyManagerPage)
