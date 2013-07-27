// TestNetDllDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestNetDll.h"
#include "TestNetDllDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if _DEBUG
#pragma comment(lib,"..\\NetSDKDLL\\Debug\\NetSDKDLL.lib")
#else
#pragma comment(lib,"..\\NetSDKDLL\\Release\\NetSDKDLL.lib")
#endif
#pragma comment(lib,"comsuppw.lib")

// CTestNetDllDlg �Ի���


//LONG CALLBACK OnAUXResponse(LONG lUser,LONG nType,char *pResponse,void *pUser);
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



int     CTestNetDllDlg::OnMediaDataRecv(long nPort,char * pBuf,long nSize,FRAME_INFO * pFrameInfo)
{
	//4:1:1   //ת��RGB
	//����Ϊ:pFrameInfo->nHeight* pFrameInfo->nLinseSize[0];
	char * yBuffer= pBuf+16;
	
	//����Ϊ:pFrameInfo-> nHeight * pFrameInfo->nLinseSize[1]/2;
	char * uBuffer= yBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[0];
	
	//����Ϊ:pFrameInfo->nHeight * pFrameInfo->nLinseSize[2]/2;
	char * vBuffer= uBuffer + pFrameInfo->nHeight * pFrameInfo->nLinseSize[1]/2;


	BYTE *dBuffer = new BYTE[3*pFrameInfo->nWidth*pFrameInfo->nHeight];
	

	BITMAPINFOHEADER bih;
	bih.biSize=40;		// header size
	bih.biWidth=pFrameInfo->nWidth;
	bih.biHeight=pFrameInfo->nHeight;
	bih.biPlanes=1;
	bih.biBitCount=24;     // RGB encoded, 24 bit
	bih.biCompression=BI_RGB;   // no compression ��ѹ��
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

	/*
	temp = 1.164383*(y- 16) + 0 + 1.596016*(v - 128);
	dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	temp = (1.164383*(y- 16) - 0.391762*(u - 128) - 0.812969*(v - 128));
	dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
	temp = 1.164383*(y- 16) + 2.017230*(u - 128) + 0;
	dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
   */
	long i = 0, j = 0, k = 0;
	double temp;
	unsigned char y,u,v;
	for( ;i < pFrameInfo->nLinseSize[1]/2*pFrameInfo->nHeight; i++ )
	{
	   u = (unsigned char)uBuffer[i];
	   v = (unsigned char)vBuffer[i];

	   k = 0;
	   for( ; k < 4;k++ )
	   {
		   y = (unsigned char)yBuffer[4*i + k];
		   temp = 1.164383*(y- 16) + 0 + 1.596016*(v - 128);
		   dBuffer[j] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
		   temp = (1.164383*(y- 16) - 0.391762*(u - 128) - 0.812969*(v - 128));
		   dBuffer[j + 1] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
		   temp = 1.164383*(y- 16) + 2.017230*(u - 128) + 0;
		   dBuffer[j + 2] = (unsigned char)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
		   j+=3;
	   }

	   //��ȥ���ಿ��
	   if( i%(pFrameInfo->nLinseSize[1]/2) == (pFrameInfo->nWidth/4 - 1) )
		   i+=( (pFrameInfo->nLinseSize[1]/2) - (pFrameInfo->nWidth/4) );
	}


	
	//��תͼ��
	DWORD WidthBytes = pFrameInfo->nWidth * 3;	
	while((WidthBytes & 3) != 0)WidthBytes++;

	BYTE *Start_Souce,*To,*pInverImage;
	DWORD Size=WidthBytes*pFrameInfo->nHeight;
	pInverImage=new BYTE[Size];

	To=pInverImage;
	if(pInverImage)
	{
		for (DWORD H = 0;H < pFrameInfo->nHeight; H++)
		{		
			Start_Souce=dBuffer+WidthBytes*(pFrameInfo->nHeight-H-1);  //Beging of Last Line          	
			for (DWORD k=0;k<pFrameInfo->nWidth*3;k++) //����һ�� mark
			{
				*(To++)=*(Start_Souce++);
			}		
		}		
	}
	BYTE *tempBuffer = dBuffer;
	dBuffer = pInverImage;
	delete []tempBuffer;
	



	CFile file;
	CFileException e;

	if(m_pBGbuffer == NULL)
	{
		m_pBGbuffer = new BYTE[3*pFrameInfo->nWidth*pFrameInfo->nHeight];
		memcpy(m_pBGbuffer,dBuffer,3*pFrameInfo->nWidth*pFrameInfo->nHeight);

		try
		{
			if(file.Open(_T("E:\\background.bmp"),CFile::modeWrite | CFile::modeCreate,&e) )
			{//д���ļ�
				file.Write((LPSTR)&bfh,sizeof(BITMAPFILEHEADER));
				file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));
				file.Write(m_pBGbuffer,3*pFrameInfo->nWidth*pFrameInfo->nHeight);///////////////////////////////
				file.Close();
			}
		}
		catch (...) 
		{
			file.Close();
			//AfxMessageBox("MyDC::SaveDIB2Bmp");
		}
	}

	try
	{
		if(file.Open(_T("E:\\b_get.bmp"),CFile::modeWrite | CFile::modeCreate,&e) )
		{//д���ļ�
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
	



	//�������======================================================
	BYTE *lpImgData,*lpImgData2,*lpBGData;
	BYTE r,g,b,r1,g1,b1;

	DWORD bytesPerLine=pFrameInfo->nWidth*3;//����ÿ��ͼ����ռ���ֽ���
	//if(bytesperline%4 != 0)
	//{
	//	bytesperline=(bytesperline/4 + 1)*4;
	//}


	BYTE deta = 95;//������ַ���ֵ���ɵ���
	

	for( i = 0; i < pFrameInfo->nHeight; i++ )
	{	
		for( j = 0; j < pFrameInfo->nWidth; j++ )
		{
			lpImgData= dBuffer + bytesPerLine*(pFrameInfo->nHeight-1-i)+j*3;
			lpImgData2 = lpImgData;
			b=*(lpImgData++);
			g=*(lpImgData++);
			r=*(lpImgData++);

			lpBGData= m_pBGbuffer + bytesPerLine*(pFrameInfo->nHeight-1-i)+j*3;
			b1=*(lpBGData++);
			g1=*(lpBGData++);
			r1=*(lpBGData++);

			
		//	if(abs((long)(b-b1)) <=deta )//����û�б仯����Ϊ�Ǳ���
			if( abs(b-b1)<=deta || abs(g-g1)<=deta || abs(r-r1)<=deta )
			{
				*(lpImgData2++) = 255;
				*(lpImgData2++) = 255;
				*(lpImgData2++) = 255;
			}
			else if (i == 0||j==0||i==pFrameInfo->nHeight-1 ||j==pFrameInfo->nWidth-1) //ͼ��߽�㲻����
			{
				*(lpImgData2++) = 255;
				*(lpImgData2++) = 255;
				*(lpImgData2++) = 255;
			}
			else
			{
				*(lpImgData2++) = 0;
				*(lpImgData2++) = 0;
				*(lpImgData2++) = 0;//��ɫ
			}	
			

		}
	}
	//memcpy(dBuffer,lpImgData2,3*pFrameInfo->nWidth*pFrameInfo->nHeight);
	//==============================================================
	try
	{
		if(file.Open(_T("E:\\b_set.bmp"),CFile::modeWrite | CFile::modeCreate,&e) )
		{//д���ļ�
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

	
	//ȥ���ӵ�
	Del_Noise(dBuffer, pFrameInfo->nWidth, pFrameInfo->nHeight, 6);
	Del_Noise(dBuffer, pFrameInfo->nWidth, pFrameInfo->nHeight, 7);
	for( i = 0; i < 5; i++ )
		QuanFangXiangFuShi(dBuffer, pFrameInfo->nWidth, pFrameInfo->nHeight);
	for( i = 0; i < 5; i++ )
		QuanFangXiangPengZhang(dBuffer, pFrameInfo->nWidth, pFrameInfo->nHeight);
	
	try
	{
		if(file.Open(_T("E:\\b_set_2.bmp"),CFile::modeWrite | CFile::modeCreate,&e) )
		{//д���ļ�
			file.Write((LPSTR)&bfh,sizeof(BITMAPFILEHEADER));
			file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));
			file.Write(dBuffer,3*pFrameInfo->nWidth*pFrameInfo->nHeight);
			file.Close();
		}
	}
	catch (...) 
	{
		file.Close();
		//AfxMessageBox("MyDC::SaveDIB2Bmp");
	}


	//EdgeDetection
	D2POINT ImageEdgePoints[1000];//��ŷָ����Ƶı�Ե��
	D2POINT ImageEdgePoints1[1000];
	D2POINT ImageEdgePoints2[1000];
	/*
	EdgeDetection(ImageEdgePoints,dBuffer,pFrameInfo->nWidth,pFrameInfo->nHeight);
	double max_num = 0.0,min_num = 0.0;//y coordinates
	for ( i = 0;i < edgepointsNO;i++ ) //ͳ�Ʋ��뽻���첲�ĳ���
	{
		if (max_num < ImageEdgePoints[i].y)
		{
			max_num = ImageEdgePoints[i].y;
		}
		if (min_num > ImageEdgePoints[i].y)
		{
			min_num = ImageEdgePoints[i].y;
		}
	}

	double asdfasdfadsf = abs(max_num - min_num);///////////////////////////////////////
	*/

	EdgeDetection_2user(ImageEdgePoints1,ImageEdgePoints2,dBuffer,pFrameInfo->nWidth,pFrameInfo->nHeight);
	double max_num = 0.0,min_num = 0.0;//y coordinates
	for ( i = 0;i < edgepointsNO1;i++ ) //ͳ�Ʋ��뽻���첲�ĳ���
	{
		if (max_num < ImageEdgePoints1[i].y)
		{
			max_num = ImageEdgePoints1[i].y;
		}
		if (min_num > ImageEdgePoints1[i].y)
		{
			min_num = ImageEdgePoints1[i].y;
		}
	}
	double tempaaaaaa = abs(max_num-min_num);
	max_num = 0.0,min_num = 0.0;//y coordinates
	for ( i = 0;i < edgepointsNO2;i++ ) //ͳ�Ʋ��뽻���첲�ĳ���
	{
		if (max_num < ImageEdgePoints2[i].y)
		{
			max_num = ImageEdgePoints2[i].y;
		}
		if (min_num > ImageEdgePoints2[i].y)
		{
			min_num = ImageEdgePoints2[i].y;
		}
	}


	FILE *sp;
	try
	{
		sp=fopen("E:\\Pose.txt","w");
		fprintf(sp,"%lf\n%lf\n",tempaaaaaa,abs(max_num-min_num));
		fclose(sp);
	}
	catch(...)
	{
		fclose(sp);
	}
	
	
	delete []dBuffer;
	return 0;
}



/*!
* @brief Del_Noise
*
* ͼ��ȥ�룬������ȥ��
* @param pImageBuffer ��ǰ֡ͼ����Ϣ
* @param WidthBytes ÿ�б�����
* @param points ����׵���Ŀ
*/
void CTestNetDllDlg::Del_Noise(BYTE *pImageBuffer, const long nWidth, const long nHeight, const int points)
{
	long WidthBytes = nWidth*3;
	long nn, mm;
	BYTE *lpSrc,*lpSrc1,*lpTemp1,*lpTemp2,*lpTemp3,*lpTemp4,*lpTemp5,*lpTemp6,*lpTemp7,*lpTemp8;

	for( nn=1;nn<nHeight-1;nn++ )
	{
		for( mm=1;mm<nWidth-1;mm++ )
		{
			lpSrc = (BYTE *)pImageBuffer+WidthBytes*nn+mm*3;//WidthBytes
			lpTemp5=(BYTE *)pImageBuffer+WidthBytes*(nn-1)+(mm-1)*3;//����
			lpTemp1=(BYTE *)pImageBuffer+WidthBytes*(nn-1)+mm*3;//��
			lpTemp6=(BYTE *)pImageBuffer+WidthBytes*(nn-1)+(mm+1)*3;//����
			lpTemp7=(BYTE *)pImageBuffer+WidthBytes*(nn+1)+(mm-1)*3;//����
			lpTemp2=(BYTE *)pImageBuffer+WidthBytes*(nn+1)+mm*3;//��
			lpTemp8=(BYTE *)pImageBuffer+WidthBytes*(nn+1)+(mm+1)*3;//����
			lpTemp3=(BYTE *)pImageBuffer+WidthBytes*nn+(mm-1)*3;//��
			lpTemp4=(BYTE *)pImageBuffer+WidthBytes*nn+(mm+1)*3;//��

			unsigned long p1,p2,p3,p4,p5,p6,p7,p8,e;
			p1=p2=p3=p4=p5=p6=p7=p8=e=0;
			p1=*lpTemp1+*(lpTemp1+1)+*(lpTemp1+2);
			p2=*lpTemp2+*(lpTemp2+1)+*(lpTemp2+2);
			p3=*lpTemp3+*(lpTemp3+1)+*(lpTemp3+2);
			p4=*lpTemp4+*(lpTemp4+1)+*(lpTemp4+2);
			p5=*lpTemp5+*(lpTemp5+1)+*(lpTemp5+2);
			p6=*lpTemp6+*(lpTemp6+1)+*(lpTemp6+2);
			p7=*lpTemp7+*(lpTemp7+1)+*(lpTemp7+2);
			p8=*lpTemp8+*(lpTemp8+1)+*(lpTemp8+2);

			lpSrc1 = lpSrc;
			e = *lpSrc1 + *(lpSrc1+1) + *(lpSrc1 + 2);//����˵����ǰ׵�Ͳ����ٹ�����

			if((e != 765 && (int)(p1+p2+p3+p4+p5+p6+p7+p8) >= 255*3*points))//�����Χ�İ˸�������points���ϸ��׵� ���ô˵�Ϊ�׵�
			{
				*lpSrc = 255;
				*(lpSrc+1)=255;
				*(lpSrc+2)=255;
			}
		}
	}

}




/*!
* @brief QuanFangXiangFuShi
*
* ��̬ѧ������ʴ����
* @param pImageBuffer ��ǰ֡ͼ����Ϣ
* @param width DIB�Ŀ��
* @param height DIB�ĸ߶�
* @param bytesPerLine ͼ��ÿ���ֽ���
*/
void CTestNetDllDlg::QuanFangXiangFuShi(BYTE *pImageBuffer, const long nWidth, const long nHeight)
{
	long bytesPerLine = nWidth*3;
	long i, j, m, n;

	LPBYTE p_data; 	//ָ��DIB����ָ��
	LPBYTE lpSrc; 	//ָ��Դͼ���ָ��
	LPBYTE lpDst; 	//ָ�򻺴�ͼ���ָ��

	p_data = pImageBuffer; 	//�ҵ�DIBͼ��������ʼλ��

	BYTE *p_temp=new BYTE[nHeight*bytesPerLine];

	int B[9] = {0, 1, 0,
				0, 1, 0,
				0, 1, 0}; 	//3��3�ĽṹԪ��

	for ( j = 1; j < nHeight-1; j++ ) 	//ʹ��ȫ����ĽṹԪ�ؽ��и�ʴ
	{
		for ( i = 3; i < bytesPerLine-3; i+=3 )	//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ����������ҡ��ϡ����ıߵ�����
		{
			lpSrc = (unsigned char *)(p_data + bytesPerLine * j + i); //ָ��Դͼ������j�У���i�����ص�ָ��			

			lpDst = (unsigned char *)(p_temp + bytesPerLine * j + i);//ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			

			*lpDst = 0; // Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			*(lpDst+1)=0;
			*(lpDst+2)=0;


			//���Դͼ����3��3�ṹԪ�ض�Ӧλ���а׵�,��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ

			for(int l=0;l<3;l++)
			{
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (B[m + n] == 1)
							continue;
						if (*(lpSrc + (1 - m) * bytesPerLine +(n - 1)*3 ) > 128)//m=0,1,2,��m=0ʱ,�ǵ�����,m=1ʱ�ǵڶ���,m=2ʱ�ǵ�һ��
						{
							*lpDst = 255;
							break;
						}
						else 
							*lpDst = *lpSrc;
					}
				}
				*lpSrc++;
				*lpDst++;
			}
		}
	}
	memcpy(p_data, p_temp, bytesPerLine * nHeight); //���Ƹ�ʴ���ͼ��

	delete []p_temp;//�ͷ��ڴ�
}


/*!
* @brief QuanFangXiangPengZhang
*
* ��̬ѧ�������Ͳ���
* @param pImageBuffer ��ǰ֡ͼ����Ϣ
* @param width DIB�Ŀ��
* @param height DIB�ĸ߶�
* @param bytesPerLine ͼ��ÿ���ֽ���
*/
void CTestNetDllDlg::QuanFangXiangPengZhang(BYTE *pImageBuffer, const long nWidth, const long nHeight)
{
	long bytesPerLine = nWidth*3;
	long i, j, m, n;

	LPBYTE p_data; 	// ָ��DIB����ָ��
	LPBYTE lpSrc;	// ָ��Դͼ���ָ��
	LPBYTE lpDst;	// ָ�򻺴�ͼ���ָ��
	
	p_data = pImageBuffer;	// �ҵ�DIBͼ��������ʼλ��

	BYTE *p_temp=new BYTE[nHeight*bytesPerLine];

	if(p_temp==NULL)
	{
		MessageBox(_T("�����ڴ�ʧ��!"));
		return;
	}
	int B[9] = {1, 0, 1,
				0, 0, 0,
				1, 0, 1}; 	// 3��3�ĽṹԪ��

	for (j = 1; j <  nHeight-1; j++)	// ʹ��ȫ����ĽṹԪ�ؽ�����ײ
	{
		for (i = 3; i < bytesPerLine-3; i +=3)
		{
			lpSrc = (unsigned char *)(p_data + bytesPerLine * j + i);
			lpDst = (unsigned char *)(p_temp + bytesPerLine * j + i);
			*lpDst = *lpSrc;			 
			*(lpDst+1) = *(lpSrc+1);
			*(lpDst+2) = *(lpSrc+2);
			for(int l=0;l<3;l++)
			{
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (B[m + n] == 1)
							continue;
						if (*(lpSrc + (1 - m) * bytesPerLine +(n - 1)*3 ) < 128)
						{
							*lpDst = 0;	
							break;
						}
					}
				}
				*lpSrc++;
				*lpDst++;
			}
		}
	}


	memcpy(p_data, p_temp, bytesPerLine * nHeight );
	delete []p_temp;
}


void CTestNetDllDlg::EdgeDetection(D2POINT edgepoint[], BYTE *pBuffer,const long nWidth,const long nHeight)
{
	edgepointsNO = 0; //��¼�ܵ����ظ���

	DWORD WidthBytes = 3* nWidth;//ͼ���һ���ֽ���
	BYTE *lpSrc,  *lpTemp1,*lpTemp2,*lpTemp3,*lpTemp4,*lpTemp5,*lpTemp6,*lpTemp7,*lpTemp8;
	BYTE r,g,b;
	long nn, mm ;
	
	for( nn = 1; nn < nHeight-1; nn=nn+5)
	{
		for( mm = 1; mm < nWidth-1; mm=mm+5)
		{			
			lpSrc=(BYTE *)pBuffer+WidthBytes*nn+mm*3;//WidthBytes
			lpTemp5=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm-1)*3;//����
			lpTemp1=(BYTE *)pBuffer+WidthBytes*(nn-1)+mm*3;//��
			lpTemp6=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm+1)*3;//����
			lpTemp7=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm-1)*3;//����
			lpTemp2=(BYTE *)pBuffer+WidthBytes*(nn+1)+mm*3;//��
			lpTemp8=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm+1)*3;//����
			lpTemp3=(BYTE *)pBuffer+WidthBytes*nn+(mm-1)*3;//��
			lpTemp4=(BYTE *)pBuffer+WidthBytes*nn+(mm+1)*3;//��

			unsigned long p1,p2,p3,p4,p5,p6,p7,p8;
			p1=p2=p3=p4=p5=p6=p7=p8=0;
			p1=*lpTemp1+*(lpTemp1+1)+*(lpTemp1+2);
			p2=*lpTemp2+*(lpTemp2+1)+*(lpTemp2+2);
			p3=*lpTemp3+*(lpTemp3+1)+*(lpTemp3+2);
			p4=*lpTemp4+*(lpTemp4+1)+*(lpTemp4+2);
			p5=*lpTemp5+*(lpTemp5+1)+*(lpTemp5+2);
			p6=*lpTemp6+*(lpTemp6+1)+*(lpTemp6+2);
			p7=*lpTemp7+*(lpTemp7+1)+*(lpTemp7+2);
			p8=*lpTemp8+*(lpTemp8+1)+*(lpTemp8+2);

			b=*(lpSrc++);			//��ȡRGB��Ϣ
			g=*(lpSrc++);
			r=*(lpSrc++);
			if (b == 255 && g ==255 && r == 255)  //����ǰ׵�Ļ�
				continue;
			else //����Ǻ��Ļ�
			{
				if(((p1+p2+p3+p4+p5+p6+p7+p8)!=255*3*8) && ((p1+p2+p3+p4+p5+p6+p7+p8)!=255*8))//�����Χ�İ˸��㲻ȫ�Ǻ�ɫ����ȫ���ǰ�ɫ�Ļ�
				{
					edgepoint[edgepointsNO].x = mm;		//�Ѹõ�������������edgepoints��
					edgepoint[edgepointsNO].y = nn;
					edgepointsNO ++;
				}			
			}

		}//for
	}//for
}


void CTestNetDllDlg::EdgeDetection_2user(D2POINT edgepoint1[], D2POINT edgepoint2[],BYTE *pBuffer,const long nWidth,const long nHeight)
{
	edgepointsNO = 0; //��¼�ܵ����ظ���
	edgepointsNO1 = 0;
	edgepointsNO2 = 0;

	DWORD WidthBytes = 3* nWidth ;    //ͼ���һ���ֽ���
	BYTE *lpSrc,  *lpTemp1,*lpTemp2,*lpTemp3,*lpTemp4,*lpTemp5,*lpTemp6,*lpTemp7,*lpTemp8;
	BYTE r,g,b;
	long nn, mm ;

	//==================�ֳ�����������м���������2������======================================
	for( nn = 1; nn < nHeight-1; nn=nn+5)
	{
		for( mm = 1; mm < nWidth/2-1; mm=mm+5)
		{			
			lpSrc=(BYTE *)pBuffer+WidthBytes*nn+mm*3;//WidthBytes
			lpTemp5=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm-1)*3;//����
			lpTemp1=(BYTE *)pBuffer+WidthBytes*(nn-1)+mm*3;//��
			lpTemp6=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm+1)*3;//����
			lpTemp7=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm-1)*3;//����
			lpTemp2=(BYTE *)pBuffer+WidthBytes*(nn+1)+mm*3;//��
			lpTemp8=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm+1)*3;//����
			lpTemp3=(BYTE *)pBuffer+WidthBytes*nn+(mm-1)*3;//��
			lpTemp4=(BYTE *)pBuffer+WidthBytes*nn+(mm+1)*3;//��

			unsigned long p1,p2,p3,p4,p5,p6,p7,p8;
			p1=p2=p3=p4=p5=p6=p7=p8=0;
			p1=*lpTemp1+*(lpTemp1+1)+*(lpTemp1+2);
			p2=*lpTemp2+*(lpTemp2+1)+*(lpTemp2+2);
			p3=*lpTemp3+*(lpTemp3+1)+*(lpTemp3+2);
			p4=*lpTemp4+*(lpTemp4+1)+*(lpTemp4+2);
			p5=*lpTemp5+*(lpTemp5+1)+*(lpTemp5+2);
			p6=*lpTemp6+*(lpTemp6+1)+*(lpTemp6+2);
			p7=*lpTemp7+*(lpTemp7+1)+*(lpTemp7+2);
			p8=*lpTemp8+*(lpTemp8+1)+*(lpTemp8+2);

			//��ȡRGB��Ϣ
			b=*(lpSrc++);
			g=*(lpSrc++);
			r=*(lpSrc++);
			if (b == 255 && g ==255 && r == 255)  //����ǰ׵�Ļ�
				continue;
			else               //����Ǻ��Ļ�
			{
				//�����Χ�İ˸��㲻ȫ�Ǻ�ɫ����ȫ���ǰ�ɫ�Ļ�
				if(((p1+p2+p3+p4+p5+p6+p7+p8)!=255*3*8) && ((p1+p2+p3+p4+p5+p6+p7+p8)!=255*8))
				{
					edgepoint1[edgepointsNO1].x = mm;		//�Ѹõ�������������edgepoints��
					edgepoint1[edgepointsNO1].y = nn;
					edgepointsNO1 ++;
				}			
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	for( nn = 1; nn < nHeight-1; nn=nn+5)
	{
		for( mm = nWidth/2; mm < nWidth; mm=mm+5)
		{			
			lpSrc=(BYTE *)pBuffer+WidthBytes*nn+mm*3;//WidthBytes
			lpTemp5=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm-1)*3;//����
			lpTemp1=(BYTE *)pBuffer+WidthBytes*(nn-1)+mm*3;//��
			lpTemp6=(BYTE *)pBuffer+WidthBytes*(nn-1)+(mm+1)*3;//����
			lpTemp7=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm-1)*3;//����
			lpTemp2=(BYTE *)pBuffer+WidthBytes*(nn+1)+mm*3;//��
			lpTemp8=(BYTE *)pBuffer+WidthBytes*(nn+1)+(mm+1)*3;//����
			lpTemp3=(BYTE *)pBuffer+WidthBytes*nn+(mm-1)*3;//��
			lpTemp4=(BYTE *)pBuffer+WidthBytes*nn+(mm+1)*3;//��

			unsigned long p1,p2,p3,p4,p5,p6,p7,p8;
			p1=p2=p3=p4=p5=p6=p7=p8=0;
			p1=*lpTemp1+*(lpTemp1+1)+*(lpTemp1+2);
			p2=*lpTemp2+*(lpTemp2+1)+*(lpTemp2+2);
			p3=*lpTemp3+*(lpTemp3+1)+*(lpTemp3+2);
			p4=*lpTemp4+*(lpTemp4+1)+*(lpTemp4+2);
			p5=*lpTemp5+*(lpTemp5+1)+*(lpTemp5+2);
			p6=*lpTemp6+*(lpTemp6+1)+*(lpTemp6+2);
			p7=*lpTemp7+*(lpTemp7+1)+*(lpTemp7+2);
			p8=*lpTemp8+*(lpTemp8+1)+*(lpTemp8+2);

			//��ȡRGB��Ϣ
			b=*(lpSrc++);
			g=*(lpSrc++);
			r=*(lpSrc++);
			if (b == 255 && g ==255 && r == 255)  //����ǰ׵�Ļ�
				continue;
			else  //����Ǻ��Ļ�
			{
				//�����Χ�İ˸��㲻ȫ�Ǻ�ɫ����ȫ���ǰ�ɫ�Ļ�
				if(((p1+p2+p3+p4+p5+p6+p7+p8)!=255*3*8) && ((p1+p2+p3+p4+p5+p6+p7+p8)!=255*8))
				{
					edgepoint2[edgepointsNO2].x = mm;//�Ѹõ�������������edgepoints��
					edgepoint2[edgepointsNO2].y = nn;
					edgepointsNO2 ++;
				}			
			}
		}//for
	}//for
}



CTestNetDllDlg::CTestNetDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestNetDllDlg::IDD, pParent)//,readIpcConfig(this)
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
	DDX_Control(pDX, IDC_VIDEORECT, m_VideoRect);
}

BEGIN_MESSAGE_MAP(CTestNetDllDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_WM_DESTROY() 
	ON_BN_CLICKED(IDC_PLAY, &CTestNetDllDlg::OnBnClickedPlay)
	ON_WM_TIMER()
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
			if(pPlayItem!=NULL)
			{
				if(pPlayItem!=NULL)
				{
					int playport=pPlayItem->m_nPlayPort;
					int size=sizeof(VIDEO_PARAM);
					IP_TPS_OpenStream(playport,(BYTE *)&pParam->videoParam,size,0,40);					
					HWND playHwnd=pPlayItem->m_hPlayHwnd;
					IP_TPS_SetDecCallBack(playport,OnMediaDataRecv,pUser);//��������ֱ�ӣ�Ҫ�����IP_TPS_Play(lRealHandle,NULL); 
					if(pParam->bHaveAudio)
					{
						IP_TPS_OpenStream(playport,(BYTE *)&pParam->audioParam,sizeof(AUDIO_PARAM),1,40);
						//SetupAACDecoder((BYTE *)&pParam->audioParam,sizeof(AUDIO_PARAM));
					}

					//IP_TPS_Play(playport,playHwnd);//NULL);//�������һ��������ΪNULL�����Դ˴���ֱ�Ӳ��ţ������ǽ���
					IP_TPS_Play(playport,0);
				}
			}

		}
		break;
	case EVENT_CONNECTING:
		{//���ӳɹ�
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
			_bstr_t itemmsg="��¼�ɹ�";
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_PTZALARM:
		{//�澯��Ϣ
			ALARM_ITEM * pAlarmItem=(ALARM_ITEM *)pResponse;
			_bstr_t itemmsg="";
			_bstr_t alarmtypename="";
			switch(pAlarmItem->code)
			{//�Բ�ͬ�������������Ա�ʾ
			default:
				{
					alarmtypename="ת������";
				}
				break;
			}
			char timestr[50];
			sprintf(timestr,"ʱ��--%02d:%02d:%02d",pAlarmItem->time.hour,pAlarmItem->time.minute,pAlarmItem->time.second);
			itemmsg=itemmsg+pAlarmItem->data+timestr;
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	//case EVENT_PTZPRESETINFO://Ԥ�Ƶ���Ϣ 
	
	case EVENT_SENDPTZOK:
		{			
			_bstr_t itemmsg="������̨����ɹ�";
			//������ֱ�ӵ��ã������п��ܻ��������
			//parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_DOWNLOADOK:
		{
			_bstr_t itemmsg="";
			itemmsg=itemmsg+"���سɹ�"+pResponse;
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_UPLOADOK:
		{
			_bstr_t itemmsg="";
			itemmsg=itemmsg+"�ϴ��ɹ�"+pResponse;
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
//			parent->SetDlgItemText(IDC_STARTAUDIO,_bstr_t("������Ƶ"));
			_bstr_t itemmsg="������Ƶʧ��";
			parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
		}
		break;
	case EVENT_STARTAUDIOOK:
		{//���IPC�Խ��ɹ�
			int error=IP_NET_DVR_AddTalk(lUser);
			if(error)
			{
				_bstr_t itemmsg="��IPC������һ�£����ܼ���㲥";
				parent->SetDlgItemText(IDC_LASTALARMTEXT,itemmsg);
			}
		}
		break;
	default:
		{
			_bstr_t itemmsg="δ�����¼�";
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
 

// CTestNetDllDlg ��Ϣ�������

BOOL CTestNetDllDlg::OnInitDialog()
{
	IP_NET_DVR_Init();
	CDialog::OnInitDialog();
	m_lRealHandle=0;
	m_SwitchFromlRealHandle=0;

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetDlgItemText(IDC_PTZIP,_T("192.168.0.254"));
	SetDlgItemText(IDC_PTZPORT,_T("8091"));


	SetDlgItemText(IDC_PTZUSER,_T("admin"));
	SetDlgItemText(IDC_PTZPASS,_T("123456"));
	SetDlgItemText(IDC_VIDEOPORT,_T("554"));

 
	//IP_CFG_SetReadAndWirteCallBack(OnReadAndSave,this);//���ý���DLL�汾

 
	IP_NET_DVR_SetStatusEventCallBack(OnStateEvent,this);//����״̬�ص�
//	IP_NET_DVR_SetAUXResponseCallBack(OnAUXResponse,this);//����������Ϣ�ص�

	
	IP_NET_DVR_SetReplayDataCallBack(::OnPlayBackDataRecv,this);//���ûط����ݻص�
	IP_NET_DVR_SetPlayActionEventCallBack(::OnPlayActionEvent,this);//���ÿ���״̬�ص�

	//m_logList.InsertColumn(0,_T("��־�ļ�"),0,160);
	//m_logList.InsertColumn(1,_T("��С"),0,60);
	SetDlgItemText(IDC_UPFILENAME,_T("c:\\164�����ļ�.bin"));

	SetTimer(100,100,NULL);


	m_pBGbuffer = NULL;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CTestNetDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CTestNetDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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
	{//û�д�����յ���Ƶ�������Ż����Ϊ�յ����
		parent->playlist.erase(lRealHandle);
		return -1;
	}
	if(parent->m_lRealHandle!=lRealHandle)// && parent->m_SwitchFromlRealHandle!=lRealHandle)
	{//����������Ƶ�ǰ������һ·
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

			pItem->m_nPlayPort=nowPortIndex++;//ָ��Ϊ1�Ų��Ų��Ŵ���Ƶ
			pItem->m_nHaveSetup=0;
			pItem->m_nStreamId=retValue;
			CWnd * pCWnd=GetDlgItem(IDC_VIDEORECT);
			pItem->m_hPlayHwnd=pCWnd->m_hWnd;
		}

		pItem=playlist[m_lRealHandle];
		if(pItem)
		{
			IP_TPS_SetVideoOn(pItem->m_nPlayPort,0);//ֹͣ��ǰ���ڲ��ŵ���Ƶ
		}else
		{
			playlist.erase(m_lRealHandle);
		}
		
		m_lRealHandle=retValue;
	}

}



void CTestNetDllDlg::OnBnClickedAutoview()
{
	
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

	return CDialog::PreTranslateMessage(pMsg);
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
			{//˵����ִ���ϴ�����
				LONG proValue=IP_NET_DVR_GetUpgradeProgress(m_nLastCmdId);
				sprintf(showmsg,"�ϴ�����Ϊ:%d",proValue);				
				SetDlgItemText(IDC_LASTALARMTEXT,_bstr_t(showmsg));
			}
			else if(IP_NET_DVR_GetDownloadState(m_nLastCmdId)==0)
			{//˵������������
				LONG proValue=IP_NET_DVR_GetDownloadPos(m_nLastCmdId);
				sprintf(showmsg,"���ؽ���Ϊ:%d",proValue);				
				SetDlgItemText(IDC_LASTALARMTEXT,_bstr_t(showmsg));
			}
		}
	}

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
			//TiXmlDocument doc;
			//doc.Parse(pData);
			//if(doc.Error()==0)
			//{
			//	TiXmlElement * pRoot=doc.RootElement();
			//	const char * videoparan=pRoot->Attribute("VideoParam");
			//	const char * audioparan=pRoot->Attribute("AudioParam");
			//	const char * videosecs=pRoot->Attribute("VideoSeconds");
			//	char paramdata[5120];				
			//	IP_TPS_OpenStream(lUser,(PBYTE)videoparan,strlen(videoparan),0,40);				
			//	IP_TPS_OpenStream(lUser,(PBYTE)audioparan,strlen(audioparan),1,40);

			//	IP_TPS_Play(lUser,m_VideoRect.m_hWnd);

			//}
		}
	}
	return 0;
}
