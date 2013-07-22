#pragma once

class CPlayerClass
{
public:
	CPlayerClass(void);
public:
	~CPlayerClass(void);
public:
	int m_nPlayPort;
	int m_nStreamId;
	int m_nHaveSetup;
	HWND m_hPlayHwnd;
};
