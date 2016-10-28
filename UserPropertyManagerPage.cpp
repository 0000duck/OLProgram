// UserPropertyManagerPage.cpp : Implementation of CUserPropertyManagerPage

#include "stdafx.h"
#include "UserPropertyManagerPage.h"
#include "PMPageHandler.h"
#include "EliteSoftWare.h"

// CUserPropertyManagerPage

void CUserPropertyManagerPage::Init(CEliteSoftWare *app)
{
	userAddin = app;
	iSwApp = userAddin->GetSldWorksPtr();
	CreatePropertyManagerPage();
}

//Create the handler and property page, and add the controls
void CUserPropertyManagerPage::CreatePropertyManagerPage()
{
	CComObject<CPMPageHandler>::CreateInstance(&handler);
	handler->Init(userAddin);
	handler->AddRef();
	CComBSTR title;
	title.LoadString(IDS_PMP_TITLE);
	long errors = 0;
	long options = swPropertyManagerOptions_OkayButton | swPropertyManagerOptions_CancelButton;
	iSwApp->ICreatePropertyManagerPage(title,options,handler,&errors, &swPropertyPage);
	if (swPropertyPage != NULL)
		AddControls();
}

void CUserPropertyManagerPage::Destroy()
{
	swPropertyPage->Close(VARIANT_FALSE); //same as pressing cancel
	swPropertyPage = NULL;

	handler->Release();
	handler = NULL;
}

//Display the property page
void CUserPropertyManagerPage::Show()
{
	long rval;
	swPropertyPage->Show(&rval);
}

//Adds the two control groups, and adds the controls to each group
void CUserPropertyManagerPage::AddControls()
{
	//VARS
	CComBSTR caption;
	long options = -1;
	CComBSTR tip;

	// 创建 group1
	//////////////////////////////////////////////////////////////////////////
	caption.LoadString(IDS_PMP_GROUP1_TITLE);
	options = swGroupBoxOptions_Visible | swGroupBoxOptions_Expanded;
	swPropertyPage->IAddGroupBox(GROUP1, caption, options, &group1);
	options = swControlOptions_Visible | swControlOptions_Enabled;
	caption.LoadString(IDS_PMP_SELECT_FACE_BASETUBE);
	tip.LoadString(IDS_PMP_SELECTTIP_BASETUBEFACE);
	group1->IAddControl(SELECTION_BASETUBEFACE, swControlType_Selectionbox, caption, swControlAlign_LeftEdge, options, tip, &control);
	control->QueryInterface(__uuidof(IPropertyManagerPageSelectionbox), (void**)&selection_BaseTubeFace);
	if (selection_BaseTubeFace != NULL)
	{
		long filterArray[] = { swSelFACES };
		selection_BaseTubeFace->ISetSelectionFilters(1, filterArray);
		selection_BaseTubeFace->put_Height(15);
		selection_BaseTubeFace->put_SingleEntityOnly(VARIANT_TRUE);
	}
	//////////////////////////////////////////////////////////////////////////

	// 创建 group2
	//////////////////////////////////////////////////////////////////////////
	options = swGroupBoxOptions_Visible | swGroupBoxOptions_Expanded;
	swPropertyPage->IAddGroupBox(GROUP2, L"选择孔边线", options, &group2);
	options = swControlOptions_Visible | swControlOptions_Enabled;
 	caption.LoadString(IDS_PMP_SELECT_EDGE_HOLE);
 	tip.LoadString(IDS_PMP_SELECTTIP_HOLEEDGE);
 	group2->IAddControl(SELECTION_HOLEEDGE, swControlType_Selectionbox, caption, swControlAlign_LeftEdge, options, tip, &control);
 	control->QueryInterface(__uuidof(IPropertyManagerPageSelectionbox), (void**)&selection_HoleEdges);
 	if (selection_HoleEdges != NULL)
 	{
 		long filterArray[] = { swSelEDGES };
 		selection_HoleEdges->ISetSelectionFilters(1, filterArray);
 		selection_HoleEdges->put_Height(40);
 		selection_HoleEdges->put_SingleEntityOnly(VARIANT_FALSE);
 	}
	//////////////////////////////////////////////////////////////////////////

	// 创建 group3
	//////////////////////////////////////////////////////////////////////////
	options = swGroupBoxOptions_Visible | swGroupBoxOptions_Expanded;
	swPropertyPage->IAddGroupBox(GROUP3, L"参数设置", options, &group3);
	options = swControlOptions_Visible | swControlOptions_Enabled;
	caption.LoadString(IDS_PMP_BUTTON_SETPARAM);
	tip.LoadString(IDS_PMP_BUTTONTIP_SETPARAM);
	group3->IAddControl(BUTTON_SETPARAM, swControlType_Button, caption, swControlAlign_LeftEdge, options, tip, &control);
	control->QueryInterface(__uuidof(IPropertyManagerPageButton), (void**)&button_SetParam);
	//////////////////////////////////////////////////////////////////////////

	// 创建 group4
	//////////////////////////////////////////////////////////////////////////
	options = swGroupBoxOptions_Visible | swGroupBoxOptions_Expanded;
	swPropertyPage->IAddGroupBox(GROUP3, L"路径设置", options, &group4);
	options = swControlOptions_Visible | swControlOptions_Enabled;
	caption.LoadString(IDS_PMP_BUTTON_CLEARPATH);
	tip.LoadString(IDS_PMP_BUTTONTIP_CLEARPATH);
	group4->IAddControl(BUTTON_CLEARPATH, swControlType_Button, caption, swControlAlign_LeftEdge, options, tip, &control);
	control->QueryInterface(__uuidof(IPropertyManagerPageButton), (void**)&button_ClearPath);

	caption.LoadString(IDS_PMP_BUTTON_CALCUL);
	tip.LoadString(IDS_PMP_BUTTONTIP_CALCUL);
	group4->IAddControl(BUTTON_CALCULPATH, swControlType_Button, caption, swControlAlign_LeftEdge, options, tip, &control);
	control->QueryInterface(__uuidof(IPropertyManagerPageButton), (void**)&button_CalculPath);
	//////////////////////////////////////////////////////////////////////////
}

