// SYNetCardDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SYNetCardDemo.h"
#include "SYNetCardDemoDlg.h"

#include "SYNetCardManger.h"


SYNetCardManger *g_syNetCard = NULL;
std::vector<SYNetCardInfo*> *g_pVecNetCardInfo = NULL;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSYNetCardDemoDlg dialog




CSYNetCardDemoDlg::CSYNetCardDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSYNetCardDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSYNetCardDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSYNetCardDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSYNetCardDemoDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CSYNetCardDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSYNetCardDemoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSYNetCardDemoDlg message handlers

BOOL CSYNetCardDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	g_syNetCard = SYNetCardManger::Instance();

	_listBox1 = (CListBox*)GetDlgItem(IDC_LIST1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSYNetCardDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSYNetCardDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSYNetCardDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSYNetCardDemoDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	//clear listbox	
	_listBox1->SetCurSel(-1);
	_listBox1->ResetContent();

	g_pVecNetCardInfo = g_syNetCard->Scan();
	for (unsigned int i=0; i<g_pVecNetCardInfo->size(); i++)
	{
		SYNetCardInfo *info = g_pVecNetCardInfo->at(i);
		
		CString tmp = L"";
		tmp.AppendFormat(L"Desc:%s    ", info->wszDescription);
		//tmp.AppendFormat(L"Connected:%s   ", info->bCurrentConnected?L"Yes":L"No");
		//tmp.AppendFormat(L"Mac:%02x-%02x-%02x-%02x-%02x-%02x  ", info->MAC_ADDRESS[0], info->MAC_ADDRESS[1], info->MAC_ADDRESS[2], info->MAC_ADDRESS[3], info->MAC_ADDRESS[4], info->MAC_ADDRESS[5]);
		_listBox1->AddString(tmp);

		OutputDebugString(L"---------------------------------\n");
		info->Print();
		OutputDebugString(L"---------------------------------\n");
	}

	WCHAR buf[256];
	wsprintf(buf, L"total:%d\n", g_pVecNetCardInfo->size());
	OutputDebugString(buf);

}

void CSYNetCardDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	delete g_syNetCard;
}

void CSYNetCardDemoDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	
	int index = _listBox1->GetCurSel();
	if (index == -1) return;



	bool bResult = g_syNetCard->EnableOrDisableWithGUID(g_pVecNetCardInfo->at(index)->wszGUID, true);
/*
	if (bResult)
	{
		AfxMessageBox(L"Please push 'Scan' Btn again");
	}else{
		AfxMessageBox(L"Can't connected");
	}
	*/
}

void CSYNetCardDemoDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	int index = _listBox1->GetCurSel();
	if (index == -1) return;



	bool bResult = g_syNetCard->EnableOrDisableWithGUID(g_pVecNetCardInfo->at(index)->wszGUID, false);
	/*
	if (bResult)
	{
	AfxMessageBox(L"Please push 'Scan' Btn again");
	}else{
	AfxMessageBox(L"Can't connected");
	}
	*/
}
