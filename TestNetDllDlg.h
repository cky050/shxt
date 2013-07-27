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
	LONG m_nLastCmdId;
public:
	afx_msg void OnBnClickedAutoview();	  
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	LONG m_lRealHandle;
	LONG m_SwitchFromlRealHandle;
public:
	CListCtrl m_logList;
public:
	afx_msg void OnBnClickedStartaudio();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	LONG    OnPlayerStateEvent(long nPort,LONG nStateCode,char *pResponse);
	int     OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo);
	LONG   OnPlayBackDataRecv(LONG nPtzId,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData);
	LONG   OnPlayActionEvent(LONG lUser,LONG nType,LONG nFlag,char * pData);
public:
	CStatic m_VideoRect;
	int m_bTracking;
private :
	int CheckCmdIdOk();
	int m_nAutoViewId;
public:
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
public:
//	CReadIpcConfig readIpcConfig;
//	CIpcSearchDlg m_searchDlg;
public:
	BYTE *m_pBGbuffer;
	
	void Del_Noise(BYTE *pImageBuffer, const long nWidth, const long nHeight, const int points);
	void QuanFangXiangFuShi(BYTE *pImageBuffer, const long nWidth, const long nHeight);
	void QuanFangXiangPengZhang(BYTE *pImageBuffer, const long nWidth, const long nHeight);

	typedef struct
	{
		int x;
		int y;
	}D2POINT;
	void EdgeDetection(D2POINT edgepoint[], BYTE *pBuffer,const long nWidth,const long nHeight);
	void EdgeDetection_2user(D2POINT edgepoint1[], D2POINT edgepoint2[],BYTE *pBuffer,const long nWidth,const long nHeight);

};

static long edgepointsNO;
static long edgepointsNO1;
static long edgepointsNO2;