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

// ����ϵ
struct _ref_fm3d
{
	VEC3D X, Y, Z;
	PNT3D O;
};
typedef struct _ref_fm3d RFRAME;

class CHCombParam;
//�����׵Ĳ���
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
	CHCombParam* GetParentComb()
	{
		return m_pParentComb;
	}
	void SetParentComb(CHCombParam* pParentComb)
	{
		m_pParentComb = pParentComb;
	}
};
typedef CTypedPtrList<CPtrList, CHoleParam*> LHParamList;//ÿ���ĵ���·����������

// ÿ���ĵ��Ŀײ�������
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
		m_nSketID = 2;  //��ʼֵΪ2����Ϊ������ܺ������ߵ�ʱ�������������ͼ���������ǹ̶��ġ�
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
	// �ܲ���
	double m_dTubeDia;   // �ܼ�ֱ��
	double m_dTubeThick; // �ܼ����
	double m_dTubeLength;// �ܼ�����

	BSTR m_sParamName; // ���ڱ�ǵ�ǰ·�����������������ĸ��ĵ�
	BOOL m_bBulidTube; // ���ڱ���Ƿ���Ҫ����Բ��
	int m_nHoleID;     // ���ڼ�¼��ǰ�ĵ��Ŀ�ID
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

typedef CTypedPtrList<CPtrList, CHCombParam*> LHCombParamList;//����ĵ���·����������


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
};
typedef CTypedPtrList<CPtrList, CPathNode*> LNodeList; // һ��·�������п�·���������¿�·����

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
			AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
			return 0.;
		}

		double a = (m_pHoleParam->m_bChanged)?(m_pHoleParam->m_dOffsetX-m_pHoleParam->m_dChangedVal):(m_pHoleParam->m_dOffsetX);
		return a;
	}

	double GetExRotAng()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
			return 0.;
		}

		return m_pHoleParam->m_dExRotAng;
	}

	double GetOrgRotAng()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
			return 0.;
		}

		return m_pHoleParam->m_dOrgRotAng;
	}

	int GetAddOrder()
	{
		if (NULL == m_pHoleParam)
		{
			AfxMessageBox(_T("��ǰ·������Ϊ�գ��ĵ��������������ؽ��ĵ���"));
			return 0;
		}

		return m_pHoleParam->m_nAddOrder;
	}

	double GetTubeR()
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
typedef CTypedPtrList<CPtrList, CMovePath*> LPathList;// �ӹ�һ���׶�Ӧ��·�������԰����п�·�������¿�·����

//һ�μ������ɵ�·��
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

//ÿ���ĵ���·������
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
	BSTR m_sPathCombName; // ���ڱ�ǵ�ǰ·�����������ĸ��ĵ�
public:
	void Realse() //�ͷ�·����
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
typedef CTypedPtrList<CPtrList, CPathCombList*> LPathEntityList;//����ĵ���·������


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
	int m_nExportOrder; // ���·���������ʽ

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
	void ExportPathToTXT(); // ���·��
	void SetExportRFrame(RFRAME& _export_rframe); // �����������ϵ
	CHCombParam*   GetCurHoleParam(); // ��ȡ��ǰ�����ĵ��Ĺܲ���
	CPathCombList* GetCurPComb();      // ��ȡ��ǰ�����ĵ���·������
	void SetPCombExOrder(CPathCombList* pPathCombs, int nOrder); // �������˳��
	void BuildTubeAndHole();

	// ����·��������ϵ
	//////////////////////////////////////////////////////////////////////////
	// ͨ��3D��ͼ����·��
	void DrawPathCombs(CPathCombList* pPathCombs); // ���������ĵ�����·��
	void DrawPathComb(CPathComb* pPathComb); // ����һ��·������һ��3D��ͼ��
	// ͨ�����߻�������ϵ
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
