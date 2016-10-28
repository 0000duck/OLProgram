#pragma once
// 路径输出顺序标记
#define EXPORT_XORDER      0  // 根据X向偏移从小到大输出
#define EXPORT_ANGORDER    1  // 根据管旋转角度从小到大输出
#define EXPORT_CUSTOMORDER 2  // 根据用户添加孔的顺序输出

// CExportDlg 对话框

class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nExportOrder;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedRadioXdir();
	afx_msg void OnBnClickedRadioRotangdir();
	afx_msg void OnBnClickedRadioCustom();
	virtual BOOL OnInitDialog();
};
