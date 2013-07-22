// TestNetDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestNetDll.h"
#include "TestNetDllDlg.h"
#include "tinyxml.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if _DEBUG
#pragma comment(lib,"..\\NetSDKDLL\\Debug\\NetSDKDLL.lib")
#else
#pragma comment(lib,"..\\NetSDKDLL\\Release\\NetSDKDLL.lib")
#endif
#pragma comment(lib,"comsuppw.lib")

// CTestNetDllDlg 对话框


LONG CALLBACK OnAUXResponse(LONG lUser,LONG nType,char *pResponse,void *pUser);
LONG CALLBACK OnPlayActionEvent(LONG lUser,LONG nType,LONG nFlag,char * pData,void * pUser); 
LONG CALLBACK OnPlayBackDataRecv(LONG nPtzId,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData);


int  CALLBACK  OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo, void * pUser,long nReserved2)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pUser;
	if(parent)
	{
		return parent->OnMediaDataRecv(nPort,pBuf,nSize,pFrameInfo);
	}
	return 0;
}



LONG CALLBACK OnPlayActionEvent(LONG lUser,LONG nType,LONG nFlag,char * pData,void * pUser)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pUser;
	if(parent)
	{
		return parent->OnPlayActionEvent(lUser,nType,nFlag,pData);
	}
	return 0;

}


LONG CALLBACK OnPlayBackDataRecv(LONG nPtzId,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pExtData->pUserData;
	if(parent)
	{
		return parent->OnPlayBackDataRecv(nPtzId,dwDataType,pBuffer,dwBufSize,pExtData);
	}
	return 0;
}



LONG CALLBACK  OnPlayerStateEvent(long nPort,LONG nStateCode,char *pResponse,void *pUser)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pUser;
	if(parent)
	{
		return parent->OnPlayerStateEvent(nPort,nStateCode,pResponse);
	}
	return 0;
}



LONG   CTestNetDllDlg::OnPlayerStateEvent(long nPort,LONG nStateCode,char *pResponse)
{
	TRACE0("recv event code=%d\r\n",nStateCode);
	return 0;
}


















/*
To reconstract pixels in sets of 4, 6 bytes of data are required. The bytes are arranged as u, y1, y2, v, y3, y4. The total number of bytes in the image stream is 6 x width x height / 4.

void Convert411(unsigned char *yuv,BITMAP4 *rgb1,BITMAP4 *rgb2,BITMAP4 *rgb3,BITMAP4 *rgb4)
{  
   int y1,y2,y3,y4,u,v;
   
   // Extract yuv components
   u  = yuv[0];
   y1 = yuv[1];
   y2 = yuv[2];
   v  = yuv[3];
   y3 = yuv[4];
   y4 = yuv[5];

   // yuv to rgb
   *rgb1 = YUV_to_Bitmap(y1,u,v);
   *rgb2 = YUV_to_Bitmap(y2,u,v);
   *rgb3 = YUV_to_Bitmap(y3,u,v);
   *rgb4 = YUV_to_Bitmap(y4,u,v);
}
Conversion to RGB
The plain C source provided here is written for clarify not efficiency, in general these conversions would be performed by the way of lookup tables.

BITMAP4 YUV_to_Bitmap(int y,int u,int v)
{
   int r,g,b;
   BITMAP4 bm = {0,0,0,0};
   
   // u and v are +-0.5
   u -= 128;
   v -= 128;

   // Conversion
   r = y + 1.370705 * v;
   g = y - 0.698001 * v - 0.337633 * u;
   b = y + 1.732446 * u;

//   r = y + 1.402 * v;
//   g = y - 0.344 * u - 0.714 * v;
//   b = y + 1.772 * u;

//   y -= 16;
//   r = 1.164 * y + 1.596 * v;
//   g = 1.164 * y - 0.392 * u - 0.813 * v;
//   b = 1.164 * y + 2.017 * u;

   // Clamp to 0..1
   if (r < 0) r = 0;
   if (g < 0) g = 0;
   if (b < 0) b = 0;
   if (r > 255) r = 255;
   if (g > 255) g = 255;
   if (b > 255) b = 255;

   bm.r = r;
   bm.g = g;
   bm.b = b;
   bm.a = 0;

   return(bm);
}
*/

/*
//保存buffer到bmp文件

//iWidth：图像宽； iHeight：图像高；pBuffer：图像RGB数据；filePath：存储路径；fileName：保存文件名；fileNum：保存文件编号
//SaveDIB2Bmp(100, "frame", "D://screenshot")
bool MyDC::SaveDIB2Bmp(int fileNum, const char * fileName, const char * filePath, int iWidth, int iHeight, BYTE *pBuffer)
{
 
BITMAPINFOHEADER bih;
ConstructBih(iWidth,iHeight,bih);
BITMAPFILEHEADER bhh;
ContructBhh(iWidth,iHeight,bhh);
 
 TCHAR BMPFileName[1024];
 int widthStep = (((iWidth * 24) + 31) & (~31)) / 8 ; //每行实际占用的大小（每行都被填充到一个4字节边界）
 int DIBSize = widthStep * iHeight ;  //buffer的大小 （字节为单位）

 //save
 char path[1024];
 char str[1024];

 sprintf(str,"%d",fileNum);  
 strcat(str, fileName);
 strcat(str,".bmp");  //frame100.bmp
 sprintf(path,"%s", filePath);
 strcat(path,str); //Path//frame100.bmp

 strcpy(BMPFileName,path);
CFile file;
try
{
if(file.Open(BMPFileName,CFile::modeWrite | CFile::modeCreate))
{//写入文件
  
file.Write((LPSTR)&bhh,sizeof(BITMAPFILEHEADER));
file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));
file.Write(pBuffer,DIBSize);
file.Close();
return true;
}

}
catch (...) 
{
AfxMessageBox("MyDC::SaveDIB2Bmp");
}
 return false;
}

 

//构建BMP位图文件头
void MyDC::ContructBhh(int nWidth,int nHeight,BITMAPFILEHEADER& bhh) //add 2010-9-04
{
int widthStep = (((nWidth * 24) + 31) & (~31)) / 8 ; //每行实际占用的大小（每行都被填充到一个4字节边界）
bhh.bfType = ((WORD) ('M' << 8) | 'B');  //'BM'
bhh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + widthStep * nHeight;
bhh.bfReserved1 = 0;
bhh.bfReserved2 = 0;
bhh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

}


//构建BMP文件信息头
void MyDC::ConstructBih(int nWidth,int nHeight,BITMAPINFOHEADER& bih) 
{
 int widthStep = (((nWidth * 24) + 31) & (~31)) / 8 ; 

 bih.biSize=40;       // header size
 bih.biWidth=nWidth;
 bih.biHeight=nHeight;
 bih.biPlanes=1;
 bih.biBitCount=24;     // RGB encoded, 24 bit
 bih.biCompression=BI_RGB;   // no compression 非压缩
 bih.biSizeImage=widthStep*nHeight*3;
 bih.biXPelsPerMeter=0;
 bih.biYPelsPerMeter=0;
 bih.biClrUsed=0;
 bih.biClrImportant=0; 

}
*/




int     CTestNetDllDlg::OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo)
{
	TRACE1("recv media data len=%d,is video=%d\r\n",nSize,pFrameInfo->bIsVideo);
	
	//4:1:1
	//长度为:pFrameInfo->nHeight* pFrameInfo->nLinseSize[0];
	char * yBuffer= pBuf+16;
	
	//长度为:pFrameInfo-> nHeight * pFrameInfo->nLinseSize[1]/2;
	char * uBuffer= yBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[0];
	
	//长度为:pFrameInfo->nHeight * pFrameInfo->nLinseSize[2]/2;
	char * vBuffer= uBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[1]/2;





	char *dBuffer = new char[3*pFrameInfo->nWidth*pFrameInfo->nHeight];///////////////////////////////////////////////////

	BITMAPINFOHEADER bih;
	bih.biSize=40;       // header size
	bih.biWidth=pFrameInfo->nWidth;
	bih.biHeight=pFrameInfo->nHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;     // RGB encoded, 24 bit
	bih.biCompression=BI_RGB;   // no compression 非压缩
	bih.biSizeImage=((((pFrameInfo->nWidth * 24) + 31) & (~31)) / 8)*pFrameInfo->nHeight*3;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;

	BITMAPFILEHEADER bfh;
	bfh.bfType = ((WORD) ('M' << 8) | 'B');  //'BM'
	bfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + ((((pFrameInfo->nWidth * 24) + 31) & (~31)) / 8) * pFrameInfo->nHeight;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);


	//转成RGB
	int i = 0;
	int j = 0;
	double temp;
	for(;i < pFrameInfo->nLinseSize[1]/2;i++)
	{
	   char y,u,v;
	   y = yBuffer[4*i];
	   u = uBuffer[i]-128;
	   v = vBuffer[i]-128;// -128

	   temp = y		+	1.370705*v		+	0	;
	   dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		-	0.698001*v	-	0.337633*u;
	   dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		+		0		+	1.732446*u;
	   dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   //dBuffer[j + 3] = 0;
	   j+=3;

	   y = yBuffer[4*i+1];
	   temp = y		+	1.370705*v		+	0	;
	   dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		-	0.698001*v	-	0.337633*u;
	   dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		+		0		+	1.732446*u;
	   dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   //dBuffer[j + 3] = 0;
	   j+=3;

	   y = yBuffer[4*i+2];
	   temp = y		+	1.370705*v		+	0	;
	   dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		-	0.698001*v	-	0.337633*u;
	   dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		+		0		+	1.732446*u;
	   dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   //dBuffer[j + 3] = 0;
	   j+=3;

	   y = yBuffer[4*i+3];
	   temp = y		+	1.370705*v		+	0	;
	   dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		-	0.698001*v	-	0.337633*u;
	   dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = y		+		0		+	1.732446*u;
	   dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   //dBuffer[j + 3] = 0;
	   j+=3;
	}




	CFile file;
	CFileException e;
	try
	{
		if(file.Open(_T("E:\\0000000.bmp"),CFile::modeWrite | CFile::modeCreate,&e))
		{//写入文件
			file.Write((LPSTR)&bfh,sizeof(BITMAPFILEHEADER));
			file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));
			file.Write(dBuffer,3*pFrameInfo->nWidth*pFrameInfo->nHeight);///////////////////////////////
			file.Close();
		}
	}
	catch (...) 
	{
		file.Close();
		//AfxMessageBox("MyDC::SaveDIB2Bmp");
	}



	delete []dBuffer;

	return 0;
}


/*
int     CTestNetDllDlg::OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo)
{
	TRACE1("recv media data len=%d,is video=%d\r\n",nSize,pFrameInfo->bIsVideo);


	//4:1:1
	//长度为:pFrameInfo->nHeight* pFrameInfo->nLinseSize[0];
	char * yBuffer= pBuf+16;
	
	//长度为:pFrameInfo-> nHeight * pFrameInfo->nLinseSize[1]/2;
	char * uBuffer= yBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[0];
	
	//长度为:pFrameInfo->nHeight * pFrameInfo->nLinseSize[2]/2;
	char * vBuffer= uBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[1]/2;


///////////////////////////////
	//读文件头结构
	char pFileHeader[sizeof(BITMAPFILEHEADER)];
	CFile cfile;
	CFileException e;
	cfile.Open(_T("E:\\bmp1.bmp"),CFile::modeRead,&e);
	cfile.Read(pFileHeader,sizeof(BITMAPFILEHEADER));
	cfile.Close();
	BITMAPFILEHEADER *pBmpFileHeader = (BITMAPFILEHEADER *)pFileHeader;
	pBmpFileHeader->bfSize = 3*pFrameInfo->nWidth*pFrameInfo->nHeight+1024;

	//读信息头结构  未命名.
	char pInfoHeader[sizeof(BITMAPINFOHEADER)];
	cfile.Open(_T("E:\\bmp1.bmp"),CFile::modeRead,&e);
	cfile.Read(pInfoHeader,sizeof(BITMAPINFOHEADER));
	cfile.Close();
	BITMAPINFOHEADER *pBmpInfoHeader = (BITMAPINFOHEADER *)pInfoHeader;
	//pBmpInfoHeader
/////////////////////////////
/////////////////////////////
	char *dBuffer = new char[3*pFrameInfo->nWidth*pFrameInfo->nHeight+1024];
	int i=0,j=0;
	for(;i<sizeof(BITMAPFILEHEADER);i++)
	{
		*dBuffer = pFileHeader[i];
		dBuffer++;
	}
	i=0;
	for(;i<sizeof(BITMAPINFOHEADER);i++)
	{
		*dBuffer = pInfoHeader[i];
		dBuffer++;
	}
	
	char *dBuf = dBuffer;
////////////////////////



	char *dBuffer = new char[3*pFrameInfo->nWidth*pFrameInfo->nHeight+1024];
	char BufferFirst[] = { 
				   0x42,0x4d,0x38,0xd2,0x0f,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
				   0x00,0x00,0xD0,0x02,0x00,0x00,0xe0,0x01,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
				   0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x0b,0x00,0x00,0x12,0x0b,0x00,0x00,0x00,0x00,
				   0x00,0x00,0x00,0x00,0x00,0x00
				};
	int i=0,j=0;
	for(;i<54;i++)
	{
		*dBuffer = BufferFirst[i];
		dBuffer++;
	}
	char *dBuf = dBuffer;



	//转成RGB
	i = 0;
	j = 0;
	for(;i < pFrameInfo->nLinseSize[0];i++ )
	{
	   char y,u,v;
	   y = yBuffer[2*i];
	   u = uBuffer[i];
	   v = vBuffer[i];
	   double temp;
	   temp = 1.164383*(y- 16) + 0 + 1.596016*(v - 128);
	   dBuf[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = (1.164383*(y- 16) - 0.391762*(u - 128) - 0.812969*(v - 128));
	   dBuf[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = 1.164383*(y- 16) + 2.017230*(u - 128) + 0;
	   dBuf[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   j+=3;

	   y = yBuffer[2*i+1];
	   temp = 1.164383*(y- 16) + 0 + 1.596016*(v - 128);
	   dBuf[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = (1.164383*(y- 16) - 0.391762*(u - 128) - 0.812969*(v - 128));
	   dBuf[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   temp = 1.164383*(y- 16) + 2.017230*(u - 128) + 0;
	   dBuf[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	   j+=3;
	}

	
	FILE *file = fopen("E:\\0000000.bmp","wb");
	if( file == NULL )
	{
		TRACE0("--------\n");
		fclose(file);
		return 0;
	}else
	{
		fwrite(dBuffer,(3*pFrameInfo->nWidth*pFrameInfo->nHeight+1024),1,file);
		fclose(file);
	}


	return 0;
}
*/



CTestNetDllDlg::CTestNetDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestNetDllDlg::IDD, pParent),readIpcConfig(this)
{
	m_nLastCmdId=0;
	m_nAutoViewId=0;
	m_bTracking=false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_lRealHandle=0; 
	m_SwitchFromlRealHandle=0;

	IP_NET_DVR_SetRealDataCallBack(NULL,NULL);
}

void CTestNetDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ISTCP, m_isTcp);
	DDX_Control(pDX, IDC_PLAYD1, m_isD1);
	DDX_Control(pDX, IDC_TREE1, m_ListViewTree);
	DDX_Control(pDX, IDC_COMBO2, m_pComBox);
	DDX_Control(pDX, IDC_LOGLIST, m_logList);
	DDX_Control(pDX, IDC_VIDEORECT, m_VideoRect);
	DDX_Control(pDX, IDC_PTZLIST, m_ptzTreeList);
	DDX_Control(pDX, IDC_INCLUDEAUDIO, m_haveAudio);
}

BEGIN_MESSAGE_MAP(CTestNetDllDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CTestNetDllDlg::OnBnClickedAdd)
	ON_WM_DESTROY() 
	ON_BN_CLICKED(IDC_PLAY, &CTestNetDllDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_READCONFIG, &CTestNetDllDlg::OnBnClickedReadconfig)
	ON_BN_CLICKED(IDC_SAVECONFIG, &CTestNetDllDlg::OnBnClickedSaveconfig)
	ON_BN_CLICKED(IDC_ADDPRESET, &CTestNetDllDlg::OnBnClickedAddpreset)
	ON_BN_CLICKED(IDC_DELPRESET, &CTestNetDllDlg::OnBnClickedDelpreset)
	ON_BN_CLICKED(IDC_CALLPRESET, &CTestNetDllDlg::OnBnClickedCallpreset)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTestNetDllDlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_SEARCHLOG, &CTestNetDllDlg::OnBnClickedSearchlog)
	ON_BN_CLICKED(IDC_UPLOADCONFIG, &CTestNetDllDlg::OnBnClickedUploadconfig)
	ON_BN_CLICKED(IDC_DOWNLOADLOG, &CTestNetDllDlg::OnBnClickedDownloadlog)
	ON_BN_CLICKED(IDC_STARTWRITELOG, &CTestNetDllDlg::OnBnClickedStartwritelog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOGLIST, &CTestNetDllDlg::OnLvnItemchangedLoglist)
	ON_BN_CLICKED(IDC_REBOOT, &CTestNetDllDlg::OnBnClickedReboot) 
	ON_BN_CLICKED(IDC_STARTUPLOAD, &CTestNetDllDlg::OnBnClickedStartupload)
	ON_BN_CLICKED(IDC_STOPUPLOAD, &CTestNetDllDlg::OnBnClickedStopupload)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOPDOWN, &CTestNetDllDlg::OnBnClickedStopdown)
	ON_BN_CLICKED(IDC_SEARCHAVI, &CTestNetDllDlg::OnBnClickedSearchavi)
	ON_BN_CLICKED(IDC_CAPIMAGE, &CTestNetDllDlg::OnBnClickedCapimage)
	ON_BN_CLICKED(IDC_READSN, &CTestNetDllDlg::OnBnClickedReadsn)
	ON_BN_CLICKED(IDC_DISZOOM, &CTestNetDllDlg::OnBnClickedDiszoom)
	ON_BN_CLICKED(IDC_ZOOMIN, &CTestNetDllDlg::OnBnClickedZoomin)
	ON_BN_CLICKED(IDC_ZOOMOUT, &CTestNetDllDlg::OnBnClickedZoomout)
	ON_BN_CLICKED(IDC_ZOOMMOVE, &CTestNetDllDlg::OnBnClickedZoommove)
	ON_BN_CLICKED(IDC_FULLFILL, &CTestNetDllDlg::OnBnClickedFullfill)
	ON_BN_CLICKED(IDC_SEARCHIPC, &CTestNetDllDlg::OnBnClickedSearchipc)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PTZLIST, &CTestNetDllDlg::OnTvnSelchangedPtzlist)
	ON_BN_CLICKED(IDC_G711AUDIO, &CTestNetDllDlg::OnBnClickedG711audio)
	ON_BN_CLICKED(IDC_STOPAUDO, &CTestNetDllDlg::OnBnClickedStopaudo)
	ON_BN_CLICKED(IDC_AACAUDIO, &CTestNetDllDlg::OnBnClickedAacaudio)
	ON_BN_CLICKED(IDC_STARTIPCAUDIO, &CTestNetDllDlg::OnBnClickedStartipcaudio)
	ON_BN_CLICKED(IDC_STOPIPCAUDIO, &CTestNetDllDlg::OnBnClickedStopipcaudio)
	ON_BN_CLICKED(IDC_DOWNLOADCONFIG, &CTestNetDllDlg::OnBnClickedDownloadconfig)
	ON_BN_CLICKED(IDC_AUTOVIEWVIDEO, &CTestNetDllDlg::OnBnClickedAutoviewvideo)
	ON_BN_CLICKED(IDC_PLAYFILE, &CTestNetDllDlg::OnBnClickedPlayfile)
	ON_BN_CLICKED(IDC_STOPPLAY, &CTestNetDllDlg::OnBnClickedStopplay)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CTestNetDllDlg::OnNMClickTree1)
	ON_BN_CLICKED(IDC_CONFIGIPC, &CTestNetDllDlg::OnBnClickedConfigipc)
	ON_BN_CLICKED(IDC_CAPBYFILE, &CTestNetDllDlg::OnBnClickedCapbyfile)
	ON_BN_CLICKED(IDC_CREATEIFRAME, &CTestNetDllDlg::OnBnClickedCreateiframe)
	ON_BN_CLICKED(IDC_SAVEUSERDATA, &CTestNetDllDlg::OnBnClickedSaveuserdata)
	ON_BN_CLICKED(IDC_SETUSERDATA, &CTestNetDllDlg::OnBnClickedSetuserdata)
	ON_BN_CLICKED(IDC_TEST, &CTestNetDllDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_BUTTONTEST1, &CTestNetDllDlg::OnClickedButtonTest1)
//	ON_BN_CLICKED(IDC_BUTTONTEST2, &CTestNetDllDlg::OnClickedButtontest2)
END_MESSAGE_MAP()



//netsdkdll
LONG CALLBACK OnStateEvent(LONG lUser,LONG nStateCode,char *pResponse,void *pUser)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg *)pUser;
	int ErrorCode=nStateCode & 0xff000000;
	int ActionCode=nStateCode & 0xffffff;
	switch(nStateCode)
	{
	case EVENT_RECVVIDEOAUDIOPARAM:
		{
			STREAM_AV_PARAM * pParam=(STREAM_AV_PARAM*)pResponse;
			
			LONG lRealHandle=lUser;
			CPlayerClass * pPlayItem=parent->playlist[lRealHandle];
		/*	IP_TPS_OpenStream(pPlayItem->m_nPlayPort,(BYTE *)&pParam->videoParam,sizeof(VIDEO_PARAM),0,40);	
			IP_TPS_SetDecCallBack(pPlayItem->m_nPlayPort,OnMediaDataRecv,pUser);
			IP_TPS_Play(pPlayItem->m_nPlayPort,0);
		*/

			
			if(pPlayItem!=NULL)
			{
				if(pPlayItem!=NULL)
				{
					int playport=pPlayItem->m_nPlayPort;
					int size=sizeof(VIDEO_PARAM);
					IP_TPS_OpenStream(playport,(BYTE *)&pParam->videoParam,size,0,40);					
					HWND playHwnd=pPlayItem->m_hPlayHwnd;
					IP_TPS_SetDecCallBack(playport,OnMediaDataRecv,pUser);//解码数据直接，要求调用IP_TPS_Play(lRealHandle,NULL); 
					if(pParam->bHaveAudio)
					{
						IP_TPS_OpenStream(playport,(BYTE *)&pParam->audioParam,sizeof(AUDIO_PARAM),1,40);
						//SetupAACDecoder((BYTE *)&pParam->audioParam,sizeof(AUDIO_PARAM));
					}

					//IP_TPS_Play(playport,playHwnd);//NULL);//由于最后一个参数不为NULL，所以此处是直接播放，而不是解码
					IP_TPS_Play(playport,0);
				}
			}
			

		}
		break;
	case EVENT_CONNECTING:
		{//连接成功
			CPTZClass * pItem=parent->ptzList[lUser];
			pItem->m_nStateValue=EVENT_CONNECTING;
		}
		break;
	case EVENT_CONNECTOK:
		{
			CPTZClass * pItem=parent->ptzList[lUser];
			if(pItem)
			{
				pItem->m_nStateValue=EVENT_CONNECTOK;
			}
		}
		break;
	case EVENT_CONNECTFAILED:
		{
			CPTZClass * pItem=parent->ptzList[lUser];
			if(pItem)
			{
				pItem->m_nStateValue=EVENT_CONNECTFAILED;
			}
		}
		break;
	case EVENT_LOGINOK:
		{
			_bstr_t itemmsg="登录成功";
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_PTZALARM:
		{//告警信息
			ALARM_ITEM * pAlarmItem=(ALARM_ITEM *)pResponse;
			_bstr_t itemmsg="";
			_bstr_t alarmtypename="";
			switch(pAlarmItem->code)
			{//对不同的类用其它语言表示
			default:
				{
					alarmtypename="转换类型";
				}
				break;
			}
			char timestr[50];
			sprintf(timestr,"时间--%02d:%02d:%02d",pAlarmItem->time.hour,pAlarmItem->time.minute,pAlarmItem->time.second);
			itemmsg=itemmsg+pAlarmItem->data+timestr;
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_PTZPRESETINFO:
		{// pResponse=3^1^2^6^ //表示3个预置点，分别为1，2，6
			CPTZClass * pItem=parent->ptzList[lUser];
			_bstr_t msg(pResponse);
			if(pItem!=NULL && pItem->m_strLastPtzInfo!=msg)
			{
				pItem->m_strLastPtzInfo=msg;
				while(parent->m_pComBox.GetCount()>0)
				{
					parent->m_pComBox.DeleteString(0);
				}
				CStringArray retls;
				int count=TiXmlElement::Split(msg,"^",1,retls);
				for(int idx=1;idx<count;idx++)
				{
					parent->m_pComBox.InsertString(0,retls[idx]);
				}
			}
		}
		break;
	case EVENT_SENDPTZOK:
		{			
			_bstr_t itemmsg="发送云台命令成功";
			//不允许直接调用，否则有可能会造成死锁
			//parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_DOWNLOADOK:
		{
			_bstr_t itemmsg="";
			itemmsg=itemmsg+"下载成功"+pResponse;
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_UPLOADOK:
		{
			_bstr_t itemmsg="";
			itemmsg=itemmsg+"上传成功"+pResponse;
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_UPLOADFAILED:
		{
		}
		break;
	case EVENT_DOWNLOADFAILED:
		{
		}
		break;
	case EVENT_STARTAUDIOFAILED:
		{
//			parent->SetDlgItemText(IDC_STARTAUDIO,_bstr_t("开启音频"));
			_bstr_t itemmsg="启动音频失败";
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_STARTAUDIOOK:
		{//请成IPC对讲成功
			int error=IP_NET_DVR_AddTalk(lUser);
			if(error)
			{
				_bstr_t itemmsg="此IPC参数不一致，不能加入广播";
				parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
			}
		}
		break;
	default:
		{
			_bstr_t itemmsg="未处理事件";
			//parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	}
	return 0;
}


LONG __stdcall OnReadAndSave(LONG lUser,const char * msgType,LONG lMsgCode,LONG flag,const char * xml,void * pUser)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pUser;
	IP_NET_DVR_WriteAUXStringEx(lUser,(char *)msgType,lMsgCode,flag,(char *)xml);
	return 0;
}
 

// CTestNetDllDlg 消息处理程序

BOOL CTestNetDllDlg::OnInitDialog()
{
	IP_NET_DVR_Init();
	CDialog::OnInitDialog();
	m_lRealHandle=0;
	m_SwitchFromlRealHandle=0;

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetDlgItemText(IDC_PTZIP,_T("192.168.3.254"));
	SetDlgItemText(IDC_PTZPORT,_T("8091"));


	SetDlgItemText(IDC_PTZUSER,_T("admin"));
	SetDlgItemText(IDC_PTZPASS,_T("123456"));
	SetDlgItemText(IDC_VIDEOPORT,_T("554"));

 
	//IP_CFG_SetReadAndWirteCallBack(OnReadAndSave,this);//配置界面DLL版本

 
	IP_NET_DVR_SetStatusEventCallBack(OnStateEvent,this);//设置状态回调
	IP_NET_DVR_SetAUXResponseCallBack(OnAUXResponse,this);//设置配置消息回调

	
	IP_NET_DVR_SetReplayDataCallBack(::OnPlayBackDataRecv,this);//设置回放数据回调
	IP_NET_DVR_SetPlayActionEventCallBack(::OnPlayActionEvent,this);//设置控制状态回调

	m_logList.InsertColumn(0,_T("日志文件"),0,160);
	m_logList.InsertColumn(1,_T("大小"),0,60);
	SetDlgItemText(IDC_UPFILENAME,_T("c:\\164升级文件.bin"));

	SetTimer(100,100,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CTestNetDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTestNetDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestNetDllDlg::OnBnClickedAdd()
{
	CString temp;
	GetDlgItemText(IDC_PTZIP,temp);
	_bstr_t ip=temp;
	GetDlgItemText(IDC_PTZUSER,temp);
	_bstr_t user=temp;
	GetDlgItemText(IDC_PTZPASS,temp);
	_bstr_t pass=temp;
	int port=GetDlgItemInt(IDC_PTZPORT);
	LONG retValue;
	m_nLastCmdId=IP_NET_DVR_Login(ip,port,user,pass,NULL);
	if(m_nLastCmdId==0)
	{
		MessageBox(_T("增加登陆失败!"));
		return ;
	}else
	{

	}
	CPTZClass * pItem=new CPTZClass();	
	ptzList[m_nLastCmdId]=pItem;
	pItem->m_nUserId=m_nLastCmdId;
	_bstr_t itemname=ip;
	if(m_ptzTreeList.GetRootItem()!=NULL)
	{
		HTREEITEM itemid=m_ptzTreeList.InsertItem(itemname,NULL);
		m_ptzTreeList.SetItemData(itemid,m_nLastCmdId);
	}
	else
	{
		//HTREEITEM hItem=m_ListViewTree.GetRootItem();
		HTREEITEM itemid=m_ptzTreeList.InsertItem(itemname,NULL);
		m_ptzTreeList.SetItemData(itemid,m_nLastCmdId);
	}
}



void CTestNetDllDlg::OnDestroy()
{
	KillTimer(100);
	KillTimer(200);
	IP_NET_DVR_Cleanup();
	IP_TPS_ReleaseAll();
	map<LONG,CPlayerClass*>::iterator item;
	item=playlist.begin();
	while(item!=playlist.end())
	{
		CPlayerClass* pItem=item->second;
		delete pItem;pItem=item->second=NULL;
		item++;
	}
	playlist.clear();
	map<LONG,CPTZClass*>::iterator item2;
	item2=ptzList.begin();
	while(item2!=ptzList.end())
	{
		CPTZClass* pItem=item2->second;
		delete pItem;
		item2->second=pItem=NULL;
		item2++;
	}
	ptzList.clear();
	CDialog::OnDestroy();
}



LONG CALLBACK  OnMediaRecv(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg *)pExtData->pUserData;
	CPlayerClass * pPlayItem=parent->playlist[lRealHandle];	

	if(pPlayItem==NULL)
	{//没有处理接收到视频参数，才会出现为空的情况
		parent->playlist.erase(lRealHandle);
		return -1;
	}
	if(parent->m_lRealHandle!=lRealHandle)// && parent->m_SwitchFromlRealHandle!=lRealHandle)
	{//用这个来控制当前播放哪一路
		return 0;
	}

	if(dwDataType==0)
	{//video

		int rdv=rand()%100;
		//if(rdv>90){}else
		{
			IP_TPS_InputVideoData(pPlayItem->m_nPlayPort,pBuffer,dwBufSize,pExtData->bIsKey,(DWORD)pExtData->timestamp);
		}
	}
	else if(dwDataType==1)
	{//audio

		/*
		static FILE * f=NULL;
		if(f==NULL)
		{
			f=fopen("c:\\g711.g711","wb+");
		}
		fwrite(pBuffer,1,dwBufSize,f);
		*/
		 IP_TPS_InputAudioData(pPlayItem->m_nPlayPort,pBuffer,dwBufSize,(DWORD)pExtData->timestamp);
	}else{
		//
	}

	return 0;
}



void CTestNetDllDlg::OnBnClickedStopplay()
{
	if(m_lRealHandle==0)
	{
		MessageBox(_T("请选择要停止的视频"));
		return;
	}
	IP_NET_DVR_StopRealPlay(m_lRealHandle);
	CPlayerClass * pItem=playlist[m_lRealHandle];
	playlist.erase(m_lRealHandle);
	if(pItem)
	{
		HTREEITEM hItem=m_ListViewTree.GetRootItem();
		while(hItem)
		{
			LONG tagData=(LONG)m_ListViewTree.GetItemData(hItem);
			if(tagData==m_lRealHandle)
			{
				m_ListViewTree.DeleteItem(hItem);
				break;
			}
			hItem=m_ListViewTree.GetNextSiblingItem(hItem);
		}
		IP_TPS_DeleteStream(pItem->m_nPlayPort);
		delete pItem;
	}
	m_lRealHandle=0;
}




int nowPortIndex=1;
void CTestNetDllDlg::OnBnClickedPlay()
{
	int videoport=GetDlgItemInt(IDC_VIDEOPORT);

	//int isD1=GetDlgItemInt(IDC_PLAYD1);
	int isD1=m_isD1.GetCheck();

	//int isTcp=GetDlgItemInt(IDC_ISTCP);
	int isTcp=m_isTcp.GetCheck();

	CString temp;
	GetDlgItemText(IDC_PTZIP,temp);
	_bstr_t ip=temp;
	GetDlgItemText(IDC_PTZUSER,temp);
	_bstr_t user=temp;
	GetDlgItemText(IDC_PTZPASS,temp);
	_bstr_t pass=temp;
	int port=GetDlgItemInt(IDC_PTZPORT);

/*
	IP_NET_DVR_Login(ip, videoport, user, pass, NULL);
	IP_NET_DVR_Init();
	VIDEO_PARAM pParamBuffer;
	IP_TPS_OpenStream(0,pParamBuffer,sizeof(VIDEO_PARAM),0,40);
	IP_TPS_SetDecCallBack(0,OnMediaDataRecv,this);
	IP_TPS_Play(0,0);
	IP_TPS_InputVideoData(0,PBYTE pBuf,DWORD nSize,int isKey,DWORD timestamp);
*/

	
	USRE_VIDEOINFO userinfo;
	userinfo.bIsTcp=isTcp;
	userinfo.nVideoPort=videoport;
	userinfo.nVideoChannle=(0<<16) | (isD1? 0:1);//ipc
	//userinfo.nVideoChannle=(1<<16) | (isD1? 0:1);//dvs
	userinfo.pUserData=this;

	
	LONG retValue=IP_NET_DVR_RealPlayEx((char *)ip,(char *)user,(char *)pass,OnMediaRecv,&userinfo,1);

	if(retValue!=0)
	{
		CPlayerClass * pItem=playlist[retValue];
		if(pItem==NULL)
		{
			pItem=new CPlayerClass();
			playlist[retValue]=pItem;

			pItem->m_nPlayPort=nowPortIndex++;//指定为1号播放播放此视频
			pItem->m_nHaveSetup=0;
			pItem->m_nStreamId=retValue;
			CWnd * pCWnd=GetDlgItem(IDC_VIDEORECT);
			pItem->m_hPlayHwnd=pCWnd->m_hWnd;
		}

		pItem=playlist[m_lRealHandle];
		if(pItem)
		{
			IP_TPS_SetVideoOn(pItem->m_nPlayPort,0);//停止当前正在播放的视频
		}else
		{
			playlist.erase(m_lRealHandle);
		}
		
		m_lRealHandle=retValue;



		_bstr_t itemname;
		itemname="";
		itemname=itemname+"视频"+ip;
		if(m_ListViewTree.GetRootItem()!=NULL)
		{
			HTREEITEM itemid=m_ListViewTree.InsertItem(itemname,NULL);
			m_ListViewTree.SetItemData(itemid,retValue);
		}
		else
		{
			//HTREEITEM hItem=m_ListViewTree.GetRootItem();
			HTREEITEM itemid=m_ListViewTree.InsertItem(itemname,NULL);
			m_ListViewTree.SetItemData(itemid,retValue);
		}
		
	}
	


	//if(retValue!=0)
	//{
	//	IP_TPS_OpenStream(nPort,PBYTE pParam,DWORD pSize,int isAudioParam,DWORD nBufPoolSize);
	//	DLLPLAYER_API int __stdcall IP_TPS_Play(LONG nPort, HWND hWnd);
	//}
}



void CTestNetDllDlg::OnBnClickedAutoview()
{
	
}




LONG CALLBACK OnAUXResponse(LONG lUser,LONG nType,char *pResponse,void *pUser)
{
	CTestNetDllDlg * parent=(CTestNetDllDlg*)pUser;

	parent->readIpcConfig.OnAUXResponse(lUser,nType,pResponse);
	

	//IP_CFG_InputAUXResponse(lUser,nType,pResponse);//配置界面DLL版本

	CPTZClass * pItem=parent->ptzList[lUser];
	int ErrorCode=nType & 0xff000000;
	int ActionCode=nType & 0xffffff;
	switch(ActionCode)
	{
	case CMD_SYSTEM_MANAGE_BASE + 34:
		{
			int error=1;
			//parent->SetDlgItemText(IDC_USERDATA,_bstr_t(pResponse));
		}
		break;
	case CMD_GET_SERIALNUMBER:
		{
			_bstr_t itemmsg="SerialNumber=";
			itemmsg=itemmsg+pResponse;
			TiXmlDocument doc;
			if(doc.Parse(pResponse) && doc.RootElement()!=NULL)
			{
				const char * pSnString=doc.RootElement()->Attribute("SerialNumber");
				itemmsg="SerialNumber=";
				itemmsg=itemmsg+pSnString;
			}
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
		case CMD_GET_LOGFILE_LIST:
		{//日志返回信息
			_bstr_t logfilelist(pResponse);
			TiXmlDocument doc;
			doc.Parse(pResponse);
			if(!doc.Error())
			{
				int logfilecount=0;
				TiXmlElement * pItem=doc.RootElement()->FirstChildElement();
				parent->m_logList.DeleteAllItems();
				int idx=0;
				while(pItem)
				{
					const char * filename=pItem->Attribute("FilePath");
					int itemId=parent->m_logList.InsertItem(idx++,_bstr_t(filename));
					const char * filesize=pItem->Attribute("FileLength");
					parent->m_logList.SetItemText(itemId,1,_bstr_t(filesize)); 
					pItem=pItem->NextSiblingElement();
				}
			}
		}
		break;
		case CMD_GET_RECORD_FILE_LIST:
			{
				_bstr_t logfilelist(pResponse);
				TiXmlDocument doc;
				doc.Parse(pResponse);
				if(!doc.Error())
				{
					int logfilecount=0;
					TiXmlElement * pItem=doc.RootElement()->FirstChildElement();
					parent->m_logList.DeleteAllItems();
					int idx=0;
					while(pItem)
					{
						const char * filename=pItem->Attribute("FilePath");
						int itemId=parent->m_logList.InsertItem(idx++,_bstr_t(filename));
						const char * filesize=pItem->Attribute("FileLength");
						parent->m_logList.SetItemText(itemId,1,_bstr_t(filesize)); 
						pItem=pItem->NextSiblingElement();
					}
				}
			}
			break;
		case CMD_GET_SYSTEMCONTROLSTRING:
		{//获取能力集
			_bstr_t controlstring;
			controlstring=pResponse;
		}
		break;
		case CMD_GET_MEDIA_VIDEO_CONFIG:
			{
				TiXmlDocument doc;
				doc.Parse(pResponse);
				if(!doc.Error())
				{
					TiXmlNode * pNode=doc.RootElement()->SelectNode("Capture");
					if(pNode)
					{
						TiXmlElement * pCapture=pNode->ToElement();
						_bstr_t itemValue;
						itemValue=pCapture->Attribute("Brightness");
						parent->SetDlgItemText(IDC_B,itemValue);						
						itemValue=pCapture->Attribute("SaturationCb");
						parent->SetDlgItemText(IDC_CB,itemValue);						
						itemValue=pCapture->Attribute("SaturationCr");
						parent->SetDlgItemText(IDC_CR,itemValue);
						itemValue=pCapture->Attribute("Contrast");
						parent->SetDlgItemText(IDC_H,itemValue);
					}
				}
			}
			break;
		case CMD_SET_MEDIA_VIDEO_CAPTURE:
			{
				if(ErrorCode!=0)
				{
					parent->MessageBox(_T("保存配置失败"),_T("保存配置失败"));
				}else
				{
					parent->MessageBox(_T("保存配置成功"),_T("保存配置成功"));
				}
			}
			break;
	}
	return 0;
}



void CTestNetDllDlg::OnBnClickedReadconfig()
{//读取配置	
	CheckCmdIdOk();
	IP_NET_DVR_GetDVRConfig(m_nLastCmdId,CMD_GET_MEDIA_VIDEO_CONFIG,0,"",0,0); 
}


void CTestNetDllDlg::OnBnClickedSaveconfig()
{//保存配置
	
	CheckCmdIdOk();
	_bstr_t xml;
	CString b;
	CString cb;
	CString cr;
	CString h;
	GetDlgItemText(IDC_B,b);
	GetDlgItemText(IDC_CB,cb);
	GetDlgItemText(IDC_CR,cr);
	GetDlgItemText(IDC_H,h);
	xml="";
	xml=xml+"<Capture VideoFormat=\"PAL\"   Brightness=\""+b.GetBuffer(0)+"\"   SaturationCb=\""+cb.GetBuffer(0)\
		+"\"  SaturationCr=\""+cr.GetBuffer(0)+"\"  Contrast=\""+h.GetBuffer(0)+"\"  />";
	const char * pXml=(const char *)xml;
	IP_NET_DVR_SetDVRConfig(m_nLastCmdId,CMD_SET_MEDIA_VIDEO_CAPTURE,0,(LPVOID)pXml,strlen(pXml)); 
}



BOOL CTestNetDllDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd==m_VideoRect.m_hWnd || pMsg->message==WM_MOUSEWHEEL)
	{
		if(pMsg->message==WM_MOUSEWHEEL)
		{
			int ok=1;
		}
		if(playlist.size()>0)
		{
			map<LONG,CPlayerClass*>::iterator item;
			item=playlist.begin();
			while(item!=playlist.end())
			{
				CPlayerClass * pPlayItem=item->second;
				if(pPlayItem!=NULL)
				{
					if(pPlayItem->m_hPlayHwnd==pMsg->hwnd|| pMsg->message==WM_MOUSEWHEEL)
					{
						IP_TPS_InputMouseEvent(pPlayItem->m_nPlayPort,pMsg->message,pMsg->wParam,pMsg->lParam);
						break;
					}
				}
				item++;
			}

			if(pMsg->message==WM_MOUSEMOVE)
			{			
				if (!m_bTracking)
				{
					TRACKMOUSEEVENT t = 
					{
						sizeof(TRACKMOUSEEVENT),
							TME_LEAVE,
							m_VideoRect.m_hWnd,
							0
					};
					if (::_TrackMouseEvent(&t))
					{
						m_bTracking = true;
						//Invalidate();
					}
				}
			}else if(pMsg->message==WM_MOUSELEAVE)
			{
				m_bTracking=false;
			}
		}
	} 

	if(pMsg->message==WM_LBUTTONDOWN)
	{
		map<LONG,CPTZClass*>::iterator item;
		item=ptzList.begin();
		if(m_nLastCmdId!=0)
		{
			//CPTZClass * pItem=item->second;
			if(pMsg->hwnd==GetDlgItem(IDC_LEFT)->m_hWnd)
			{
				IP_NET_DVR_PTZControl(m_nLastCmdId,PAN_LEFT,5,5);
			}else if(pMsg->hwnd==GetDlgItem(IDC_RIGHT)->m_hWnd)
			{
				IP_NET_DVR_PTZControl(m_nLastCmdId,PAN_RIGHT,5,5);
			}else if(pMsg->hwnd==GetDlgItem(IDC_UP)->m_hWnd)
			{
				IP_NET_DVR_PTZControl(m_nLastCmdId,TILT_UP,5,5);
			}else if(pMsg->hwnd==GetDlgItem(IDC_DOWN)->m_hWnd)
			{
				IP_NET_DVR_PTZControl(m_nLastCmdId,TILT_DOWN,5,5);
			}
		}
	}else if(pMsg->message==WM_LBUTTONUP)
	{
		map<LONG,CPTZClass*>::iterator item;
		item=ptzList.begin();
		if(m_nLastCmdId!=0)//item!=ptzList.end())
		{
			//CPTZClass * pItem=item->second;
			if(pMsg->hwnd==GetDlgItem(IDC_LEFT)->m_hWnd
				|| pMsg->hwnd==GetDlgItem(IDC_RIGHT)->m_hWnd
				|| pMsg->hwnd==GetDlgItem(IDC_UP)->m_hWnd
				|| pMsg->hwnd==GetDlgItem(IDC_DOWN)->m_hWnd
			  )
			{
				IP_NET_DVR_PTZControl(m_nLastCmdId,STOPACTION,5,5);
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}



void CTestNetDllDlg::OnBnClickedAddpreset()
{//增加预置点 IDC_PRESET	
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	if(m_nLastCmdId!=0)//item!=ptzList.end())
	{
		//CPTZClass * pItem=item->second;
		int preset=GetDlgItemInt(IDC_PRESET);
		IP_NET_DVR_PTZPreset(m_nLastCmdId,SET_PRESET,preset);
	}
}


void CTestNetDllDlg::OnBnClickedDelpreset()
{
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	if(m_nLastCmdId!=0)//item!=ptzList.end())
	{
		//CPTZClass * pItem=item->second;
		int preset=GetDlgItemInt(IDC_PRESET);
		IP_NET_DVR_PTZPreset(m_nLastCmdId,CLE_PRESET,preset);
	}
}



void CTestNetDllDlg::OnBnClickedCallpreset()
{
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	if(m_nLastCmdId!=0)//item!=ptzList.end())
	{
		//CPTZClass * pItem=item->second;
		int preset=GetDlgItemInt(IDC_PRESET);
		IP_NET_DVR_PTZPreset(m_nLastCmdId,GOTO_PRESET,preset);
	}
}


void CTestNetDllDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem=m_ListViewTree.GetSelectedItem();
	m_SwitchFromlRealHandle=m_lRealHandle;
	m_lRealHandle=(LONG)m_ListViewTree.GetItemData(hItem);
	if(m_lRealHandle)
	{
		//IP_TPS_SetVideoOn(oldplayid,0);
		//IP_TPS_SetVideoOn(m_lRealHandle,1);
		CPlayerClass * player1=playlist[m_SwitchFromlRealHandle];
		CPlayerClass * player2=playlist[m_lRealHandle];
		if(player1!=NULL && player2!=NULL)
		{
			//IP_TPS_SwitchVideo(player1->m_nPlayPort,player2->m_nPlayPort);
		}
		if(player1)
		{
			IP_TPS_SetVideoOn(player1->m_nPlayPort,0);
		}
		if(player2)
		{
			IP_TPS_SetVideoOn(player2->m_nPlayPort,1);
		}
	}
	*pResult = 0;
}

void CTestNetDllDlg::OnBnClickedSearchlog()
{
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	if(m_nLastCmdId!=0)//item!=ptzList.end())
	{
		CPTZClass * pItem=item->second;
		IP_NET_DVR_FindDVRLogFile(m_nLastCmdId);
	}
}


void CTestNetDllDlg::OnBnClickedUploadconfig()
{
	if(m_nLastCmdId!=0)
	{
		IP_NET_DVR_SetConfigFile(m_nLastCmdId,"c:\\20110728151435_config.xml");
	}
}


void CTestNetDllDlg::OnBnClickedDownloadlog()
{	
	CheckCmdIdOk();
	CString logfile;
	GetDlgItemText(IDC_SELECTFILE,logfile);
	_bstr_t logfilenamt(logfile);
	IP_NET_DVR_GetFileByName(m_nLastCmdId,LOG_FILE,(char *)logfilenamt,"c:\\");
	 	
}

void CTestNetDllDlg::OnBnClickedStartwritelog()
{
	//3表示写文件，同时要用软件也能看到
	IP_NET_DVR_SetLogToFile(3,"c:\\",0);
}

void CTestNetDllDlg::OnLvnItemchangedLoglist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);	
	CString logfile=m_logList.GetItemText(pNMLV->iItem,0);
	SetDlgItemText(IDC_SELECTFILE,logfile);
	*pResult = 0;
}




void CTestNetDllDlg::OnBnClickedReboot()
{	
	CheckCmdIdOk();//m_nLastCmdId
	IP_NET_DVR_RebootDVR(m_nLastCmdId); 
}



int CTestNetDllDlg::CheckCmdIdOk()
{
	if(m_nLastCmdId==0)
	{
		map<LONG,CPTZClass*>::iterator item;
		item=ptzList.begin();
		if(item!=ptzList.end())
		{
			CPTZClass * pItem=item->second;
			m_nLastCmdId=pItem->m_nUserId;
		}
	}
	return m_nLastCmdId;
}

void CTestNetDllDlg::OnBnClickedStartupload()
{
	CheckCmdIdOk();
	CString filename;
	GetDlgItemText(IDC_UPFILENAME,filename);
	int retValue=IP_NET_DVR_Upgrade(m_nLastCmdId,(char *)_bstr_t(filename)); 
}


void CTestNetDllDlg::OnBnClickedStopupload()
{
	CheckCmdIdOk();
	CString filename;
	GetDlgItemText(IDC_UPFILENAME,filename);
	IP_NET_DVR_CloseUpgradeHandle(m_nLastCmdId);		 
}



DWORD nGetProcessTick=0;
void CTestNetDllDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==100)
	{		
		CheckCmdIdOk();
		if(::GetTickCount()-nGetProcessTick<1000)
		{
			return;
		}
		nGetProcessTick=::GetTickCount();
		char showmsg[100];
		if(m_nLastCmdId!=0)
		{
			if(IP_NET_DVR_GetUpgradeState(m_nLastCmdId)==0)
			{//说明在执行上传任务
				LONG proValue=IP_NET_DVR_GetUpgradeProgress(m_nLastCmdId);
				sprintf(showmsg,"上传进度为:%d",proValue);				
				SetDlgItemText(IDC_LASTALARMTEXT,_bstr_t(showmsg));
			}
			else if(IP_NET_DVR_GetDownloadState(m_nLastCmdId)==0)
			{//说明正在下载中
				LONG proValue=IP_NET_DVR_GetDownloadPos(m_nLastCmdId);
				sprintf(showmsg,"下载进度为:%d",proValue);				
				SetDlgItemText(IDC_LASTALARMTEXT,_bstr_t(showmsg));
			}
		}
	}else if(nIDEvent==200)
	{//自动轮询


		/*
		OnBnClickedStopplay();
		OnBnClickedPlay();
		return ;
		*/

		/*
		if(m_nAutoViewId==1)
		{
			m_VideoRect.SetWindowPos(NULL,0,0,400,300,0);
		}else
		{
			m_VideoRect.SetWindowPos(NULL,0,0,600,400,0);
		}
		*/
		int count=m_ListViewTree.GetCount();
		if(count>1)
		{
			if(m_nAutoViewId>=count)
			{
				m_nAutoViewId=0;
			}
			int idx=0;
			HTREEITEM hItem=m_ListViewTree.GetRootItem();
			while(idx<m_nAutoViewId)
			{
				hItem=m_ListViewTree.GetNextSiblingItem(hItem);
				if(hItem==NULL)
				{
					break;
				}
				idx++;
			}
			if(hItem)
			{
				m_nAutoViewId++;
				m_SwitchFromlRealHandle=m_lRealHandle;
				m_lRealHandle=m_ListViewTree.GetItemData(hItem);

				CPlayerClass * player1=playlist[m_SwitchFromlRealHandle];
				CPlayerClass * player2=playlist[m_lRealHandle];
				if(player1!=NULL && player2!=NULL)
				{
					//IP_TPS_SwitchVideo(player1->m_nPlayPort,player2->m_nPlayPort);
				} 
				if(player1)
				{
					IP_TPS_SetVideoOn(player1->m_nPlayPort,0);
				}
				if(player2)
				{
					IP_TPS_SetVideoOn(player2->m_nPlayPort,1);
				} 
			}
		}
	}
}

void CTestNetDllDlg::OnBnClickedStopdown()
{	
	CheckCmdIdOk();
	if(IP_NET_DVR_GetDownloadState(m_nLastCmdId)==0)
	{
		LONG ret=IP_NET_DVR_StopGetFile(m_nLastCmdId);		
		if(ret!=0)
		{
			MessageBox(_T("停止上传失败"),_T("停止上传失败"));
		}
	} 
}


void CTestNetDllDlg::OnBnClickedSearchavi()
{	
	CheckCmdIdOk();
	char searchxml[500];	 
	char datestr[50]; 
	char datestr2[50];
	SYSTEMTIME time;
	::GetSystemTime(&time);
	CTime timeItem=time;
	sprintf(datestr,"%04d%02d%02d",timeItem.GetYear(),timeItem.GetMonth()-1,1);
	sprintf(datestr2,"%04d%02d%02d",timeItem.GetYear(),timeItem.GetMonth(),timeItem.GetDay()+1);
	//搜索今天的音视频录像文件的第一页

	_bstr_t mediatype;
	if(m_haveAudio.GetCheck())
	{
		mediatype="AUDIOVIDEO";
	}else
	{
		mediatype="VIDEO";
	}

	sprintf(searchxml,"<REQUEST_PARAM\nRecordMode=\"ALL\"StartTime=\"%s 00:00:00\"\nEndTime=\"%s 23:59:59\"\nMediaType=\"%s\"\nStreamIndex=\"-1\"\nMinSize=\"-1\"\nMaxSize=\"-1\"\nPage=\"0\"\n/>"
		,datestr,datestr2,(char *)mediatype);
	LONG ret;
	TRACE1("%s",searchxml);
	ret=IP_NET_DVR_SystemControl(m_nLastCmdId,(long)CMD_GET_RECORD_FILE_LIST,0,(char *)searchxml);
	if(ret!=0)
	{
		MessageBox(_T("查询失败"),_T("查询失败"));
	} 
}



//抓图
void CTestNetDllDlg::OnBnClickedCapimage()
{
	if(playlist.size()<=0)
	{
		return ;
	}

	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{
		int RetValue=IP_TPS_CatchPic(player->m_nPlayPort,"c:\\");
		if(RetValue)
		{
			MessageBox(_T("抓图失败"),_T("抓图失败"));
		}
	}else
	{
		playlist.erase(m_lRealHandle);
	}
}



void CTestNetDllDlg::OnBnClickedReadsn()
{	
	CheckCmdIdOk();
	LONG ret=IP_NET_DVR_SystemControl(m_nLastCmdId,(long)CMD_GET_SERIALNUMBER,0,"");
	if(ret<0)
	{
		MessageBox(_T("读取SN失败"),_T("读取失败"));
	}
}

void CTestNetDllDlg::OnBnClickedDiszoom()
{
	if(playlist.size()<=0)
	{
		return ;
	}

	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{
		int RetValue=IP_TPS_SetZoomRectOn(player->m_nPlayPort,0);
	}else
	{
		playlist.erase(m_lRealHandle);
	}

}



void CTestNetDllDlg::OnBnClickedZoomin()
{
	if(playlist.size()<=0)
	{
		return ;
	}
	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{ 
		int RetValue=IP_TPS_SetZoomRectOn(player->m_nPlayPort ,1);
	}else
	{
		playlist.erase(m_lRealHandle);
	}
}



void CTestNetDllDlg::OnBnClickedZoomout()
{
	if(playlist.size()<=0)
	{
		return ;
	}
	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{
		int RetValue=IP_TPS_SetZoomRectOn(player->m_nPlayPort ,2);
	}else
	{
		playlist.erase(m_lRealHandle);
	}
}




void CTestNetDllDlg::OnBnClickedZoommove()
{
	if(playlist.size()<=0)
	{
		return ;
	}
	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{ 
		int RetValue=IP_TPS_SetZoomRectOn( player->m_nPlayPort,3);
	}else
	{
		playlist.erase(m_lRealHandle);
	}
}



void CTestNetDllDlg::OnBnClickedFullfill()
{
	if(playlist.size()<=0)
	{
		return ;
	}
	
	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{ 
		int nowstatus=IP_TPS_GetFullFillStatus(player->m_nPlayPort);
		if(nowstatus)
		{
			IP_TPS_SetFullFillStatus(player->m_nPlayPort,0);
		}
		else
		{
			IP_TPS_SetFullFillStatus(player->m_nPlayPort,1);
		}
	}else
	{
		playlist.erase(m_lRealHandle);
	}
	
}

void CTestNetDllDlg::OnBnClickedSearchipc()
{
	m_searchDlg.DoModal();
}



//选择云台
void CTestNetDllDlg::OnTvnSelchangedPtzlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem=m_ptzTreeList.GetSelectedItem();
	if(hItem!=NULL)
	{
		m_nLastCmdId=(LONG)m_ptzTreeList.GetItemData(hItem);
	}
	*pResult = 0;
}


//开启广播
void CTestNetDllDlg::OnBnClickedG711audio()
{
	//int audiotype, int samplerate, int bitspersample, int channels
	int error=IP_NET_DVR_StartTalk(0,16000,16,2);
	if(error)
	{
		MessageBox(_T("启动广播失败"));
	}
}

void CTestNetDllDlg::OnBnClickedStopaudo()
{
	int error=IP_NET_DVR_StopTalk();
	if(error)
	{
		MessageBox(_T("停止广播失败"));
	}
}

void CTestNetDllDlg::OnBnClickedAacaudio()
{
	int error=IP_NET_DVR_StartTalk(1,16000,16,2);
	if(error)
	{
		MessageBox(_T("启动广播失败"));
	}
}

void CTestNetDllDlg::OnBnClickedStartipcaudio()
{	
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	CPTZClass * pItem=NULL;
	while(item!=ptzList.end())
	{
		pItem=item->second;
		if(m_nLastCmdId==pItem->m_nUserId)
		{
			break;
		}
		item++;
	} 
	if(pItem!=NULL)
	{
		if(pItem->m_bAudioIsOn==0)
		{
			int error=IP_NET_DVR_StartVoiceCom(m_nLastCmdId,0,1,NULL,NULL);
			if(error)
			{
				MessageBox(_T("关闭失败"));
			}else
			{
				pItem->m_bAudioIsOn=1;
			}
		}else
		{
			MessageBox(_T("当前设备处于对讲状态，打开失败"));
		}
	}

}


void CTestNetDllDlg::OnBnClickedStopipcaudio()
{
	map<LONG,CPTZClass*>::iterator item;
	item=ptzList.begin();
	CPTZClass * pItem=NULL;
	while(item!=ptzList.end())
	{
		pItem=item->second;
		if(m_nLastCmdId==pItem->m_nUserId)
		{
			break;
		}
		item++;
	} 
	if(pItem!=NULL)
	{ 
		int error=IP_NET_DVR_StopVoiceCom(m_nLastCmdId);
		if(error)
		{
			MessageBox(_T("关闭失败"));
		}
		pItem->m_bAudioIsOn=0;  
	}
}



void CTestNetDllDlg::OnBnClickedDownloadconfig()
{
	if(m_nLastCmdId!=0)
	{
		IP_NET_DVR_GetConfigFile(m_nLastCmdId,"c:\\20110728151435_config.xml");
	}
}



void CTestNetDllDlg::OnBnClickedAutoviewvideo()
{
	int value=GetDlgItemInt(IDC_TIMEVALUE);
	if(value<3000)
	{
		value=3000;
	}
	CString butmsg;
	GetDlgItemText(IDC_AUTOVIEWVIDEO,butmsg);
	if(butmsg.Compare(_T("开始轮询"))==0)
	{
		SetDlgItemText(IDC_AUTOVIEWVIDEO,_T("停止轮询"));
		SetTimer(200,value,NULL);
	}else
	{
		SetDlgItemText(IDC_AUTOVIEWVIDEO,_T("开始轮询"));
		KillTimer(200);
	}
}



void CTestNetDllDlg::OnBnClickedPlayfile()
{
	int canreplay=IP_NET_DVR_GetReplayAblity(m_nLastCmdId);
	if(canreplay==0)
	{
		MessageBox(_T("不支持回放功能!"));
		return;
	}
	CString filename;
	GetDlgItemText(IDC_SELECTFILE,filename);
	_bstr_t myfile(filename);
	IP_NET_DVR_PlayDeviceFile(m_nLastCmdId,(char *)myfile);
}



LONG   CTestNetDllDlg::OnPlayBackDataRecv(LONG nPtzId,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,LPFRAME_EXTDATA  pExtData)
{
	UpdPackHead * head=(UpdPackHead *)pBuffer;
	int size=sizeof(UpdPackHead);
	if(dwDataType==0)
	{//video
		IP_TPS_InputVideoData(nPtzId,pBuffer+size,dwBufSize-size,pExtData->bIsKey,pExtData->timestamp);
	}else if(dwDataType==1)
	{
		IP_TPS_InputAudioData(nPtzId,pBuffer+size,dwBufSize-size,pExtData->timestamp);
	}
	return 0;
}



LONG   CTestNetDllDlg::OnPlayActionEvent(LONG lUser,LONG nType,LONG nFlag,char * pData)
{
	if(ACTION_PLAY==nType)
	{
		if(nFlag==0)
		{
			TiXmlDocument doc;
			doc.Parse(pData);
			if(doc.Error()==0)
			{
				TiXmlElement * pRoot=doc.RootElement();
				const char * videoparan=pRoot->Attribute("VideoParam");
				const char * audioparan=pRoot->Attribute("AudioParam");
				const char * videosecs=pRoot->Attribute("VideoSeconds");
				char paramdata[5120];				
				IP_TPS_OpenStream(lUser,(PBYTE)videoparan,strlen(videoparan),0,40);				
				IP_TPS_OpenStream(lUser,(PBYTE)audioparan,strlen(audioparan),1,40);

				IP_TPS_Play(lUser,m_VideoRect.m_hWnd);

			}
		}
	}
	return 0;
}



void CTestNetDllDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem=m_ListViewTree.GetSelectedItem();
	if(hItem)
	{
		m_SwitchFromlRealHandle=m_lRealHandle;
		m_lRealHandle=(LONG)m_ListViewTree.GetItemData(hItem);
		if(m_lRealHandle)
		{
			CPlayerClass * player1=playlist[m_SwitchFromlRealHandle];
			CPlayerClass * player2=playlist[m_lRealHandle];
			//IP_TPS_SwitchVideo(player1->m_nPlayPort,player2->m_nPlayPort);
			if(player1)
			{
				IP_TPS_SetVideoOn(player1->m_nPlayPort,0);
			}
			if(player2)
			{
				IP_TPS_SetVideoOn(player2->m_nPlayPort,1);
				IP_TPS_PlaySound(player2->m_nPlayPort);
			}
		}
	}
	*pResult = 0;
}



void CTestNetDllDlg::OnBnClickedConfigipc()
{	
	/*if(m_nLastCmdId<=0)
	{
		MessageBox(_T("请选择要配置的设备!"));
		return ;
	}*/
	m_configDlg.DoModal();
}


void CTestNetDllDlg::OnBnClickedCapbyfile()
{
	if(playlist.size()<=0)
	{
		return ;
	}
	CPlayerClass * player= playlist[m_lRealHandle];
	if(player!=NULL)
	{
		int RetValue=IP_TPS_CatchPicByFileName(player->m_nPlayPort,"c:\\test.bmp",0);
		if(RetValue)
		{
			MessageBox(_T("文件抓图失败"),_T("抓图失败"));
		}
	}else
	{
		playlist.erase(m_lRealHandle);
	}
}

void CTestNetDllDlg::OnBnClickedCreateiframe()
{
	if(m_nLastCmdId!=0)
	{
		IP_NET_DVR_CreateIFrame(m_nLastCmdId,0);//主码流产生一个I帧
		//IP_NET_DVR_CreateIFrame(m_nLastCmdId,1);//子码流产生一个I帧
	}
	IP_NET_DVR_Reconnect(m_nLastCmdId);
}


void CTestNetDllDlg::OnBnClickedSaveuserdata()
{
	if(m_nLastCmdId!=0)
	{//char * pOutBuffer,int* nInOutLen
		int maxcount=102400;
		char buffer[102400];
		IP_NET_DVR_GetUserData(m_nLastCmdId,buffer,&maxcount);
		if(maxcount>0)
		{
			SetDlgItemText(IDC_USERDATA,_bstr_t(buffer));
		}
		else
		{
			SetDlgItemText(IDC_USERDATA,_bstr_t("读取用户数据失败"));
		}
		int len=0;
	}
}


void CTestNetDllDlg::OnBnClickedSetuserdata()
{
	if(m_nLastCmdId!=0)
	{
		CString buffer;
		GetDlgItemText(IDC_USERDATA,buffer);
		_bstr_t buf(buffer);
		IP_NET_DVR_SetUserData(m_nLastCmdId,(char *)buf,buf.length());
	}
}


void CTestNetDllDlg::OnBnClickedTest()
{
	readIpcConfig.DoModal();
}


void CTestNetDllDlg::OnClickedButtonTest1()
{
	// TODO: Add your control notification handler code here
	VIDEO_PARAM param;
	IP_NET_DVR_GetVideoParam(m_lRealHandle,&param);
	
	TRACE1("%s\n",param.codec);

//	::MessageBoxA(NULL,(LPCSTR)param.codec,"aa",MB_OK);
	int width = param.width;
	int height = param.height;
	int colorbits = param.colorbits;
	int framerate = param.framerate;
	int bitrate = param.bitrate;
//	::MessageBoxA(NULL,(LPCSTR)param.vol_data,"aa",MB_OK);

	BYTE *pByte = new BYTE[width*height*2];
	memset(pByte,0,(width*height*2));
	long xxxx = sizeof(pByte);
	long yyyy = sizeof(*pByte);


	TRACE1("%s\n",pByte);
//	IP_TPS_InputVideoData(LONG nPort,PBYTE pBuf,DWORD nSize,int isKey,DWORD timestamp);
	DWORD nSize = width*height*2;

	int zzzz = nowPortIndex;
	IP_TPS_InputVideoData(nowPortIndex-1,pByte,nSize,1,1.1);
	TRACE1("%s\n",pByte);

	
	delete []pByte;
}


//void CTestNetDllDlg::OnClickedButtontest2()
//{
//	// TODO: Add your control notification handler code here
//
//	int videoport=GetDlgItemInt(IDC_VIDEOPORT);
//	int isD1=m_isD1.GetCheck();
//	int isTcp=m_isTcp.GetCheck();
//
//	CString temp;
//	GetDlgItemText(IDC_PTZIP,temp);
//	_bstr_t ip=temp;
//	GetDlgItemText(IDC_PTZUSER,temp);
//	_bstr_t user=temp;
//	GetDlgItemText(IDC_PTZPASS,temp);
//	_bstr_t pass=temp;
//	int port=GetDlgItemInt(IDC_PTZPORT);
//
//	USRE_VIDEOINFO userinfo;
//	userinfo.bIsTcp=isTcp;
//	userinfo.nVideoPort=videoport;
//	userinfo.nVideoChannle=(0<<16) | (isD1? 0:1);//ipc
//	userinfo.pUserData=this;
//
//	LONG retValue=IP_NET_DVR_RealPlayEx((char *)ip,(char *)user,(char *)pass,OnMediaRecv,&userinfo,1);
//
//	if(retValue!=0)
//	{
//		int size=sizeof(VIDEO_PARAM);
//		VIDEO_PARAM param;
//		IP_TPS_OpenStream(10,(BYTE *)&param,size,0,40);		
//
//		//IP_TPS_SetDecCallBack(playport,OnMediaDataRecv,pUser); 
//		
//		IP_TPS_Play(0,0);
//
//		VIDEO_PARAM param2;
//		IP_NET_DVR_GetVideoParam(m_lRealHandle,&param2);
//		//
//
//		BYTE *pByte = new BYTE[param2.width*param2.height*2];
//		DWORD nSize = param2.width*param2.height*2;
//		IP_TPS_InputVideoData(10,pByte,nSize,1,40);
//
//		delete []pByte;
//	}
//
//
//}
