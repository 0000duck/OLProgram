//////////////////////////////////////////////////////////////////////////
//
// �����ܼ��͹ᴩ�׵ĶԻ���
//
// 2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define ID_CURCTL_NULL -100;
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
	int m_nCurCtrID;  // ��ǰ����Ŀؼ�ID
	BOOL m_bEditMode; // �Ƿ�Ϊ�༭����״̬
	//////////////////////////////////////////////////////////////////////////
	CMenu m_cMenu ; // �Ҽ��˵�

public:
	// �ӿں���
	//////////////////////////////////////////////////////////////////////////
	BOOL CheckParam(BOOL bEditMode = FALSE, CHoleParam* pHParam = NULL);// ���ڼ������������
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
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	//////////////////////////////////////////////////////////////////////////
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditTubedia();
	afx_msg void OnEnKillfocusEditTubedia();
//	CStatic m_cTDia;
	afx_msg void OnEnSetfocusEditTubethick();
	afx_msg void OnEnKillfocusEditTubethick();
	afx_msg void OnEnSetfocusEditTubelength();
	afx_msg void OnEnKillfocusEditTubelength();
	afx_msg void OnEnSetfocusEditHoledia();
	afx_msg void OnEnKillfocusEditHoledia();
	afx_msg void OnEnSetfocusEditHolexoffset();
	afx_msg void OnEnKillfocusEditHolexoffset();
	afx_msg void OnEnSetfocusEditHolezoffset();
	afx_msg void OnEnKillfocusEditHolezoffset();
	afx_msg void OnEnSetfocusEditHolethroughang();
	afx_msg void OnEnKillfocusEditHolethroughang();
	afx_msg void OnEnSetfocusEditHolerotang();
	afx_msg void OnEnKillfocusEditHolerotang();
	afx_msg void OnNMRClickListHParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListHparam(NMHDR *pNMHDR, LRESULT *pResult);
};