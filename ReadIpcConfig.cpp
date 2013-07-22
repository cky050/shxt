

// ReadIpcConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "TestNetDll.h"
#include "ReadIpcConfig.h"
#include "TestNetDllDlg.h"

// CReadIpcConfig 对话框

IMPLEMENT_DYNAMIC(CReadIpcConfig, CDialog)

CReadIpcConfig::CReadIpcConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CReadIpcConfig::IDD, pParent)
{
	m_parent=(CTestNetDllDlg *)pParent;
}


CReadIpcConfig::~CReadIpcConfig()
{
}


void CReadIpcConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReadIpcConfig, CDialog)
	ON_BN_CLICKED(IDC_READCONFIG, &CReadIpcConfig::OnBnClickedReadconfig)
	ON_BN_CLICKED(IDC_SYSTEMCONTROL, &CReadIpcConfig::OnSystemControl)
END_MESSAGE_MAP()




void CReadIpcConfig::OnBnClickedReadconfig()
{
	int configId=m_lastSendCmd=GetDlgItemInt(IDC_CONFIGID);
	IP_NET_DVR_WriteAUXStringEx(m_parent->m_nLastCmdId,
					"SYSTEM_CONFIG_GET_MESSAGE",configId,0,"");
}



void CReadIpcConfig::OnSystemControl()
{
	int configId=m_lastSendCmd=GetDlgItemInt(IDC_CONFIGID);
	CString msg;
	GetDlgItemText(IDC_XMLMSG,msg);
	IP_NET_DVR_SystemControl(m_parent->m_nLastCmdId,configId,0,(char *)_bstr_t(msg));
}


int CReadIpcConfig::OnAUXResponse(LONG lUser,LONG nStateCode,char *pResponse)
{
	if(m_lastSendCmd==nStateCode)
	{
		_bstr_t msg=pResponse;
		SetDlgItemText(IDC_RETTXT,msg);
	}
	return 0;
}