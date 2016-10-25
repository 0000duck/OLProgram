#pragma once
#include "afxwin.h"


// CCutParamDlg �Ի���

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
	double m_dCutWidth; // �¿ڿ��(��ʱ����Ϊ�ܼ��ں�)
	double m_dChodTol;  // ���߾���
	double m_dStepTol;  // ��������
	double m_dToolDis;  // �������վ���
	BOOL m_bTwiceCut;   // �Ƿ��¿���׷ֿ��и�

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTwicecut();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
