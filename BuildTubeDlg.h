#pragma once
#include "afxcmn.h"


// CBuildTubeDlg 对话框

class CBuildTubeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBuildTubeDlg)

public:
	CBuildTubeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBuildTubeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BUILDTUBE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	// 管件参数
	double m_dTubeDia;   // 管件直径
	double m_dTubeThick; // 管件厚度
	double m_dTubeLength;// 管件长度

	// 孔参数
	double m_dHoleDia;        // 孔直径
	double m_dHoleXOffset;    // 孔X向偏移距离
	double m_dHoleZOffset;    // 孔Z向偏移距离
	double m_dHoleThroughAng; // 孔贯穿角度
	double m_dHoleRotAng;     // 孔旋转角度

	CListCtrl CHParamListCtrl;
	CHCombParam* m_pHCombParam;
	int m_nHoleID;
	BOOL m_bBulidTube;
public:
	virtual BOOL OnInitDialog();	
	BOOL CheckParam();
	void SetHParamList(CHCombParam* pHCombList);
	void CorrectParams(); //对路径参数组进行参数调整
public:
	afx_msg void OnBnClickedButtonHoleadd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
