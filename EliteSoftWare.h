// EliteSoftWare.h : Declaration of the CEliteSoftWare

#pragma once

#include "EliteSoftWare_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <map>

const double PI = 3.1415926536;
const double CVALUE = 0.01;

typedef double PNT3D[3];
typedef double VEC3D[3];

// 坐标系
struct _ref_fm3d
{
	VEC3D X, Y, Z;
	PNT3D O;
};
typedef struct _ref_fm3d RFRAME;

class CHCombParam;
//单个孔的参数
class CHoleParam
{
public:
	CHoleParam()
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
	};
	~CHoleParam()
	{
	};
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
	CHCombParam* GetParentComb()
	{
		return m_pParentComb;
	}
	void SetParentComb(CHCombParam* pParentComb)
	{
		m_pParentComb = pParentComb;
	}
};
typedef CTypedPtrList<CPtrList, CHoleParam*> LHParamList;//每个文档的路径参数集合

// 每个文档的孔参数集合
class CHCombParam
{
public:
	CHCombParam(BSTR sParamName)
	{
		m_dTubeDia    = 1;
		m_dTubeThick  = 0.03  ;
		m_dTubeLength = 4;

		m_sParamName = sParamName;
		m_bBulidTube = TRUE;
		m_nHoleID = 0;
		m_nSketID = 2;  //初始值为2，因为创建大管和中心线的时候会生成两个草图，这两个是固定的。
		m_nRefPlaneID = 0; 
		m_LHParamList.RemoveAll();
	};
	~CHCombParam()
	{
		Realse();
	};
public:
	LHParamList m_LHParamList;
public:
	// 管参数
	double m_dTubeDia;   // 管件直径
	double m_dTubeThick; // 管件厚度
	double m_dTubeLength;// 管件长度

	BSTR m_sParamName; // 用于标记当前路径参数集合是属于哪个文档
	BOOL m_bBulidTube; // 用于标记是否需要创建圆管
	int m_nHoleID;     // 用于记录当前文档的孔ID
	int m_nSketID;
	int m_nRefPlaneID;
public:
	void Realse()
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
};

typedef CTypedPtrList<CPtrList, CHCombParam*> LHCombParamList;//多个文档的路径参数集合


class CPathNode
{
public:
	CPathNode()
	{
		Init();
	};
	~CPathNode()
	{
	}

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
	void Init()
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
    CPathNode* CopySelf()
	{
		CPathNode* pNode = new CPathNode;
		for (int i=0; i<3; i++)
		{
			pNode->m_OrgPosition[i] = m_OrgPosition[i];      // 孔边缘原始坐标
			pNode->m_OrgDirection[i] = m_OrgDirection[i];     // 孔边缘原始路径点法向
			pNode->m_OffsetDirection[i] = m_OffsetDirection[i];  // 路径点偏移后的法向.

			pNode->m_OffsetPosition[i] = m_OffsetPosition[i];   // 根据坡口宽度偏移后的路径点坐标
			pNode->m_OffsetDrawEndPnt[i] = m_OffsetDrawEndPnt[i]; // 绘制标识段末点

			pNode->m_OrgCutPosition[i] = m_OrgCutPosition[i];   // 增加刀头悬空距离后的孔边缘点
			pNode->m_OrgCutDrawEndPnt[i] = m_OrgCutDrawEndPnt[i]; // 增加刀头悬空距离后的孔边缘标识段末点
		}
		return pNode;
	}
};
typedef CTypedPtrList<CPtrList, CPathNode*> LNodeList; // 一条路径（如切孔路径或者切坡口路径）

class CMovePath
{
public:
	CMovePath()
	{
		m_PathNodeList.RemoveAll();
		m_nRefId   = 0;
		m_pHoleParam = NULL;
	}
	~CMovePath()
	{
		Realse();
	}
protected:
	CHoleParam* m_pHoleParam;
public:
	LNodeList m_PathNodeList;
	int    m_nRefId;
public:
	void SetHParam(CHoleParam* pHoleParam)
	{
		m_pHoleParam = pHoleParam;
	}
	CHoleParam* GetHParam()
	{
		return m_pHoleParam;
	}
	double GetOffsetX()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
			return 0.;
		}

		double a = (m_pHoleParam->m_bChanged)?(m_pHoleParam->m_dOffsetX-m_pHoleParam->m_dChangedVal):(m_pHoleParam->m_dOffsetX);
		return a;
	}

	double GetExRotAng()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
			return 0.;
		}

		return m_pHoleParam->m_dExRotAng;
	}

	double GetOrgRotAng()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
			return 0.;
		}

		return m_pHoleParam->m_dOrgRotAng;
	}

	int GetAddOrder()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("当前路径参数为空，文档计算结果有误，请重建文档。"));
			return 0;
		}

		return m_pHoleParam->m_nAddOrder;
	}

	double GetTubeR()
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

	void Realse()
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
	CMovePath* CopySelf()
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
};
typedef CTypedPtrList<CPtrList, CMovePath*> LPathList;// 加工一个孔对应的路径（可以包含切孔路径和切坡口路径）

//一次计算生成的路径
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
	void Release();
	CPathComb* CopySelf()
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

	void AddPath(CMovePath* pAdd)
	{
		if (NULL == pAdd)
			return;
		pAdd->m_nRefId = m_nRefID;
		m_PathList.AddTail(pAdd);
	}

};
typedef CTypedPtrList<CPtrList, CPathComb*> LPathCombList;

//每个文档的路径集合
class CPathCombList
{
public:
	CPathCombList(BSTR sPathCombName)
	{
		m_sPathCombName = sPathCombName;
		m_LPathCombList.RemoveAll();
	};
	~CPathCombList()
	{
		Realse();
	};
public:
	LPathCombList m_LPathCombList;
	BSTR m_sPathCombName; // 用于标记当前路径组是属于哪个文档
public:
	void Realse() //释放路径点
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
	CPathCombList* CopySelf()
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
};
typedef CTypedPtrList<CPtrList, CPathCombList*> LPathEntityList;//多个文档的路径集合


class CSwDocument;
typedef std::map<IUnknown*, CSwDocument*> TMapIUnknownToDocument;

class CUserPropertyManagerPage;
class CBitmapHandler;

#define ID_SLDWORKS_EVENTS 0
#define MAIN_CMD_GROUP_ID 0 
#define MAIN_ITEM_ID1 0
#define MAIN_ITEM_ID2 1
#define MAIN_ITEM_ID3 2
#define FLYOUT_GROUP_ID 99
// CEliteSoftWare


class ATL_NO_VTABLE CEliteSoftWare :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEliteSoftWare, &CLSID_EliteSoftWare>,
	public IDispatchImpl<IEliteSoftWare, &IID_IEliteSoftWare, &LIBID_EliteSoftWareLib, 1, 0>,
	public ISwAddin,
	public IDispEventImpl<ID_SLDWORKS_EVENTS, CEliteSoftWare, &__uuidof(DSldWorksEvents), &LIBID_SldWorks, ID_SLDWORKS_TLB_MAJOR, ID_SLDWORKS_TLB_MINOR> 
	{
	typedef IDispEventImpl<ID_SLDWORKS_EVENTS, CEliteSoftWare, &__uuidof(DSldWorksEvents), &LIBID_SldWorks, ID_SLDWORKS_TLB_MAJOR, ID_SLDWORKS_TLB_MINOR> CSldWorksEvents;

private:
	CComPtr<ISldWorks> iSwApp;
	CComPtr<ICommandManager> iCmdMgr;
	CComObject<CBitmapHandler> *iBmp;
	long addinID;
	long toolbarID;
	long m_swMajNum;
	long m_swMinNum;
public:
	LPathEntityList m_LPathEntityList;
	LHCombParamList  m_LHoleParamList;
	int m_nExportOrder; // 标记路径输出排序方式

//This mapping will contain references to all open Documents, and ensure 
//that we do not attempt to attach event handlers to an already opened doc. 
	TMapIUnknownToDocument openDocs;
	CComObject<CUserPropertyManagerPage> *userPropertyPage;

public:
	CEliteSoftWare()
	{
	}
	~CEliteSoftWare();
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ELITESOFTWARE)

	DECLARE_NOT_AGGREGATABLE(CEliteSoftWare)

	BEGIN_COM_MAP(CEliteSoftWare)
		COM_INTERFACE_ENTRY(IEliteSoftWare)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISwAddin)
	END_COM_MAP()


	// IEliteSoftWare
public:

	// ISwAddin Methods
public:
	CComPtr<ISldWorks> GetSldWorksPtr() { return iSwApp != NULL ? iSwApp : NULL; }
	//These methods will connect and disconnect this addin to the SolidWorks events
	VARIANT_BOOL AttachEventHandlers();
	VARIANT_BOOL DetachEventHandlers();
	//These methods will connect and disconnect this addin to the SolidWorks Model events
	VARIANT_BOOL AttachModelEventHandler(CComPtr<IModelDoc2> iModelDoc2);	
	VARIANT_BOOL DetachModelEventHandler(IUnknown *iUnk);
	HRESULT AttachEventsToAllDocuments();
TMapIUnknownToDocument OpenDocumentsTable() { return openDocs; }
	
	int GetSldWorksTlbMajor() {return (m_swMajNum >= ID_SLDWORKS_TLB_MAJOR ) ? m_swMajNum : 0;}
	int GetSldWorksTlbMinor() {return m_swMinNum;}

	void AddCommandManager();
	void RemoveCommandManager();
	bool CompareIDs(long * storedIDs, long storedSize, long * addinIDs, long addinSize);
	void AddPMP();
	void RemovePMP();
	BSTR GetCurrentFile();
	void TransWorldPathComb(RFRAME& local_frame, LPathCombList &PathCombList);
	void TransLocalPathComb(RFRAME& local_frame, LPathCombList &PathCombList);
	void TransWorldPath(RFRAME& local_frame, CMovePath* pMovePath);
	void TransLocalPath(RFRAME& local_frame, CMovePath* pMovePath);
	BOOL CheckTransFrame(RFRAME& local_frame, LPathCombList &PathCombList);
	void ExportPathToTXT(); // 输出路径
	void SetExportRFrame(RFRAME& _export_rframe); // 设置输出坐标系
	CHCombParam*   GetCurHoleParam(); // 获取当前激活文档的管参数
	CPathCombList* GetCurPComb();      // 获取当前激活文档的路径集合
	void SetPCombExOrder(CPathCombList* pPathCombs, int nOrder); // 设置输出顺序
	void BuildTubeAndHole();

	// 绘制路径和坐标系
	//////////////////////////////////////////////////////////////////////////
	// 通过3D草图绘制路径
	void DrawPathCombs(CPathCombList* pPathCombs); // 绘制整个文档所有路径
	void DrawPathComb(CPathComb* pPathComb); // 绘制一条路径（即一个3D草图）
	// 通过曲线绘制坐标系
	void DrawRFrame(RFRAME& rframe, double dAixLength = 0.5);
	//////////////////////////////////////////////////////////////////////////
	
	//Event Handlers
	//These are the methods that are called when certain SolidWorks events are fired
	STDMETHOD(OnDocChange)(void);
	STDMETHOD(OnModelDocChange)(void);
	STDMETHOD(OnDocLoad)(BSTR docTitle, BSTR docPath);
	STDMETHOD(OnFileNew)(LPDISPATCH newDoc, long docType, BSTR templateName);
	STDMETHOD(OnFileOpenPostNotify)(BSTR fileName);

	
	BEGIN_SINK_MAP(CEliteSoftWare)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppActiveDocChangeNotify, OnDocChange)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppDocumentLoadNotify, OnDocLoad)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppFileNewNotify2, OnFileNew)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppActiveModelDocChangeNotify, OnModelDocChange)
		SINK_ENTRY_EX(ID_SLDWORKS_EVENTS, __uuidof(DSldWorksEvents), swAppFileOpenPostNotify, OnFileOpenPostNotify)
	END_SINK_MAP( )
	
	// ISwAddin Methods
	//These are the methods inherited from the ISwAddin interface that 
	//need to be implemented in order to connect the addin to SolidWorks
public:
	STDMETHOD(ConnectToSW)(LPDISPATCH ThisSW, long Cookie, VARIANT_BOOL * IsConnected);
	STDMETHOD(DisconnectFromSW)(VARIANT_BOOL * IsDisconnected);


	STDMETHOD(ToolbarCallbackPathExport)(void);
	STDMETHOD(ToolbarEnablePathExport)(long* status);
	STDMETHOD(ToolbarBuildTube)(void);
	STDMETHOD(ToolbarEnableBuildTube)(long* status);
	STDMETHOD(FlyoutCallback)(void);
	STDMETHOD(FlyoutCallback0)(void);
	STDMETHOD(FlyoutCallback1)(void);
	STDMETHOD(FlyoutEnable0)(long* status);
	STDMETHOD(FlyoutEnableCallback0)(long* status);
	STDMETHOD(GeneratePath)(void);
	STDMETHOD(EnableGeneratePath)(long* status);
};

OBJECT_ENTRY_AUTO(__uuidof(EliteSoftWare), CEliteSoftWare)
