#pragma once
#include "MathBase.h"


//////////////////////////////////////////////////////////////////////////
// CHoleParam：单个孔的孔参数
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CHCombParam;
class CHoleParam
{
public:
	CHoleParam();
	~CHoleParam();
public:
	//参数
	double m_dHoleR;          // 孔半径
	double m_dExRotAng;       // 孔在管上的旋转位置（路径输出使用）象限变换后的角度
	double m_dOrgRotAng;      // 孔在管上的旋转位置，未做象限变换的角度
	double m_dOffsetX;        // 孔在管的X向偏移（路径输出使用）
	double m_dCenterDis;      // 孔与管的轴心距
	double m_dThroughAng;     // 孔与管的轴心夹角
	int    m_nAddOrder;       // 孔添加顺序
	double m_nExOrder;        // 孔输出顺序
	VEC3D  m_dThroughVec;     // 贯穿方向

	// 路径参数所属的参数组
	CHCombParam* m_pParentComb;

	// 修正参数，用于路径匹配孔参数
	BOOL m_bChanged;      
	double m_dChangedVal; //用于标记修正值

public:
	CHCombParam* GetParentComb();
	void SetParentComb(CHCombParam* pParentComb);
};
//每个文档的路径参数集合
typedef CTypedPtrList<CPtrList, CHoleParam*> LHParamList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CHCombParam：每个文档的所有孔参数集合
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
	// 管参数
	double m_dTubeDia;   // 管件直径
	double m_dTubeThick; // 管件厚度
	double m_dTubeLength;// 管件长度

	BSTR m_sParamName; // 用于标记当前路径参数集合是属于哪个文档
	BOOL m_bBulidTube; // 用于标记是否需要创建圆管
	int  m_nHoleID;    // 用于记录当前文档的孔ID
	int  m_nSketID;
	int  m_nRefPlaneID;
public:
	void Realse();
};
//多个文档的路径参数集合
typedef CTypedPtrList<CPtrList, CHCombParam*> LHCombParamList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathNode：路径节点
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CPathNode
{
public:
	CPathNode();
	~CPathNode();

public:
	// 未考虑刀头悬空距离的原始数据
	//////////////////////////////////////////////////////////////////////////
	PNT3D m_OrgPosition;      // 孔边缘原始坐标
	VEC3D m_OrgDirection;     // 孔边缘原始路径点法向
	VEC3D m_OffsetDirection;  // 路径点偏移后的法向
	//////////////////////////////////////////////////////////////////////////

	// 根据坡口进行偏移后的点和刀头悬空点是最终需要绘制和输出的点
	//////////////////////////////////////////////////////////////////////////
	// 坡口切割路径的刀具悬空点
	PNT3D m_OffsetPosition;   // 根据坡口宽度偏移后的路径点坐标
	PNT3D m_OffsetDrawEndPnt; // 绘制标识段末点

	// 孔边缘原始点的刀具悬空点
	PNT3D m_OrgCutPosition;   // 增加刀头悬空距离后的孔边缘点
	PNT3D m_OrgCutDrawEndPnt; // 增加刀头悬空距离后的孔边缘标识段末点
	//////////////////////////////////////////////////////////////////////////
public:
	void Init();
	CPathNode* CopySelf();
};
// 一条路径（如一个孔的切割路径）
typedef CTypedPtrList<CPtrList, CPathNode*> LNodeList; 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CMovePath：一条加工路径
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
	// 设置和获取路径对应的孔参数
	void        SetHParam(CHoleParam* pHoleParam);
	CHoleParam* GetHParam();

	// 获取孔参数数据
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
// 加工路径树上一个路径节点
typedef CTypedPtrList<CPtrList, CMovePath*> LPathList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathComb：加工路径树上一个路径节点
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
	// 参考ID，当添加路径时，将当前参考ID赋值给当前加入的路径，用于给路径组里的路径添加序号 
	int m_nRefID;

public:
	void AddPath(CMovePath* pAdd);
public:
	void       Release();
	CPathComb* CopySelf();
};
// 一个文档的路径集合
typedef CTypedPtrList<CPtrList, CPathComb*> LPathCombList;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// CPathCombList：每个文档的路径集合
// 2016.10.28 by qqs
//////////////////////////////////////////////////////////////////////////
class CPathCombList
{
public:
	CPathCombList(BSTR sPathCombName);
	~CPathCombList();
public:
	LPathCombList m_LPathCombList;
	BSTR m_sPathCombName; // 用于标记当前路径组是属于哪个文档
public:
	void Realse();
	CPathCombList* CopySelf();
};
//多个文档的路径集合
typedef CTypedPtrList<CPtrList, CPathCombList*> LPathEntityList;
//////////////////////////////////////////////////////////////////////////