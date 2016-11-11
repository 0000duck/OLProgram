// EliteSoftWare.h : Declaration of the CEliteSoftWare

#pragma once

#include "EliteSoftWare_i.h"
#include "PathModel.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <map>

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
	long m_swMajNum;
	long m_swMinNum;
public:
	LPathEntityList m_LPathEntityList; // ������е�·������
	LHCombParamList  m_LHoleParamList; // ������еĿײ�������
	int m_nExportOrder; // ���·���������ʽ
	double m_dFixAng; // ·�������λ�Ƕ�
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
	void TransWorldPathCombs(RFRAME& local_frame, CPathCombList* pPCombs);
	void TransLocalPathCombs(RFRAME& local_frame, CPathCombList* pPCombs);
	void TransWorldPath(RFRAME& local_frame, CMovePath* pMovePath);
	void TransLocalPath(RFRAME& local_frame, CMovePath* pMovePath);
	BOOL CheckTransFrame(RFRAME& local_frame, LPathCombList &PathCombList);
	void ExportPathToTXT(); // ���·��
	void PathCheck(); // ���·��������
	void TransPathToWRFrame(CMovePath* pMovePath);// ��·���任���������ϵ��
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
	void DrawMovePath(CMovePath* pMovePath, BOOL bHolePrecut = TRUE);     // ����һ��MovePath
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
	STDMETHOD(GeneratePath)(void);
	STDMETHOD(EnableGeneratePath)(long* status);
};

OBJECT_ENTRY_AUTO(__uuidof(EliteSoftWare), CEliteSoftWare)
