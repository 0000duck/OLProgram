//////////////////////////////////////////////////////////////////////////
//
//  �����и�����Ի���
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxwin.h"


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
	BOOL m_bTwiceCut;   // �Ƿ��¿���׷ֿ��и�

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTwicecut();
	virtual BOOL OnInitDialog();
};
