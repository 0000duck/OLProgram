//////////////////////////////////////////////////////////////////////////
//
// �����ܼ��͹ᴩ�׵ĶԻ���
//
// 2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxcmn.h"


// CBuildTubeDlg �Ի���

class CBuildTubeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBuildTubeDlg)

public:
	CBuildTubeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBuildTubeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BUILDTUBE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ܼ�����
	//////////////////////////////////////////////////////////////////////////
	double m_dTubeDia;   // �ܼ�ֱ��
	double m_dTubeThick; // �ܼ����
	double m_dTubeLength;// �ܼ�����
	//////////////////////////////////////////////////////////////////////////

	// �ײ���
	//////////////////////////////////////////////////////////////////////////
	double m_dHoleDia;        // ��ֱ��
	double m_dHoleXOffset;    // ��X��ƫ�ƾ���
	double m_dHoleZOffset;    // ��Z��ƫ�ƾ���
	double m_dHoleThroughAng; // �׹ᴩ�Ƕ�
	double m_dHoleRotAng;     // ����ת�Ƕ�
	//////////////////////////////////////////////////////////////////////////

	// ��������
	//////////////////////////////////////////////////////////////////////////
	CListCtrl  CHParamListCtrl; // ��list�ؼ�
	CHCombParam* m_pHCombParam; // ��ǰ�ĵ��ײ�������
	int  m_nHoleID;             // ��ID
	BOOL m_bBulidTube;          // ��ǹܼ��Ƿ��Ѿ�����
	CBrush m_CtrBrush;  
	CFont  m_CtrFont;//����
	int m_nCurCtrID;
	//////////////////////////////////////////////////////////////////////////
public:
	// �ӿں���
	//////////////////////////////////////////////////////////////////////////
	BOOL CheckParam();  // ���ڼ������������
	void SetHParamList(CHCombParam* pHCombList); 
	void CorrectParams(); //��·����������в�������
	virtual BOOL OnInitDialog();	
	//////////////////////////////////////////////////////////////////////////
public:
	// ��Ϣ����
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnBnClickedButtonHoleadd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//////////////////////////////////////////////////////////////////////////
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditTubedia();
	afx_msg void OnEnKillfocusEditTubedia();
};
