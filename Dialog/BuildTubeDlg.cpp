//////////////////////////////////////////////////////////////////////////
//
// 创建管件和贯穿孔的对话框
//
// 2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////
// BuildTubeDlg.cpp : 实现文件


#include "stdafx.h"
#include "EliteSoftWare.h"
#include "BuildTubeDlg.h"
#include "afxdialogex.h"

// CBuildTubeDlg 对话框
IMPLEMENT_DYNAMIC(CBuildTubeDlg, CDialogEx)

CBuildTubeDlg::CBuildTubeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBuildTubeDlg::IDD, pParent)
	, m_dTubeDia(300)
	, m_dTubeThick(10)
	, m_dTubeLength(1000)
	, m_dHoleDia(100)
	, m_dHoleXOffset(100)
	, m_dHoleZOffset(0)
	, m_dHoleThroughAng(90)
	, m_dHoleRotAng(0)
	, m_nHoleID(0)
{
	m_pHCombParam = NULL;
	m_bBulidTube = TRUE;
	m_nCurCtrID = ID_CURCTL_NULL;
}

CBuildTubeDlg::~CBuildTubeDlg()
{
}

void CBuildTubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TUBEDIA, m_dTubeDia);
	DDV_MinMaxDouble(pDX, m_dTubeDia, 0.0000001, 1000000);
	DDX_Text(pDX, IDC_EDIT_TUBETHICK, m_dTubeThick);
	DDV_MinMaxDouble(pDX, m_dTubeThick, 0.0000001, 10000000);
	DDX_Text(pDX, IDC_EDIT_TUBELENGTH, m_dTubeLength);
	DDV_MinMaxDouble(pDX, m_dTubeLength, 0.0000001, 10000000);
	DDX_Control(pDX, IDC_LIST_HPARAM, CHParamListCtrl);
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
//	DDX_Control(pDX, IDC_STATIC_PIC_TDIA, m_cTDia);
}

BOOL CBuildTubeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	CHParamListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);

	CHParamListCtrl.InsertColumn(0, _T("序号"),LVCFMT_LEFT,38,-1);
	CHParamListCtrl.InsertColumn(1, _T("割孔直径"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(2, _T("X向距离"),LVCFMT_LEFT,55,-1);
	CHParamListCtrl.InsertColumn(3, _T("轴心偏移"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(4, _T("贯穿角度"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(5, _T("旋转角度"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.DeleteAllItems();
	if (!m_bBulidTube)
	{
		GetDlgItem(IDC_EDIT_TUBEDIA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TUBETHICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TUBELENGTH)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CBuildTubeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HOLEADD, &CBuildTubeDlg::OnBnClickedButtonHoleadd)
	ON_BN_CLICKED(IDOK, &CBuildTubeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBuildTubeDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_TUBEDIA, &CBuildTubeDlg::OnEnSetfocusEditTubedia)
	ON_EN_KILLFOCUS(IDC_EDIT_TUBEDIA, &CBuildTubeDlg::OnEnKillfocusEditTubedia)
	ON_EN_SETFOCUS(IDC_EDIT_TUBETHICK, &CBuildTubeDlg::OnEnSetfocusEditTubethick)
	ON_EN_KILLFOCUS(IDC_EDIT_TUBETHICK, &CBuildTubeDlg::OnEnKillfocusEditTubethick)
	ON_EN_SETFOCUS(IDC_EDIT_TUBELENGTH, &CBuildTubeDlg::OnEnSetfocusEditTubelength)
	ON_EN_KILLFOCUS(IDC_EDIT_TUBELENGTH, &CBuildTubeDlg::OnEnKillfocusEditTubelength)
	ON_EN_SETFOCUS(IDC_EDIT_HOLEDIA, &CBuildTubeDlg::OnEnSetfocusEditHoledia)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLEDIA, &CBuildTubeDlg::OnEnKillfocusEditHoledia)
	ON_EN_SETFOCUS(IDC_EDIT_HOLEXOFFSET, &CBuildTubeDlg::OnEnSetfocusEditHolexoffset)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLEXOFFSET, &CBuildTubeDlg::OnEnKillfocusEditHolexoffset)
	ON_EN_SETFOCUS(IDC_EDIT_HOLEZOFFSET, &CBuildTubeDlg::OnEnSetfocusEditHolezoffset)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLEZOFFSET, &CBuildTubeDlg::OnEnKillfocusEditHolezoffset)
	ON_EN_SETFOCUS(IDC_EDIT_HOLETHROUGHANG, &CBuildTubeDlg::OnEnSetfocusEditHolethroughang)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLETHROUGHANG, &CBuildTubeDlg::OnEnKillfocusEditHolethroughang)
	ON_EN_SETFOCUS(IDC_EDIT_HOLEROTANG, &CBuildTubeDlg::OnEnSetfocusEditHolerotang)
	ON_EN_KILLFOCUS(IDC_EDIT_HOLEROTANG, &CBuildTubeDlg::OnEnKillfocusEditHolerotang)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_HPARAM, &CBuildTubeDlg::OnNMRClickListHParam)
	ON_COMMAND(ID_EDIT, OnEdit)
	ON_COMMAND(ID_DELETE, OnDelete)
END_MESSAGE_MAP()

// 检查输入的孔参数的合理性
BOOL CBuildTubeDlg::CheckParam()
{
	// 1.最大值与最小值合理性判断
	/* 此处不满足条件的提示对话框由系统自动弹出*/
	if ((m_dTubeDia<0.0000001||m_dTubeDia>100000000) ||
		(m_dTubeThick<0.0000001||m_dTubeThick>100000000) ||
		(m_dTubeLength<0.0000001||m_dTubeLength>100000000) ||
		(m_dHoleDia<0.0000001||m_dHoleDia>100000000) ||
		(m_dHoleXOffset<-100000000||m_dHoleXOffset>100000000) ||
		(m_dHoleZOffset<-100000000||m_dHoleZOffset>100000000) ||
		(m_dHoleThroughAng<0.0000001||m_dHoleThroughAng>179.9999999) ||
		(m_dHoleRotAng<0||m_dHoleRotAng>360))
	{
		return FALSE;
	}
	// 2.X向负向偏移必须大于孔半径判断
	if (m_dHoleXOffset<(-m_dTubeDia*0.5))
	{
		AfxMessageBox(_T("X向负向偏移必须大于孔半径。"));
		return FALSE;
	}
	// 3.X向偏移不能大于或等于管长度+孔半径判断
	if (m_dHoleXOffset>((m_dTubeLength+m_dTubeDia*0.5)-0.0000001))
	{
		AfxMessageBox(_T("X向偏移不能大于或等于管长度与孔半径之和。"));
		return FALSE;
	}
	// 4.Z向偏移绝对值不能大于或等于管半径加孔半径判断
	if (abs(m_dHoleZOffset)>((m_dTubeDia*0.5+m_dTubeDia*0.5)-0.0000001))
	{
		AfxMessageBox(_T("轴心偏移的绝对值不能大于或等于管半径与孔半径之和。"));
		return FALSE;
	}
	// 5.孔直径必须小于管直径判断
	if (m_dTubeDia<(m_dHoleDia+0.0000001))
	{
		AfxMessageBox(_T("孔直径必须小于管直径。"));
		return FALSE;
	}
	// 6.X向偏移值不能等于孔半径
	if (abs(m_dHoleXOffset-(m_dHoleDia*0.5))<0.0000001)
	{
		AfxMessageBox(_T("X向偏移值不能等于孔半径。"));
		return FALSE;
	}
	return TRUE;
}

// CBuildTubeDlg 消息处理程序
void CBuildTubeDlg::OnBnClickedButtonHoleadd()
{
	UpdateData(TRUE);
	if (!CheckParam())
		return;
	// TODO: 在此添加控件通知处理程序代码
	CHoleParam* pHParam = new CHoleParam();
	pHParam->m_dOffsetZ = m_dHoleZOffset;
	pHParam->m_dThroughAng = m_dHoleThroughAng;
	pHParam->m_dHoleR = m_dHoleDia*0.5;
	pHParam->m_dOffsetX = m_dHoleXOffset;
	pHParam->m_dExRotAng = m_dHoleRotAng;
	CHParamListCtrl.InsertItem(m_nHoleID,_T(""),0);

	CString str;
	str.Format( _T("%d"), m_nHoleID+1 ) ;
	CHParamListCtrl.SetItemText( m_nHoleID, 0, str ) ;
	str.Format( _T("%f"), m_dHoleDia ) ;
	TrimZeroForCSring(str);
	CHParamListCtrl.SetItemText( m_nHoleID, 1,str) ;
	str.Format( _T("%f"), m_dHoleXOffset ) ;
	TrimZeroForCSring(str);
	CHParamListCtrl.SetItemText( m_nHoleID, 2,str) ;
	str.Format( _T("%f"), m_dHoleZOffset ) ;
	TrimZeroForCSring(str);
	CHParamListCtrl.SetItemText( m_nHoleID, 3,str) ;
	str.Format( _T("%f"), m_dHoleThroughAng ) ;
	TrimZeroForCSring(str);
	CHParamListCtrl.SetItemText( m_nHoleID, 4,str) ;
	str.Format( _T("%f"), m_dHoleRotAng ) ;
	TrimZeroForCSring(str);
	CHParamListCtrl.SetItemText( m_nHoleID, 5,str) ;

	CHParamListCtrl.SetItemData(m_nHoleID,(DWORD_PTR)pHParam);
	m_nHoleID++;
	UpdateData(FALSE);
}

void CBuildTubeDlg::SetHParamList(CHCombParam* pHCombList)
{
	m_pHCombParam = pHCombList;
}

// 不论什么情况，只需对X进行修正就可以，因为就算斜向贯穿管，X的最大和最小值的平均值，也是X偏向距离
// 修正参数用于后期路径与路径参数匹配
void CBuildTubeDlg::CorrectParams()
{
	POSITION pos = m_pHCombParam->m_LHParamList.GetHeadPosition();
	while(pos)
	{
		CHoleParam* pHParam = m_pHCombParam->m_LHParamList.GetNext(pos);
		if (NULL == pHParam)
			continue;
		POSITION tmpPos = m_pHCombParam->m_LHParamList.GetHeadPosition();;
		int nXSameCount = 1;
		while(tmpPos)
		{
			CHoleParam* pTmpParam = m_pHCombParam->m_LHParamList.GetNext(tmpPos);
			if (NULL == pTmpParam || pTmpParam == pHParam)
				continue;
			// 当X值相同时，进行修正
			if (abs(pHParam->m_dOffsetX-pTmpParam->m_dOffsetX)<0.000001)
			{
				pTmpParam->m_dOffsetX += nXSameCount*CVALUE;
				pTmpParam->m_bChanged = TRUE;
				pTmpParam->m_dChangedVal = nXSameCount*CVALUE;
				nXSameCount++;
			}
		}
	}
}

void CBuildTubeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAddOrder = 0;
	if (NULL == m_pHCombParam)
	{
		AfxMessageBox(_T("当前文档孔参数集合为空。"));
		return;
	}
	else
	{
		int nCount = CHParamListCtrl.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			CHoleParam* tmp = ( CHoleParam*)CHParamListCtrl.GetItemData(i);
			if(NULL == tmp)
				continue;
			m_pHCombParam->m_LHParamList.AddTail(tmp);
			tmp->SetParentComb(m_pHCombParam);
			tmp->m_nAddOrder = nAddOrder;
			tmp->m_nExOrder = nAddOrder;
			nAddOrder++;
			tmp->m_dHoleR /= 1000.;
			tmp->m_dOffsetX /= 1000.;
			tmp->m_dOffsetZ /= 1000.;
			tmp->m_dThroughAng = tmp->m_dThroughAng*PI/180.;
			tmp->m_dOrgRotAng = tmp->m_dExRotAng;
			double ang = tmp->m_dExRotAng;
			ang = 180.-ang;
			if (ang<0.)
			{
				ang += 360.;
			}

			tmp->m_dExRotAng = ang*PI/180.;
		}
		CorrectParams();
	}

	CDialogEx::OnOK();
}

void CBuildTubeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount = CHParamListCtrl.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CHoleParam* tmp = ( CHoleParam*)CHParamListCtrl.GetItemData(i);
		if(NULL == tmp)
			continue;
		delete tmp;
		tmp = NULL;
	}
	CHParamListCtrl.RemoveAllGroups();
	CDialogEx::OnCancel();
}

HBRUSH CBuildTubeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CBuildTubeDlg::OnEnSetfocusEditTubedia()
{
	m_nCurCtrID = IDC_STATIC_PIC_TDIA;
	GetDlgItem(IDC_STATIC_PIC_TDIA)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditTubedia()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_TDIA)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditTubethick()
{
	m_nCurCtrID = IDC_STATIC_PIC_TTHK;
	GetDlgItem(IDC_STATIC_PIC_TTHK)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditTubethick()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_TTHK)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditTubelength()
{
	m_nCurCtrID = IDC_STATIC_PIC_TLEN;
	GetDlgItem(IDC_STATIC_PIC_TLEN)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditTubelength()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_TLEN)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditHoledia()
{
	m_nCurCtrID = IDC_STATIC_PIC_HDIA;
	GetDlgItem(IDC_STATIC_PIC_HDIA)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditHoledia()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_HDIA)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditHolexoffset()
{
	m_nCurCtrID = IDC_STATIC_PIC_XOFF;
	GetDlgItem(IDC_STATIC_PIC_XOFF)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditHolexoffset()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_XOFF)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditHolezoffset()
{
	m_nCurCtrID = IDC_STATIC_PIC_CDIS;
	GetDlgItem(IDC_STATIC_PIC_CDIS)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditHolezoffset()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_CDIS)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditHolethroughang()
{
	m_nCurCtrID = IDC_STATIC_PIC_TANG;
	GetDlgItem(IDC_STATIC_PIC_TANG)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditHolethroughang()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_TANG)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnSetfocusEditHolerotang()
{
	m_nCurCtrID = IDC_STATIC_PIC_RANG;
	GetDlgItem(IDC_STATIC_PIC_RANG)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnEnKillfocusEditHolerotang()
{
	m_nCurCtrID = ID_CURCTL_NULL;
	GetDlgItem(IDC_STATIC_PIC_RANG)-> InvalidateRect(NULL); 
}

void CBuildTubeDlg::OnNMRClickListHParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (CHParamListCtrl.GetSelectedCount() == 0)
	{
		*pResult = 0;
		return;
	}

	m_cMenu.LoadMenu(IDR_MENU_HOLE_EDIT);	
	CPoint CurPoint;
	GetCursorPos(&CurPoint);
    if (CHParamListCtrl.GetSelectedCount() > 1)
	{
		m_cMenu.RemoveMenu(ID_EDIT, MF_BYCOMMAND);
	}

	UINT nTestFlags;	
	CRect rect;
	int nCurrentID = CHParamListCtrl.HitTest(pNMItemActivate->ptAction, &nTestFlags);
	CHParamListCtrl.GetItemRect(nCurrentID, &rect,nTestFlags);
	TrackPopupMenu(m_cMenu.GetSubMenu(0)->m_hMenu,0,CurPoint.x,CurPoint.y,0,this->GetSafeHwnd(),&rect);
	*pResult = 0;
}

void CBuildTubeDlg::OnEdit()
{

}

void CBuildTubeDlg::OnDelete()
{
	while (1)
	{
		POSITION pos = CHParamListCtrl.GetFirstSelectedItemPosition();
		if (NULL == pos)
			break;
		int nSelectID = CHParamListCtrl.GetNextSelectedItem(pos);
		CHoleParam* tmp = ( CHoleParam*)CHParamListCtrl.GetItemData(nSelectID);
		if(NULL != tmp)
		{
			delete tmp;
			tmp = NULL;
		}

		CHParamListCtrl.DeleteItem(nSelectID);
		m_nHoleID--;
	}

	// 更新孔参数ID号
	int nCount = CHParamListCtrl.GetItemCount();
	for (int nID=0; nID<nCount; nID++)
	{
		CString str;
		str.Format( _T("%d"), nID+1 ) ;
		CHParamListCtrl.SetItemText( nID, 0, str) ;
	}
}