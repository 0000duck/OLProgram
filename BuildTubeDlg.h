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
//	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	// �ܼ�����
	double m_dTubeDia;   // �ܼ�ֱ��
	double m_dTubeThick; // �ܼ����
	double m_dTubeLength;// �ܼ�����

	// �ײ���
	double m_dHoleDia;        // ��ֱ��
	double m_dHoleXOffset;    // ��X��ƫ�ƾ���
	double m_dHoleZOffset;    // ��Z��ƫ�ƾ���
	double m_dHoleThroughAng; // �׹ᴩ�Ƕ�
	double m_dHoleRotAng;     // ����ת�Ƕ�

	CListCtrl CHParamListCtrl;
	CHCombParam* m_pHCombParam;
	int m_nHoleID;
	BOOL m_bBulidTube;
public:
	virtual BOOL OnInitDialog();	
	BOOL CheckParam();
	void SetHParamList(CHCombParam* pHCombList);
	void CorrectParams(); //��·����������в�������
public:
	afx_msg void OnBnClickedButtonHoleadd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
