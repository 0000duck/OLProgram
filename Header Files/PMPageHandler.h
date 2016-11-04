// PMPageHandler.h : Declaration of the CPMPageHandler

#pragma once
#include "EliteSoftWare_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include "EliteSoftWare.h"
class CEliteSoftWare;

#define SELECTION_BASETUBEFACE 11
#define SELECTION_BASETUBEEDGE 12
#define SELECTION_HOLEEDGE     13
#define BUTTON_CALCULPATH      14
#define BUTTON_SETPARAM        15
#define BUTTON_CLEARPATH       16

class ATL_NO_VTABLE CPMPageHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPMPageHandler, &CLSID_PMPageHandler>,
	public IDispatchImpl<IPMPageHandler, &IID_IPMPageHandler, &LIBID_EliteSoftWareLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPropertyManagerPage2Handler9
{
public:
	CPMPageHandler()
	{
	}
	~CPMPageHandler()
	{
	}

public:
	CEliteSoftWare *userAddin;
	CComPtr<ISldWorks> iSwApp;
	// 切割工艺参数
	//////////////////////////////////////////////////////////////////////////
	double m_dCutAng;     // 坡口角度
	double m_dCutDepth;   // 坡口深度
	double m_dChodTol;    // 弓高精度
	double m_dStepTol;    // 步长精度
	double m_dToolDis;    // 刀具悬空距离
	BOOL   m_bHolePrecut; // 是否预先割孔
	//////////////////////////////////////////////////////////////////////////

public:
	void Init(CEliteSoftWare *app);
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_PMPAGEHANDLER)

	DECLARE_NOT_AGGREGATABLE(CPMPageHandler)

	BEGIN_COM_MAP(CPMPageHandler)
		COM_INTERFACE_ENTRY(IPMPageHandler)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IPropertyManagerPage2Handler9)
	END_COM_MAP()

	// IPMPageHandler
public:
	void CalMovePath();  // 计算路径
	void ClearAllPath(); // 删除当前激活文档中的所有路径
	void SetCutParam();  // 设置加工参数
	void CalPathNode(int ptNum, double* ptArray, ISurface* swSurface, CMovePath* movePath, BOOL bRevOffVec = FALSE);
	CHoleParam* GetHParam(int ptNum, double* ptArray);
	CHCombParam* GetCurHoleParams();


	// IPropertyManagerPage2Handler2 Methods
public:
	STDMETHOD(OnClose)(long Reason);
	STDMETHOD(AfterClose)()
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnHelp)(VARIANT_BOOL * retval)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnGroupExpand)(long Id, VARIANT_BOOL Expanded)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnGroupCheck)(long Id, VARIANT_BOOL Checked)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnCheckboxCheck)(long Id, VARIANT_BOOL Checked)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnOptionCheck)(long Id)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnButtonPress)(long Id);

	STDMETHOD(OnTextboxChanged)(long Id, BSTR Text)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnNumberboxChanged)(long Id, double Value)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnNumberBoxTrackingCompleted)(long Id, double Value)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnComboboxSelectionChanged)(long Id, long Item)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnListboxSelectionChanged)(long Id, long Item)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnSelectionboxFocusChanged)(long Id)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnSelectionboxListChanged)(long Id, long Count)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnSelectionboxCalloutCreated)(long Id)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnSelectionboxCalloutDestroyed)(long Id)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnComboboxEditChanged)(long Id, BSTR Text)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnActiveXControlCreated)(long Id, VARIANT_BOOL Status, long * retval)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnPreviousPage)(VARIANT_BOOL * retval)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnNextPage)(VARIANT_BOOL * retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(AfterActivation)(void)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnKeystroke)(long Wparam, long Message, long Lparam, long Id, VARIANT_BOOL *retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnPopupMenuItem)(long Id)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnPopupMenuItemUpdate)(long Id, long *retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnPreview)(VARIANT_BOOL *retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnSliderPositionChanged)(long Id, double Value)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnSliderTrackingCompleted)(long Id, double Value)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnSubmitSelection)(long Id, LPDISPATCH Selection, long SelType, BSTR *ItemText, VARIANT_BOOL *retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnTabClicked)(long Id, VARIANT_BOOL *retval)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnUndo)(void)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnWhatsNew)(void)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnGainedFocus)(long id)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnLostFocus)(long id)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnRedo)(void)
	{
		return E_NOTIMPL;
	}
	
    STDMETHOD(OnWindowFromHandleControlCreated)(long Id, VARIANT_BOOL Status, long * retVal)
    {
        return E_NOTIMPL;
    }

	STDMETHOD(OnListboxRMBUp)(long Id, long PosX, long PosY)
	{
		return E_NOTIMPL;
	}

};

OBJECT_ENTRY_AUTO(__uuidof(PMPageHandler), CPMPageHandler)
