//////////////////////////////////////////////////////////////////////////
//
//  设置切割参数对话框
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"

#define ID_CURCTL_NULL -100;


// 切割参数设置对话框

class CCutParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCutParamDlg)

public:
	CCutParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCutParamDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dCutAng;   // 坡口角度
	double m_dCutDepth; // 坡口深度
	double m_dChodTol;  // 弓高精度
	double m_dStepTol;  // 步长精度
	double m_dToolDis;  // 刀具悬空距离
	BOOL m_bHolePrecut; // 是否预先割孔
	int  m_nCurCtrID ;  // 当前激活的控件ID
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckPreCut();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEditCutangle();
	afx_msg void OnEnKillfocusEditCutangle();
	afx_msg void OnEnSetfocusEditCutdepth();
	afx_msg void OnEnKillfocusEditCutdepth();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
