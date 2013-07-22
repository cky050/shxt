#pragma once

#include "afxcmn.h"

#include "../NetSDKDLL/NetSDKDLL.h"
// CIpcSearchDlg 对话框

class CIpcSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CIpcSearchDlg)

public:
	CIpcSearchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIpcSearchDlg();

// 对话框数据
	enum { IDD = IDD_IPCSEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartsearch();
public:
	CListCtrl m_ipclist;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nLastCount;
public:
	afx_msg void OnBnClickedModify1();
public:
	afx_msg void OnBnClickedModify2();
public:
	afx_msg void OnDestroy();
};
