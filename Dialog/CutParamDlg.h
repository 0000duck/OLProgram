//////////////////////////////////////////////////////////////////////////
//
//  �����и�����Ի���
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"

#define ID_CURCTL_NULL -100;


// �и�������öԻ���

class CCutParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCutParamDlg)

public:
	CCutParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCutParamDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dCutAng;   // �¿ڽǶ�
	double m_dCutDepth; // �¿����
	double m_dChodTol;  // ���߾���
	double m_dStepTol;  // ��������
	double m_dToolDis;  // �������վ���
	BOOL m_bHolePrecut; // �Ƿ�Ԥ�ȸ��
	int  m_nCurCtrID ;  // ��ǰ����Ŀؼ�ID
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckPreCut();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEditCutangle();
	afx_msg void OnEnKillfocusEditCutangle();
	afx_msg void OnEnSetfocusEditCutdepth();
	afx_msg void OnEnKillfocusEditCutdepth();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
