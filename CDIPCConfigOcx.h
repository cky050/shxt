// CDIPCConfigOcx.h : 由 Microsoft Visual C++ 创建的 ActiveX 控件包装类的声明

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CIPCConfigOcx

class CIPCConfigOcx : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIPCConfigOcx)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x901653B3, 0x5B0D, 0x4D2C, { 0x95, 0xFC, 0x9E, 0xB5, 0x71, 0x9F, 0x71, 0x1C } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
				pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:

// 操作
public:

	long ShowConfigDlg(LPCTSTR strIp, long nPort, LPCTSTR strUser, LPCTSTR strPass)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strIp, nPort, strUser, strPass);
		return result;
	}
	long ReloadConfig()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetLanguage(long bIsEnglish)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bIsEnglish);
		return result;
	}

};
