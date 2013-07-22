#pragma once
#include "comutil.h"

class CPTZClass
{
public:
	CPTZClass(void);
public:
	~CPTZClass(void);
public:
	int m_nUserId;
	int m_nStateValue;
	int m_bAudioIsOn;
	_bstr_t m_strLastPtzInfo;
};
