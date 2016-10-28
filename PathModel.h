#pragma once
#include "MathBase.h"


//////////////////////////////////////////////////////////////////////////
// CHoleParam�������׵Ŀײ���
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CHCombParam;
class CHoleParam
{
public:
	CHoleParam();
	~CHoleParam();
public:
	//����
	double m_dHoleR;          // �װ뾶
	double m_dExRotAng;       // ���ڹ��ϵ���תλ�ã�·�����ʹ�ã����ޱ任��ĽǶ�
	double m_dOrgRotAng;      // ���ڹ��ϵ���תλ�ã�δ�����ޱ任�ĽǶ�
	double m_dOffsetX;        // ���ڹܵ�X��ƫ�ƣ�·�����ʹ�ã�
	double m_dCenterDis;      // ����ܵ����ľ�
	double m_dThroughAng;     // ����ܵ����ļн�
	int    m_nAddOrder;       // �����˳��
	double m_nExOrder;        // �����˳��
	VEC3D  m_dThroughVec;     // �ᴩ����

	// ·�����������Ĳ�����
	CHCombParam* m_pParentComb;

	// ��������������·��ƥ��ײ���
	BOOL m_bChanged;      
	double m_dChangedVal; //���ڱ������ֵ

public:
	CHCombParam* GetParentComb();
	void SetParentComb(CHCombParam* pParentComb);
};
//ÿ���ĵ���·����������
typedef CTypedPtrList<CPtrList, CHoleParam*> LHParamList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CHCombParam��ÿ���ĵ������пײ�������
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CHCombParam
{
public:
	CHCombParam(BSTR sParamName);
	~CHCombParam();
public:
	LHParamList m_LHParamList;
public:
	// �ܲ���
	double m_dTubeDia;   // �ܼ�ֱ��
	double m_dTubeThick; // �ܼ����
	double m_dTubeLength;// �ܼ�����

	BSTR m_sParamName; // ���ڱ�ǵ�ǰ·�����������������ĸ��ĵ�
	BOOL m_bBulidTube; // ���ڱ���Ƿ���Ҫ����Բ��
	int  m_nHoleID;    // ���ڼ�¼��ǰ�ĵ��Ŀ�ID
	int  m_nSketID;
	int  m_nRefPlaneID;
public:
	void Realse();
};
//����ĵ���·����������
typedef CTypedPtrList<CPtrList, CHCombParam*> LHCombParamList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathNode��·���ڵ�
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CPathNode
{
public:
	CPathNode();
	~CPathNode();

public:
	// δ���ǵ�ͷ���վ����ԭʼ����
	//////////////////////////////////////////////////////////////////////////
	PNT3D m_OrgPosition;      // �ױ�Եԭʼ����
	VEC3D m_OrgDirection;     // �ױ�Եԭʼ·���㷨��
	VEC3D m_OffsetDirection;  // ·����ƫ�ƺ�ķ���
	//////////////////////////////////////////////////////////////////////////

	// �����¿ڽ���ƫ�ƺ�ĵ�͵�ͷ���յ���������Ҫ���ƺ�����ĵ�
	//////////////////////////////////////////////////////////////////////////
	// �¿��и�·���ĵ������յ�
	PNT3D m_OffsetPosition;   // �����¿ڿ��ƫ�ƺ��·��������
	PNT3D m_OffsetDrawEndPnt; // ���Ʊ�ʶ��ĩ��

	// �ױ�Եԭʼ��ĵ������յ�
	PNT3D m_OrgCutPosition;   // ���ӵ�ͷ���վ����Ŀױ�Ե��
	PNT3D m_OrgCutDrawEndPnt; // ���ӵ�ͷ���վ����Ŀױ�Ե��ʶ��ĩ��
	//////////////////////////////////////////////////////////////////////////
public:
	void Init();
	CPathNode* CopySelf();
};
// һ��·������һ���׵��и�·����
typedef CTypedPtrList<CPtrList, CPathNode*> LNodeList; 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CMovePath��һ���ӹ�·��
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CMovePath
{
public:
	CMovePath();
	~CMovePath();

protected:
	CHoleParam* m_pHoleParam;
public:
	LNodeList m_PathNodeList;
	int       m_nRefId;

public:
	// ���úͻ�ȡ·����Ӧ�Ŀײ���
	void        SetHParam(CHoleParam* pHoleParam);
	CHoleParam* GetHParam();

	// ��ȡ�ײ�������
	//////////////////////////////////////////////////////////////////////////
	double GetOffsetX();
	double GetExRotAng();
	double GetOrgRotAng();
	int    GetAddOrder();
	double GetTubeR();
	//////////////////////////////////////////////////////////////////////////

	void Realse();
	CMovePath* CopySelf();
};
// �ӹ�·������һ��·���ڵ�
typedef CTypedPtrList<CPtrList, CMovePath*> LPathList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathComb���ӹ�·������һ��·���ڵ�
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CPathComb
{
public:
	CPathComb();
	~CPathComb();
public:
	LPathList m_PathList;
	BOOL m_bTwiceCut;
	// �ο�ID�������·��ʱ������ǰ�ο�ID��ֵ����ǰ�����·�������ڸ�·�������·�������� 
	int m_nRefID;

public:
	void AddPath(CMovePath* pAdd);
public:
	void       Release();
	CPathComb* CopySelf();
};
// һ���ĵ���·������
typedef CTypedPtrList<CPtrList, CPathComb*> LPathCombList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathCombList��ÿ���ĵ���·������
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CPathCombList
{
public:
	CPathCombList(BSTR sPathCombName);
	~CPathCombList();
public:
	LPathCombList m_LPathCombList;
	BSTR m_sPathCombName; // ���ڱ�ǵ�ǰ·�����������ĸ��ĵ�
public:
	void Realse();
	CPathCombList* CopySelf();
};
//����ĵ���·������
typedef CTypedPtrList<CPtrList, CPathCombList*> LPathEntityList;
//////////////////////////////////////////////////////////////////////////