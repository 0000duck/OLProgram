//////////////////////////////////////////////////////////////////////////
//
//  设置切割参数对话框
//
//  2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////

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
	, m_dCutAng(30.)
	, m_dCutDepth(10.)
	, m_bHolePrecut(TRUE)
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
	DDX_Text(pDX, IDC_EDIT_CUTDEPTH, m_dCutDepth);
	DDV_MinMaxDouble(pDX, m_dCutDepth, 0, 9999999);
	DDX_Check(pDX, IDC_CHECK_HPRECUT, m_bHolePrecut);
	DDX_Text(pDX, IDC_EDIT_CHODTOL, m_dChodTol);
	DDV_MinMaxDouble(pDX, m_dChodTol, 0.001, 1000);
	DDX_Text(pDX, IDC_EDIT_STEPTOL, m_dStepTol);
	DDV_MinMaxDouble(pDX, m_dStepTol, 0.001, 1000);
	DDX_Text(pDX, IDC_EDIT_TOOLDIS, m_dToolDis);
	DDV_MinMaxDouble(pDX, m_dToolDis, 0, 9999);
}

BEGIN_MESSAGE_MAP(CCutParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCutParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_HPRECUT, &CCutParamDlg::OnBnClickedCheckPreCut)
	ON_EN_SETFOCUS(IDC_EDIT_CUTANGLE, &CCutParamDlg::OnEnSetfocusEditCutangle)
	ON_EN_KILLFOCUS(IDC_EDIT_CUTANGLE, &CCutParamDlg::OnEnKillfocusEditCutangle)
	ON_EN_SETFOCUS(IDC_EDIT_CUTDEPTH, &CCutParamDlg::OnEnSetfocusEditCutdepth)
	ON_EN_KILLFOCUS(IDC_EDIT_CUTDEPTH, &CCutParamDlg::OnEnKillfocusEditCutdepth)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CCutParamDlg 消息处理程序
void CCutParamDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

void CCutParamDlg::OnBnClickedCheckPreCut()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CButton *pCheckbox = (CButton*)GetDlgItem(IDC_CHECK_HPRECUT);
	pCheckbox->SetCheck(m_bHolePrecut);
	if (m_bHolePrecut)
	{
		GetDlgItem(IDC_STATIC_PIC_DET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CUTDEPTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DET_UNIT)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_STATIC_GROOVE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GROOVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_AUTO)->ShowWindow(SW_HIDE);		
	}
	else
	{
		GetDlgItem(IDC_STATIC_PIC_DET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CUTDEPTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DET_UNIT)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_STATIC_GROOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GROOVE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_AUTO)->ShowWindow(SW_SHOW);		
	}
}

BOOL CCutParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_bHolePrecut)
	{
		GetDlgItem(IDC_STATIC_PIC_DET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CUTDEPTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DET_UNIT)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_STATIC_GROOVE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GROOVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_AUTO)->ShowWindow(SW_HIDE);		
	}
	else
	{
		GetDlgItem(IDC_STATIC_PIC_DET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CUTDEPTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DET_UNIT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_STATIC_GROOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GROOVE2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_AUTO)->ShowWindow(SW_SHOW);		
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCutParamDlg::OnEnSetfocusEditCutangle()
{
	m_nCurCtrID = IDC_STATIC_PIC_ANG;
	GetDlgItem(IDC_STATIC_PIC_TDIA)-> InvalidateRect(NULL); 
}

void CCutParamDlg::OnEnKillfocusEditCutangle()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_TDIA)-> InvalidateRect(NULL); 
}

void CCutParamDlg::OnEnSetfocusEditCutdepth()
{
	m_nCurCtrID = IDC_STATIC_PIC_DET;
	GetDlgItem(IDC_STATIC_PIC_DET)-> InvalidateRect(NULL); 
}

void CCutParamDlg::OnEnKillfocusEditCutdepth()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_DET)-> InvalidateRect(NULL); 
}


HBRUSH CCutParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if( pWnd->GetDlgCtrlID() == m_nCurCtrID) // m_nCurCtrID为用户需要改变颜色的Static Text控件ID
	{
		pDC->SetBkMode(TRANSPARENT);         // 设置字体背景色为透明，防止字体背景色覆盖控件背景色
		HBRUSH brush = CreateSolidBrush(RGB(255,240,119)); // 设置控件背景色
		return (HBRUSH) brush;
	}
	return hbr;
}
