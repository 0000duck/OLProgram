#pragma once
// ·�����˳����
#define EXPORT_XORDER      0  // ����X��ƫ�ƴ�С�������
#define EXPORT_ANGORDER    1  // ���ݹ���ת�Ƕȴ�С�������
#define EXPORT_CUSTOMORDER 2  // �����û���ӿ׵�˳�����

// CExportDlg �Ի���

class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nExportOrder;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedRadioXdir();
	afx_msg void OnBnClickedRadioRotangdir();
	afx_msg void OnBnClickedRadioCustom();
	virtual BOOL OnInitDialog();
};
