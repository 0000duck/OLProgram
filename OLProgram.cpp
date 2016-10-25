// OLProgram.cpp : Implementation of COLProgram

#include "stdafx.h"
#include "OLProgram.h"
#include "SwDocument.h"
#include <string>
#include <list>
#include "UserPropertyManagerPage.h"
#include "BitmapHandler.h"
#include "fstream"
#include "fstream"
#include "ExportDlg.h"
#include "BuildTubeDlg.h"
using namespace std;



extern void TrimZeroForCSring(CString& str)
{
	int pos = str.Find('.');
	if (-1 == pos)
		return;
	int nLength = str.GetLength();
	for (int i=0; i<nLength; i++)
	{
		CString tmp = str.Right(1);
		if (tmp == '0')
		{
			str = str.Left(str.GetLength()-1);
		}
		else if(tmp == '.')
		{
			str = str.Left(str.GetLength()-1);
			return;
		}
	}
}

double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir)
{   /* direction is assumed to be unit vector */
	double t ;
	VEC3D v ;

	t = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1]+(p[2]-begin[2])*dir[2] ;
	v[0] = begin[0]+dir[0]*t-p[0] ;
	v[1] = begin[1]+dir[1]*t-p[1] ;
	v[2] = begin[2]+dir[2]*t-p[2] ;

	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
}


double mathDis3D(PNT3D pt1, PNT3D pt2)
{
	return sqrt((pt1[0]-pt2[0])*(pt1[0]-pt2[0])+(pt1[1]-pt2[1])*(pt1[1]-pt2[1])+(pt1[2]-pt2[2])*(pt1[2]-pt2[2]));
};
void mathRotPnt(VEC3D axis,	PNT3D pivot, double angle, PNT3D p, PNT3D rot_p);
void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec);

void mathRotVec(VEC3D axis,
	PNT3D pivot, 
	double angle,
	VEC3D v,
	VEC3D rot_v)
{
	int k ;
	PNT3D p ;

	for( k = 0 ; k < 3 ; k++ )
		p[k] = pivot[k]+v[k] ;
	mathRotPnt(axis, pivot, angle, p, p) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = p[k]-pivot[k] ;
}

void mathPrjPntLin(PNT3D p, PNT3D begin, VEC3D dir, PNT3D prj_p)
{
	double d ;

	d = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1]+(p[2]-begin[2])*dir[2] ;
	prj_p[0] = begin[0]+d*dir[0] ;
	prj_p[1] = begin[1]+d*dir[1] ;
	prj_p[2] = begin[2]+d*dir[2] ;

	return ;
}

// �������ᾭ��p�����ת��axis��ת
void mathRotPnt(VEC3D axis,
	PNT3D pivot,
	double angle,
	PNT3D p,
	PNT3D rot_p)
{
	int k ;
	double cosRotateAngle, sinRotateAngle ;
	VEC3D e1, e2 ;
	PNT3D temp ;

	mathPrjPntLin(p, pivot, axis, temp) ; // nt modify 2002/6/3: mathPrjPntLinEx
	for( k = 0 ; k < 3 ; k++ )
		e1[k] = p[k]-temp[k] ;
	mathVProduct(axis, e1, e2) ;
	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_p[k] = temp[k]+(cosRotateAngle*e1[k]+sinRotateAngle*e2[k]) ;

	return ;
}

// ������ϵ���ᾭ��p�����ת��axis��ת
void mathRotateRFrame(PNT3D p, VEC3D axis, double angle, RFRAME& pLF)
{
	mathRotPnt(axis, p, angle, pLF.O, pLF.O) ;
	mathRotVec(axis, p, angle, pLF.X, pLF.X) ;
	mathRotVec(axis, p, angle, pLF.Y, pLF.Y) ;
	mathRotVec(axis, p, angle, pLF.Z, pLF.Z) ;

	return ;
}

// ��ʼ������ϵ
void mathInitRFrame(RFRAME& pLF)
{
	pLF.O[0] = pLF.O[1] = pLF.O[2] = 0.0 ;
	pLF.X[0] = 1.0 ;
	pLF.X[1] = pLF.X[2] = 0.0 ;
	pLF.Y[1] = 1.0 ;
	pLF.Y[0] = pLF.Y[2] = 0.0 ;
	pLF.Z[2] = 1.0 ;
	pLF.Z[1] = pLF.Z[0] = 0.0 ;
//	pLF.scale = 1.0 ;

	return ;
}

// ƽ������ϵ
void mathMoveRFrame(VEC3D translateVector, RFRAME& pLF)
{
	pLF.O[0] += translateVector[0] ;
	pLF.O[1] += translateVector[1] ;
	pLF.O[2] += translateVector[2] ;

	return ;
}

// ��֪����������ϵ�µ�����㣬�����ھֲ�����ϵ�е�����ֵ
void mathTransWorldPnt3D(RFRAME& local_frame,
	PNT3D world_point,
	PNT3D local_point)
{
	VEC3D trans_point ;

	trans_point[0] = world_point[0]-local_frame.O[0] ;
	trans_point[1] = world_point[1]-local_frame.O[1] ;
	trans_point[2] = world_point[2]-local_frame.O[2] ;

	local_point[0] = trans_point[0]*local_frame.X[0] +
		trans_point[1]*local_frame.X[1] +
		trans_point[2]*local_frame.X[2] ;
	local_point[1] = trans_point[0]*local_frame.Y[0] +
		trans_point[1]*local_frame.Y[1] +
		trans_point[2]*local_frame.Y[2] ;
	local_point[2] = trans_point[0]*local_frame.Z[0] +
		trans_point[1]*local_frame.Z[1] +
		trans_point[2]*local_frame.Z[2] ;
}

// ��֪���ھֲ�����ϵ�µ�����㣬��������������ϵ�е�����ֵ
void mathTransLocalPnt3D(RFRAME& local_frame,
	PNT3D local_point,
	PNT3D world_point)
{
	int k ;
	PNT3D p ;

	for( k = 0 ; k < 3 ; k++ )
		p[k] = local_point[0]*local_frame.X[k] +
		local_point[1]*local_frame.Y[k] +
		local_point[2]*local_frame.Z[k] +
		local_frame.O[k] ;
	world_point[0] = p[0] ;
	world_point[1] = p[1] ;
	world_point[2] = p[2] ;
}

// �������
void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec)
{
	VEC3D vector ;

	vector[0] = v1[1]*v2[2]-v1[2]*v2[1] ;
	vector[1] = v1[2]*v2[0]-v1[0]*v2[2] ;
	vector[2] = v1[0]*v2[1]-v1[1]*v2[0] ;

	rtvec[0] = vector[0] ;
	rtvec[1] = vector[1] ;
	rtvec[2] = vector[2] ;

	return ;
}

// ��λ������
BOOL mathUniVec(VEC3D v, double min_len = 0.0000001)
{
	double len ;

	len = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
	if( len < min_len )
	{
		v[0] = 0.0 ;
		v[1] = 0.0 ;
		v[2] = 0.0 ;
		return FALSE ;
	}
	else
	{
		v[0] /= len ;
		v[1] /= len ;
		v[2] /= len ;
		return TRUE ;
	}
}

// ������e1��e2��תangle�Ƕ�
void mathRotVecXY(VEC3D e1, VEC3D e2, double angle, VEC3D rot_v)
{
	int k ;
	double cosRotateAngle, sinRotateAngle ;

	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;
	for( k = 0 ; k < 3 ; k++ )
		rot_v[k] = cosRotateAngle*e1[k]+sinRotateAngle*e2[k] ;
}

double mathACos(double cosAlfa)
{
	if( cosAlfa > 0.999999999999 )
		return 0.0 ;
	else
		if( cosAlfa < -0.999999999999 )
			return PI ;
		else
			return acos(cosAlfa) ;
}

/*  
 *  ��������3D�����ļн�: 0.0 <= angle <= PI1
 */
double mathGetAngle(VEC3D v1, VEC3D v2, double min_len)
{
    double d1, d2 ;

	d1 = sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]) ;
	d2 = sqrt(v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2]) ;
	if( d1 < min_len ||
		d2 < min_len )
		return -1.0 ;
	else
	    return mathACos((v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])/(d1*d2)) ;
}

// CPathComb
CPathComb::CPathComb()
{
	m_bTwiceCut = TRUE;
	m_PathList.RemoveAll();
	m_nRefID = 0;
}
CPathComb::~CPathComb()
{
	Release();
}
void CPathComb::Release()
{
	POSITION pos = m_PathList.GetHeadPosition();
	while(pos)
	{
		CMovePath* pMovePath = m_PathList.GetNext(pos);
		if (NULL == pMovePath)
			continue;
		delete pMovePath;
		pMovePath = NULL;
	}
	m_PathList.RemoveAll();
}

// COLProgram
COLProgram::~COLProgram()
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

void COLProgram::AddCommandManager()
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
// 	callback.LoadString(IDS_TOOLBAR_CALLBACK0);
// 	enable.LoadString(IDS_TOOLBAR_ENABLE0);
	//tip.LoadString(IDS_TOOLBAR_TIP_PATHEXPORT);
	//hint.LoadString(IDS_TOOLBAR_HINT_PATHEXPORT);
	hret = icmdGroup->AddCommandItem2(L"�����ܼ�", -1, L"ͨ������������Զ������ܼ�ģ�͡�", L"�����ܼ�", 0, L"ToolbarBuildTube", L"ToolbarEnableBuildTube", MAIN_ITEM_ID1, menuToolbarOption, &cmdIndex0);

// 	callback.LoadString(IDS_TOOLBAR_PMPCALLBACK);
// 	enable.LoadString(IDS_TOOLBAR_PMPENABLE);
	tip.LoadString(IDS_TOOLBAR_PMPTIP_CALPATH);
	hint.LoadString(IDS_TOOLBAR_PMPHINT_CALPATH);
	hret = icmdGroup->AddCommandItem2(tip, -1, hint, tip, 0, L"GeneratePath", L"EnableGeneratePath", MAIN_ITEM_ID2, menuToolbarOption, &cmdIndex1);

	//callback.LoadString(IDS_TOOLBAR_CALLBACK0);
	//enable.LoadString(IDS_TOOLBAR_ENABLE0);
	tip.LoadString(IDS_TOOLBAR_TIP_PATHEXPORT);
	hint.LoadString(IDS_TOOLBAR_HINT_PATHEXPORT);
	hret = icmdGroup->AddCommandItem2(tip, -1, hint, tip, 0, L"ToolbarCallbackPathExport", L"ToolbarEnablePathExport", MAIN_ITEM_ID3, menuToolbarOption, &cmdIndex2);

	

	icmdGroup->put_HasToolbar(true);
	icmdGroup->put_HasMenu(true);
	icmdGroup->Activate(&cmdActivated);

	bool bresult = false;

	CComPtr<IFlyoutGroup> flyGroup; 
	CComBSTR smallIcon;
	CComBSTR largeIcon;
	CComBSTR smallImageList;
	CComBSTR largeImageList;

	icmdGroup->get_SmallMainIcon(&smallIcon);
	icmdGroup->get_LargeMainIcon(&largeIcon);
	icmdGroup->get_SmallIconList(&smallImageList);
	icmdGroup->get_LargeIconList(&largeImageList);

	callback.LoadString(IDS_FLYOUT_CALLBACK);
	enable.LoadString(IDS_FLYOUT_ENABLE0);
	tip.LoadString(IDS_FLYOUT_TIP0);
	hint.LoadString(IDS_FLYOUT_HINT0);
	iCmdMgr->CreateFlyoutGroup(FLYOUT_GROUP_ID, L"Dynamic Flyout", L"Clicking this opens the Flyout menu", L"Click this to open the Flyout menu",
		smallIcon, largeIcon, smallImageList, largeImageList, callback, enable, &flyGroup);

	flyGroup->RemoveAllCommandItems();
	//��������ǲ���tab��
	long flyoutType = (long)swCommandFlyoutStyle_Simple;
	flyGroup->put_FlyoutType(flyoutType);

	for(int i=0; i < numDocumentTypes; i++)
	{
		CComPtr<ICommandTab> cmdTab = NULL;
		long tabCount, docType = docTypes[i];

		iCmdMgr->GetCommandTabCount(docType, &tabCount);

		// check if tab exists
		iCmdMgr->GetCommandTab(docType, title, &cmdTab);

		//If tab exists, but we have ignored the registry info, re-create the tab. Otherwise the ids won't matchup and the tab will be blank 
		if((cmdTab != NULL) && !getDataResult || ignorePrevious)
		{
			VARIANT_BOOL res;
			iCmdMgr->RemoveCommandTab(cmdTab, &res);
			cmdTab = NULL;
		}

		//If cmdTab is null, must be first load (possibly after reset), add the commands to the tabs
		if(cmdTab == NULL)
		{

			// if not, add one
			iCmdMgr->AddCommandTab(docType, title, &cmdTab);

			CComPtr<ICommandTabBox> cmdBox;

			cmdTab->AddCommandTabBox(&cmdBox);

			// create 3 commands on this tab
			long CommandIDCount = 2;


			long CommandIDs[2];
			long TextDisplayStyles[2];

			long cmdID = 0;

			// the 3 command buttons have different text styles
			icmdGroup->get_CommandID(cmdIndex0, &cmdID);      
			CommandIDs[0] = cmdID;
			TextDisplayStyles[0] = swCommandTabButton_TextHorizontal;

			icmdGroup->get_CommandID(cmdIndex1, &cmdID);
			CommandIDs[1] = cmdID;
			TextDisplayStyles[1] = swCommandTabButton_TextHorizontal;

			icmdGroup->get_ToolbarId(&cmdID);
			CommandIDs[2] = cmdID;
			TextDisplayStyles[2] = swCommandTabButton_TextHorizontal;

			VARIANT_BOOL vbResult = VARIANT_FALSE;

			cmdBox->IAddCommands(CommandIDCount, CommandIDs, TextDisplayStyles, &vbResult);

			long CommandIDs1[1];
			long TextDisplayStyles1[1];

			CommandIDCount = 1;

			//flyGroup->get_CmdID(&cmdID);
			CommandIDs1[0] = cmdID;
			TextDisplayStyles1[0] = swCommandTabButton_TextBelow | swCommandTabButton_ActionFlyout;


			CComPtr<ICommandTabBox> cmdBox1;

			cmdTab->AddCommandTabBox(&cmdBox1);

			cmdBox1->IAddCommands(CommandIDCount, CommandIDs1, TextDisplayStyles1, &vbResult);

			CComPtr<ICommandTabBox> cmdBoxNew1; 

			cmdTab->AddSeparator(cmdBox1, cmdID, &cmdBoxNew1);
		}
	}
	//Clean up
	delete [] registryIDs;
}

void COLProgram::RemoveCommandManager()
{
	iBmp->Dispose();
	iBmp->Release();

	VARIANT_BOOL cmdRemoved = VARIANT_FALSE; 
	VARIANT_BOOL rtOnly = VARIANT_TRUE;
	int ret = 0;

	ret = iCmdMgr->RemoveFlyoutGroup(FLYOUT_GROUP_ID, &cmdRemoved);
	ret = iCmdMgr->RemoveCommandGroup(MAIN_CMD_GROUP_ID, &cmdRemoved);
}

bool COLProgram::CompareIDs(long * storedIDs, long storedSize, long * addinIDs, long addinSize)
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

void COLProgram::AddPMP()
{
	CComObject<CUserPropertyManagerPage>::CreateInstance(&userPropertyPage);
	userPropertyPage->AddRef();
	userPropertyPage->Init(this);
}

//Remove the user Property Manager Page
void COLProgram::RemovePMP()
{
	userPropertyPage->Destroy();
	userPropertyPage->Release();
	userPropertyPage = NULL;
}

STDMETHODIMP COLProgram::OnDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called after a document is opened
STDMETHODIMP COLProgram::OnFileOpenPostNotify(BSTR fileName)
{
	HRESULT hres = S_OK;
	hres = AttachEventsToAllDocuments();
	return hres;
}

//Called when a new document is created or a document is loaded
STDMETHODIMP COLProgram::OnDocLoad(BSTR docTitle, BSTR docPath)
{

	return S_OK;
}
VARIANT_BOOL COLProgram::AttachEventHandlers()
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

HRESULT COLProgram::AttachEventsToAllDocuments()
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
				ATLTRACE("\tCOLProgram::DocumentLoadNotify current part not found\n");
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
VARIANT_BOOL COLProgram::AttachModelEventHandler(CComPtr<IModelDoc2> iModelDoc2)
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
		pDoc->Init((CComObject<COLProgram>*)this, iModelDoc2);

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
VARIANT_BOOL COLProgram::DetachEventHandlers()
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
VARIANT_BOOL COLProgram::DetachModelEventHandler(IUnknown *iUnk)
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
STDMETHODIMP COLProgram::OnModelDocChange(void)
{
	// TODO: Add your implementation code here
	return S_OK;
}

//Called when a new file is created
STDMETHODIMP COLProgram::OnFileNew(LPDISPATCH newDoc, long docType, BSTR templateName)
{
	HRESULT hres = S_OK;
	hres = AttachEventsToAllDocuments();
   return hres;
}

STDMETHODIMP COLProgram::ConnectToSW(LPDISPATCH ThisSW, long Cookie, VARIANT_BOOL * IsConnected)
{
	ThisSW->QueryInterface(__uuidof(ISldWorks), (void**)&iSwApp);
	addinID = Cookie;
	iSwApp->GetCommandManager(Cookie,&iCmdMgr);

	VARIANT_BOOL status = VARIANT_FALSE;

	iSwApp->SetAddinCallbackInfo((long)_AtlBaseModule.GetModuleInstance(), static_cast<IOLProgram*>(this), addinID, &status);
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
	return S_OK;
}
STDMETHODIMP COLProgram::DisconnectFromSW(VARIANT_BOOL * IsDisconnected)
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

STDMETHODIMP COLProgram::ToolbarCallbackPathExport(void)
{
	ExportPathToTXT();
	return S_OK;
}

STDMETHODIMP COLProgram::ToolbarBuildTube(void)
{
    BuildTubeAndHole();
	return S_OK;
}

// �����������ܼ�
void COLProgram::BuildTubeAndHole()
{
	long docType = -1;
	CComPtr<IModelDoc2> iSwModel;
	//iSwModel->SelectedFeatureProperties()
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
// 
// 	pHCombParams->m_dTubeDia = pHCombParams->m_dTubeDia/1000.;
// 	pHCombParams->m_dTubeLength = pHCombParams->m_dTubeLength/1000.;
// 	pHCombParams->m_dTubeThick = pHCombParams->m_dTubeThick/1000.;

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
//	m_dCutWidth = BuildTubeDlg.m_dTubeThick;
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
			AfxMessageBox(_T("�޷�ѡ��ǰ�ӻ�׼�棬�޷�����������"));
			iSwModel = NULL;
			return;
		}
		CComPtr<ISketchSegment> swSketchCLine;
		swSketMgr->CreateCenterLine(0,0,0,1,0,0,&swSketchCLine);
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
	// ���Թᴩ�Ƕ�
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
			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷����ɻ�׼��"));
			iSwModel = NULL;
			return;
		}

		swDocExt->SelectByID2(L"Line1@��ͼ2", L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
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
		swSketMgr->CreateCenterLine(pHParam->m_dOffsetX,0,0,pHParam->m_dOffsetX,1,0,&swSketchLine);
		pHCombParams->m_nSketID++;

		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
		sID.Format(_T("Line1@��ͼ%d"),pHCombParams->m_nSketID);
		bstrText = sID.AllocSysString();
		swDocExt->SelectByID2(bstrText, L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
		if (VARIANT_FALSE == retval)
		{
			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
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
			pHParam->m_dCenterDis*pHParam->m_dCenterDis);
		double dis2 = 1./tan(pHParam->m_dThroughAng)*dis;
		double dCenterXOffset = dis2+pHParam->m_dOffsetX;
		PNT3D ptCenter;
		ptCenter[0] = pHParam->m_dCenterDis;
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

	// ���´���������������ᴩ�Ƕ�Ϊ90�ȵĿף���ʱ�ȱ���
	//////////////////////////////////////////////////////////////////////////
	// 	int nHoleID = 0;
	// 	POSITION paraPos = pHCombParamList->m_LHParamList.GetHeadPosition();
	// 	while(paraPos)
	// 	{
	// 		CHoleParam* pPathParam = pHCombParamList->m_LHParamList.GetNext(paraPos);
	// 		if(NULL == pPathParam)
	// 			continue;
	// 		if (nHoleID < pHCombParamList->m_nHoleID)
	// 		{// ��ֹ�ظ��������ӵĿ�
	// 			nHoleID++;
	// 			continue;
	// 		}
	// 
	// 		nHoleID++;
	// 		// step1��������׼��		
	// 		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);
	// 		if (VARIANT_FALSE == retval)
	// 		{
	// 			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷����ɻ�׼��"));
	// 			iSwModel = NULL;
	// 			return;
	// 		}
	// 
	// 		swDocExt->SelectByID2(L"Line1@��ͼ2", L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
	// 		if (VARIANT_FALSE == retval)
	// 		{
	// 			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
	// 			iSwModel = NULL;
	// 			return;
	// 		}
	// 
	// 		CComQIPtr<IDispatch> pRefPlane;
	// 		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, pPathParam->m_dRotAng, 
	// 			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlane);
	// 		pHCombParamList->m_nHoleID++;
	// 
	// 		// step2�����ƴ��и��
	// 		CString sID;
	// 		sID.Format(_T("��׼��%d"),pHCombParamList->m_nHoleID);
	// 		//BSTR bstrText = _bstr_t(s); 
	// 		BSTR bstrText = sID.AllocSysString();
	// 		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
	// 		swSketMgr->InsertSketch(VARIANT_TRUE);
	// 
	// 		CComPtr<ISketchSegment> swSketchHole;
	// 		swSketMgr->CreateCircleByRadius(pPathParam->m_dOffsetX/*����X��ƫ��*/,pPathParam->m_dCenterDis/*�������ľ�*/,
	// 			                            0., pPathParam->m_dHoleR/*����װ뾶*/,&swSketchHole); 
	// 		if (NULL == swSketchHole)
	// 		{
	// 			AfxMessageBox(_T("��%d���״���ʧ�ܡ�"),pHCombParamList->m_nHoleID);
	// 		}
	// 		swSketMgr->InsertSketch(VARIANT_TRUE);
	// 
	// 		iSwModel->FeatureCut(1,0,VARIANT_TRUE,0,0,m_dTubeDia/1000.,m_dTubeDia/1000.,0,0,0,0,2,2,0,0);
	// 		
	// 	}
	//////////////////////////////////////////////////////////////////////////

	// end:
	iSwModel->SetAddToDB(VARIANT_FALSE);
	iSwModel->SetDisplayWhenAdded(VARIANT_TRUE);
	//////////////////////////////////////////////////////////////////////////

	iSwModel = NULL;
	return;
}

// ���·���ı任����ϵ�Ƿ���
BOOL COLProgram::CheckTransFrame(RFRAME& local_frame, LPathCombList &PathCombList)
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
void COLProgram::TransWorldPathComb(RFRAME& local_frame, LPathCombList &PathCombList)
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
void COLProgram::TransLocalPathComb(RFRAME& local_frame, LPathCombList &PathCombList)
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
void COLProgram::TransWorldPath(RFRAME& local_frame, CMovePath* pMovePath)
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
		mathTransWorldPnt3D(local_frame,pPathNode->m_OrgDirection,pPathNode->m_OrgDirection);
		mathTransWorldPnt3D(local_frame,pPathNode->m_OrgCutDrawEndPnt,pPathNode->m_OrgCutDrawEndPnt);

		mathTransWorldPnt3D(local_frame,pPathNode->m_OffsetPosition,pPathNode->m_OffsetPosition);
		mathTransWorldPnt3D(local_frame,pPathNode->m_OffsetDirection,pPathNode->m_OffsetDirection);
		mathTransWorldPnt3D(local_frame,pPathNode->m_OffsetDrawEndPnt,pPathNode->m_OffsetDrawEndPnt);
		mathUniVec(pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OrgDirection);
	}
}

// ��һ��·�����������������ϵת�����ֲ�����ϵ��
void COLProgram::TransLocalPath(RFRAME& local_frame, CMovePath* pMovePath)
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
		mathTransLocalPnt3D(local_frame,pPathNode->m_OrgDirection,pPathNode->m_OrgDirection);
		mathTransLocalPnt3D(local_frame,pPathNode->m_OrgCutDrawEndPnt,pPathNode->m_OrgCutDrawEndPnt);
		mathTransLocalPnt3D(local_frame,pPathNode->m_OffsetPosition,pPathNode->m_OffsetPosition);
		mathTransLocalPnt3D(local_frame,pPathNode->m_OffsetDirection,pPathNode->m_OffsetDirection);
		mathTransLocalPnt3D(local_frame,pPathNode->m_OffsetDrawEndPnt,pPathNode->m_OffsetDrawEndPnt);
		mathUniVec(pPathNode->m_OffsetDirection);
		mathUniVec(pPathNode->m_OrgDirection);
	}
}

void COLProgram::SetPathCombListParam(LPathCombList &PathCombList)
{
	CHCombParam* pHParamComb = GetCurHoleParam();
	if (NULL == pHParamComb)
		return;

	POSITION combpos = PathCombList.GetHeadPosition();
	while(combpos)
	{
		CPathComb* pPathComb = PathCombList.GetNext(combpos);
		if(NULL == pPathComb)
			continue;
		POSITION pathpos = pPathComb->m_PathList.GetHeadPosition();
		while (pathpos)
		{
			CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pathpos);
			if (NULL == pMovePath)
				continue;
			SetMovePathParam(pMovePath, pHParamComb);
		}
	}
}

void COLProgram::SetMovePathParam(CMovePath* pMovePath, CHCombParam* pPathParamComb)
{
	if(NULL==pMovePath || pPathParamComb==NULL)
		return;
	// �����ϴ���˴��Ĳ����飬Ӧ��û��Xƫ�ƻ�Yƫ��ֵ��ȵ��������Ϊ�������Ѿ��ڴ�list���ȡ�ײ���
	// �����������ʱ�򣬾��Ѿ����иı䲢��ӱ���ˡ�
	//  ��ͨ��yƽ��ֵѰ�Ҷ�Ӧ�����飬����Ҳ�������ͨ��yƽ��ֵѰ��
	//  �����ͨ��yƽ��ֵ�ҵ��Ĳ��������޸ı�ǣ�������������Ϊy��Ӱ������ˣ�
	// ��ƽ��ֵ��ʹ�����е㣬x��y�����ֵ��x��y����Сֵ�ĺͳ���2��
	BOOL bFindPara = FALSE;
	double dXMax = -10.e8;
	double dXMin = 10.e8;
	POSITION pos = pMovePath->m_PathNodeList.GetHeadPosition();
	while (pos)
	{
		CPathNode* pNode = pMovePath->m_PathNodeList.GetNext(pos);
		if (NULL == pNode)
			continue;
		if (pNode->m_OrgPosition[0]>dXMax)
		{
			dXMax = pNode->m_OrgPosition[0];
		}
		if (pNode->m_OrgPosition[0]<dXMin)
		{
			dXMin = pNode->m_OrgPosition[0];
		}
	}
	// step1��ͨ��xƽ��ֵ���Ҷ�Ӧ����
	double dPathAvgX = (dXMax+dXMin)*0.5;
	POSITION paramPos = pPathParamComb->m_LHParamList.GetHeadPosition();
	while(paramPos)
	{
		CHoleParam* pPathParam = pPathParamComb->m_LHParamList.GetNext(paramPos);
		if (NULL == pPathParam)
			continue;
		if (fabs(pPathParam->m_dOffsetX-dPathAvgX)>0.003)
			continue;
		pMovePath->SetHParam(pPathParam);
// 		if (pPathParam->m_bChanged) // ������޸Ĺ��ģ���Xƫ�Ƽ�ȥpPathParam->m_dChangedVal
// 			pMovePath->m_dOffsetX = pPathParam->m_dOffsetX - pPathParam->m_dChangedVal;
// 		else
// 			pMovePath->m_dOffsetX = pPathParam->m_dOffsetX;
// 		pMovePath->m_dRTube  = pPathParamComb->m_dTubeDia*0.5;
// 		pMovePath->m_dExRotAng = pPathParam->m_dExRotAng;
// 		pMovePath->m_dOrgRotAng = pPathParam->m_dOrgRotAng;
// 		pMovePath->m_nAddOrder = pPathParam->m_nAddOrder;
		bFindPara = TRUE;
		break;
	}
	if (!bFindPara)
	{
		AfxMessageBox(_T("·��δ�ҵ�ƥ�����"));
	}
}

CHCombParam* COLProgram::GetCurHoleParam()
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

CPathCombList* COLProgram::GetCurPComb()
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

void COLProgram::SetExportRFrame(RFRAME& _export_rframe)
{
	CHCombParam* pHCombParam = GetCurHoleParam();
	if (NULL == pHCombParam)
		return;
	mathInitRFrame(_export_rframe);
	_export_rframe.O[2] = -pHCombParam->m_dTubeDia*0.5;
}

void COLProgram::DrawPathComb(CPathCombList* pPathCombs)
{
	CComPtr<IModelDoc2> iSwModel;
	iSwApp->get_IActiveDoc2(&iSwModel);
	CComPtr<ISketchManager> iswSketchManger;
	iSwModel->get_SketchManager(&iswSketchManger);
	CComPtr<ISketchSegment> iSegment; 	
	POSITION testPos = pPathCombs->m_LPathCombList.GetHeadPosition();
	while(testPos)
	{
		CPathComb* pPathComb = pPathCombs->m_LPathCombList.GetNext(testPos);
		if (NULL == pPathComb)
			continue;
		iSwModel->Insert3DSketch2(VARIANT_TRUE);
		iSwModel->SetAddToDB(VARIANT_TRUE);
		POSITION pos = pPathComb->m_PathList.GetHeadPosition();
		while(pos)
		{
			CMovePath* pMovePath = (CMovePath*)pPathComb->m_PathList.GetNext(pos);
			if (NULL == pMovePath)
				continue;

			////����ˮƽ��Xƫ�ƺ�ת���Ƕȣ������Ӧ������
			//double dOffsetX = pMovePath->m_dOffsetX; //���룺X����ƫ�ƾ���
			//double dOffsetZ = pMovePath->m_dRTube;     // Z��ƫ��Ϊ��ܵİ뾶
			//double dRotAng = PI-pMovePath->m_dRotAng;        //���룺����ת�ĽǶ�
			//VEC3D RFrameOffset;
			//RFrameOffset[0] = dOffsetX;
			//RFrameOffset[1] = 0.;
			//RFrameOffset[2] = dOffsetZ;

			////double dRotAng = PI/180.*dRotAng1;
			////double dRotAng = 0.;
			////�����µ�����ϵ������ƽ����ת��
			//RFRAME ChangeFrame;
			//mathInitRFrame(ChangeFrame);
			////step0:��X��ƽ���µ�����ϵ
			//mathMoveRFrame(RFrameOffset,ChangeFrame);
			////step1:��·��������ӻ�������ϵת������X��ƽ�ƺ������ϵ��
			//TransWorldPath(ChangeFrame, pMovePath);
			////step2:��ƽ�ƺ������ϵ��תdRotAng��·���������ת��������ת������겻�����ı䡣
			//mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,dRotAng,ChangeFrame);

			//// �����������ϵ
			//PNT3D stX,endX, stY,endY, stZ,endZ;
			//for (int i=0; i<3; i++)
			//{
			//	endX[i] = ChangeFrame.O[i]+0.5*ChangeFrame.X[i];
			//	endY[i] = ChangeFrame.O[i]+0.5*ChangeFrame.Y[i];
			//	endZ[i] = ChangeFrame.O[i]+0.5*ChangeFrame.Z[i];
			//	stX[i] = stY[i] = stZ[i] = ChangeFrame.O[i];
			//}
			//VARIANT_BOOL bRet;
			//iSwModel->InsertCurveFileBegin();
			//iSwModel->InsertCurveFilePoint(stX[0],stX[1],stX[2],&bRet);
			//iSwModel->InsertCurveFilePoint(endX[0],endX[1],endX[2],&bRet);
			//iSwModel->InsertCurveFileEnd(&bRet);
			//iSwModel->InsertCurveFileBegin();
			//iSwModel->InsertCurveFilePoint(stY[0],stY[1],stY[2],&bRet);
			//iSwModel->InsertCurveFilePoint(endY[0],endY[1],endY[2],&bRet);
			//iSwModel->InsertCurveFileEnd(&bRet);
			//iSwModel->InsertCurveFileBegin();
			//iSwModel->InsertCurveFilePoint(stZ[0],stZ[1],stZ[2],&bRet);
			//iSwModel->InsertCurveFilePoint(endZ[0],endZ[1],endZ[2],&bRet);
			//iSwModel->InsertCurveFileEnd(&bRet);

			////return;

			////step3:����ת�������ϵ�е�·��������ת������������ϵ�С�
			//TransLocalPath(ChangeFrame,pMovePath);
			////step4:����������ϵ�е�����ת����ƽ��X�������ϵ�С�
			//VEC3D offsetX= {dOffsetX,0.,0.};
			//mathInitRFrame(ChangeFrame);
			//mathMoveRFrame(offsetX,ChangeFrame);
			//TransWorldPath(ChangeFrame, pMovePath);

			POSITION nodepos = pMovePath->m_PathNodeList.GetHeadPosition();
			while(nodepos)
			{
				CPathNode* pNode = pMovePath->m_PathNodeList.GetNext(nodepos);
				if (pNode == NULL)
					continue;
//				iSegment = NULL;
				//iswSketchManger->CreateLine(pNode->m_OrgPosition[0],pNode->m_OrgPosition[1],pNode->m_OrgPosition[2],
				//	pNode->m_OrgCutDrawEndPnt[0],pNode->m_OrgCutDrawEndPnt[1],pNode->m_OrgCutDrawEndPnt[2],&iSegment);
// 				CPathNode* pNextNode = NULL;
// 				if (NULL == nodepos)
// 					pNextNode = pMovePath->m_PathNodeList.GetHead();
// 				else
// 					pNextNode = pMovePath->m_PathNodeList.GetAt(nodepos);
// 
// 				if (NULL == pNextNode)
// 					continue;
				iSegment = NULL;
				//iswSketchManger->CreateLine(pNode->m_OrgPosition[0],pNode->m_OrgPosition[1],pNode->m_OrgPosition[2],
				//	pNextNode->m_OrgPosition[0],pNextNode->m_OrgPosition[1],pNextNode->m_OrgPosition[2],&iSegment);
				iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
				pNode->m_OffsetDrawEndPnt[0],pNode->m_OffsetDrawEndPnt[1],pNode->m_OffsetDrawEndPnt[2],&iSegment);
				if (pPathComb->m_bTwiceCut)
				{
					iSegment = NULL;
					iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
						pNode->m_OrgCutDrawEndPnt[0],pNode->m_OrgCutDrawEndPnt[1],pNode->m_OrgCutDrawEndPnt[2],&iSegment);
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
				if (pPathComb->m_bTwiceCut)
				{
					iSegment = NULL;
					iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
						pNextNode->m_OrgCutPosition[0],pNextNode->m_OrgCutPosition[1],pNextNode->m_OrgCutPosition[2],&iSegment);
				}
			}
		}	
		iSwModel->SetAddToDB(VARIANT_FALSE);
		iSwModel->Insert3DSketch2(VARIANT_FALSE);
    }
}

void COLProgram::SetPCombExOrder(CPathCombList* pPathCombs, int nOrder)
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
void COLProgram::ExportPathToTXT()
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

	// �����������ϵ
	RFRAME exportFrame;
	SetExportRFrame(exportFrame);

	// ��ȡ��ǰ�ĵ�·������
	CPathCombList* pPathCombList = GetCurPComb();
	if(NULL == pPathCombList)
	{
		AfxMessageBox(_T("���ȼ����и�·����"));
		return;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CExportDlg dlg;
	dlg.m_nExportOrder = m_nExportOrder;
	if (IDOK != dlg.DoModal())
		return;
	m_nExportOrder = dlg.m_nExportOrder ;
	// �µĲ��Ҳ����㷨
	//////////////////////////////////////////////////////////////////////////
//	CPathCombList* pTmpPCombs = pPathCombList->CopySelf();
// 	RFRAME rf;
// 	mathInitRFrame(rf);
	

//	TransWorldPathComb(exportFrame, pTmpPCombs->m_LPathCombList);
	// ·������ϵ�任
	//POSITION pos11 = pTmpPCombs->m_LPathCombList.GetHeadPosition();
	//while(pos11)
	//{
	//	CPathComb* pPathComb = pTmpPCombs->m_LPathCombList.GetNext(pos11);
	//	if (NULL == pPathComb)
	//		continue;
	//	POSITION pos22 = pPathComb->m_PathList.GetHeadPosition();
	//	while(pos22)
	//	{
	//		CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pos22);
	//		if(NULL == pMovePath)
	//			continue;
	//		mathRotateRFrame(exportFrame.O,exportFrame.X,pMovePath->m_dRotAng,rf);
	//		TransWorldPath(rf,pMovePath);
	//	}
	//}
//	DrawPathComb(pTmpPCombs);

//	return;
	//////////////////////////////////////////////////////////////////////////
	CPathCombList* pCpyPCombs = pPathCombList->CopySelf();
	//����ǰ·������任���������ϵ�С�
	TransWorldPathComb(exportFrame, pCpyPCombs->m_LPathCombList);
	
//	SetPathCombListParam(pCpyPCombs->m_LPathCombList);
	//���ÿ�·����ز���
//	SetPathCombListParam(pCpyPCombs->m_LPathCombList);
	SetPCombExOrder(pCpyPCombs, m_nExportOrder);
	//////////////////////////////////////////////////////////////////////////

	// ·������ϵ�任
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
			mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,dRotAng,ChangeFrame);
			//step3:����ת�������ϵ�е�·��������ת������������ϵ�С�
			TransLocalPath(ChangeFrame,pMovePath);
			//step4:����������ϵ�е�����ת����ƽ��X�������ϵ�С�
			VEC3D offsetX= {dOffsetX,0.,0.};
			mathInitRFrame(ChangeFrame);
			mathMoveRFrame(offsetX,ChangeFrame);
			TransWorldPath(ChangeFrame, pMovePath);
		}
	}
	// ����·��(������)
	//////////////////////////////////////////////////////////////////////////
	// DrawPathComb(pCpyPCombs);
	//////////////////////////////////////////////////////////////////////////

	 	
//	iSwModel->SetAddToDB(VARIANT_FALSE);
//	iSwModel->Insert3DSketch2(VARIANT_FALSE);
		
	//			
	//// �����������ϵ
	//PNT3D stX,endX, stY,endY, stZ,endZ;
	//for (int i=0; i<3; i++)
	//{
	//	endX[i] = exportFrame.O[i]+0.5*exportFrame.X[i];
	//	endY[i] = exportFrame.O[i]+0.5*exportFrame.Y[i];
	//	endZ[i] = exportFrame.O[i]+0.5*exportFrame.Z[i];
	//	stX[i] = stY[i] = stZ[i] = exportFrame.O[i];
	//}
	//VARIANT_BOOL bRet;
	//iSwModel->InsertCurveFileBegin();
	//iSwModel->InsertCurveFilePoint(stX[0],stX[1],stX[2],&bRet);
	//iSwModel->InsertCurveFilePoint(endX[0],endX[1],endX[2],&bRet);
	//iSwModel->InsertCurveFileEnd(&bRet);
	//iSwModel->InsertCurveFileBegin();
	//iSwModel->InsertCurveFilePoint(stY[0],stY[1],stY[2],&bRet);
	//iSwModel->InsertCurveFilePoint(endY[0],endY[1],endY[2],&bRet);
	//iSwModel->InsertCurveFileEnd(&bRet);
	//iSwModel->InsertCurveFileBegin();
	//iSwModel->InsertCurveFilePoint(stZ[0],stZ[1],stZ[2],&bRet);
	//iSwModel->InsertCurveFilePoint(endZ[0],endZ[1],endZ[2],&bRet);
	//iSwModel->InsertCurveFileEnd(&bRet);
//	return;
	
	//////////////////////////////////////////////////////////////////////////

//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
//	CString sCurrentFileName = GetCurrentFile();
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
		double dRotAng  = 10;        //���룺����ת�ĽǶ�
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

			/*/�ò������ڽ�·�����ݻ�����X����ƽ�Ƶľ���͹ܵ���ת�ĽǶȽ�������任
		    //////////////////////////////////////////////////////////////////////////
			//����ˮƽ��Xƫ�ƺ�ת���Ƕȣ������Ӧ������
			double dOffsetX = pMovePath->m_dOffsetX; //���룺X����ƫ�ƾ���
			double dOffsetZ = pMovePath->m_dRTube;     // Z��ƫ��Ϊ��ܵİ뾶
			double dRotAng = PI-pMovePath->m_dRotAng;        //���룺����ת�ĽǶ�
			VEC3D RFrameOffset;
			RFrameOffset[0] = dOffsetX;
			RFrameOffset[1] = 0.;
			RFrameOffset[2] = dOffsetZ;

			//double dRotAng = PI/180.*dRotAng1;
			//double dRotAng = 0.;
			//�����µ�����ϵ������ƽ����ת��
			RFRAME ChangeFrame;
			mathInitRFrame(ChangeFrame);
			//step0:��X��ƽ���µ�����ϵ
			mathMoveRFrame(RFrameOffset,ChangeFrame);
			//step1:��·��������ӻ�������ϵת������X��ƽ�ƺ������ϵ��
			TransWorldPath(ChangeFrame, pMovePath);
			//step2:��ƽ�ƺ������ϵ��תdRotAng��·���������ת��������ת������겻�����ı䡣
			mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,dRotAng,ChangeFrame);
			//step3:����ת�������ϵ�е�·��������ת������������ϵ�С�
			TransLocalPath(ChangeFrame,pMovePath);
			//step4:����������ϵ�е�����ת����ƽ��X�������ϵ�С�
			VEC3D offsetX= {dOffsetX,0.,0.};
			mathInitRFrame(ChangeFrame);
			mathMoveRFrame(offsetX,ChangeFrame);
			TransWorldPath(ChangeFrame, pMovePath);

// 			//����ˮƽ��Xƫ�ƺ�ת���Ƕȣ������Ӧ������
// 			double dOffsetX = pMovePath->m_dOffsetX; //���룺X����ƫ�ƾ���
// 			double dOffsetZ = pMovePath->m_dRTube;     // Z��ƫ��Ϊ��ܵİ뾶
// 			double dRotAng = pMovePath->m_dRotAng;        //���룺����ת�ĽǶ�
// 			VEC3D RFrameOffset;
// 			RFrameOffset[0] = dOffsetX;
// 			RFrameOffset[1] = 0.;
// 			RFrameOffset[2] = dOffsetZ;
// 
// 			//�����µ�����ϵ������ƽ����ת��
// 			RFRAME ChangeFrame;
// 			mathInitRFrame(ChangeFrame);
// 			//step0:��X��Z��ƽ���µ�����ϵ
// 			mathMoveRFrame(RFrameOffset,ChangeFrame);
// 			//step1:��·��������ӻ�������ϵת������X��ƽ�ƺ������ϵ��
// 			TransWorldPath(ChangeFrame, pMovePath);
// 			//step2:��ƽ�ƺ������ϵ��תdRotAng��·���������ת��������ת������겻�����ı䡣
// 			mathRotateRFrame(ChangeFrame.O,ChangeFrame.X,-dRotAng,ChangeFrame);
// 			//step3:����ת�������ϵ�е�·��������ת������������ϵ�С�
// 			TransLocalPath(ChangeFrame,pMovePath);
// 			//step4:����������ϵ�е�����ת����ƽ��X�������ϵ�С�
// 			VEC3D offsetX= {dOffsetX,0.,0.};
// 			mathInitRFrame(ChangeFrame);
// 			mathMoveRFrame(offsetX,ChangeFrame);
// 			TransWorldPath(ChangeFrame, pMovePath);
			*//////////////////////////////////////////////////////////////////////////
			fout<<"��"<<nPathId<<"���׵�X�����Ϊ��"<<pMovePath->GetOffsetX()*1000.<<";"
				<<"����ת�Ƕ�Ϊ��"<<pMovePath->GetOrgRotAng()<<";"<<"\t\n";
			fout<<"���и�·���㼰����"<<"\t\n";
			nPathId++;
			if (pPathComb->m_bTwiceCut)
			{
				fout<<"���и�·����"<<"\t\n";
			}
			while(nodePos && pPathComb->m_bTwiceCut)
			{
				CPathNode* pPathNode = pMovePath->m_PathNodeList.GetNext(nodePos);
				if (NULL == pPathNode)
					continue;

				fout<<pPathNode->m_OrgCutPosition[0]*1000<<" "<<pPathNode->m_OrgCutPosition[1]*1000<<" "<<pPathNode->m_OrgCutPosition[2]*1000<<" "
					<<pPathNode->m_OrgDirection[0]<<" "<<pPathNode->m_OrgDirection[1]<<" "<<pPathNode->m_OrgDirection[2]<<" "<<"\t\n";
			}

			if (pPathComb->m_bTwiceCut)
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
	/*
	// �����������ϵ
	PNT3D stX,endX, stY,endY, stZ,endZ;
	for (int i=0; i<3; i++)
	{
		endX[i] = exportFrame.O[i]+0.005*exportFrame.X[i];
		endY[i] = exportFrame.O[i]+0.005*exportFrame.Y[i];
		endZ[i] = exportFrame.O[i]+0.005*exportFrame.Z[i];
		stX[i] = stY[i] = stZ[i] = exportFrame.O[i];
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
*/
	if (NULL != pCpyPCombs)
	{
		delete pCpyPCombs;
		pCpyPCombs = NULL;
	}
	
	AfxMessageBox(_T("·�������ɡ�"));
}

STDMETHODIMP COLProgram::ToolbarEnablePathExport(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

STDMETHODIMP COLProgram::ToolbarEnableBuildTube(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

STDMETHODIMP COLProgram::FlyoutCallback(void)
{
	CComBSTR tip;
	CComBSTR callback;
	CComBSTR enable;
	CComBSTR hint;
	CComPtr<IFlyoutGroup> flyGroup;
	
	HRESULT hr = iCmdMgr->GetFlyoutGroup(FLYOUT_GROUP_ID, &flyGroup);
	flyGroup->RemoveAllCommandItems();
	
	callback.LoadString(IDS_FLYOUT_CALLBACK0);	
	enable.LoadString(IDS_FLYOUT_ENABLE_CALLBACK0);
	hint.LoadString(IDS_FLYOUT_HINT0);
	long cmdCount;
	flyGroup->AddCommandItem(GetCurrentFile(), hint, 0, callback, enable, &cmdCount);

	callback.LoadString(IDS_FLYOUT_CALLBACK1);
	enable.LoadString(IDS_FLYOUT_ENABLE_CALLBACK0);
	hint.LoadString(IDS_FLYOUT_HINT1);
	flyGroup->AddCommandItem(L"FlyoutCommand 1", hint, 0, callback, enable, &cmdCount);

	return S_OK;
}

STDMETHODIMP COLProgram::FlyoutCallback0(void)
{
	// TODO: Add your implementation code here
	::WinExec("Notepad.exe", SW_SHOW);
	return S_OK;
}

STDMETHODIMP COLProgram::FlyoutCallback1(void)
{
	// TODO: Add your implementation code here
	
	return S_OK;
}

STDMETHODIMP COLProgram::FlyoutEnable0(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

STDMETHODIMP COLProgram::FlyoutEnableCallback0(long* status)
{
	// TODO: Add your implementation code here
	*status = 1;
	return S_OK;
}

BSTR COLProgram::GetCurrentFile()
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


STDMETHODIMP COLProgram::GeneratePath(void)
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

STDMETHODIMP COLProgram::EnableGeneratePath(long* status)
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
