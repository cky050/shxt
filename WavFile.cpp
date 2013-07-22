#include "StdAfx.h"
#include "WavFile.h"

CWavFile::CWavFile(void)
{
	m_pFile=NULL;
	m_pBuffer=NULL;
}


CWavFile::~CWavFile(void)
{
	Close();
}




int CWavFile::Close()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
	if(m_pBuffer)
	{
		delete m_pBuffer;
		m_pBuffer=NULL;
	}
	return 0;
}

int CWavFile::Open(WAVEFORMAT * pFormat,char * filename)
{
	if(m_pFile)
	{
		Close();
	}

	// b :Open in binary (untranslated) mode; 
	//translations involving carriage-return and linefeed characters are suppressed. 
	m_pFile=fopen(filename,"rb+");

	if(!m_pFile)
	{
		return -1;
	}

	fseek(m_pFile,0,2);//fseek(m_pFile,0,SEEK_SET);   SEEK_CUR  SEEK_END  SEEK_SET
	int len=ftell(m_pFile);
	if(len<=0)
	{
		Close();
		return -2;
	}
	while(m_pBuffer==NULL)
	{
		m_pBuffer=new char[len+10];
		if(m_pBuffer==NULL)
		{
			Sleep(10);
		}
		else
		{
			break;
		}
	}
	fseek(m_pFile,0,0);
	int readlen=fread(m_pBuffer,1,len,m_pFile);
	if(readlen!=len)
	{
		Close();
		return -3;
	}
	char * buf=m_pBuffer;
	CHUNK_HEADER * pChunk=(CHUNK_HEADER*)buf;
	if(strncmp(pChunk->szRiffID,"RIFF",4)!=0)
	{
		Close();
		return -4;
	}
	pChunk=(CHUNK_HEADER*)(buf+8);
	if(strncmp(pChunk->szRiffID,"WAVE",4)!=0)
	{
		Close();
		return -5;
	}

	return 0;
}
