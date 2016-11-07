//////////////////////////////////////////////////////////////////////////
//
//  基础路径类以及不同类型路径参数类
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"       
#include "PathModel.h"


//////////////////////////////////////////////////////////////////////////
// CHoleParam
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CHoleParam::CHoleParam()
{
	m_dHoleR      = 0.;
	m_dOffsetX    = 0.;
	m_dCenterDis  = 0.;
	m_dExRotAng   = 0.;
	m_dOrgRotAng  = 0.;
	m_dThroughAng = 0.;
	m_bChanged = FALSE ;
	m_pParentComb = NULL;
	for (int i=0; i<3; i++)
	{
		m_dThroughVec[i] = 0;
	}
}

CHoleParam::~CHoleParam()
{
};

CHCombParam* CHoleParam::GetParentComb()
{
	return m_pParentComb;
}
void CHoleParam::SetParentComb(CHCombParam* pParentComb)
{
	m_pParentComb = pParentComb;
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CHCombParam
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CHCombParam::CHCombParam(BSTR sParamName)
{
	m_dTubeDia    = 0.3;
	m_dTubeThick  = 0.01  ;
	m_dTubeLength = 1;

	m_sParamName = sParamName;
	m_bBulidTube = TRUE;
	m_nHoleID = 0;
	m_nSketID = 2;  //初始值为2，因为创建大管和中心线的时候会生成两个草图，这两个是固定的。
	m_nRefPlaneID = 0; 
	m_LHParamList.RemoveAll();
}

CHCombParam::~CHCombParam()
{
	Realse();
}

void CHCombParam::Realse()
{
	POSITION pos = m_LHParamList.GetHeadPosition();
	while(pos)
	{
		CHoleParam* pHPParm = m_LHParamList.GetNext(pos);
		if(NULL == pHPParm)
			continue;
		delete pHPParm;
		pHPParm = NULL;
	}
	m_LHParamList.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathNode
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CPathNode::CPathNode()
{
	Init();
}

CPathNode::~CPathNode()
{
}

void CPathNode::Init()
{
	for (int i=0; i<3; i++)
	{
		m_OrgPosition[i]     = 0.;
		m_OrgDirection[i]    = 0.;
		m_OffsetDirection[i] = 0.; 
		m_OffsetPosition[i]   = 0.;
		m_OrgCutPosition[i]   = 0.; 
	}
}

CPathNode* CPathNode::CopySelf()
{
	CPathNode* pNode = new CPathNode;
	for (int i=0; i<3; i++)
	{
		pNode->m_OrgPosition[i] = m_OrgPosition[i];          // 孔边缘原始坐标
		pNode->m_OrgDirection[i] = m_OrgDirection[i];        // 孔边缘原始路径点法向
		pNode->m_OffsetDirection[i] = m_OffsetDirection[i];  // 路径点偏移后的法向
		pNode->m_OffsetPosition[i] = m_OffsetPosition[i];    // 根据坡口宽度偏移后的路径点坐标
		pNode->m_OrgCutPosition[i] = m_OrgCutPosition[i];    // 增加刀头悬空距离后的孔边缘点
	}
	return pNode;
}
void CPathNode::GetDrawEnd(int nFlag, PNT3D dDrawPt, double dLength)
{
	if (0==nFlag)
	{
		for (int i=0; i<3; i++)
		{
			dDrawPt[i] = m_OrgCutPosition[i]+dLength*m_OrgDirection[i];
		}
	}
	if (1==nFlag)
	{
		for (int i=0; i<3; i++)
		{
			dDrawPt[i] = m_OffsetPosition[i]+dLength*m_OffsetDirection[i];
		}
	}
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CMovePath
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CMovePath::CMovePath()
{
	m_PathNodeList.RemoveAll();
	m_nRefId   = 0;
	m_pHoleParam = NULL;
}

CMovePath::~CMovePath()
{
	Realse();
}

void CMovePath::SetHParam(CHoleParam* pHoleParam)
{
	m_pHoleParam = pHoleParam;
}

CHoleParam* CMovePath::GetHParam()
{
	return m_pHoleParam;
}

double CMovePath::GetOffsetX()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0.;
	}

	return (m_pHoleParam->m_bChanged)?(m_pHoleParam->m_dOffsetX-m_pHoleParam->m_dChangedVal):(m_pHoleParam->m_dOffsetX);
}

double CMovePath::GetExRotAng()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0.;
	}

	return m_pHoleParam->m_dExRotAng;
}

double CMovePath::GetOrgRotAng(BOOL bArc)
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0.;
	}

	return bArc?(m_pHoleParam->m_dOrgRotAng*PI/180.):m_pHoleParam->m_dOrgRotAng;
}

int CMovePath::GetAddOrder()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0;
	}

	return m_pHoleParam->m_nAddOrder;
}

double CMovePath::GetTubeR()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0.;
	}
	if (NULL == m_pHoleParam->GetParentComb())
	{
		AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
		return 0.;
	}
	return m_pHoleParam->GetParentComb()->m_dTubeDia*0.5;
}

BOOL CMovePath::IsClosed()
{
	CPathNode* pHead = m_PathNodeList.GetHead();
	CPathNode* pEnd = m_PathNodeList.GetTail();
	if (mathDis3D(pHead->m_OrgPosition,pEnd->m_OrgPosition)<MIN_LEN)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CMovePath::GetLength(double dLength[2]) // dLength为二维数组，[0]为孔边缘原始点的长度，[1]为偏移后路径长度
{
//	BOOL bClosed = IsClosed();
	for (int i=0; i<2; i++)
	{
		dLength[i] = 0.;
	}

	POSITION pos = m_PathNodeList.GetHeadPosition();
	while(pos)
	{
		CPathNode* pFirstNode = m_PathNodeList.GetNext(pos);
		if (NULL == pFirstNode)
			continue;
		CPathNode* pNextNode  = NULL;
		if (pos)
		{
			pNextNode = m_PathNodeList.GetAt(pos);
		}
		if (NULL == pNextNode)
			continue;
		dLength[0] += mathDis3D(pFirstNode->m_OrgCutPosition,pNextNode->m_OrgCutPosition);
		dLength[1] += mathDis3D(pFirstNode->m_OffsetPosition,pNextNode->m_OffsetPosition);
	}
	return ;
}

void CMovePath::Realse()
{		
	POSITION pos = m_PathNodeList.GetHeadPosition();
	while(pos)
	{
		CPathNode* pPathNode = m_PathNodeList.GetNext(pos);
		if (NULL == pPathNode)
			continue;
		delete pPathNode;
		pPathNode = NULL;
	}
	m_PathNodeList.RemoveAll();
}

CMovePath* CMovePath::CopySelf()
{
	CMovePath* pMovePath = new CMovePath;
	pMovePath->m_pHoleParam = m_pHoleParam;
	pMovePath->m_nRefId = m_nRefId;
	POSITION pos = m_PathNodeList.GetHeadPosition();
	while(pos)
	{
		CPathNode* pNode = m_PathNodeList.GetNext(pos);
		if (NULL == pNode)
			continue;
		pMovePath->m_PathNodeList.AddTail(pNode->CopySelf());
	}
	return pMovePath;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathComb
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CPathComb::CPathComb()
{
	m_bHolePrecut = TRUE;
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

CPathComb* CPathComb::CopySelf()
{
	CPathComb* pPComb = new CPathComb;
	pPComb->m_bHolePrecut = m_bHolePrecut;
	pPComb->m_nRefID = m_nRefID;
	POSITION pos = this->m_PathList.GetHeadPosition();
	while(pos)
	{
		CMovePath* pMovePath = this->m_PathList.GetNext(pos);
		if(NULL == pMovePath)
			continue;
		pPComb->m_PathList.AddTail(pMovePath->CopySelf());
	}
	return pPComb;
}

void CPathComb::AddPath(CMovePath* pAdd)
{
	if (NULL == pAdd)
		return;
	pAdd->m_nRefId = m_nRefID;
	m_PathList.AddTail(pAdd);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathCombList
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
CPathCombList::CPathCombList(BSTR sPathCombName)
{
	m_sPathCombName = sPathCombName;
	m_LPathCombList.RemoveAll();
}

CPathCombList::~CPathCombList()
{
	Realse();
}

void CPathCombList::Realse() //释放路径点
{
	POSITION pos = m_LPathCombList.GetHeadPosition();
	while(pos)
	{
		CPathComb* pPathComb = m_LPathCombList.GetNext(pos);
		if (NULL == pPathComb)
			continue;
		delete pPathComb;
		pPathComb = NULL;
	}
	m_LPathCombList.RemoveAll();
}

CPathCombList* CPathCombList::CopySelf()
{
	CPathCombList* pCyp = new CPathCombList(this->m_sPathCombName);
	POSITION pos = this->m_LPathCombList.GetHeadPosition();
	while(pos)
	{
		CPathComb* pPComb = this->m_LPathCombList.GetNext(pos);
		if (NULL == pPComb)
			continue;
		pCyp->m_LPathCombList.AddTail(pPComb->CopySelf());
	}
	return pCyp;
}
