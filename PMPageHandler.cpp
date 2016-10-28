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
		AfxMessageBox(_T("当前文档App类为空，请关闭当前文档，并重建文档。"));
	}

	m_dCutAng   = 45.;
	m_dCutWidth = 10.;
	m_dChodTol  = 0.01;
	m_dStepTol  = 0.01;
	m_dToolDis  = 3. ;
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

// <out>dLength[0]:获取未偏移的路径长度；
//      dLength[1]:获取偏移后的路径长度；
// <in> bClosed:TRUE表示路径闭合，FALSE表示路径非闭合；
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
		
		// 第二点为空时，如果曲线闭合，则取路径首点作为第二点
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

// 根据偏移后的路径长度，来判断路径的偏移方向是否正确
BOOL CheckPathComb(CMovePath* pPath)
{
	if (NULL == pPath)
		return FALSE;
	double dLength[2] = {0., 0.};
	GetPathLength(pPath,dLength);
	// 如果原始的曲线长度大于偏移后的曲线长度，说明偏移方向错了
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

//　根据原始路径点，获取对应的孔参数
CHoleParam* CPMPageHandler::GetHParam(int ptNum, double* ptArray)
{
	CHCombParam* pPathParamComb = GetCurHoleParams();
	if (NULL == pPathParamComb || NULL == ptArray)
		return NULL;
	// 理论上传入此处的参数组，应该没有X偏移或Y偏移值相等的情况，因为这个情况已经在从list里读取孔参数
	// 创建参数组的时候，就已经进行改变并添加标记了。
	//  先通过y平均值寻找对应参数组，如果找不到，再通过y平均值寻找
	//  （如果通过y平均值找到的参数组有修改标记，不需变回来，因为y不影响输出了）
	// 求平均值，使用所有点，x或y的最大值和x或y的最小值的和除以2。
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
	// step1：通过x平均值，找对应参数
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
		AfxMessageBox(_T("路径未找到匹配参数."));
	}
	return pPathParam;
}

#define MIN_DBL 1.0e-10
extern double mathDistPntLin(PNT3D p, PNT3D begin, VEC3D dir);
extern double mathGetAngle(VEC3D v1, VEC3D v2, double min_len);

// 用于计算路径偏移后的节点
void CPMPageHandler::CalPathNode(int ptNum, double* ptArray, ISurface* swSurface, CMovePath* movePath, BOOL bRevOffVec)
{
	// 第一步：先计算原始路径，用于参数匹配，求出对应的贯穿方向
	//////////////////////////////////////////////////////////////////////////
	CHoleParam* pHParam = GetHParam(ptNum,ptArray);
	if (NULL == pHParam)
	{
		AfxMessageBox(_T("孔参数获取失败，当前文档存在问题，请重建文档。"));
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
		// 求切孔边缘深度
		//////////////////////////////////////////////////////////////////////////
		// 1.点轴心据，通过图形可知，点的轴心据，就是点的Z坐标值
		// 2.点对应的Z轴方向壁厚
		if (NULL == pHParam->GetParentComb())
		{
			AfxMessageBox(_T("当前文档孔数据参数存在问题，请重建文档。"));
			return;
		}
		PNT3D pt1={0,0,0};
		PNT3D pt2 = {0,pNode->m_OrgPosition[1],pNode->m_OrgPosition[2]};
		VEC3D vec = {0,pHParam->m_dThroughVec[1],pHParam->m_dThroughVec[2]};
		double ptCDis = mathDistPntLin(pt1,pt2,vec);
		double dRWTube = pHParam->GetParentComb()->m_dTubeDia*0.5;
		double dRNTube = pHParam->GetParentComb()->m_dTubeDia*0.5 - pHParam->GetParentComb()->m_dTubeThick;
		double dZThick = sqrt(dRWTube*dRWTube - ptCDis*ptCDis) - sqrt(dRNTube*dRNTube - ptCDis*ptCDis);
		double dHThick = dZThick/sin(pHParam->m_dThroughAng); // 孔在管上的总深度

		double dDunDeep = m_dCutWidth/1000;////坡口深度，用户设置，超过dHThick，则提醒或者自动设为dHThick
		if (dDunDeep>dHThick)
		{
			dDunDeep = dHThick;
		}
		//////////////////////////////////////////////////////////////////////////

		// 当为闭合曲线时，当前点为最后一个点时，下一个点使用曲线的起点
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
		
		// 判断是否需要反向偏移方向
		if (bRevOffVec)
		{
			for (int i=0; i<3; i++)
			{
				offsetVec[i] = -offsetVec[i];
			}
		}

		double pokouAng = PI/180.*m_dCutAng; // 用户设置的坡口角度

		//　求主管与支管的坡口处夹角
		//////////////////////////////////////////////////////////////////////////
		// 1.将当前点沿切口水平的方向偏移和设置的坡口宽度相同的距离，求偏移后的点。
		PNT3D tmpPt;
		for (int i=0; i<3; i++)
		{
			tmpPt[i] = vecStart[i]+0.001*offsetVec[i]*0.001;
		}
		swSurface->IGetClosestPointOn(tmpPt[0],tmpPt[1],tmpPt[2],retFacePt);
		// 2.用原始点和偏移后的点构成主管上的向量
		VEC3D tmpVec;
		for (int i=0; i<3; i++)
		{
			tmpVec[i] = retFacePt[i]-pNode->m_OrgPosition[i];
		}
		mathUniVec(tmpVec);
		// 3.求出主管向量与贯穿向量的夹角
		double tmpAng = mathGetAngle(tmpVec,pHParam->m_dThroughVec,MIN_DBL);
		// 4.判断夹角，如果小于45度，则使用夹角的二分之一
		if (tmpAng<(90.*PI/180.-0.001))
		{
			pokouAng = tmpAng*0.5;
		}
		//////////////////////////////////////////////////////////////////////////


		VEC3D pokouVec;
		mathRotVecXY(dThroughVec,offsetVec,pokouAng,pokouVec);
		mathUniVec(pokouVec);

		//////////////////////////////////////////////////////////////////////////
		// 求钝边点
		PNT3D ptDun; // 实际钝边点坐标
		PNT3D ptBaseDun;// 孔基线钝边点坐标
		for (int i=0; i<3; i++)
		{
			ptDun[i] = pNode->m_OrgPosition[i]-dDunDeep*pHParam->m_dThroughVec[i];
		}
		for (int i=0; i<3; i++)
		{
			ptBaseDun[i] = pNode->m_OrgPosition[i]-dHThick*pHParam->m_dThroughVec[i];
		}
		//////////////////////////////////////////////////////////////////////////
		// 求坡口与管的交点
		//////////////////////////////////////////////////////////////////////////
		// 1.求坡口向量在YZ平面上的投影向量
		VEC3D YZVec = {0.,pokouVec[1], pokouVec[2]};
		mathUniVec(YZVec);
		// 2.求坡口向量与YZ分量的夹角
		double vecAng = mathGetAngle(pokouVec,YZVec,MIN_DBL);
		// 3.求YZ平面上，基线坡口向量的轴心距离
		PNT3D p={0,0,0};
		PNT3D p1= {0,ptBaseDun[1],ptBaseDun[2]};
		double d = mathDistPntLin(p,p1,YZVec);
		// 4.计算基线坡口在YZ平面上的投影长度
		double dPkL = sqrt(dRWTube*dRWTube - d*d) - sqrt(dRNTube*dRNTube - d*d);
		// 5.实际坡口的长度
		double dRealPkL = dPkL*(dDunDeep)/dHThick;
		// 6.通过投影长度和夹角，求出原始长度
		double dOffsetVecLength = dRealPkL/cos(vecAng);
		for(int i=0; i<3; i++)
		{
			vecStart[i] = ptDun[i]+pokouVec[i]*dOffsetVecLength;
		}
		//////////////////////////////////////////////////////////////////////////

		// 法向量标线
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

		//刀头悬空距离
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

// 删除当前激活文档中的所有路径
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
		AfxMessageBox(_T("打开的不是零件。"),MB_OK|MB_ICONINFORMATION);
		iSwModel = NULL;
		return ;
	}

	//查找当前文档中的路径，并释放
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
		AfxMessageBox(_T("该文档所有路径已清除。"));
		return;
	}
	pPathCombList->Realse();//释放当前文档中的路径
	//////////////////////////////////////////////////////////////////////////
	
	AfxMessageBox(_T("该文档所有路径已清除。"));
}

// 计算路径
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
		AfxMessageBox(_T("打开的文档不是零件。"),MB_OK|MB_ICONINFORMATION);
		iSwModel = NULL;
		return ;
	}

	CComPtr<ISelectionMgr>swSelectionMgr;
	iSwModel->get_ISelectionManager(&swSelectionMgr);
	long count = 0;
	swSelectionMgr->GetSelectedObjectCount(&count);
	if (count < 2)
	{
		AfxMessageBox(_T("请按提示选择面和线。"));
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
		AfxMessageBox(_T("为选择大管外表面。"));
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
		double dChordTol = m_dChodTol/1000.; // 弦高误差
		double dStepTol  = m_dStepTol/1000.; // 步长精度

		PNT3D ptStart, ptEnd;
		for (int i=0; i<3; i++)
		{
			ptStart[i] = dParam[i];
			ptEnd[i]   = dParam[i+3];
		}

		// 将加工孔的边线进行离散
		LONG ptNum = 0;
		swCurve->IGetTessPtsSize(dChordTol,dStepTol,ptStart,ptEnd,&ptNum);
		double* ptArray = new double[ptNum];
		swCurve->IGetTessPts(dChordTol,dStepTol,ptStart,ptEnd,ptArray);
		CalPathNode(ptNum, ptArray, swSurface, movePath);

		// 判断当前路径偏移方向是否正确
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

 	// 通过3D草图的形式，绘制路径
 	userAddin->DrawPathComb(pPathComb);
	
	iSwModel = NULL;
	return;
}