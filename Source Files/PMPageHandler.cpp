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
	if (NULL != userAddin)
	{
		iSwApp = userAddin->GetSldWorksPtr();
	}
	else
	{
		AfxMessageBox(_T("��ǰ�ĵ�App��Ϊ�գ���رյ�ǰ�ĵ������ؽ��ĵ���"));
	}

	m_dCutAng   = 30.;
	m_dCutDepth = 10.;
	m_dChodTol  = 0.01;
	m_dStepTol  = 0.01;
	m_dToolDis  = 3. ;
	m_bHolePrecut = TRUE;
	m_bStrHole = FALSE;
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
	dlg.m_dCutDepth = m_dCutDepth;
	dlg.m_dChodTol = m_dChodTol;
	dlg.m_dStepTol = m_dStepTol;
	dlg.m_dToolDis = m_dToolDis;
	dlg.m_bHolePrecut = m_bHolePrecut;
	dlg.m_bStrHole = m_bStrHole;
	if (IDOK == dlg.DoModal())
	{
		m_dCutAng = dlg.m_dCutAng;
		m_dCutDepth = dlg.m_dCutDepth;
		m_dChodTol = dlg.m_dChodTol;
		m_dStepTol = dlg.m_dStepTol;
		m_dToolDis = dlg.m_dToolDis;
		m_bHolePrecut = dlg.m_bHolePrecut;
		m_bStrHole = dlg.m_bStrHole;
	}
}

BOOL CPMPageHandler::CheckOffsetVec(CMovePath* pPath, int bFlag)
{
	if (NULL == pPath)
		return FALSE;
	POSITION offptPos = pPath->m_PathNodeList.GetHeadPosition();
	while(offptPos)
	{
		CPathNode* pOffNode = pPath->m_PathNodeList.GetNext(offptPos);
		if (NULL == pOffNode)
			continue;
		double maxDis = 0.;
		POSITION orgptPos = pPath->m_PathNodeList.GetHeadPosition();
		while(orgptPos)
		{
			CPathNode* pOrgNode = pPath->m_PathNodeList.GetNext(orgptPos);
			if (NULL == pOrgNode)
				continue;

			switch(bFlag)
			{
			case PROJPLANE_YZ:
				pOffNode->m_OffsetPosition[0] = 0.;
				pOffNode->m_OrgCutPosition[0] = 0.;
				pOrgNode->m_OffsetPosition[0] = 0.;
				pOrgNode->m_OrgCutPosition[0] = 0.;
				break;
			case PROJPLANE_XY:
				pOffNode->m_OffsetPosition[2] = 0.;
				pOffNode->m_OrgCutPosition[2] = 0.;
				pOrgNode->m_OffsetPosition[2] = 0.;
				pOrgNode->m_OrgCutPosition[2] = 0.;
				break;
			case PROJPLANE_XZ:
				pOffNode->m_OffsetPosition[1] = 0.;
				pOffNode->m_OrgCutPosition[1] = 0.;
				pOrgNode->m_OffsetPosition[1] = 0.;
				pOrgNode->m_OrgCutPosition[1] = 0.;
				break;
			}		

			double dis = mathDis3D(pOffNode->m_OffsetPosition, pOrgNode->m_OrgCutPosition);
			if (dis > maxDis)
			{
				maxDis = dis;
			}

			if (dis>((pPath->GetHParam()->m_dHoleR*2.)+MIN_LEN))
			{
				return TRUE;
			}
			continue;
		}
	}
	return FALSE;
}

// ����·���ڻ�׼���ϵ�ͶӰ�������ж�
BOOL CPMPageHandler::CheckPathComb(CMovePath* pPath)
{
	if (NULL == pPath)
		return FALSE;

	// ��·���任����ת�������ϵ��
	//////////////////////////////////////////////////////////////////////////
	RFRAME pathFrame;
	mathInitRFrame(pathFrame);
	PNT3D rotPnt = {0., 0., 0.};
	VEC3D rotX = {1.,0.,0.};
	VEC3D rotZ = {0.,0.,1.};
	mathRotateRFrame(rotPnt,rotX,-pPath->GetOrgRotAng(TRUE),pathFrame);
	mathRotVec(rotX,rotPnt,-pPath->GetOrgRotAng(TRUE),rotZ,rotZ);
	mathRotateRFrame(rotPnt,rotZ,pPath->GetHParam()->m_dThroughAng,pathFrame);
	CMovePath* tmpPath = pPath->CopySelf();
	userAddin->TransWorldPath(pathFrame,tmpPath);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ͨ���ж�ƫ�Ƶ��ԭʼ����YZƽ���ϵ�ͶӰ�У�ƫ�Ƶ��Ƿ���ԭʼ���ڲ������ж�
	// ƫ�Ʒ����Ƿ����
	//////////////////////////////////////////////////////////////////////////
	BOOL bRet = CheckOffsetVec(tmpPath,PROJPLANE_YZ);
	if (NULL != tmpPath)
	{
		tmpPath->Realse();
		delete tmpPath;
		tmpPath = NULL;
	}
	return bRet;
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

//������ԭʼ·���㣬��ȡ��Ӧ�Ŀײ���
CHoleParam* CPMPageHandler::GetHParam(int ptNum, double* ptArray)
{
	CHCombParam* pPathParamComb = GetCurHoleParams();
	if (NULL == pPathParamComb || NULL == ptArray)
		return NULL;
	// �����ϴ���˴��Ĳ����飬Ӧ��û��Xƫ��ֵ��ȵ��������Ϊ�������Ѿ�
	// �ڴ�list���ȡ�ײ���,�����������ʱ�򣬾��Ѿ����иı䲢��ӱ���ˡ�
	// ��ƽ��ֵ��ʹ�����е㣬x�����ֵ��x����Сֵ�ĺͳ���2��
	BOOL bFindPara = FALSE;
	double dXMax = -MAX_DBL;
	double dXMin = MAX_DBL;
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
		AfxMessageBox(_T("·��δ�ҵ�ƥ�������"));
	}
	return pPathParam;
}

// ���ڼ���·��ƫ�ƺ�Ľڵ�
void CPMPageHandler::CalPathNode(int ptNum, double* ptArray, ISurface* swSurface, CMovePath* pMovePath,BOOL bClosed, BOOL bRevOffVec)
{
	// ��һ�����ȼ���ԭʼ·�������ڲ���ƥ�䣬�����Ӧ�Ĺᴩ����
	//////////////////////////////////////////////////////////////////////////
	CHoleParam* pHParam = GetHParam(ptNum,ptArray);
	if (NULL == pHParam)
	{
		AfxMessageBox(_T("�ײ�����ȡʧ�ܣ���ǰ�ĵ��������⣬���ؽ��ĵ���"));
		return;
	}
	pMovePath->SetHParam(pHParam);
	//////////////////////////////////////////////////////////////////////////
	BOOL bHolePrecut = pMovePath->m_bHolePrecut;
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

		PNT3D vecStart;
		for (int i=0; i<3; i++)
		{
			vecStart[i] = retFacePt[i];
			pNode->m_OrgPosition[i] = retFacePt[i];
		}

		//////////////////////////////////////////////////////////////////////////
		// �����ֱ��ʱ��ֻ�轫ԭʼ�ױ�Ե��������ռ���
		//////////////////////////////////////////////////////////////////////////
		if (pMovePath->m_bStrHole)
		{
			for (int i=0; i<3; i++)
			{//��ͷ���վ���
				pNode->m_OrgCutPosition[i] = pNode->m_OrgPosition[i]+0.001*pNode->m_OrgDirection[i]*m_dToolDis;
			}

			pMovePath->m_PathNodeList.AddTail(pNode);
			continue;
		}
		//////////////////////////////////////////////////////////////////////////

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
		double dis1 = dRWTube*dRWTube - ptCDis*ptCDis;
		if (abs(dis1)<MIN_LEN )
		{
			dis1 = 0;
		}
		if (dRWTube<ptCDis)
		{
			AfxMessageBox(_T("��ǰ�ײ����޷����ɼӹ�·�������ؽ��ĵ���"));
			return;
		}

		double dis2 = dRNTube*dRNTube - ptCDis*ptCDis;
		if (abs(dis2)<MIN_LEN )
		{
			dis2 = 0;
		}
		if (dRNTube<(ptCDis-MIN_LEN))
		{
			dis2 = 0.;
		}

		double dZThick = sqrt(dis1) - sqrt(dis2);
		double dHThick = dZThick/sin(pHParam->m_dThroughAng); // ���ڹ��ϵ������

		double dCutDepth = m_dCutDepth/1000;//�¿���ȣ��û����ã�������dHThick�Զ���ΪdHThick
		if (dCutDepth>dHThick || !bHolePrecut) // ����û�û��Ԥ���пף���ֱ�����¿ڣ��и����Ĭ�ϵ���
		{
			dCutDepth = dHThick;
		}

		// ��ʱ˵�����и�ܱڣ��ײ����Զ��������������������Ԥ�и�ײ�������ʱ�и����Ĭ��Ϊ�ں�
		if (m_bHolePrecut != bHolePrecut)
		{
			dCutDepth = pMovePath->GetHParam()->GetParentComb()->m_dTubeThick;
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
		//// 1.����ǰ�����п�ˮƽ�ķ���ƫ�ƺ����õ��¿ڿ����ͬ�ľ��룬��ƫ�ƺ�ĵ㡣
		//PNT3D tmpPt;
		//for (int i=0; i<3; i++)
		//{
		//	tmpPt[i] = vecStart[i]+0.001*offsetVec[i]*0.001;
		//}
		//swSurface->IGetClosestPointOn(tmpPt[0],tmpPt[1],tmpPt[2],retFacePt);
		//// 2.��ԭʼ���ƫ�ƺ�ĵ㹹�������ϵ�����
		//VEC3D tmpVec;
		//for (int i=0; i<3; i++)
		//{
		//	tmpVec[i] = retFacePt[i]-pNode->m_OrgPosition[i];
		//}
		//mathUniVec(tmpVec);
		//// 3.�������������ᴩ�����ļн�
		//double tmpAng = mathGetAngle(tmpVec,pHParam->m_dThroughVec,MIN_DBL);
		//// 4.�жϼнǣ����С��45�ȣ���ʹ�üнǵĶ���֮һ
		//if (tmpAng<(90.*PI/180.-0.001))
		//{
		//	pokouAng = tmpAng*0.5;
		//}
		//////////////////////////////////////////////////////////////////////////

		VEC3D pokouVec;
		mathRotVecXY(dThroughVec,offsetVec,pokouAng,pokouVec);
		mathUniVec(pokouVec);

		//////////////////////////////////////////////////////////////////////////
		// ��۱ߵ�
		PNT3D ptDun; // ʵ�ʶ۱ߵ�����
	//	PNT3D ptBaseDun;// �׻��߶۱ߵ�����
		for (int i=0; i<3; i++)
		{
			ptDun[i] = pNode->m_OrgPosition[i]-dCutDepth*pHParam->m_dThroughVec[i];
		}
// 		for (int i=0; i<3; i++)
// 		{
// 			ptBaseDun[i] = pNode->m_OrgPosition[i]-dHThick*pHParam->m_dThroughVec[i];
// 		}
		//////////////////////////////////////////////////////////////////////////
		// ���¿���ܵĽ���
		//////////////////////////////////////////////////////////////////////////
		// 1.���¿�������YZƽ���ϵ�ͶӰ����
		VEC3D YZVec = {0.,pokouVec[1], pokouVec[2]};
		mathUniVec(YZVec);
		// 2.���¿�������YZ�����ļн�
		double vecAng = mathGetAngle(pokouVec,YZVec,MIN_DBL);

		// ����ʵ���¿ڵĳ���
		//////////////////////////////////////////////////////////////////////////
		PNT3D pt2DCenter = {0.,0.,0.};
		PNT3D pt2DDun = {0,ptDun[1],ptDun[2]};
		double d2DCDis = mathDistPntLin(pt2DCenter,pt2DDun,YZVec);
		double dDis1 = dRWTube;
		double dDis2 = mathDis3D(pt2DDun,pt2DCenter);
		double dLengthPK = sqrt(dDis1*dDis1 - d2DCDis*d2DCDis) - sqrt(dDis2*dDis2 - d2DCDis*d2DCDis);
		//////////////////////////////////////////////////////////////////////////

		/*/////////////////////////////////////////////////////////////////////////
		// 3.��YZƽ���ϣ������¿�����������ƫ��
		PNT3D p={0,0,0};
		PNT3D p1= {0,ptBaseDun[1],ptBaseDun[2]};
		double d = mathDistPntLin(p,p1,YZVec);
		// 4.��������¿���YZƽ���ϵ�ͶӰ����
		if (d>dRNTube)
		{
			AfxMessageBox(_T("error"));
		}
		double dLengthPK = sqrt(dRWTube*dRWTube - d*d) - sqrt(dRNTube*dRNTube - d*d);
		// 5.ʵ���¿ڵĳ���
		double dRealPkL = dLengthPK*(dCutDepth)/dHThick;

		*//////////////////////////////////////////////////////////////////////////

		// 6.ͨ��ͶӰ���Ⱥͼнǣ����ԭʼ����
		double dOffsetVecLength = dLengthPK/cos(vecAng);
		for(int i=0; i<3; i++)
		{
			vecStart[i] = ptDun[i]+pokouVec[i]*dOffsetVecLength;
		}
		//////////////////////////////////////////////////////////////////////////

		// ����������
		for (int i=0; i<3; i++)
		{
			pNode->m_OffsetPosition[i]   = vecStart[i];
			pNode->m_OffsetDirection[i]  = pokouVec[i];
		}

		//��ͷ���վ���
		for (int i=0; i<3; i++)
		{
			pNode->m_OrgCutPosition[i] = pNode->m_OrgPosition[i]+0.001*pNode->m_OrgDirection[i]*m_dToolDis;
			pNode->m_OffsetPosition[i] = pNode->m_OffsetPosition[i]+0.001*pNode->m_OffsetDirection[i]*m_dToolDis;
		}

		pMovePath->m_PathNodeList.AddTail(pNode);
	}

	// ������߱պϣ���·���׵㣬��ӵ�·��ĩβ��ʹ��ĩ���غ�
	if (bClosed)
	{
		pMovePath->m_PathNodeList.AddTail(pMovePath->m_PathNodeList.GetHead()->CopySelf());
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
		AfxMessageBox(_T("�򿪵Ĳ��������"),MB_OK|MB_ICONINFORMATION);
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
		AfxMessageBox(_T("���ĵ�����·���������"));
		return;
	}
	pPathCombList->Realse();//�ͷŵ�ǰ�ĵ��е�·��
	//////////////////////////////////////////////////////////////////////////
	
	AfxMessageBox(_T("���ĵ�����·���������"));
}

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
		AfxMessageBox(_T("�򿪵��ĵ����������"),MB_OK|MB_ICONINFORMATION);
		iSwModel = NULL;
		return ;
	}

	CComPtr<ISelectionMgr>swSelectionMgr;
	iSwModel->get_ISelectionManager(&swSelectionMgr);
	long count = 0;
	swSelectionMgr->GetSelectedObjectCount(&count);
	if (count < 2)
	{
		AfxMessageBox(_T("�밴��ʾѡ������ߡ�"));
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
		AfxMessageBox(_T("Ϊѡ��������档"));
		iSwModel = NULL;
		return;
	}
	CComQIPtr<ISurface> swSurface;
	swBaseTubeFace->IGetSurface(&swSurface);
	CPathComb* pPathComb = new CPathComb();
	for (int i=0; i<count; i++)
	{
		type = -1;
		swUnkonwnObject = NULL;
		swSelectionMgr->GetSelectedObjectType2(i+1,&type);
		swSelectionMgr->IGetSelectedObject4(i+1,&swUnkonwnObject);
		if(swSelEDGES != type)
			continue;

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

		PNT3D tmpSt, tmpEnd;
		VARIANT_BOOL bIsClosed, bIsPeriodic, bRet;
		swCurve->GetEndParams(tmpSt,tmpEnd,&bIsClosed, &bIsPeriodic,&bRet);
		CMovePath* pMovePath = new CMovePath;
		pMovePath->m_bHolePrecut = m_bHolePrecut;
		pMovePath->m_bStrHole = m_bStrHole;
		BOOL bClosed = bIsClosed;
		CalPathNode(ptNum, ptArray, swSurface, pMovePath,bClosed);

		// �жϵ�ǰ·��ƫ�Ʒ����Ƿ���ȷ(�и�ֱ��ʱ�����ж�)
		if ((!m_bStrHole)&&!CheckPathComb(pMovePath))
		{
			if (NULL == pMovePath)
			{
				if (ptArray != NULL)
				{
					delete[] ptArray;
					ptArray = NULL;
				}				
				return ;
			}
			pMovePath->Realse();
			CalPathNode(ptNum, ptArray, swSurface, pMovePath, bClosed,TRUE);
		}
		pPathComb->AddPath(pMovePath);
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
 	userAddin->DrawPathComb(pPathComb);
	
	iSwModel = NULL;
	return;
}