// PMPageHandler.cpp : Implementation of CPMPageHandler

#include "stdafx.h"
#include "PMPageHandler.h"
#include "EliteSoftWare.h"
#include "CutParamDlg.h"
#include "BuildTubeDlg.h"


// CPMPageHandler
void CPMPageHandler::Init(CEliteSoftWare *app)
{
	userAddin = app;
	iSwApp = userAddin->GetSldWorksPtr();

	m_dCutAng   = 45.;
	m_dCutWidth = 10.;
	m_dChodTol  = 0.01;
	m_dStepTol  = 0.01;
	m_dToolDis  = 3. ;

// 	m_dTubeDia    = 1000;
// 	m_dTubeThick  = 30;
// 	m_dTubeLength = 4000;
	m_bTwiceCut = TRUE;
}

HRESULT __stdcall CPMPageHandler::OnClose(long Reason)
{
	return E_NOTIMPL;
}

HRESULT __stdcall CPMPageHandler::OnButtonPress(long Id)
{
	switch (Id)
	{
	case BUTTON_CALCULPATH:
		CalMovePath();
		break;
	case BUTTON_SETPARAM:
		SetCutParam();
		break;
	case BUTTON_CLEARPATH:
		ClearAllPath();
		break;
// 	case BUTTON_BUILDTUBE:
// 		BuildTubeAndHole();
// 		break;
	default:
		break;
	}
	return E_NOTIMPL;
}

void CPMPageHandler::SetCutParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCutParamDlg dlg;
	dlg.m_dCutAng = m_dCutAng;
	dlg.m_dCutWidth = m_dCutWidth;
	dlg.m_dChodTol = m_dChodTol;
	dlg.m_dStepTol = m_dStepTol;
	dlg.m_dToolDis = m_dToolDis;
	dlg.m_bTwiceCut = m_bTwiceCut;
	if (IDOK == dlg.DoModal())
	{
		m_dCutAng = dlg.m_dCutAng;
		m_dCutWidth = dlg.m_dCutWidth;
		m_dChodTol = dlg.m_dChodTol;
		m_dStepTol = dlg.m_dStepTol;
		m_dToolDis = dlg.m_dToolDis;
		m_bTwiceCut = dlg.m_bTwiceCut;
	}
}
extern void mathVProduct(VEC3D v1, VEC3D v2, VEC3D rtvec);
extern BOOL mathUniVec(VEC3D v, double min_len = 0.0000001);
extern void mathRotVecXY(VEC3D e1, VEC3D e2, double angle, VEC3D rot_v);
extern double mathDis3D(PNT3D pt1, PNT3D pt2);

// <out>dLength[0]:��ȡδƫ�Ƶ�·�����ȣ�
//      dLength[1]:��ȡƫ�ƺ��·�����ȣ�
// <in> bClosed:TRUE��ʾ·���պϣ�FALSE��ʾ·���Ǳպϣ�
void GetPathLength(CMovePath* pPath, double dLength[2], BOOL bFlag = TRUE)
{
	if (NULL == pPath)
		return ;
	for (int i=0; i<2; i++)
	{
		dLength[i] = 0.;
	}

	POSITION pos = pPath->m_PathNodeList.GetHeadPosition();
	while(pos)
	{
		CPathNode* pFirstNode = pPath->m_PathNodeList.GetNext(pos);
		if (NULL == pFirstNode)
			continue;
		CPathNode* pNextNode  = NULL;
		if (NULL != pos)
		{
			pNextNode = pPath->m_PathNodeList.GetAt(pos);
		}
		
		// �ڶ���Ϊ��ʱ��������߱պϣ���ȡ·���׵���Ϊ�ڶ���
		if (NULL == pNextNode || NULL == pos)
		{
			if (!bFlag)
				break;
			POSITION tmpPos = pPath->m_PathNodeList.GetHeadPosition();
			pNextNode = pPath->m_PathNodeList.GetAt(tmpPos);
		}
		if (NULL == pNextNode)
			continue;
		dLength[0] += mathDis3D(pFirstNode->m_OrgCutPosition,pNextNode->m_OrgCutPosition);
		dLength[1] += mathDis3D(pFirstNode->m_OffsetPosition,pNextNode->m_OffsetPosition);
	}
	return;
}

// ����ƫ�ƺ��·�����ȣ����ж�·����ƫ�Ʒ����Ƿ���ȷ
BOOL CheckPathComb(CMovePath* pPath)
{
	if (NULL == pPath)
		return FALSE;
	double dLength[2] = {0., 0.};
	GetPathLength(pPath,dLength);
	// ���ԭʼ�����߳��ȴ���ƫ�ƺ�����߳��ȣ�˵��ƫ�Ʒ������
	if (dLength[0]>dLength[1])
		return FALSE;
	return TRUE;
}

CHCombParam* CPMPageHandler::GetCurHoleParams()
{
	BSTR sCurrentFileName = SysAllocString(L"No File Currently Open");
	CComPtr<IModelDoc2> iSwModel;

	iSwApp->get_IActiveDoc2(&iSwModel);
	if(iSwModel != NULL)
	{
		iSwModel->GetTitle(&sCurrentFileName);
	}
	CHCombParam* pHParamComb = NULL;
	BOOL bFind = FALSE;
	POSITION pos = userAddin->m_LHoleParamList.GetHeadPosition();
	while(pos)
	{
		pHParamComb = userAddin->m_LHoleParamList.GetNext(pos);
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

CHoleParam* CPMPageHandler::GetHParam(int ptNum, double* ptArray)
{
	CHCombParam* pPathParamComb = GetCurHoleParams();
	if (NULL == pPathParamComb || NULL == ptArray)
		return NULL;
	// �����ϴ���˴��Ĳ����飬Ӧ��û��Xƫ�ƻ�Yƫ��ֵ��ȵ��������Ϊ�������Ѿ��ڴ�list���ȡ�ײ���
	// �����������ʱ�򣬾��Ѿ����иı䲢��ӱ���ˡ�
	//  ��ͨ��yƽ��ֵѰ�Ҷ�Ӧ�����飬����Ҳ�������ͨ��yƽ��ֵѰ��
	//  �����ͨ��yƽ��ֵ�ҵ��Ĳ��������޸ı�ǣ�������������Ϊy��Ӱ������ˣ�
	// ��ƽ��ֵ��ʹ�����е㣬x��y�����ֵ��x��y����Сֵ�ĺͳ���2��
	BOOL bFindPara = FALSE;
	double dXMax = -10.e8;
	double dXMin = 10.e8;
	for (int num = 0; num<ptNum; num+=3)
	{
		if (ptArray[num]>dXMax)
		{
			dXMax = ptArray[num];
		}
		if (ptArray[num]<dXMin)
		{
			dXMin = ptArray[num];
		}
	}
	CHoleParam* pPathParam = NULL;
	// step1��ͨ��xƽ��ֵ���Ҷ�Ӧ����
	double dPathAvgX = (dXMax+dXMin)*0.5;
	POSITION paramPos = pPathParamComb->m_LHParamList.GetHeadPosition();
	while(paramPos)
	{
		pPathParam = pPathParamComb->m_LHParamList.GetNext(paramPos);
		if (NULL == pPathParam)
			continue;
		if (fabs(pPathParam->m_dOffsetX-dPathAvgX)>0.003)
			continue;
		bFindPara = TRUE;
		break;
	}
	if (!bFindPara)
	{
		AfxMessageBox(_T("·��δ�ҵ�ƥ�����"));
	}
	return pPathParam;
}
#define MIN_DBL 1.0e-10
extern double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir);
extern double mathGetAngle(VEC3D v1, VEC3D v2, double min_len);
// ���ڼ���·��ƫ�ƺ�Ľڵ�
void CPMPageHandler::CalPathNode(int ptNum, double* ptArray, ISurface* swSurface, CMovePath* movePath, BOOL bRevOffVec)
{
	// ��һ�����ȼ���ԭʼ·�������ڲ���ƥ�䣬�����Ӧ�Ĺᴩ����
	//////////////////////////////////////////////////////////////////////////
	CHoleParam* pHParam = GetHParam(ptNum,ptArray);
	if (NULL == pHParam)
	{
		AfxMessageBox(_T("�ײ�����ȡʧ�ܣ���ǰ�ĵ��������⣬���ؽ��ĵ���"));
		return;
	}
	movePath->SetHParam(pHParam);
	//////////////////////////////////////////////////////////////////////////

	PNT3D edgePnt, nextEdgePnt;
	VEC3D dThroughVec, edgeVec, offsetVec;
	double retFacePt[5] = {0.,0.,0.,0.,0.};

	for (int i=0; i<3; i++)
	{
		dThroughVec[i] = pHParam->m_dThroughVec[i];
	}
	for (int num = 0; num<ptNum; num+=3)
	{
		CPathNode* pNode = new CPathNode();
		for (int i=0; i<3; i++)
		{
			edgePnt[i] = ptArray[i+num];
		}

		swSurface->IGetClosestPointOn(edgePnt[0],edgePnt[1],edgePnt[2],retFacePt);

		for (int i=0; i<3; i++)
		{
			pNode->m_OrgDirection[i] = dThroughVec[i];
		}
		mathUniVec(pNode->m_OrgDirection);

		PNT3D vecStart,vecEnd;
		for (int i=0; i<3; i++)
		{
			vecStart[i] = retFacePt[i];
			pNode->m_OrgPosition[i] = retFacePt[i];
		}
		//////////////////////////////////////////////////////////////////////////
		// ���пױ�Ե���
		//////////////////////////////////////////////////////////////////////////
		// 1.�����ľݣ�ͨ��ͼ�ο�֪��������ľݣ����ǵ��Z����ֵ
		// 2.���Ӧ��Z�᷽��ں�
		if (NULL == pHParam->GetParentComb())
		{
			AfxMessageBox(_T("��ǰ�ĵ������ݲ����������⣬���ؽ��ĵ���"));
			return;
		}
		PNT3D pt1={0,0,0};
		PNT3D pt2 = {0,pNode->m_OrgPosition[1],pNode->m_OrgPosition[2]};
		VEC3D vec = {0,pHParam->m_dThroughVec[1],pHParam->m_dThroughVec[2]};
		double ptCDis = mathDistPntLin(pt1,pt2,vec);
		double dRWTube = pHParam->GetParentComb()->m_dTubeDia*0.5;
		double dRNTube = pHParam->GetParentComb()->m_dTubeDia*0.5 - pHParam->GetParentComb()->m_dTubeThick;
		double dZThick = sqrt(dRWTube*dRWTube - ptCDis*ptCDis) - sqrt(dRNTube*dRNTube - ptCDis*ptCDis);
		double dHThick = dZThick/sin(pHParam->m_dThroughAng); // ���ڹ��ϵ������

		//double dDThick = m_dCutWidth/1000;//�¿���ȣ��û�����
		double dDunDeep = m_dCutWidth/1000;////�¿���ȣ��û����ã�����dHThick�������ѻ����Զ���ΪdHThick
		if (dDunDeep>dHThick)
		{
			dDunDeep = dHThick;
		}
		//////////////////////////////////////////////////////////////////////////

		// ��Ϊ�պ�����ʱ����ǰ��Ϊ���һ����ʱ����һ����ʹ�����ߵ����
		if ((num+3)<ptNum)
		{
			for (int i=0; i<3; i++)
			{
				nextEdgePnt[i] = ptArray[i+num+3];
			}
		}
		else
		{
			for (int i=0; i<3; i++)
			{
				nextEdgePnt[i] = ptArray[i];
				edgeVec[i] = nextEdgePnt[i]-edgePnt[i];
			}
		}

		for (int i =0; i<3; i++)
		{
			edgeVec[i] = nextEdgePnt[i] - edgePnt[i];
		}

		mathVProduct(edgeVec,dThroughVec,offsetVec);
		if(!mathUniVec(offsetVec))
		{
			if(NULL != pNode)
			{
				delete pNode;
				pNode = NULL;
			}
			
			continue;; 
		}
		
		// �ж��Ƿ���Ҫ����ƫ�Ʒ���
		if (bRevOffVec)
		{
			for (int i=0; i<3; i++)
			{
				offsetVec[i] = -offsetVec[i];
			}
		}

		double pokouAng = PI/180.*m_dCutAng; // �û����õ��¿ڽǶ�

		//����������֧�ܵ��¿ڴ��н�
		//////////////////////////////////////////////////////////////////////////
		// 1.����ǰ�����п�ˮƽ�ķ���ƫ�ƺ����õ��¿ڿ����ͬ�ľ��룬��ƫ�ƺ�ĵ㡣
		PNT3D tmpPt;
		for (int i=0; i<3; i++)
		{
			tmpPt[i] = vecStart[i]+0.001*offsetVec[i]*0.001;
		}
		swSurface->IGetClosestPointOn(tmpPt[0],tmpPt[1],tmpPt[2],retFacePt);
		// 2.��ԭʼ���ƫ�ƺ�ĵ㹹�������ϵ�����
		VEC3D tmpVec;
		for (int i=0; i<3; i++)
		{
			tmpVec[i] = retFacePt[i]-pNode->m_OrgPosition[i];
		}
		mathUniVec(tmpVec);
		// 3.�������������ᴩ�����ļн�
		double tmpAng = mathGetAngle(tmpVec,pHParam->m_dThroughVec,MIN_DBL);
		// 4.�жϼнǣ����С��45�ȣ���ʹ�üнǵĶ���֮һ
		if (tmpAng<(90.*PI/180.-0.001))
		{
			pokouAng = tmpAng*0.5;
		}
		//////////////////////////////////////////////////////////////////////////
// 		for (int i=0; i<3; i++)
// 		{
// 			vecStart[i] = retFacePt[i];
// 		}
		VEC3D pokouVec;
		mathRotVecXY(dThroughVec,offsetVec,pokouAng,pokouVec);
		mathUniVec(pokouVec);

		//////////////////////////////////////////////////////////////////////////
		// ��۱ߵ�
		PNT3D ptDun; // ʵ�ʶ۱ߵ�����
		PNT3D ptBaseDun;// �׻��߶۱ߵ�����
		for (int i=0; i<3; i++)
		{
			ptDun[i] = pNode->m_OrgPosition[i]-dDunDeep*pHParam->m_dThroughVec[i];
		}
		for (int i=0; i<3; i++)
		{
			ptBaseDun[i] = pNode->m_OrgPosition[i]-dHThick*pHParam->m_dThroughVec[i];
		}
		//////////////////////////////////////////////////////////////////////////
		// ���¿���ܵĽ���
		//////////////////////////////////////////////////////////////////////////
		// 1.���¿�������YZƽ���ϵ�ͶӰ����
		VEC3D YZVec = {0.,pokouVec[1], pokouVec[2]};
		mathUniVec(YZVec);
		// 2.���¿�������YZ�����ļн�
		double vecAng = mathGetAngle(pokouVec,YZVec,MIN_DBL);
		// 3.��YZƽ���ϣ������¿����������ľ���
		PNT3D p={0,0,0};
		PNT3D p1= {0,ptBaseDun[1],ptBaseDun[2]};
		double d = mathDistPntLin(p,p1,YZVec);
		// 4.��������¿���YZƽ���ϵ�ͶӰ����
		double dPkL = sqrt(dRWTube*dRWTube - d*d) - sqrt(dRNTube*dRNTube - d*d);
		// 5.ʵ���¿ڵĳ���
		double dRealPkL = dPkL*(dDunDeep)/dHThick;
		// 6.ͨ��ͶӰ���Ⱥͼнǣ����ԭʼ����
		double dOffsetVecLength = dRealPkL/cos(vecAng);
		for(int i=0; i<3; i++)
		{
			vecStart[i] = ptDun[i]+pokouVec[i]*dOffsetVecLength;
		}
		//////////////////////////////////////////////////////////////////////////

		// ����������
		for (int i=0; i<3; i++)
		{
			vecEnd[i] = vecStart[i]+0.005*pokouVec[i];
		}
		for (int i=0; i<3; i++)
		{
			pNode->m_OffsetPosition[i]   = vecStart[i];
			pNode->m_OffsetDirection[i]  = pokouVec[i];
			pNode->m_OffsetDrawEndPnt[i] = vecEnd[i];
		}

// 		for (int i=0; i<3; i++)
// 		{
// 			pNode->m_OrgCutPosition[i]   = ptDun[i];
// 			pNode->m_OrgDirection[i]  = pokouVec[i];
// 			pNode->m_OrgCutDrawEndPnt[i] = vecEnd[i];
// 		}

		//��ͷ���վ���
		for (int i=0; i<3; i++)
		{
			pNode->m_OrgCutPosition[i] = pNode->m_OrgPosition[i]+0.001*pNode->m_OrgDirection[i]*m_dToolDis;
			pNode->m_OrgCutDrawEndPnt[i] = pNode->m_OrgCutPosition[i]+0.005*pNode->m_OrgDirection[i];

			pNode->m_OffsetPosition[i] = pNode->m_OffsetPosition[i]+0.001*pNode->m_OffsetDirection[i]*m_dToolDis;
			pNode->m_OffsetDrawEndPnt[i] = pNode->m_OffsetDrawEndPnt[i]+0.005*pNode->m_OffsetDirection[i];
		}

		movePath->m_PathNodeList.AddTail(pNode);
	}
}

// ɾ����ǰ�����ĵ��е�����·��
void CPMPageHandler::ClearAllPath()
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

	//���ҵ�ǰ�ĵ��е�·�������ͷ�
	//////////////////////////////////////////////////////////////////////////
	BSTR sFileName;
	iSwModel->GetTitle(&sFileName);
	BOOL bFind = FALSE;
	CPathCombList* pPathCombList = NULL;
	POSITION entPos = userAddin->m_LPathEntityList.GetHeadPosition();
	while(entPos)
	{
		pPathCombList = userAddin->m_LPathEntityList.GetNext(entPos);
		if (NULL == pPathCombList)
			continue;
		_bstr_t filename(sFileName), pathname(pPathCombList->m_sPathCombName);
		if (filename != pathname)
			continue;
		else
		{
			bFind = TRUE;
			break;
		}
	}
	if (!bFind)
	{
		pPathCombList = NULL;
		AfxMessageBox(_T("���ĵ�����·�������"));
		return;
	}
	pPathCombList->Realse();//�ͷŵ�ǰ�ĵ��е�·��
	//////////////////////////////////////////////////////////////////////////
	
	AfxMessageBox(_T("���ĵ�����·�������"));
}

//// �����������ܼ�
//void CPMPageHandler::BuildTubeAndHole()
//{
//	long docType = -1;
//	CComPtr<IModelDoc2> iSwModel;
//
//	iSwApp->get_IActiveDoc2(&iSwModel);
//
//	if(iSwModel != NULL)
//	{
//		iSwModel->GetType(&docType);
//	}
//
//	if (docType != swDocPART && docType != swDocASSEMBLY)
//	{
//		AfxMessageBox(_T("�򿪵Ĳ������"),MB_OK|MB_ICONINFORMATION);
//		iSwModel = NULL;
//		return ;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	BSTR sFileName;
//	iSwModel->GetTitle(&sFileName);
//	CHCombParam* pHCombParams = userAddin->GetCurHoleParam();
//	if (NULL == pHCombParams)
//	{
//		pHCombParams = new CHCombParam(sFileName);
//		userAddin->m_LHoleParamList.AddTail(pHCombParams);
//	}
//
//	pHCombParams->m_dTubeDia = m_dTubeDia/1000.;
//	pHCombParams->m_dTubeLength = m_dTubeLength/1000.;
//	pHCombParams->m_dTubeThick = m_dTubeThick/1000.;
//
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	CBuildTubeDlg BuildTubeDlg;
//	BuildTubeDlg.m_dTubeDia    = m_dTubeDia;
//	BuildTubeDlg.m_dTubeLength = m_dTubeLength;
//	BuildTubeDlg.m_dTubeThick  = m_dTubeThick;
//	BuildTubeDlg.SetHParamList(pHCombParams);
//	BuildTubeDlg.m_bBulidTube = pHCombParams->m_bBulidTube;
//	if (BuildTubeDlg.DoModal() != IDOK)
//		return;
//	m_dTubeDia    = BuildTubeDlg.m_dTubeDia;
//	m_dTubeLength = BuildTubeDlg.m_dTubeLength;
//	m_dTubeThick  = BuildTubeDlg.m_dTubeThick;
//	m_dCutWidth = BuildTubeDlg.m_dTubeThick;
//	double dRWCircle = m_dTubeDia*0.5/1000.;                  // ��Բֱ��
//	double dRNCircle = (m_dTubeDia*0.5 - m_dTubeThick)/1000.; // ��Բֱ��
//
//	CComPtr<IModelDocExtension> swDocExt;
//	CComPtr<ISketchManager> swSketMgr;
//	CComPtr<IFeatureManager> swFeatmgr;
//	iSwModel->get_Extension(&swDocExt);
//	iSwModel->get_SketchManager(&swSketMgr);
//	iSwModel->get_FeatureManager(&swFeatmgr);
//
//	if (NULL == swFeatmgr)
//	{
//		AfxMessageBox(_T("swFeature == NULL"));
//		return;
//	}
//
//	VARIANT_BOOL retval;	
//	// �����������ܼ�
//	//////////////////////////////////////////////////////////////////////////
//	if (pHCombParams->m_bBulidTube) //���
//	{
//		// prepare: ���û��Ʒ�ʽ����ʾ��ʽ	
//		iSwModel->SetAddToDB(VARIANT_TRUE);
//		iSwModel->SetDisplayWhenAdded(VARIANT_FALSE);
//		iSwModel->ClearSelection2(VARIANT_TRUE);
//
//		// step1���������и�Բ��
//		iSwModel->SelectByID(_T("���ӻ�׼��"),_T("PLANE"),0.,0.,0.,&retval);
//		if (VARIANT_FALSE == retval)
//		{
//			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷�����Բ��"));
//			iSwModel = NULL;
//			return;
//		}
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//		CComPtr<ISketchSegment> swSketchWArc;
//		CComPtr<ISketchSegment> swSketchNArc;
//		swSketMgr->CreateCircleByRadius(0.,0.,0.,dRWCircle,&swSketchWArc);
//		swSketMgr->CreateCircleByRadius(0.,0.,0.,dRNCircle,&swSketchNArc);
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//		iSwModel->FeatureBoss(1,0,0,0,0,m_dTubeLength/1000.,m_dTubeLength/1000.,0,0,0,0,2,2,0,0);
//
//		// step2������������ߣ����ڴ����Ļ�׼��
//		iSwModel->SelectByID(_T("ǰ�ӻ�׼��"),_T("PLANE"),0.,0.,0.,&retval);
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//		if (VARIANT_FALSE == retval)
//		{
//			AfxMessageBox(_T("�޷�ѡ��ǰ�ӻ�׼�棬�޷�����������"));
//			iSwModel = NULL;
//			return;
//		}
//		CComPtr<ISketchSegment> swSketchCLine;
//		swSketMgr->CreateCenterLine(0,0,0,1,0,0,&swSketchCLine);
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//		iSwModel->ClearSelection2(VARIANT_TRUE);
//		iSwModel->SetAddToDB(VARIANT_FALSE);
//		iSwModel->SetDisplayWhenAdded(VARIANT_TRUE);
//		pHCombParams->m_bBulidTube = FALSE;
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//	// ������������
//	//////////////////////////////////////////////////////////////////////////
//	iSwModel->SetAddToDB(VARIANT_TRUE);
//	iSwModel->SetDisplayWhenAdded(VARIANT_FALSE);
//	//////////////////////////////////////////////////////////////////////////
//	// ���Թᴩ�Ƕ�
//	// ���ӻ�׼��ID�仯�Ͳ�ͼID�仯����������������б�Źᴩ�Ŀ�
//	int nHoleID = 0;
//	POSITION paraPos = pHCombParams->m_LHParamList.GetHeadPosition();
//	while(paraPos)
//	{
//		CHoleParam* pPathParam = pHCombParams->m_LHParamList.GetNext(paraPos);
//		if(NULL == pPathParam)
//			continue;
// 		if (nHoleID < pHCombParams->m_nHoleID)
// 		{// ��ֹ�ظ��������ӵĿ�
// 			nHoleID++;
// 			continue;
// 		}
//		nHoleID++;
//
//		// step1��������׼��		
//		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);
//		if (VARIANT_FALSE == retval)
//		{
//			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷����ɻ�׼��"));
//			iSwModel = NULL;
//			return;
//		}
//
//		swDocExt->SelectByID2(L"Line1@��ͼ2", L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
//		if (VARIANT_FALSE == retval)
//		{
//			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
//			iSwModel = NULL;
//			return;
//		}
//
//		CComQIPtr<IDispatch> pRefPlane;
//		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, pPathParam->m_dExRotAng, 
//			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlane);
//		pHCombParams->m_nRefPlaneID++;
//
//		CString sID;
//		BSTR bstrText;
//		// step2�����ƴ��и��
//		sID.Format(_T("��׼��%d"),pHCombParams->m_nRefPlaneID);
//		bstrText = sID.AllocSysString();
//		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//
//		CComPtr<ISketchSegment> swSketchLine;
//		swSketMgr->CreateCenterLine(pPathParam->m_dOffsetX,0,0,pPathParam->m_dOffsetX,1,0,&swSketchLine);
//		pHCombParams->m_nSketID++;
//
//		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
//		sID.Format(_T("Line1@��ͼ%d"),pHCombParams->m_nSketID);
//		bstrText = sID.AllocSysString();
//		swDocExt->SelectByID2(bstrText, L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
//		if (VARIANT_FALSE == retval)
//		{
//			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
//			iSwModel = NULL;
//			return;
//		}
//
//		CComQIPtr<IDispatch> pRefPlaneForThough;
//		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, (PI*0.5+pPathParam->m_dThroughAng), 
//			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlaneForThough);
//		pHCombParams->m_nRefPlaneID++;
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//
//		sID.Format(_T("��׼��%d"),pHCombParams->m_nRefPlaneID);
//		bstrText = sID.AllocSysString();
//		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//
//		// ��������ϵ
//// 		CComPtr<ISketchSegment> swSketchLine1;
//// 		swSketMgr->CreateCenterLine(0,0,0,0,0,1,&swSketchLine1);
//// 		CComPtr<ISketchSegment> swSketchLineX;
//// 		swSketMgr->CreateCenterLine(0,0,0,1.23,0,0,&swSketchLineX);
//// 		CComPtr<ISketchSegment> swSketchLineY;
//// 		swSketMgr->CreateCenterLine(0,0,0,0,2,0,&swSketchLineY);
//
//		//////////////////////////////////////////////////////////////////////////
//		// ����Ϊ����Xƫ��֮����Ĳ�ͬ�������������ִ��룬����ʵ���������һ��ʹ�ܾͿ���
//		// 1.Xƫ�����Ϊ�ᴩ�����Բ����ͶӰ��ƽ��󽻵�ľ���
//// 		CComPtr<ISketchSegment> swSketchHole;
//// 		PNT3D ptCenter;
//// 		ptCenter[0] = pPathParam->m_dCenterDis;
//// 		ptCenter[1] = cos(PI*0.5-pPathParam->m_dThroughAng)*(pPathParam->m_dOffsetX);
//// 		ptCenter[2] = 0;
//// 		swSketMgr->CreateCircleByRadius(ptCenter[0],ptCenter[1], ptCenter[2], pPathParam->m_dHoleR, &swSketchHole); 
//
//		// 2.Xƫ�����Ϊ�ᴩ������������Բ����ľ���
//		CComPtr<ISketchSegment> swSketchHole;
//		double dis = sqrt((pHCombParams->m_dTubeDia*0.5)*(pHCombParams->m_dTubeDia*0.5)-
//			pPathParam->m_dCenterDis*pPathParam->m_dCenterDis);
//		double dis2 = 1./tan(pPathParam->m_dThroughAng)*dis;
//		double dCenterXOffset = dis2+pPathParam->m_dOffsetX;
//		PNT3D ptCenter;
//		ptCenter[0] = pPathParam->m_dCenterDis;
//		ptCenter[1] = cos(PI*0.5-pPathParam->m_dThroughAng)*dCenterXOffset;
//		ptCenter[2] = 0;
//		swSketMgr->CreateCircleByRadius(ptCenter[0], ptCenter[1], ptCenter[2], pPathParam->m_dHoleR, &swSketchHole);
//		//////////////////////////////////////////////////////////////////////////
//		pHCombParams->m_nSketID++;
//		pHCombParams->m_nHoleID++;
//		swSketMgr->InsertSketch(VARIANT_TRUE);
//		iSwModel->FeatureCut(1,0,VARIANT_FALSE,0,0,m_dTubeDia/10.,m_dTubeDia/10.,0,0,0,0,2,2,0,0);
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//	// ���´���������������ᴩ�Ƕ�Ϊ90�ȵĿף���ʱ�ȱ���
//	//////////////////////////////////////////////////////////////////////////
//// 	int nHoleID = 0;
//// 	POSITION paraPos = pHCombParamList->m_LHParamList.GetHeadPosition();
//// 	while(paraPos)
//// 	{
//// 		CHoleParam* pPathParam = pHCombParamList->m_LHParamList.GetNext(paraPos);
//// 		if(NULL == pPathParam)
//// 			continue;
//// 		if (nHoleID < pHCombParamList->m_nHoleID)
//// 		{// ��ֹ�ظ��������ӵĿ�
//// 			nHoleID++;
//// 			continue;
//// 		}
//// 
//// 		nHoleID++;
//// 		// step1��������׼��		
//// 		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);
//// 		if (VARIANT_FALSE == retval)
//// 		{
//// 			AfxMessageBox(_T("�޷�ѡ�����ӻ�׼�棬�޷����ɻ�׼��"));
//// 			iSwModel = NULL;
//// 			return;
//// 		}
//// 
//// 		swDocExt->SelectByID2(L"Line1@��ͼ2", L"EXTSKETCHSEGMENT", 0, 0, 0, True, 1, NULL, 0, &retval);
//// 		if (VARIANT_FALSE == retval)
//// 		{
//// 			AfxMessageBox(_T("�޷�ѡ�������ߣ��޷����ɻ�׼��"));
//// 			iSwModel = NULL;
//// 			return;
//// 		}
//// 
//// 		CComQIPtr<IDispatch> pRefPlane;
//// 		swFeatmgr->InsertRefPlane(swRefPlaneReferenceConstraint_Angle, pPathParam->m_dRotAng, 
//// 			swRefPlaneReferenceConstraint_Coincident, 0, 0, 0,&pRefPlane);
//// 		pHCombParamList->m_nHoleID++;
//// 
//// 		// step2�����ƴ��и��
//// 		CString sID;
//// 		sID.Format(_T("��׼��%d"),pHCombParamList->m_nHoleID);
//// 		//BSTR bstrText = _bstr_t(s); 
//// 		BSTR bstrText = sID.AllocSysString();
//// 		iSwModel->SelectByID(bstrText,_T("PLANE"),0.,0.,0.,&retval);
//// 		swSketMgr->InsertSketch(VARIANT_TRUE);
//// 
//// 		CComPtr<ISketchSegment> swSketchHole;
//// 		swSketMgr->CreateCircleByRadius(pPathParam->m_dOffsetX/*����X��ƫ��*/,pPathParam->m_dCenterDis/*�������ľ�*/,
//// 			                            0., pPathParam->m_dHoleR/*����װ뾶*/,&swSketchHole); 
//// 		if (NULL == swSketchHole)
//// 		{
//// 			AfxMessageBox(_T("��%d���״���ʧ�ܡ�"),pHCombParamList->m_nHoleID);
//// 		}
//// 		swSketMgr->InsertSketch(VARIANT_TRUE);
//// 
//// 		iSwModel->FeatureCut(1,0,VARIANT_TRUE,0,0,m_dTubeDia/1000.,m_dTubeDia/1000.,0,0,0,0,2,2,0,0);
//// 		
//// 	}
//	//////////////////////////////////////////////////////////////////////////
//
//	// end:
//	iSwModel->SetAddToDB(VARIANT_FALSE);
//	iSwModel->SetDisplayWhenAdded(VARIANT_TRUE);
//	//////////////////////////////////////////////////////////////////////////
//
//	iSwModel = NULL;
//	return;
//}

// ����·��
void CPMPageHandler::CalMovePath()
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

	CComPtr<ISelectionMgr>swSelectionMgr;
	iSwModel->get_ISelectionManager(&swSelectionMgr);
	long count = 0;
	swSelectionMgr->GetSelectedObjectCount(&count);
	if (count < 2)
	{
		AfxMessageBox(_T("�밴��ʾѡ�������"));
		return;
	}

	long type = -1;
	CComQIPtr<IFace2>swBaseTubeFace;
	CComPtr<IUnknown>swUnkonwnObject;
	BOOL bFineBasetube = FALSE;
	for (int i=0; i<count; i++)
	{
		swSelectionMgr->GetSelectedObjectType2(i+1,&type);
		if (swSelFACES != type)
			continue;
		bFineBasetube = TRUE;
		swSelectionMgr->IGetSelectedObject4(i+1,&swUnkonwnObject);
		swBaseTubeFace = swUnkonwnObject;
	}
	if (!bFineBasetube)
	{
		AfxMessageBox(_T("Ϊѡ��������棡"));
		iSwModel = NULL;
		return;
	}
	CComQIPtr<ISurface> swSurface;
	swBaseTubeFace->IGetSurface(&swSurface);
	CPathComb* pPathComb = new CPathComb();
	pPathComb->m_bTwiceCut = m_bTwiceCut;
	for (int i=0; i<count; i++)
	{
		type = -1;
		swUnkonwnObject = NULL;
		swSelectionMgr->GetSelectedObjectType2(i+1,&type);
		swSelectionMgr->IGetSelectedObject4(i+1,&swUnkonwnObject);
		if(swSelEDGES != type)
			continue;
		CMovePath* movePath = new CMovePath;

		CComQIPtr<IEdge>swEdge(swUnkonwnObject);
		CComPtr<ICurve>swCurve;
		swEdge->IGetCurve(&swCurve);

		double dParam[11];
		swEdge->IGetCurveParams2(dParam);
		double dChordTol = m_dChodTol/1000.; // �Ҹ����
		double dStepTol  = m_dStepTol/1000.; // ��������

		PNT3D ptStart, ptEnd;
		for (int i=0; i<3; i++)
		{
			ptStart[i] = dParam[i];
			ptEnd[i]   = dParam[i+3];
		}

		// ���ӹ��׵ı��߽�����ɢ
		LONG ptNum = 0;
		swCurve->IGetTessPtsSize(dChordTol,dStepTol,ptStart,ptEnd,&ptNum);
		double* ptArray = new double[ptNum];
		swCurve->IGetTessPts(dChordTol,dStepTol,ptStart,ptEnd,ptArray);
		CalPathNode(ptNum, ptArray, swSurface, movePath);

		// �жϵ�ǰ·��ƫ�Ʒ����Ƿ���ȷ
		if (!CheckPathComb(movePath))
		{
			if (NULL == movePath)
			{
				if (ptArray != NULL)
				{
					delete[] ptArray;
					ptArray = NULL;
				}				
				return ;
			}
			movePath->Realse();
			CalPathNode(ptNum, ptArray, swSurface, movePath, TRUE);
		}
		pPathComb->AddPath(movePath);
		if (ptArray != NULL)
		{
			delete[] ptArray;
			ptArray = NULL;
		}		
	}
	BSTR sFileName;
	iSwModel->GetTitle(&sFileName);
	BOOL bFind = FALSE;
	CPathCombList* pPathCombList = NULL;
	POSITION entPos = userAddin->m_LPathEntityList.GetHeadPosition();
	while(entPos)
	{
		pPathCombList = userAddin->m_LPathEntityList.GetNext(entPos);
		if (NULL == pPathCombList)
			continue;
		_bstr_t filename(sFileName), pathname(pPathCombList->m_sPathCombName);
		if (filename != pathname)
			continue;
		else
		{
			bFind = TRUE;
			break;
		}
	}
	if (!bFind)
	{
		pPathCombList = NULL;
	}
	if (NULL == pPathCombList)
	{
		pPathCombList = new CPathCombList(sFileName);
		userAddin->m_LPathEntityList.AddTail(pPathCombList);
	}
	pPathCombList->m_LPathCombList.AddTail(pPathComb);

 	// ͨ��3D��ͼ����ʽ������·��
	//////////////////////////////////////////////////////////////////////////
 	iSwModel->Insert3DSketch2(VARIANT_TRUE);
 	iSwModel->SetAddToDB(VARIANT_TRUE);
	
 	CComPtr<ISketchManager> iswSketchManger;
 	iSwModel->get_SketchManager(&iswSketchManger);
 	CComPtr<ISketchSegment> iSegment; 	
 	
	POSITION pos = pPathComb->m_PathList.GetHeadPosition();
	while(pos)
	{
		CMovePath* pMovePath = pPathComb->m_PathList.GetNext(pos);
		if(NULL == pMovePath)
			continue;
		POSITION nodepos = pMovePath->m_PathNodeList.GetHeadPosition();
		while(nodepos)
		{
			CPathNode* pNode = pMovePath->m_PathNodeList.GetNext(nodepos);
			iSegment = NULL;
			if (pNode == NULL)
				continue;
			iSegment = NULL;
			//iswSketchManger->CreateCircle()
			iswSketchManger->CreateLine(pNode->m_OffsetPosition[0],pNode->m_OffsetPosition[1],pNode->m_OffsetPosition[2],
				pNode->m_OffsetDrawEndPnt[0],pNode->m_OffsetDrawEndPnt[1],pNode->m_OffsetDrawEndPnt[2],&iSegment);
			if (m_bTwiceCut)
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
			if (m_bTwiceCut)
			{
				iSegment = NULL;
				iswSketchManger->CreateLine(pNode->m_OrgCutPosition[0],pNode->m_OrgCutPosition[1],pNode->m_OrgCutPosition[2],
					pNextNode->m_OrgCutPosition[0],pNextNode->m_OrgCutPosition[1],pNextNode->m_OrgCutPosition[2],&iSegment);
			}
		}
	}	

 	iSwModel->SetAddToDB(VARIANT_FALSE);
 	iSwModel->Insert3DSketch2(VARIANT_FALSE);
	VARIANT_BOOL bret;
	long color = 100;
	CComPtr<IModelDocExtension> swExt;
	iSwModel->get_Extension(&swExt);
	iSwModel->SelectedFeatureProperties(color,0,0,0,0,0,0,VARIANT_FALSE,VARIANT_FALSE,L"3D��ͼ1",&bret);
	swExt->SelectByID2(L"3D��ͼ1",L"SKETCH",0,0,0,VARIANT_FALSE,0,NULL,swSelectOptionDefault,&bret);

	//		swDocExt->SelectByID2(_T("���ӻ�׼��"), L"PLANE", 0, 0, 0, True, 0, NULL, swSelectOptionDefault,&retval);

	//////////////////////////////////////////////////////////////////////////

	iSwModel = NULL;
	return;
}