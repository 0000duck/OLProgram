//////////////////////////////////////////////////////////////////////////
//
//  ·����·�����Ͳ���
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
	m_dTubeDia    = 1;
	m_dTubeThick  = 0.03  ;
	m_dTubeLength = 4;

	m_sParamName = sParamName;
	m_bBulidTube = TRUE;
	m_nHoleID = 0;
	m_nSketID = 2;  //��ʼֵΪ2����Ϊ������ܺ������ߵ�ʱ�������������ͼ���������ǹ̶��ġ�
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
		m_OffsetDrawEndPnt[i] = 0.; 

		m_OrgCutPosition[i]   = 0.; 
		m_OrgCutDrawEndPnt[i] = 0.; 
	}
}

CPathNode* CPathNode::CopySelf()
{
	CPathNode* pNode = new CPathNode;
	for (int i=0; i<3; i++)
	{
		pNode->m_OrgPosition[i] = m_OrgPosition[i];      // �ױ�Եԭʼ����
		pNode->m_OrgDirection[i] = m_OrgDirection[i];     // �ױ�Եԭʼ·���㷨��
		pNode->m_OffsetDirection[i] = m_OffsetDirection[i];  // ·����ƫ�ƺ�ķ���.

		pNode->m_OffsetPosition[i] = m_OffsetPosition[i];   // �����¿ڿ��ƫ�ƺ��·��������
		pNode->m_OffsetDrawEndPnt[i] = m_OffsetDrawEndPnt[i]; // ���Ʊ�ʶ��ĩ��

		pNode->m_OrgCutPosition[i] = m_OrgCutPosition[i];   // ���ӵ�ͷ���վ����Ŀױ�Ե��
		pNode->m_OrgCutDrawEndPnt[i] = m_OrgCutDrawEndPnt[i]; // ���ӵ�ͷ���վ����Ŀױ�Ե��ʶ��ĩ��
	}
	return pNode;
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
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0.;
	}

	return (m_pHoleParam->m_bChanged)?(m_pHoleParam->m_dOffsetX-m_pHoleParam->m_dChangedVal):(m_pHoleParam->m_dOffsetX);
}

double CMovePath::GetExRotAng()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0.;
	}

	return m_pHoleParam->m_dExRotAng;
}

double CMovePath::GetOrgRotAng()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0.;
	}

	return m_pHoleParam->m_dOrgRotAng;
}

int CMovePath::GetAddOrder()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0;
	}

	return m_pHoleParam->m_nAddOrder;
}

double CMovePath::GetTubeR()
{
	if (NULL == m_pHoleParam)
	{
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0.;
	}
	if (NULL == m_pHoleParam->GetParentComb())
	{
		AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
		return 0.;
	}
	return m_pHoleParam->GetParentComb()->m_dTubeDia*0.5;
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

CPathComb* CPathComb::CopySelf()
{
	CPathComb* pPComb = new CPathComb;
	pPComb->m_bTwiceCut = m_bTwiceCut;
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

void CPathCombList::Realse() //�ͷ�·����
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
