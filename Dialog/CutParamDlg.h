//////////////////////////////////////////////////////////////////////////
//
//  设置切割参数对话框
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"


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
	BOOL m_bTwiceCut;   // 是否坡口与孔分开切割

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTwicecut();
	virtual BOOL OnInitDialog();
};
