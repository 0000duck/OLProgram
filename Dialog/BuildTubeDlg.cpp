//////////////////////////////////////////////////////////////////////////
//
// �����ܼ��͹ᴩ�׵ĶԻ���
//
// 2016.10.28 by qqs
//
//////////////////////////////////////////////////////////////////////////
// BuildTubeDlg.cpp : ʵ���ļ�


#include "stdafx.h"
#include "EliteSoftWare.h"
#include "BuildTubeDlg.h"
#include "afxdialogex.h"

// CBuildTubeDlg �Ի���
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
	m_nCurCtrID = -100;
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
}

BOOL CBuildTubeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CHParamListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SINGLESEL & ~LVS_EX_CHECKBOXES);

	CHParamListCtrl.InsertColumn(0, _T("���"),LVCFMT_LEFT,38,-1);
	CHParamListCtrl.InsertColumn(1, _T("���ֱ��"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(2, _T("X�����"),LVCFMT_LEFT,55,-1);
	CHParamListCtrl.InsertColumn(3, _T("���ľ���"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(4, _T("�ᴩ�Ƕ�"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.InsertColumn(5, _T("��ת�Ƕ�"),LVCFMT_LEFT,60,-1);
	CHParamListCtrl.DeleteAllItems();
	if (!m_bBulidTube)
	{
		GetDlgItem(IDC_EDIT_TUBEDIA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TUBETHICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TUBELENGTH)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CBuildTubeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_HOLEADD, &CBuildTubeDlg::OnBnClickedButtonHoleadd)
	ON_BN_CLICKED(IDOK, &CBuildTubeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBuildTubeDlg::OnBnClickedCancel)
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
ON_EN_SETFOCUS(IDC_EDIT_TUBEDIA, &CBuildTubeDlg::OnEnSetfocusEditTubedia)
ON_EN_KILLFOCUS(IDC_EDIT_TUBEDIA, &CBuildTubeDlg::OnEnKillfocusEditTubedia)
END_MESSAGE_MAP()

// �������Ŀײ����ĺ�����
BOOL CBuildTubeDlg::CheckParam()
{
	// 1.���ֵ����Сֵ�������ж�
	/* �˴���������������ʾ�Ի�����ϵͳ�Զ�����*/
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
	// 2.X����ƫ�Ʊ�����ڿװ뾶�ж�
	if (m_dHoleXOffset<(-m_dTubeDia*0.5))
	{
		AfxMessageBox(_T("X����ƫ�Ʊ�����ڿװ뾶��"));
		return FALSE;
	}
	// 3.X��ƫ�Ʋ��ܴ��ڻ���ڹܳ���+�װ뾶�ж�
	if (m_dHoleXOffset>((m_dTubeLength+m_dTubeDia*0.5)-0.0000001))
	{
		AfxMessageBox(_T("X��ƫ�Ʋ��ܴ��ڻ���ڹܳ�����װ뾶֮�͡�"));
		return FALSE;
	}
	// 4.Z��ƫ�ƾ���ֵ���ܴ��ڻ���ڹܰ뾶�ӿװ뾶�ж�
	if (abs(m_dHoleZOffset)>((m_dTubeDia*0.5+m_dTubeDia*0.5)-0.0000001))
	{
		AfxMessageBox(_T("���ľ��벻�ܴ��ڻ���ڹܰ뾶��װ뾶֮�͡�"));
		return FALSE;
	}
	// 5.��ֱ������С�ڹ�ֱ���ж�
	if (m_dTubeDia<(m_dHoleDia+0.0000001))
	{
		AfxMessageBox(_T("��ֱ������С�ڹ�ֱ����"));
		return FALSE;
	}
	// 6.X��ƫ��ֵ���ܵ��ڿװ뾶
	if (abs(m_dHoleXOffset-(m_dHoleDia*0.5))<0.0000001)
	{
		AfxMessageBox(_T("X��ƫ��ֵ���ܵ��ڿװ뾶��"));
		return FALSE;
	}
	return TRUE;
}

// CBuildTubeDlg ��Ϣ�������
void CBuildTubeDlg::OnBnClickedButtonHoleadd()
{
	UpdateData(TRUE);
	if (!CheckParam())
		return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHoleParam* pHParam = new CHoleParam();
	pHParam->m_dCenterDis = m_dHoleZOffset;
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

// ����ʲô�����ֻ���X���������Ϳ��ԣ���Ϊ����б��ᴩ�ܣ�X��������Сֵ��ƽ��ֵ��Ҳ��Xƫ�����
// �����������ں���·����·������ƥ��
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
			// ��Xֵ��ͬʱ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nAddOrder = 0;
	if (NULL == m_pHCombParam)
	{
		AfxMessageBox(_T("��ǰ�ĵ��ײ�������Ϊ�ա�"));
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
			tmp->m_dCenterDis /= 1000.;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	int n = pWnd->GetDlgCtrlID();
	if (pWnd->GetDlgCtrlID()==m_nCurCtrID)
	{
	//	pDC-> SetTextColor(RGB(255,0,0));     //����������ɫ
		pDC->SetBkColor(RGB(255,0,0)); //�������屳��Ϊ͸��
		// TODO: Return a different brush if the default is not desired
	//	return (HBRUSH)::GetStockObject(GRAY_BRUSH);     // ���ñ���ɫ
	}
	//return (HBRUSH)::GetStockObject(NULL_BRUSH);     // ���ñ���ɫ

	return hbr;
}


void CBuildTubeDlg::OnEnSetfocusEditTubedia()
{
	m_nCurCtrID = IDC_STATIC_PIC_TDIA;
	UpdateWindow();
}



void CBuildTubeDlg::OnEnKillfocusEditTubedia()
{
	//m_nCurCtrID = -100;
}
