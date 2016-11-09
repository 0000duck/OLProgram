#pragma once


// CEditHParamDlg 对话框

class CEditHParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditHParamDlg)

public:
	CEditHParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditHParamDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_HPARAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 孔参数
	//////////////////////////////////////////////////////////////////////////
	double m_dHoleDia;        // 孔直径
	double m_dHoleXOffset;    // 孔X向偏移距离
	double m_dHoleZOffset;    // 孔Z向偏移距离
	double m_dHoleThroughAng; // 孔贯穿角度 
	double m_dHoleRotAng;     // 孔旋转角度
	//////////////////////////////////////////////////////////////////////////
	CHoleParam* m_pHParam;
	void SetHParam(CHoleParam* m_pHParam);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
