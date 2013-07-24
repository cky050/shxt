// testipcconfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IpcConfigTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIpcConfigTestDlg 对话框


CIpcConfigTestDlg::CIpcConfigTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIpcConfigTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDlg=NULL;
}

void CIpcConfigTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX); 
}

BEGIN_MESSAGE_MAP(CIpcConfigTestDlg, CDialog)
	//{{AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CIpcConfigTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RELOAD, &CIpcConfigTestDlg::OnBnClickedReload)
	ON_BN_CLICKED(IDC_CH, &CIpcConfigTestDlg::OnBnClickedCh)
	ON_BN_CLICKED(IDC_EN, &CIpcConfigTestDlg::OnBnClickedEn)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CIpcConfigTestDlg 消息处理程序

BOOL CIpcConfigTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	int w=1024;
	int h=600;
	RECT rect={0,0,w,h};

	GetClientRect(&rect);
	
	//this->SetWindowPos(NULL,0,0,rect.right,rect.bottom,0);

	rect.bottom-=60;

	m_pDlg = new  CIPCConfigOcx();
	//WS_VISIBLE
	m_pDlg->Create(AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS | CS_PARENTDC, this->m_hIcon,0,0),NULL,WS_CHILD ,rect,this,0);	
	m_pDlg->SetLanguage(0);
	m_pDlg->SetWindowPos(NULL,0,0,rect.right,rect.bottom,0);
	m_pDlg->ShowWindow(SW_SHOW);
	SetDlgItemInt(IDC_PORT,8091);
	SetDlgItemText(IDC_IP,_T("192.168.0.254"));
	SetDlgItemText(IDC_USRE,_T("admin"));
	SetDlgItemText(IDC_PASS,_T("123456"));

	OnBnClickedButton1();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIpcConfigTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}



//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CIpcConfigTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIpcConfigTestDlg::OnBnClickedButton1()
{
	CString ip;
	CString user;
	CString pass;
	int port;
	GetDlgItemText(IDC_IP,ip);
	GetDlgItemText(IDC_USRE,user);
	GetDlgItemText(IDC_PASS,pass);
	port=GetDlgItemInt(IDC_PORT);
	m_pDlg->ShowConfigDlg(ip,port,user,pass);
}

void CIpcConfigTestDlg::OnBnClickedReload()
{
	m_pDlg->ReloadConfig();
}

void CIpcConfigTestDlg::OnBnClickedCh()
{
	m_pDlg->SetLanguage(0);
}

void CIpcConfigTestDlg::OnBnClickedEn()
{
	m_pDlg->SetLanguage(1);
}

void CIpcConfigTestDlg::OnDestroy()
{
	delete m_pDlg;
	m_pDlg=NULL;
	CDialog::OnDestroy(); 
}
