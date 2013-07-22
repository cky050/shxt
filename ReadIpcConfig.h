
#pragma once


class CTestNetDllDlg;
// CReadIpcConfig 对话框

class CReadIpcConfig : public CDialog
{
	DECLARE_DYNAMIC(CReadIpcConfig)

public:
	CReadIpcConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadIpcConfig();

// 对话框数据
	enum { IDD = IDD_IPCCONFIGREADER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadconfig();
	afx_msg void OnSystemControl();
	int OnAUXResponse(LONG lUser,LONG nStateCode,char *pResponse);
private:
	CTestNetDllDlg * m_parent;
	int m_lastSendCmd;
};
