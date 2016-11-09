#pragma once


// CEditHParamDlg �Ի���

class CEditHParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditHParamDlg)

public:
	CEditHParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditHParamDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_HPARAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// �ײ���
	//////////////////////////////////////////////////////////////////////////
	double m_dHoleDia;        // ��ֱ��
	double m_dHoleXOffset;    // ��X��ƫ�ƾ���
	double m_dHoleZOffset;    // ��Z��ƫ�ƾ���
	double m_dHoleThroughAng; // �׹ᴩ�Ƕ� 
	double m_dHoleRotAng;     // ����ת�Ƕ�
	//////////////////////////////////////////////////////////////////////////
	CHoleParam* m_pHParam;
	void SetHParam(CHoleParam* m_pHParam);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
