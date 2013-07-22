#pragma once

#include "resource.h" 
#include "CDIPCConfigOcx.h"
#include "afxcmn.h"

// CtestipcconfigDlg 对话框
class CIpcConfigTestDlg : public CDialog
{
// 构造
public:
	CIpcConfigTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTIPCCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CIPCConfigOcx * m_pDlg;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedReload();
	afx_msg void OnBnClickedCh();
	afx_msg void OnBnClickedEn();
	CTreeCtrl m_PtzLoginTree;
	afx_msg void OnDestroy();
};
