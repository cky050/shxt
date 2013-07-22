// TestNetDllDlg.h : 头文件
//

#pragma once
#include "comutil.h"
#include "afxwin.h"
#include "PlayerClass.h"
#include "map"
#include "string"
#include "afxcmn.h"
#include "PTZClass.h"
#include "IpcSearchDlg.h"
#include "IpcConfigTestDlg.h" 
#include "ReadIpcConfig.h"

using namespace std;


// CTestNetDllDlg 对话框
class CTestNetDllDlg : public CDialog
{
// 构造
public:
	CTestNetDllDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTNETDLL_DIALOG };

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
public:
	afx_msg void OnBnClickedAdd();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedPlay(); 
public:
	CButton m_isTcp;
public:
	CButton m_isD1;
	map<LONG,CPlayerClass*> playlist;
	map<LONG,CPTZClass*> ptzList;
public:
	CTreeCtrl m_ListViewTree;
	LONG m_nLastCmdId;
public:
	afx_msg void OnBnClickedAutoview();	  
public:
	afx_msg void OnBnClickedReadconfig();
public:
	afx_msg void OnBnClickedSaveconfig();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedAddpreset();
public:
	afx_msg void OnBnClickedDelpreset();
public:
	afx_msg void OnBnClickedCallpreset();
public:
	CComboBox m_pComBox;
	LONG m_lRealHandle;
	LONG m_SwitchFromlRealHandle;
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedSearchlog();
public:
	afx_msg void OnBnClickedUploadconfig();
public:
	CListCtrl m_logList;
public:
	afx_msg void OnBnClickedDownloadlog();
public:
	afx_msg void OnBnClickedStartwritelog();
public:
	afx_msg void OnLvnItemchangedLoglist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedReboot(); 
public:
	afx_msg void OnBnClickedStartaudio();
public:
	afx_msg void OnBnClickedStartupload();
public:
	afx_msg void OnBnClickedStopupload();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedStopdown();
public:
	afx_msg void OnBnClickedSearchavi();
public:
	afx_msg void OnBnClickedCapimage();
public:
	afx_msg void OnBnClickedReadsn();	
	LONG    OnPlayerStateEvent(long nPort,LONG nStateCode,char *pResponse);
	int     OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo);
	LONG   OnPlayBackDataRecv(LONG nPtzId,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData);
	LONG   OnPlayActionEvent(LONG lUser,LONG nType,LONG nFlag,char * pData);
public:
	CStatic m_VideoRect;
public:
	afx_msg void OnBnClickedDiszoom();
public:
	afx_msg void OnBnClickedZoomin();
public:
	afx_msg void OnBnClickedZoomout();
public:
	afx_msg void OnBnClickedZoommove();
	int m_bTracking;
public:
	afx_msg void OnBnClickedFullfill();
public:
	afx_msg void OnBnClickedSearchipc();
private :
	CIpcConfigTestDlg m_configDlg;
	CTreeCtrl m_ptzTreeList;
	afx_msg void OnTvnSelchangedPtzlist(NMHDR *pNMHDR, LRESULT *pResult);
	int CheckCmdIdOk();
	afx_msg void OnBnClickedG711audio();
	afx_msg void OnBnClickedStopaudo();
	afx_msg void OnBnClickedAacaudio();
	afx_msg void OnBnClickedStartipcaudio();
	afx_msg void OnBnClickedStopipcaudio();
	afx_msg void OnBnClickedDownloadconfig();
	afx_msg void OnBnClickedAutoviewvideo();
	int m_nAutoViewId;
	afx_msg void OnBnClickedPlayfile();
public:
public:
	CButton m_haveAudio;
public:
	afx_msg void OnBnClickedStopplay();
public:
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedConfigipc();
public:
	afx_msg void OnBnClickedCapbyfile();
public:
	afx_msg void OnBnClickedCreateiframe();
public:
	afx_msg void OnBnClickedSaveuserdata();
public:
	afx_msg void OnBnClickedSetuserdata();
public:
	afx_msg void OnBnClickedTest();
	CReadIpcConfig readIpcConfig;
	CIpcSearchDlg m_searchDlg;
	afx_msg void OnClickedButtonTest1();
//	afx_msg void OnClickedButtontest2();

public:
	char *m_pBGbuffer;
};

