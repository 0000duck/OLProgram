// UserPropertyManagerPage.h : Declaration of the CUserPropertyManagerPage

#pragma once
#include "OLProgram_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
class COLProgram;
class CPMPageHandler;

//Control IDs
#define GROUP0		0
#define GROUP1		1
#define GROUP2		2
#define GROUP3		3
#define GROUP4		4
#define	TEXT1		5
#define CHECK1		6
#define OPTION1		7
#define OPTION2		8
#define OPTION3		9	
#define LIST1		10
#define SELECTION1	11
#define NUM1		12
#define COMBO1		13



// CUserPropertyManagerPage

class ATL_NO_VTABLE CUserPropertyManagerPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUserPropertyManagerPage, &CLSID_UserPropertyManagerPage>,
	public IDispatchImpl<IUserPropertyManagerPage, &IID_IUserPropertyManagerPage, &LIBID_OLProgramLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:

private:
	COLProgram *userAddin;
	CComPtr<ISldWorks> iSwApp;
	CComPtr<IPropertyManagerPage2> swPropertyPage;
	CComObject<CPMPageHandler> *handler;

protected: 
	//Generic Control
	IPropertyManagerPageControl *control;

	//Groups
//	IPropertyManagerPageGroup *group0;
	IPropertyManagerPageGroup *group1;
	IPropertyManagerPageGroup *group2;
	IPropertyManagerPageGroup *group3;
	IPropertyManagerPageGroup *group4;

	//In Group1
	IPropertyManagerPageTextbox		*text1;
	IPropertyManagerPageCheckbox	*check1;
	IPropertyManagerPageOption		*option1;
	IPropertyManagerPageOption		*option2;
	IPropertyManagerPageOption		*option3;
	IPropertyManagerPageListbox		*list1;

	//In Group2
	IPropertyManagerPageSelectionbox *selection_BaseTubeFace;
	IPropertyManagerPageSelectionbox *selection_HoleEdges;
//	IPropertyManagerPageButton       *button_BuildTube;
	IPropertyManagerPageButton       *button_CalculPath;
	IPropertyManagerPageButton       *button_SetParam;
	IPropertyManagerPageButton       *button_ClearPath;
	IPropertyManagerPageNumberbox	 *num1;
	IPropertyManagerPageCombobox	 *combo1;
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
	void Init(COLProgram *app);
	void CreatePropertyManagerPage();
	void Destroy();
	void Show();
	void AddControls();
};

OBJECT_ENTRY_AUTO(__uuidof(UserPropertyManagerPage), CUserPropertyManagerPage)
