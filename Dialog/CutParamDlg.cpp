// CutParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EliteSoftWare.h"
#include "CutParamDlg.h"
#include "afxdialogex.h"

// CCutParamDlg 对话框

IMPLEMENT_DYNAMIC(CCutParamDlg, CDialogEx)

CCutParamDlg::CCutParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCutParamDlg::IDD, pParent)
	, m_dCutAng(45.)
	, m_dCutWidth(10.)
	, m_bTwiceCut(TRUE)
	, m_dChodTol(0.01)
	, m_dStepTol(0.01)
	, m_dToolDis(3.)
{
}

CCutParamDlg::~CCutParamDlg()
{
}

void CCutParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CUTANGLE, m_dCutAng);
	DDV_MinMaxDouble(pDX, m_dCutAng, 1, 90);
	DDX_Text(pDX, IDC_EDIT_CUTWIDTH, m_dCutWidth);
	DDV_MinMaxDouble(pDX, m_dCutWidth, 0, 9999999);
	DDX_Check(pDX, IDC_CHECK_TWICECUT, m_bTwiceCut);
	DDX_Text(pDX, IDC_EDIT_CHODTOL, m_dChodTol);
	DDV_MinMaxDouble(pDX, m_dChodTol, 0.001, 1000);
	DDX_Text(pDX, IDC_EDIT_STEPTOL, m_dStepTol);
	DDV_MinMaxDouble(pDX, m_dStepTol, 0.001, 1000);
	DDX_Text(pDX, IDC_EDIT_TOOLDIS, m_dToolDis);
	DDV_MinMaxDouble(pDX, m_dToolDis, 0, 9999);
}

BEGIN_MESSAGE_MAP(CCutParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCutParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TWICECUT, &CCutParamDlg::OnBnClickedCheckTwicecut)
END_MESSAGE_MAP()

// CCutParamDlg 消息处理程序
void CCutParamDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

void CCutParamDlg::OnBnClickedCheckTwicecut()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CButton *pCheckbox = (CButton*)GetDlgItem(IDC_CHECK_TWICECUT);
	pCheckbox->SetCheck(m_bTwiceCut);
}

BOOL CCutParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}
