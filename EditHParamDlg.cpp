// EditHParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EliteSoftWare.h"
#include "EditHParamDlg.h"
#include "afxdialogex.h"
#include "BuildTubeDlg.h"

// CEditHParamDlg 对话框

IMPLEMENT_DYNAMIC(CEditHParamDlg, CDialogEx)

CEditHParamDlg::CEditHParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditHParamDlg::IDD, pParent)
	, m_dHoleDia(100)
	, m_dHoleXOffset(100)
	, m_dHoleZOffset(0)
	, m_dHoleThroughAng(90)
	, m_dHoleRotAng(0)
{

}

CEditHParamDlg::~CEditHParamDlg()
{
}

void CEditHParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOLEDIA, m_dHoleDia);
	DDV_MinMaxDouble(pDX, m_dHoleDia, 0.0000001, 100000000);
	DDX_Text(pDX, IDC_EDIT_HOLEXOFFSET, m_dHoleXOffset);
	DDV_MinMaxDouble(pDX, m_dHoleXOffset, -100000000, 100000000);
	DDX_Text(pDX, IDC_EDIT_HOLEZOFFSET, m_dHoleZOffset);
	DDV_MinMaxDouble(pDX, m_dHoleZOffset, -100000000, 100000000);
	DDX_Text(pDX, IDC_EDIT_HOLETHROUGHANG, m_dHoleThroughAng);
	DDV_MinMaxDouble(pDX, m_dHoleThroughAng, 0.0000001, 179.9999999);
	DDX_Text(pDX, IDC_EDIT_HOLEROTANG, m_dHoleRotAng);
	DDV_MinMaxDouble(pDX, m_dHoleRotAng, 0, 360);
}


BEGIN_MESSAGE_MAP(CEditHParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditHParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditHParamDlg 消息处理程序
void CEditHParamDlg::SetHParam(CHoleParam* pHParam)
{
	m_pHParam = pHParam;
}

void CEditHParamDlg::OnBnClickedOk()
{
	if (NULL == m_pHParam)
	{
		CDialogEx::OnOK();
	}
	UpdateData(TRUE);
	CHoleParam* pCpy = m_pHParam->CopySelf();
	pCpy->m_dHoleR = m_dHoleDia*0.5;
	pCpy->m_dOrgRotAng = m_dHoleRotAng;
	pCpy->m_dThroughAng = m_dHoleThroughAng;
	pCpy->m_dOffsetX = m_dHoleXOffset;
	pCpy->m_dOffsetZ = m_dHoleZOffset;
	CBuildTubeDlg* dlg = (CBuildTubeDlg*)GetParent();
	if (!dlg->CheckParam(TRUE, pCpy))
	{
		if (pCpy != NULL)
		{
			delete pCpy;
			pCpy = NULL;
		}
		return;
	}
	m_pHParam->m_dHoleR = m_dHoleDia*0.5;
	m_pHParam->m_dOrgRotAng = m_dHoleRotAng;
	m_pHParam->m_dThroughAng = m_dHoleThroughAng;
	m_pHParam->m_dOffsetX = m_dHoleXOffset;
	m_pHParam->m_dOffsetZ = m_dHoleZOffset;
	if (pCpy != NULL)
	{
		delete pCpy;
		pCpy = NULL;
	}
	CDialogEx::OnOK();
}


BOOL CEditHParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (NULL == m_pHParam)
	{
		return TRUE;
	}
	m_dHoleDia = m_pHParam->m_dHoleR*2.;
	m_dHoleRotAng = m_pHParam->m_dOrgRotAng;
	m_dHoleThroughAng = m_pHParam->m_dThroughAng;
	m_dHoleXOffset = m_pHParam->m_dOffsetX;
	m_dHoleZOffset = m_pHParam->m_dOffsetZ;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}
