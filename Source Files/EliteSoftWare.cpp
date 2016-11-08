// EliteSoftWare.cpp : Implementation of CEliteSoftWare

#include "stdafx.h"
#include "EliteSoftWare.h"
#include "SwDocument.h"
#include <string>
#include <list>
#include "UserPropertyManagerPage.h"
#include "BitmapHandler.h"
#include "fstream"
#include "ExportDlg.h"
#include "BuildTubeDlg.h"
#include "Math.h"
using namespace std;


// CEliteSoftWare
CEliteSoftWare::~CEliteSoftWare()
{
	POSITION entPos = m_LPathEntityList.GetHeadPosition();
	while(entPos)
	{
		CPathCombList* pPathCombList = m_LPathEntityList.GetNext(entPos);
		if (NULL == pPathCombList)
			continue;
		delete pPathCombList;
		pPathCombList = NULL;
	}
	
	m_LPathEntityList.RemoveAll();

	POSITION paraPos = m_LHoleParamList.GetHeadPosition();
	while(paraPos)
	{
		CHCombParam* pHCombPara = m_LHoleParamList.GetNext(paraPos);
		if (NULL == pHCombPara)
			continue;
		delete pHCombPara;
		pHCombPara = NULL;
	}
	m_LHoleParamList.RemoveAll();
}

void CEliteSoftWare::AddCommandManager()
{
	CComPtr<ICommandGroup> icmdGroup;
	
	HRESULT hret = S_OK;

	CComObject<CBitmapHandler>::CreateInstance(&iBmp);
	iBmp->AddRef();
	long cmdIndex0, cmdIndex1, cmdIndex2;
	const int numDocumentTypes = 3;

	int docTypes[numDocumentTypes];

	docTypes[0] = swDocASSEMBLY,
	docTypes[1] = swDocDRAWING,
	docTypes[2] = swDocPART;


	CComBSTR title;
	title.LoadString(IDS_TOOLBAR_TITLE);

	CComBSTR hint;
	hint.LoadString(IDS_MENU_HINT);

	long cmdGroupErr = 0;
	bool ignorePrevious = false;
	VARIANT_BOOL getDataResult = VARIANT_FALSE;
	long count = 0;

	hret = iCmdMgr->GetCommandIDsCount(MAIN_CMD_GROUP_ID, &count);
	long * registryIDs = new long[count];


	HRESULT retVal = iCmdMgr->IGetGroupDataFromRegistry(MAIN_CMD_GROUP_ID, count, registryIDs, &getDataResult);
	long knownIDs[3] = {MAIN_ITEM_ID1, MAIN_ITEM_ID2, MAIN_ITEM_ID3};

	if(getDataResult)
	{
		if(!CompareIDs(registryIDs, count, knownIDs, 3)) // if the ID's don't match, reset the command group
		{
			ignorePrevious = true;
		}
	}
		
	
	iCmdMgr->CreateCommandGroup2(MAIN_CMD_GROUP_ID, title, title, hint, -1, ignorePrevious, &cmdGroupErr, &icmdGroup);

	CComBSTR smallImageFile;
	iBmp->CreateBitmapFileFromResource(IDB_TOOLBAR_SMALL, &smallImageFile);
	icmdGroup->put_SmallIconList(smallImageFile);

	CComBSTR largeImageFile;
	iBmp->CreateBitmapFileFromResource(IDB_TOOLBAR_LARGE, &largeImageFile);
	icmdGroup->put_LargeIconList(largeImageFile);

	CComBSTR largeIconFile;
	iBmp->CreateBitmapFileFromResource(IDB_ICON_LARGE, &largeIconFile);
	icmdGroup->put_LargeMainIcon(largeIconFile);

	CComBSTR smallIconFile;
	iBmp->CreateBitmapFileFromResource(IDB_ICON_SMALL, &smallIconFile);
	icmdGroup->put_SmallMainIcon(smallIconFile);

	int menuToolbarOption = (int)(swMenuItem | swToolbarItem);

	CComBSTR tip;
	CComBSTR callback;
	CComBSTR enable;
	VARIANT_BOOL cmdActivated;
	hret = icmdGroup->AddCommandItem2(L"�����ܼ�", -1, L"ͨ������������Զ������ܼ�ģ�͡�", L"�����ܼ�", 0, L"ToolbarBuildTube", L"ToolbarEnableBuildTube", MAIN_ITEM_ID1, menuToolbarOption, &cmdIndex0);

	tip.LoadString(IDS_TOOLBAR_PMPTIP_CALPATH);
	hint.LoadString(IDS_TOOLBAR_PMPHINT_CALPATH);
	hret = icmdGroup->AddCommandItem2(tip, -1, hint, tip, 0, L"GeneratePath", L"EnableGeneratePath", MAIN_ITEM_ID2, menuToolbarOption, &cmdIndex1);

	tip.LoadString(IDS_TOOLBAR_TIP_PATHEXPORT);
	hint.LoadString(IDS_TOOLBAR_HINT_PATHEXPORT);
	hret = icmdGroup->AddCommandItem2(tip, -1, hint, tip, 0, L"ToolbarCallbackPathExport", L"ToolbarEnablePathExport", MAIN_ITEM_ID3, menuToolbarOption, &cmdIndex2);

	icmdGroup->put_HasToolbar(true);
	icmdGroup->put_HasMenu(true);
	icmdGroup->Activate(&cmdActivated);

	//Clean up
	delete [] registryIDs;
}

void CEliteSoftWare::RemoveCommandManager()
{
	iBmp->Dispose();
	iBmp->Release();

	VARIANT_BOOL cmdRemoved = VARIANT_FALSE; 
	VARIANT_BOOL rtOnly = VARIANT_TRUE;
	int ret = 0;

	ret = iCmdMgr->RemoveFlyoutGroup(FLYOUT_GROUP_ID, &cmdRemoved);
	ret = iCmdMgr->RemoveCommandGroup(MAIN_CMD_GROUP_ID, &cmdRemoved);
}

bool CEliteSoftWare::CompareIDs(long * storedIDs, long storedSize, long * addinIDs, long addinSize)
{
	std::list<long> storedList;
    std::list<long> addinList;

	for(int i = 0; i < storedSize; i++)
	{
		storedList.push_front(storedIDs[i]);
	}
	for(int j = 0; j < addinSize; j++)
	{
		addinList.push_front(addinIDs[j]);
	}

    addinList.sort();
    storedList.sort();

    if (addinList.size() != storedList.size())
    {
        return false;
    }
    else
    {
		std::list<long>::iterator al = addinList.begin();
		//Iterate through the two lists and make sure they match
		for(std::list<long>::iterator sl = storedList.begin(); sl != storedList.end(); sl++)
		{
			if(*al != *sl)
			{
				return false;
			}
			al++;
		}
    }
    return true;
}

void CEliteSoftWare::AddPMP()
{
	CComObject<CUserPropertyManagerPage>::CreateInstance(&userPropertyPage);
	userPropertyPage->AddRef();
	userPropertyPage->Init(this);
}

//Remove the user Property Manager Page
void CEliteSoftWare::RemovePMP()
{
	userPropertyPage->Destroy();
	userPropertyPage->Release();
	userPropertyPage = NULL;
}

STDMETHODIMP CEliteSoftWare::OnDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called after a document is opened
STDMETHODIMP CEliteSoftWare::OnFileOpenPostNotify(BSTR fileName)
{
	HRESULT hres = S_OK;
	hres = AttachEventsToAllDocuments();
	return hres;
}

//Called when a new document is created or a document is loaded
STDMETHODIMP CEliteSoftWare::OnDocLoad(BSTR docTitle, BSTR docPath)
{

	return S_OK;
}
VARIANT_BOOL CEliteSoftWare::AttachEventHandlers()
{
	VARIANT_BOOL attached = VARIANT_TRUE;
	this->m_libid = LIBID_SldWorks;
	this->m_wMajorVerNum = GetSldWorksTlbMajor();
	this->m_wMinorVerNum = 0;

	CSldWorksEvents::_tih.m_wMajor = this->m_wMajorVerNum;

	// Connect to the SldWorks event sink
	HRESULT success = this->DispEventAdvise(iSwApp, &__uuidof(DSldWorksEvents));

	if (success != S_OK)
		return VARIANT_FALSE;

	// Connect event handlers to all previously open documents
	success = AttachEventsToAllDocuments();
	if (success != S_OK)
		attached = VARIANT_FALSE;
	return attached;
}

HRESULT CEliteSoftWare::AttachEventsToAllDocuments()
{
	VARIANT_BOOL attached = VARIANT_TRUE;
	USES_CONVERSION;

	TMapIUnknownToDocument::iterator iter;

	CComPtr<IModelDoc2> iModelDoc2;
	iSwApp->IGetFirstDocument2(&iModelDoc2);
	while (iModelDoc2 != NULL)
	{
			iter = openDocs.find(iModelDoc2);
			if (iter == openDocs.end())
			{
				ATLTRACE("\tCEliteSoftWare::DocumentLoadNotify current part not found\n");
				attached = AttachModelEventHandler(iModelDoc2);
				if(!attached)
					return E_FAIL;
			}
		
			else
			{
				VARIANT_BOOL connected = VARIANT_FALSE;
				CComObject<CSwDocument> *docHandler = NULL;
				docHandler = (CComObject<CSwDocument>*)openDocs[iModelDoc2];
				
				if (docHandler != NULL)
				{
				    connected = docHandler->AttachModelViewEventHandlers();
				}
			}
		
		CComPtr<IModelDoc2> iNextModelDoc2;
		iModelDoc2->IGetNext(&iNextModelDoc2);
		iModelDoc2.Release();
		iModelDoc2 = iNextModelDoc2;
	}
	return S_OK;
}

//Create an event handling object for this document and listen for the model's events
VARIANT_BOOL CEliteSoftWare::AttachModelEventHandler(CComPtr<IModelDoc2> iModelDoc2)
{
	VARIANT_BOOL attached = VARIANT_TRUE;
	if (iModelDoc2 == NULL)
		return VARIANT_FALSE;
	
	TMapIUnknownToDocument::iterator iter;
	iter = openDocs.find(iModelDoc2);
	if (iter == openDocs.end())
	{
		//Cretae a new Document event handler
		CComObject<CSwDocument> *pDoc;
		CComObject<CSwDocument>::CreateInstance( &pDoc);
		pDoc->Init((CComObject<CEliteSoftWare>*)this, iModelDoc2);

		//Listen for the doc's events
		attached = pDoc->AttachEventHandlers();
		if(!attached)
			return VARIANT_FALSE;

		//Add it to the map
		openDocs.insert(TMapIUnknownToDocument::value_type(iModelDoc2, pDoc));
		iModelDoc2.p->AddRef();
		pDoc->AddRef();
	}
	return attached;
}

//Stop listening for SolidWorks Events
VARIANT_BOOL CEliteSoftWare::DetachEventHandlers()
{
	VARIANT_BOOL detached = VARIANT_TRUE;

	// Disconnect from the SldWorks event sink
	HRESULT success = this->DispEventUnadvise(iSwApp, &__uuidof(DSldWorksEvents));

	CSldWorksEvents::_tih.m_plibid = &GUID_NULL;

	if (success != S_OK)
		return VARIANT_FALSE;

	// Disconnect all event handlers
	TMapIUnknownToDocument::iterator iter;
	iter = openDocs.begin();

	for (iter = openDocs.begin(); iter != openDocs.end(); /*iter++*/) //The iteration is performed inside the loop
	{
		TMapIUnknownToDocument::value_type obj = *iter;
		iter++;

		CComObject<CSwDocument> *pDoc = (CComObject<CSwDocument>*)obj.second;
		detached = pDoc->DetachEventHandlers();
	}
	return detached;
}

//Stop listening for events on the specified model
VARIANT_BOOL CEliteSoftWare::DetachModelEventHandler(IUnknown *iUnk)
{
	VARIANT_BOOL detached = VARIANT_TRUE;
	TMapIUnknownToDocument::iterator iter;

	iter = openDocs.find(iUnk);
	if (iter != openDocs.end())
	{
		TMapIUnknownToDocument::value_type obj = *iter;
		obj.first->Release();

		CComObject<CSwDocument> *pDoc = (CComObject<CSwDocument>*)obj.second;
		pDoc->Release();

		openDocs.erase(iter);
	}
	return detached;
}

//Called when the active model document changes in SolidWorks
STDMETHODIMP CEliteSoftWare::OnModelDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called when a new file is created
STDMETHODIMP CEliteSoftWare::OnFileNew(LPDISPATCH newDoc, long docType, BSTR templateName)
{
	HRESULT hres = S_OK;
	hres = AttachEventsToAllDocuments();
   return hres;
}

STDMETHODIMP CEliteSoftWare::ConnectToSW(LPDISPATCH ThisSW, long Cookie, VARIANT_BOOL * IsConnected)
{
	ThisSW->QueryInterface(__uuidof(ISldWorks), (void**)&iSwApp);
	addinID = Cookie;
	iSwApp->GetCommandManager(Cookie,&iCmdMgr);

	VARIANT_BOOL status = VARIANT_FALSE;

	iSwApp->SetAddinCallbackInfo((long)_AtlBaseModule.GetModuleInstance(), static_cast<IEliteSoftWare*>(this), addinID, &status);
	//Get the current type library version.
	{
		USES_CONVERSION;
		CComBSTR bstrNum;
		std::string strNum;
		char *buffer;

		iSwApp->RevisionNumber(&bstrNum);

		strNum = W2A(bstrNum);
		m_swMajNum = strtol(strNum.c_str(), &buffer, 10 );

		m_swMinNum=0;

	}
	//Create the addin's UI
	AddCommandManager();
	AddPMP();
	//Listen for events
	*IsConnected = AttachEventHandlers();
	*IsConnected = VARIANT_TRUE;
	m_nExportOrder = 0;
	m_dFixAng = 90.;
	return S_OK;
}
STDMETHODIMP CEliteSoftWare::DisconnectFromSW(VARIANT_BOOL * IsDisconnected)
{
	//Remove the addin's UI
	RemoveCommandManager();
	RemovePMP();

	//Stop listening for events
	*IsDisconnected = DetachEventHandlers();

	iCmdMgr.Release();
	//Make sure you release the SolidWorks pointer last
	iSwApp.Release();

	return S_OK;
}

STDMETHODIMP CEliteSoftWare::ToolbarCallbackPathExport(void)
{
	ExportPathToTXT();
	return S_OK;
}

STDMETHODIMP CEliteSoftWare::ToolbarBuildTube(void)
{
    BuildTubeAndHole();
	return S_OK;
}

// �����������ܼ�
void CEliteSoftWare::BuildTubeAndHole()
{
	long docType = -1;
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);

	if(iSwModel != NULL)
	{
		iSwModel->GetType(&docType);
	}

	if (docType != swDocPART && docType != swDocASSEMBLY)
	{
		AfxMessageBox(_T("�򿪵Ĳ������"),MB_OK|MB_ICONINFORMATION);
		iSwModel = NULL;
		return ;
	}

	//////////////////////////////////////////////////////////////////////////
	BSTR sFileName;
	iSwModel->GetTitle(&sFileName);
	CHCombParam* pHCombParams = GetCurHoleParam();
	if (NULL == pHCombParams)
	{
		pHCombParams = new CHCombParam(sFileName);
		m_LHoleParamList.AddTail(pHCombParams);
	}

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CBuildTubeDlg BuildTubeDlg;
	BuildTubeDlg.m_dTubeDia    = pHCombParams->m_dTubeDia*1000.;
	BuildTubeDlg.m_dTubeLength = pHCombParams->m_dTubeLength*1000.;
	BuildTubeDlg.m_dTubeThick  = pHCombParams->m_dTubeThick*1000.;
	BuildTubeDlg.SetHParamList(pHCombParams);
	BuildTubeDlg.m_bBulidTube = pHCombParams->m_bBulidTube;
	if (BuildTubeDlg.DoModal() != IDOK)
		return;
	pHCombParams->m_dTubeDia    = BuildTubeDlg.m_dTubeDia/1000.;
	pHCombParams->m_dTubeLength = BuildTubeDlg.m_dTubeLength/1000.;
	pHCombParams->m_dTubeThick  = BuildTubeDlg.m_dTubeThick/1000.;
	double dRWCircle = pHCombParams->m_dTubeDia*0.5;                  // ��Բֱ��
	double dRNCircle = pHCombParams->m_dTubeDia*0.5 - pHCombParams->m_dTubeThick; // ��Բֱ��

	CComPtr<IModelDocExtension> swDocExt;
	CComPtr<ISketchManager> swSketMgr;
	CComPtr<IFeatureManager> swFeatmgr;
	iSwModel->get_Extension(&swDocExt);
	iSwModel->get_SketchManager(&swSketMgr);
	iSwModel->get_FeatureManager(&swFeatmgr);

	if (NULL == swFeatmgr)
	{
		AfxMessageBox(_T("swFeature == NULL"));
		return;
	}

	VARIANT_BOOL retval;	
	// �����������ܼ�
	//////////////////////////////////////////////////////////////////////////
	if (pHCombParams->m_bBulidTube) //���
	{
		// prepare: ���û��Ʒ�ʽ����ʾ��ʽ	
		iSwModel->SetAddToDB(VARIANT_TRUE);
		iSwModel->SetDisplayWhenAdded(VARIANT_FALSE);
		iSwModel->ClearSelection2(VARIANT_TRUE);

		// step1���������и�Բ��
		iSwModel->SelectByID(_T("���ӻ�׼��"),_T("PLANE"),0.,0.,0.,&retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷�����Բ��"));
			iSwModel = NULL;
			return;
		}
		swSketMgr->InsertSketch(VARIANT_TRUE);
		CComPtr<ISketchSegment> swSketchWArc;
		CComPtr<ISketchSegment> swSketchNArc;
		swSketMgr->CreateCircleByRadius(0.,0.,0.,dRWCircle,&swSketchWArc);
		swSketMgr->CreateCircleByRadius(0.,0.,0.,dRNCircle,&swSketchNArc);
		swSketMgr->InsertSketch(VARIANT_TRUE);
		iSwModel->FeatureBoss(1,0,0,0,0,pHCombParams->m_dTubeLength,pHCombParams->m_dTubeLength,0,0,0,0,2,2,0,0);

		// step2������������ߣ����ڴ����Ļ�׼��
		iSwModel->SelectByID(_T("ǰ�ӻ�׼��"),_T("PLANE"),0.,0.,0.,&retval);
		swSketMgr->InsertSketch(VARIANT_TRUE);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ��ǰ�ӻ�׼�棬�޷����������ߡ�"));
			iSwModel = NULL;
			return;
		}
		CComPtr<ISketchSegment> swSketchCLine;
		swSketMgr->CreateCenterLine(0,0,0,0.1,0,0,&swSketchCLine);
		swSketMgr->InsertSketch(VARIANT_TRUE);
		iSwModel->ClearSelection2(VARIANT_TRUE);
		iSwModel->SetAddToDB(VARIANT_FALSE);
		iSwModel->SetDisplayWhenAdded(VARIANT_TRUE);
		pHCombParams->m_bBulidTube = FALSE;
	}
	//////////////////////////////////////////////////////////////////////////

	// ������������
	//////////////////////////////////////////////////////////////////////////
	iSwModel->SetAddToDB(VARIANT_TRUE);
	iSwModel->SetDisplayWhenAdded(VARIANT_FALSE);
	//////////////////////////////////////////////////////////////////////////
	// ���ӻ�׼��ID�仯�Ͳ�ͼID�仯����������������б�Źᴩ�Ŀ�
	int nHoleID = 0;
	POSITION paraPos = pHCombParams->m_LHParamList.GetHeadPosition();
	while(paraPos)
	{
		CHoleParam* pHParam = pHCombParams->m_LHParamList.GetNext(paraPos);
		if(NULL == pHParam)
			continue;


		if (nHoleID < pHCombParams->m_nHoleID)
		{// ��ֹ�ظ��������ӵĿ�
			nHoleID++;
			continue;
		}
		nHoleID++;

		// step1��������׼��		
		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷����ɻ�׼�档"));
			iSwModel = NULL;
			return;
		}

		swDocExt->SelectByID2(L"Line1@��ͼ2", L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼�档"));
			iSwModel = NULL;
			return;
		}

		CComQIPtr<IDispatch> pRefPlane;
		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, pHParam->m_dExRotAng, 
			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlane);
		pHCombParams->m_nRefPlaneID++;

		CString sID;
		BSTR bstrText;
		// step2�����ƴ��и��
		sID.Format(_T("��׼��%d"),pHCombParams->m_nRefPlaneID);
		bstrText = sID.AllocSysString();
		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
		swSketMgr->InsertSketch(VARIANT_TRUE);

		CComPtr<ISketchSegment> swSketchLine;
		swSketMgr->CreateCenterLine(pHParam->m_dOffsetX,0,0,pHParam->m_dOffsetX,0.1,0,&swSketchLine);
		pHCombParams->m_nSketID++;

		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
		sID.Format(_T("Line1@��ͼ%d"),pHCombParams->m_nSketID);
		bstrText = sID.AllocSysString();
		swDocExt->SelectByID2(bstrText, L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼�档"));
			iSwModel = NULL;
			return;
		}

		CComQIPtr<IDispatch> pRefPlaneForThough;
		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, (PI*0.5+pHParam->m_dThroughAng), 
			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlaneForThough);
		pHCombParams->m_nRefPlaneID++;
		swSketMgr->InsertSketch(VARIANT_TRUE);

		sID.Format(_T("��׼��%d"),pHCombParams->m_nRefPlaneID);
		bstrText = sID.AllocSysString();
		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
		swSketMgr->InsertSketch(VARIANT_TRUE);

		// 2.Xƫ�����Ϊ�ᴩ������������Բ����ľ���
		CComPtr<ISketchSegment> swSketchHole;
		double dis = sqrt((pHCombParams->m_dTubeDia*0.5)*(pHCombParams->m_dTubeDia*0.5)-
			pHParam->m_dOffsetZ*pHParam->m_dOffsetZ);
		double dis2 = 1./tan(pHParam->m_dThroughAng)*dis;
		double dCenterXOffset = dis2+pHParam->m_dOffsetX;
		PNT3D ptCenter;
		ptCenter[0] = pHParam->m_dOffsetZ;
		ptCenter[1] = cos(PI*0.5-pHParam->m_dThroughAng)*dCenterXOffset;
		ptCenter[2] = 0;
		swSketMgr->CreateCircleByRadius(ptCenter[0], ptCenter[1], ptCenter[2], pHParam->m_dHoleR, &swSketchHole);
		//////////////////////////////////////////////////////////////////////////
		pHCombParams->m_nSketID++;
		pHCombParams->m_nHoleID++;
		swSketMgr->InsertSketch(VARIANT_TRUE);
		iSwModel->FeatureCut(1,0,VARIANT_FALSE,0,0,pHCombParams->m_dTubeDia*100.,pHCombParams->m_dTubeDia*100.,0,0,0,0,2,2,0,0);

		//////////////////////////////////////////////////////////////////////////
		// ����ᴩ�׵Ĺᴩ����
		//////////////////////////////////////////////////////////////////////////
		RFRAME ChangeFrame;
		mathInitRFrame(ChangeFrame);
		// step1:�Ƚ���������ϵ��X����תRotAng�Ƕ�
		double dRotAng = -PI+pHParam->m_dExRotAng;
		mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,dRotAng,ChangeFrame);
		// step2: ����һ����ת�������ϵ��Z����תThroughAng�Ƕȣ���ʱX�᷽��Ϊ�ᴩ����
		dRotAng = -PI+pHParam->m_dThroughAng;
		mathRotateRFrame(ChangeFrame.O,ChangeFrame.Z,dRotAng,ChangeFrame);
		VEC3D throughVec = {1,0,0};
		mathTransLocalPnt3D(ChangeFrame,throughVec,throughVec);
		mathUniVec(throughVec);
		for (int i=0; i<3; i++)
		{
			pHParam->m_dThroughVec[i] = throughVec[i];
		}
		//////////////////////////////////////////////////////////////////////////
	}
	//////////////////////////////////////////////////////////////////////////
	iSwModel->SetAddToDB(VARIANT_FALSE);
	iSwModel->SetDisplayWhenAdded(VARIANT_TRUE);
	//////////////////////////////////////////////////////////////////////////

	iSwModel = NULL;
	return;
}

// ���·���ı任����ϵ�Ƿ���
BOOL CEliteSoftWare::CheckTransFrame(RFRAME& local_frame, LPathCombList &PathCombList)
{
	POSITION nPCombPos = PathCombList.GetHeadPosition();
	while(nPCombPos)
	{
		CPathComb* pPathComb = PathCombList.GetNext(nPCombPos);
		if (NULL == pPathComb)
			continue;

		POSITION pathPos = pPathComb->m_PathList.GetHeadPosition();
		while(pathPos)
		{
			CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pathPos);
			if (NULL == pMovePath)
				continue;

			POSITION nodePos = pMovePath->m_PathNodeList.GetHeadPosition();
			while(nodePos)
			{
				CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
				if (NULL == pPathNode)
					continue;
				PNT3D local_pnt_X;
				mathTransWorldPnt3D(local_frame,pPathNode->m_OrgCutPosition,local_pnt_X);
				if (local_pnt_X[0] < -0.00000001) // ����任��������XֵС��0��˵���������������ϵ��X������
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

// ��·������ÿ��·���ĵ��������������ϵת�����ֲ�����ϵ��
void CEliteSoftWare::TransWorldPathComb(RFRAME& local_frame, LPathCombList &PathCombList)
{
	POSITION nPCombPos = PathCombList.GetHeadPosition();
	while(nPCombPos)
	{
		CPathComb* pPathComb = PathCombList.GetNext(nPCombPos);
		if (NULL == pPathComb)
			continue;

		POSITION pathPos = pPathComb->m_PathList.GetHeadPosition();
		while(pathPos)
		{
			CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pathPos);
			if (NULL == pMovePath)
				continue;
			TransWorldPath(local_frame,pMovePath);
		}
	}
}

// ��·������ÿ��·���ĵ��������������ϵת�����ֲ�����ϵ��
void CEliteSoftWare::TransLocalPathComb(RFRAME& local_frame, LPathCombList &PathCombList)
{
	POSITION nPCombPos = PathCombList.GetHeadPosition();
	while(nPCombPos)
	{
		CPathComb* pPathComb = PathCombList.GetNext(nPCombPos);
		if (NULL == pPathComb)
			continue;

		POSITION pathPos = pPathComb->m_PathList.GetHeadPosition();
		while(pathPos)
		{
			CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pathPos);
			if (NULL == pMovePath)
				continue;
			TransLocalPath(local_frame, pMovePath);
		}
	}
}

// ��һ��·���ĵ��������������ϵת�����ֲ�����ϵ��
void CEliteSoftWare::TransWorldPath(RFRAME& local_frame, CMovePath* pMovePath)
{
 	if (NULL == pMovePath)
 		return;

	POSITION nodePos = pMovePath->m_PathNodeList.GetHeadPosition();
	while(nodePos)
	{
		CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
		if (NULL == pPathNode)
			continue;
		mathTransWorldPnt3D(local_frame,pPathNode->m_OrgCutPosition,pPathNode->m_OrgCutPosition);
		mathTransWorldVec3D(local_frame,pPathNode->m_OrgDirection,pPathNode->m_OrgDirection);

		mathTransWorldPnt3D(local_frame,pPathNode->m_OffsetPosition,pPathNode->m_OffsetPosition);
		mathTransWorldVec3D(local_frame,pPathNode->m_OffsetDirection,pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OrgDirection);
	}
}

// ��һ��·�����������������ϵת�����ֲ�����ϵ��
void CEliteSoftWare::TransLocalPath(RFRAME& local_frame, CMovePath* pMovePath)
{
	if (NULL == pMovePath)
		return;

	POSITION nodePos = pMovePath->m_PathNodeList.GetHeadPosition();
	while(nodePos)
	{
		CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
		if (NULL == pPathNode)
			continue;
		mathTransLocalPnt3D(local_frame,pPathNode->m_OrgCutPosition,pPathNode->m_OrgCutPosition);
		mathTransLocalVec3D(local_frame,pPathNode->m_OrgDirection,pPathNode->m_OrgDirection);

		mathTransLocalPnt3D(local_frame,pPathNode->m_OffsetPosition,pPathNode->m_OffsetPosition);
		mathTransLocalVec3D(local_frame,pPathNode->m_OffsetDirection,pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OrgDirection);
	}
}

CHCombParam* CEliteSoftWare::GetCurHoleParam()
{
	BSTR sCurrentFileName = GetCurrentFile();
	CHCombParam* pHParamComb = NULL;
	BOOL bFind = FALSE;
	POSITION pos = m_LHoleParamList.GetHeadPosition();
	while(pos)
	{
		pHParamComb = m_LHoleParamList.GetNext(pos);
		if(NULL == pHParamComb)
			continue;
		_bstr_t filename(sCurrentFileName), paramname(pHParamComb->m_sParamName);
		if (filename != paramname)
			continue;
		bFind = TRUE;
		break;
	}
	if(!bFind)
		return NULL;
	return pHParamComb;
}

CPathCombList* CEliteSoftWare::GetCurPComb()
{
	BSTR sCurrentFileName = GetCurrentFile();
	CPathCombList* pPCombList = NULL;
	BOOL bFind = FALSE;
	POSITION entPos = m_LPathEntityList.GetHeadPosition();
	while(entPos)
	{
		pPCombList = m_LPathEntityList.GetNext(entPos);
		if(NULL == pPCombList)
			continue;
		_bstr_t filename(sCurrentFileName), pathname(pPCombList->m_sPathCombName);
		if (filename != pathname)
			continue;
		bFind = TRUE;
	}
	if (!bFind)
		return NULL;

	return pPCombList;
}

// �����������ϵ
void CEliteSoftWare::SetExportRFrame(RFRAME& _export_rframe)
{
	CHCombParam* pHCombParam = GetCurHoleParam();
	if (NULL == pHCombParam)
		return;
	mathInitRFrame(_export_rframe);
	_export_rframe.O[2] = -pHCombParam->m_dTubeDia*0.5;
}

// ���������ĵ�����·��
void CEliteSoftWare::DrawPathCombs(CPathCombList* pPathCombs)
{
	if (NULL == pPathCombs)
		return;
	POSITION testPos = pPathCombs->m_LPathCombList.GetHeadPosition();
	while(testPos)
	{
		CPathComb* pPathComb = pPathCombs->m_LPathCombList.GetNext(testPos);
		if (NULL == pPathComb)
			continue;
		DrawPathComb(pPathComb);
    }
	return;
}
void CEliteSoftWare::DrawMovePath(CMovePath* pMovePath, BOOL bHolePrecut)
{
	if (NULL == pMovePath)
		return;
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);
	CComPtr<ISketchManager> iswSketchManger;
	iSwModel->get_SketchManager(&iswSketchManger);
	CComPtr<ISketchSegment> iSegment; 	

	iSwModel->Insert3DSketch2(VARIANT_TRUE);
	iSwModel->SetAddToDB(VARIANT_TRUE);
	POSITION nodepos = pMovePath->m_PathNodeList.GetHeadPosition();
	while(nodepos)
	{
		CPathNode* pNode = pMovePath->m_PathNodeList.GetNext(nodepos);
		if (pNode == NULL)
			continue;

		iSegment = NULL;
		PNT3D dDrawEnd;
		pNode->GetDrawEnd(1,dDrawEnd);
		iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
			dDrawEnd[0],dDrawEnd[1],dDrawEnd[2],&iSegment);
		if (bHolePrecut)
		{
			iSegment = NULL;
			pNode->GetDrawEnd(0,dDrawEnd);
			iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
				dDrawEnd[0],dDrawEnd[1],dDrawEnd[2],&iSegment);
		}

		CPathNode* pNextNode = NULL;
		if (NULL == nodepos)
			pNextNode = pMovePath->m_PathNodeList.GetHead();
		else
			pNextNode = pMovePath->m_PathNodeList.GetAt(nodepos);

		if (NULL == pNextNode)
			continue;
		iSegment = NULL;
		iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
			pNextNode->m_OffsetPosition[0],pNextNode->m_OffsetPosition[1],pNextNode->m_OffsetPosition[2],&iSegment);
		if (bHolePrecut)
		{
			iSegment = NULL;
			iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
				pNextNode->m_OrgCutPosition[0],pNextNode->m_OrgCutPosition[1],pNextNode->m_OrgCutPosition[2],&iSegment);
		}
	}

	iSwModel->SetAddToDB(VARIANT_FALSE);
	iSwModel->Insert3DSketch2(VARIANT_FALSE);
}
// ����һ��3D��ͼ��·��
void CEliteSoftWare::DrawPathComb(CPathComb* pPathComb)
{
	if (NULL == pPathComb)
		return;
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);
	CComPtr<ISketchManager> iswSketchManger;
	iSwModel->get_SketchManager(&iswSketchManger);
	CComPtr<ISketchSegment> iSegment; 	

	iSwModel->Insert3DSketch2(VARIANT_TRUE);
	iSwModel->SetAddToDB(VARIANT_TRUE);
	POSITION pos = pPathComb->m_PathList.GetHeadPosition();
	while(pos)
	{
		CMovePath* pMovePath = (CMovePath*)pPathComb->m_PathList.GetNext(pos);
		if (NULL == pMovePath)
			continue;
		POSITION nodepos = pMovePath->m_PathNodeList.GetHeadPosition();
		while(nodepos)
		{
			CPathNode* pNode = pMovePath->m_PathNodeList.GetNext(nodepos);
			if (pNode == NULL)
				continue;

			iSegment = NULL;
			PNT3D dDrawEnd;
			pNode->GetDrawEnd(1,dDrawEnd);
			iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
										dDrawEnd[0],dDrawEnd[1],dDrawEnd[2],&iSegment);
			if (pMovePath->m_bHolePrecut)
			{
				iSegment = NULL;
				pNode->GetDrawEnd(0,dDrawEnd);
				iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
					dDrawEnd[0],dDrawEnd[1],dDrawEnd[2],&iSegment);
			}

			CPathNode* pNextNode = NULL;
			if (NULL == nodepos)
				pNextNode = pMovePath->m_PathNodeList.GetHead();
			else
				pNextNode = pMovePath->m_PathNodeList.GetAt(nodepos);

			if (NULL == pNextNode)
				continue;
			iSegment = NULL;
			iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
				pNextNode->m_OffsetPosition[0],pNextNode->m_OffsetPosition[1],pNextNode->m_OffsetPosition[2],&iSegment);
			if (pMovePath->m_bHolePrecut)
			{
				iSegment = NULL;
				iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
					pNextNode->m_OrgCutPosition[0],pNextNode->m_OrgCutPosition[1],pNextNode->m_OrgCutPosition[2],&iSegment);
			}
		}
	}	
	iSwModel->SetAddToDB(VARIANT_FALSE);
	iSwModel->Insert3DSketch2(VARIANT_FALSE);

	// �����޸�·����ɫ
	// 	VARIANT_BOOL bret;
	// 	long color = 100;
	// 	CComPtr<IModelDocExtension> swExt;
	// 	iSwModel->get_Extension(&swExt);
	// 	iSwModel->SelectedFeatureProperties(color,0,0,0,0,0,0,VARIANT_FALSE,VARIANT_FALSE,L"3D��ͼ1",&bret);
	// 	swExt->SelectByID2(L"3D��ͼ1",L"SKETCH",0,0,0,VARIANT_FALSE,0,NULL,swSelectOptionDefault,&bret);

	//		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);

	//////////////////////////////////////////////////////////////////////////
}

// ��������ϵ
void CEliteSoftWare::DrawRFrame(RFRAME& rframe, double dAixLength)
{
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);
	// �����������ϵ
	PNT3D stX,endX, stY,endY, stZ,endZ;
	for (int i=0; i<3; i++)
	{
		endX[i] = rframe.O[i]+dAixLength*rframe.X[i];
		endY[i] = rframe.O[i]+dAixLength*rframe.Y[i];
		endZ[i] = rframe.O[i]+dAixLength*rframe.Z[i];
		stX[i] = stY[i] = stZ[i] = rframe.O[i];
	}
	VARIANT_BOOL bRet;
	iSwModel->InsertCurveFileBegin();
	iSwModel->InsertCurveFilePoint(stX[0],stX[1],stX[2],&bRet);
	iSwModel->InsertCurveFilePoint(endX[0],endX[1],endX[2],&bRet);
	iSwModel->InsertCurveFileEnd(&bRet);
	iSwModel->InsertCurveFileBegin();
	iSwModel->InsertCurveFilePoint(stY[0],stY[1],stY[2],&bRet);
	iSwModel->InsertCurveFilePoint(endY[0],endY[1],endY[2],&bRet);
	iSwModel->InsertCurveFileEnd(&bRet);
	iSwModel->InsertCurveFileBegin();
	iSwModel->InsertCurveFilePoint(stZ[0],stZ[1],stZ[2],&bRet);
	iSwModel->InsertCurveFilePoint(endZ[0],endZ[1],endZ[2],&bRet);
	iSwModel->InsertCurveFileEnd(&bRet);
}


void CEliteSoftWare::SetPCombExOrder(CPathCombList* pPathCombs, int nOrder)
{
	if (NULL == pPathCombs)
	{
		return;
	}
	if (nOrder != EXPORT_XORDER && nOrder != EXPORT_ANGORDER && nOrder != EXPORT_CUSTOMORDER)
	{
		return;
	}
	POSITION combPos = pPathCombs->m_LPathCombList.GetHeadPosition();		
	while(combPos)
	{
		CPathComb* pPComb = pPathCombs->m_LPathCombList.GetNext(combPos);
		if (NULL == pPComb)
			continue;
		LPathList tmpPaths;
		tmpPaths.RemoveAll();
		while(pPComb->m_PathList.GetCount()>0)
		{
			POSITION pathPos = pPComb->m_PathList.GetHeadPosition();
			CMovePath* pTmpPath = pPComb->m_PathList.GetAt(pathPos);
			if (NULL == pTmpPath)
				continue;

			while(pathPos)
			{
				CMovePath* pPath = pPComb->m_PathList.GetNext(pathPos);
				if(NULL == pPath || pPath == pTmpPath)
					continue;
				if (EXPORT_XORDER == nOrder)
				{
					if (abs(pPath->GetOffsetX()-pTmpPath->GetOffsetX())<0.0001)
					{// ���Xֵ��ȣ���ѡ����ת�ǽ�С���ȼӹ�
						if (abs(pPath->GetOrgRotAng()-pTmpPath->GetOrgRotAng())<0.0001)
						{// �����ת����ͬ�����û�����Ⱥ�˳��ӹ�
							if (pPath->GetAddOrder()<pTmpPath->GetAddOrder())
								pTmpPath = pPath;
						}
						else if(pPath->GetOrgRotAng()<pTmpPath->GetOrgRotAng())
							pTmpPath = pPath;
						continue;
					}
					else if(pPath->GetOffsetX()<pTmpPath->GetOffsetX())
						pTmpPath = pPath;
					continue;
				}
				else if(EXPORT_ANGORDER == nOrder)
				{
					if (abs(pPath->GetOrgRotAng()-pTmpPath->GetOrgRotAng())<0.0001)
					{// �����ת����ͬ����ѡ����Xƫ�ƽ�С���ȼӹ�
						if (abs(pPath->GetOffsetX()-pTmpPath->GetOffsetX())<0.0001)
						{// ���Xֵ��ȣ����û�����Ⱥ�˳��ӹ�
							if (pPath->GetAddOrder()<pTmpPath->GetAddOrder())
								pTmpPath = pPath;
						}
						else if(pPath->GetOffsetX()<pTmpPath->GetOffsetX())
							pTmpPath = pPath;
						continue;
					}
					else if(pPath->GetOrgRotAng()<pTmpPath->GetOrgRotAng())
						pTmpPath = pPath;
					continue;
				}
				else if(EXPORT_CUSTOMORDER == nOrder)
				{
					if (pPath->GetAddOrder()<pTmpPath->GetAddOrder())
								pTmpPath = pPath;
					continue;
				}
				else
					return;
						
			}
			tmpPaths.AddTail(pTmpPath);
			pPComb->m_PathList.RemoveAt(pPComb->m_PathList.Find(pTmpPath));
		}
		POSITION tmpPos = tmpPaths.GetHeadPosition();
		while(tmpPos)
		{
			CMovePath* pPath = tmpPaths.GetNext(tmpPos);
			if(NULL == pPath)
				continue;
			pPComb->m_PathList.AddTail(pPath);
		}
		tmpPaths.RemoveAll();
	}
}

// ·�����
void CEliteSoftWare::ExportPathToTXT()
{
	long docType = -1;
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);
	if(iSwModel != NULL)
	{
		iSwModel->GetType(&docType);
	}
	if (docType != swDocPART && docType != swDocASSEMBLY)
	{
		AfxMessageBox(_T("�򿪵Ĳ������"),MB_OK|MB_ICONINFORMATION);
		iSwModel = NULL;
		return ;
	}

	// ��ȡ��ǰ�ĵ�·������
	CPathCombList* pPathCombList = GetCurPComb();
	if(NULL == pPathCombList)
	{
		AfxMessageBox(_T("���ȼ����и�·����"));
		return;
	}

	// ����·�����˳��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CExportDlg dlg;
	dlg.m_dFixAng = m_dFixAng;
	dlg.m_nExportOrder = m_nExportOrder;
	if (IDOK != dlg.DoModal())
		return;
	m_nExportOrder = dlg.m_nExportOrder ;
	m_dFixAng = dlg.m_dFixAng;

	// ����·������ֹ���·��ʱ���ı䵱ǰ�ĵ��е�ԭʼ·�����ݡ�
	CPathCombList* pCpyPCombs = pPathCombList->CopySelf();

	// �����������ϵ
	RFRAME exportFrame;
	SetExportRFrame(exportFrame);

	//����ǰ·������任���������ϵ�С�
	TransWorldPathComb(exportFrame, pCpyPCombs->m_LPathCombList);

	//����·�����˳��
	SetPCombExOrder(pCpyPCombs, m_nExportOrder);

	// ·������ϵ�任
	//////////////////////////////////////////////////////////////////////////
	POSITION pcPos = pCpyPCombs->m_LPathCombList.GetHeadPosition();
	while(pcPos)
	{
		CPathComb* pPathComb = pCpyPCombs->m_LPathCombList.GetNext(pcPos);
		if (NULL == pPathComb)
			continue;

		POSITION pos = pPathComb->m_PathList.GetHeadPosition();
		while(pos)
		{
			CMovePath* pMovePath = (CMovePath*)pPathComb->m_PathList.GetNext(pos);
			if (NULL == pMovePath)
				continue;

			//����ˮƽ��Xƫ�ƺ�ת���Ƕȣ������Ӧ������
			double dOffsetX = pMovePath->GetOffsetX(); //���룺X����ƫ�ƾ���
			double dOffsetZ = pMovePath->GetTubeR();   // Z��ƫ��Ϊ��ܵİ뾶
			double dRotAng = PI-pMovePath->GetExRotAng();//���룺����ת�ĽǶ�
			VEC3D RFrameOffset;
			RFrameOffset[0] = dOffsetX;
			RFrameOffset[1] = 0.;
			RFrameOffset[2] = dOffsetZ;

			//�����µ�����ϵ������ƽ����ת��
			RFRAME ChangeFrame;
			mathInitRFrame(ChangeFrame);
			//step0:��X��ƽ���µ�����ϵ
			mathMoveRFrame(RFrameOffset,ChangeFrame);
			//step1:��·��������ӻ�������ϵת������X��ƽ�ƺ������ϵ��
			TransWorldPath(ChangeFrame, pMovePath);
			//step2:��ƽ�ƺ������ϵ��תdRotAng��·���������ת��������ת������겻�����ı䡣
			double tmpAng = mathASin(pMovePath->GetHParam()->m_dOffsetZ/dOffsetZ);
			double dRealRotAng = tmpAng-m_dFixAng*PI/180.+dRotAng;
			mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,dRealRotAng,ChangeFrame);
			//step3:����ת�������ϵ�е�·��������ת������������ϵ�С�
			TransLocalPath(ChangeFrame,pMovePath);
			//step4:����������ϵ�е�����ת����ƽ��X�������ϵ�С�
			VEC3D offsetX= {dOffsetX,0.,0.};
			mathInitRFrame(ChangeFrame);
			mathMoveRFrame(offsetX,ChangeFrame);
			TransWorldPath(ChangeFrame, pMovePath);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// ����·�����������ϵ(������)
	//////////////////////////////////////////////////////////////////////////
	 DrawPathCombs(pCpyPCombs);
	// DrawRFrame(exportFrame);
	// return;
	//////////////////////////////////////////////////////////////////////////

	BOOL isOpen = FALSE;                                 //�Ƿ��(����Ϊ����)  
	CString defaultDir = L"C:\\Users\\QQS\\Desktop\\";   //Ĭ�ϴ򿪵��ļ�·��  
	CString sCurrentFileName = GetCurrentFile();

	CString sExt;// ��ȡ��׺��
	sExt = sCurrentFileName.Right(7);
	sExt.MakeUpper();
	if (sExt == ".SLDPRT")
	{
		sCurrentFileName = sCurrentFileName.Left(sCurrentFileName.GetLength()-7); //ȥ����׺��
	}
	CString fileName = sCurrentFileName + "-·��" + ".txt";                   //Ĭ�ϴ򿪵��ļ���  
	CString filter = L"�ļ� (*.txt)|*.txt||";            //�ļ����ǵ�����  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);  
	openFileDlg.GetOFN().lpstrInitialDir = L"C:\\Users\\QQS\\Desktop\\";  
	INT_PTR result = openFileDlg.DoModal();  
	CString filePath = L"";  
	if(result == IDOK) 
	{  
		filePath = openFileDlg.GetPathName();  
	}  
	else
	{
		if (NULL != pCpyPCombs)
		{
			delete pCpyPCombs;
			pCpyPCombs = NULL;
		}
		return;
	}

	ofstream fout(filePath);
	int nPathCombId = 1;
	int nPathCombCount = (int)pCpyPCombs->m_LPathCombList.GetCount();
	std::string str = CStringA(sCurrentFileName); 
	fout<<"�ļ�"<<"\""<<str.c_str()<<"\""<<"����"<<nPathCombCount<<"���и�·��"<<"\t\n\n";
	CString exOrder;
	if (EXPORT_XORDER == m_nExportOrder)
	{
		exOrder = "X��˳��ӹ���";
	}
	else if (EXPORT_ANGORDER == m_nExportOrder)
	{
		exOrder = "��ת��˳��ӹ���";
	}
	else if (EXPORT_CUSTOMORDER == m_nExportOrder)
	{
		exOrder = "�����˳��ӹ���";
	}
	str = CStringA(exOrder);	
	fout<<"ÿ���и�·�����˳��Ϊ��"<<str.c_str()<<"\t\n\n";
	POSITION nPCombPos = pCpyPCombs->m_LPathCombList.GetHeadPosition();
	while(nPCombPos)
	{
		CPathComb* pPathComb = pCpyPCombs->m_LPathCombList.GetNext(nPCombPos);
		if (NULL == pPathComb)
		{
			fout<<"��"<<nPathCombId<<"���и�·��Ϊ�ա�"<<"\t\n\n";
			nPathCombId++;
			continue;
		}
		fout<<"��"<<nPathCombId<<"���и�·����"<<"\t\n";
		nPathCombId++;

		POSITION pathPos = pPathComb->m_PathList.GetHeadPosition();
		int nPathId = 1;
		int nPathCount = (int)pPathComb->m_PathList.GetCount();
		fout<<"��·������"<<nPathCount<<"���׵��и�·��"<<"\t\n";
		while(pathPos)
		{
			CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pathPos);
			if (NULL == pMovePath)
			{
				fout<<"��"<<nPathId<<"���׵��и�·����Ϊ�գ�"<<"\t\n\n";
				nPathId++;
				continue;
			}

			POSITION nodePos = pMovePath->m_PathNodeList.GetHeadPosition();
			if (pMovePath->m_PathNodeList.GetCount() == 0)
			{
				fout<<"��"<<nPathId<<"���׵��и�·����Ϊ�գ�"<<"\t\n\n";
				nPathId++;
				continue;
			}

			fout<<"��"<<nPathId<<"���׵�X�����Ϊ��"<<pMovePath->GetOffsetX()*1000.<<";"
				<<"����ת�Ƕ�Ϊ��"<<pMovePath->GetOrgRotAng()-m_dFixAng<<";"<<"\t\n";
			fout<<"���и�·���㼰����"<<"\t\n";
			nPathId++;
			if (pMovePath->m_bHolePrecut)
			{
				fout<<"���и�·����"<<"\t\n";
			}
			while(nodePos && pMovePath->m_bHolePrecut)
			{
				CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
				if (NULL == pPathNode)
					continue;

				fout<<pPathNode->m_OrgCutPosition[0]*1000<<" "<<pPathNode->m_OrgCutPosition[1]*1000<<" "<<pPathNode->m_OrgCutPosition[2]*1000<<" "
					<<pPathNode->m_OrgDirection[0]<<" "<<pPathNode->m_OrgDirection[1]<<" "<<pPathNode->m_OrgDirection[2]<<" "<<"\t\n";
			}

			if (pMovePath->m_bHolePrecut)
			{
				fout<<"�¿��и�·����"<<"\t\n";
			}
			nodePos = pMovePath->m_PathNodeList.GetHeadPosition();
			while(nodePos)
			{
				CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
				if (NULL == pPathNode)
					continue;
				fout<<pPathNode->m_OffsetPosition[0]*1000<<" "<<pPathNode->m_OffsetPosition[1]*1000<<" "<<pPathNode->m_OffsetPosition[2]*1000<<" "
					<<pPathNode->m_OffsetDirection[0]<<" "<<pPathNode->m_OffsetDirection[1]<<" "<<pPathNode->m_OffsetDirection[2]<<" "<<"\t\n";
			}
		}
	}
	
	if (NULL != pCpyPCombs)
	{
		delete pCpyPCombs;
		pCpyPCombs = NULL;
	}
	
	AfxMessageBox(_T("·�������ɡ�"));
}

STDMETHODIMP CEliteSoftWare::ToolbarEnablePathExport(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

STDMETHODIMP CEliteSoftWare::ToolbarEnableBuildTube(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

BSTR CEliteSoftWare::GetCurrentFile()
{	
	BSTR ret = SysAllocString(L"No File Currently Open");
	CComPtr<IModelDoc2> iSwModel;

	iSwApp->get_IActiveDoc2(&iSwModel);
	if(iSwModel != NULL)
	{
		iSwModel->GetTitle(&ret);
	}

	return ret;
}

STDMETHODIMP CEliteSoftWare::GeneratePath(void)
{
	// TODO: Add your implementation code here
	CHCombParam* pCombParam = GetCurHoleParam();
	if (NULL == pCombParam || pCombParam->m_bBulidTube )
	{
		AfxMessageBox(_T("���ȴ����ܼ��Ϳ�ģ�͡�"));
		return S_OK;
	}
	if (pCombParam->m_LHParamList.GetCount()<1)
	{
		AfxMessageBox(_T("��ǰ�ܼ�ģ��û�и�ף�������ӿ����ݡ�"));
		return S_OK;
	}

	if (userPropertyPage != NULL)
		userPropertyPage->Show();

	CComPtr<IModelDoc2> iSwModel;

	iSwApp->get_IActiveDoc2(&iSwModel);
	CComPtr<IModelDocExtension> iSwExtension;
	iSwModel->get_Extension(&iSwExtension);
	BSTR name;
	iSwExtension->GetActivePropertyManagerPage(&name);
	IPropertyManagerPage* iSwPmp = NULL;
	iSwModel->GetPropertyManagerPage(0,name,NULL,&iSwPmp);

	return S_OK;
}

STDMETHODIMP CEliteSoftWare::EnableGeneratePath(long* status)
{
	*status = 0;
	if (userPropertyPage != NULL)
	{
		CComPtr<IModelDoc2> modDoc;
		iSwApp->get_IActiveDoc2(&modDoc);
		if (modDoc != NULL)
			*status = 1;
	}
	return S_OK;
}
