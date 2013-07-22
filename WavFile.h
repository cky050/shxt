#pragma once


typedef struct __WAVEFORMAT
{
	WORD wFormatTag; 
	WORD nChannels; 
	DWORD nSamplesPerSec; 
	DWORD nAvgBytesPerSec; 
	WORD nBlockAlign; 
}WAVEFORMAT;//wave format

typedef struct __PCMWAVEFORMAT
{
	WAVEFORMAT wf;
	WORD       wBitsPerSample; 
}PCMWAVEFORMAT; // pcm wave format


struct CHUNK_HEADER
{
	char szRiffID[4]; // 'R','I','F','F'
	DWORD dwRiffSize;
};//chunk header

class CWavFile
{
public:
	CWavFile(void);
public:
	~CWavFile(void);
public:
	int Open(WAVEFORMAT * pFormat,char * filename);
	int Close();
private:
	int m_nWaveSize;
	FILE * m_pFile;
	char * m_pBuffer;
};



//
//
//1. RIFF结构
//
//      资源互换文件格式RIFF(Resources Interchange File Format)是一种多媒体文件的存储方式，常见的RIFF文件有WAV文件和AVI文件。RIFF可以看作是一种树状结构，其基本构成单位为块chunk，每个chunk由“辨别码”、“数据大小”及“数据”所组成。一般而言，chunk本身并不允许内部再包含chunk,但有两种例外，即分别以”RIFF”及“LIST”为辨别码的chunk。其中“RIFF”chuuk好比是文件系统的根目录，“LIST”chunk对应子目录，其它的chunk则对应一般的文件。RIFF文件由一些块chunk组成。
//
//读写RIFF文件简便的方法是使用以mmio开头的Win32 API函数，例如mmioOpen( ).
//
//2. Chunk结构组成解析
//
//（1）FOURCC(Four Character Code)表示4个ASCII字符，一般是四个字节，也可以是三个字节加上1个空格，但是中间不可有空格。用来表示ListType，ChunkID,起始标识等信息。
//
//（2）CHUNK 块 数据结构：ChunkID, ChunkSize, ChunkData
//
//ChunkID是1个FOURCC,标识chunk的名称；ChunkSize本身占用4个字节，表示ChunkData部分的数据大小，以字节为单位；ChunkData部分则是chunk中实质性的内容，保存chunk的具体内容。1个chunk保存的数据可以是关于音频文件的编码方式、采样等信息，也可以是音频或视频数据，具体表示哪类数据则通过ChunkID来标识。
//
//      注意 块本身必须占用偶数个Bytes, 必要时可以在结尾补0 .
//
//（3）LIST块 数据结构：“LIST” ListSize ListType ListData
//
//“LIST”也是FOURCC,即块ID，固定的，每个LIST都是以“LIST”开头。ListSize占用4个字节，表示ListType和ListData两部分加在一起的大小。ListType是一个FOURCC，是对LIST具体包含的数据内容的标识。而ListData则是该LIST的数据内容区，有CHUNK和子LIST组成，它们的个数和组成次序可以是不确定的。注意LIST也是chunk.
//
//(4) RIFF块 数据结构：”RIFF” FileSize FileType FileData
//
//“RIFF”也是一个FOURCC，用于标识该文件是一个RIFF格式的文件。FileSize是一个4字节的数据，给出文件的大小，但仅包括FileType和FileData两部分。FileType是一个FOURCC，用来说明文件类型，如”WAV”, “AVI”等。FileData部分表示文件的具体内容，可以是LIST也可以是CHUNK.
//
//3 WAV文件chunk结构
//
//   “WAVE”块包含2个子块：”fmt”块和”data”块
//
//struct RIFF_HEADER
//
//{
//
//char szRiffID[4]; // 'R','I','F','F'
//
//DWORD dwRiffSize;
//
//char szRiffFormat[4]; // 'W','A','V','E'
//
//};
//
//
//typedef struct { 
//
//    WAVEFORMAT wf; 
//
//    WORD       wBitsPerSample; 
//
//} PCMWAVEFORMAT; 
//
//
//typedef struct { 
//
//    WORD wFormatTag; 
//
//    WORD nChannels; 
//
//    DWORD nSamplesPerSec; 
//
//   DWORD nAvgBytesPerSec; 
//
//    WORD nBlockAlign; 
//
//} WAVEFORMAT;
//
//
//Fact Chunk是可选字段，一般当wav文件由某些软件转化而成，则包含该Chunk，结构定义如下：
//
//struct FACT_BLOCK
//
//{
//
//char szFactID[4]; // 'f','a','c','t'
//
//DWORD dwFactSize;
//
//};
//
//
//"data"Chunk包含真正的声音数据， Data Chunk头结构定义如下：
//
//struct DATA_BLOCK
//
//{
//
//char szDataID[4]; // 'd','a','t','a'
//
//DWORD dwDataSize;
//
//};
//
//
//4 WAV规范文件实例解析
//
//     WAVE文件常常就是1个带有“WAVE”chunk的RIFF文件，“WAVE”块由2个子块组成：“fmt”chunk 和“data“chunk
//
//     规范WAVE文件格式说明：
//
//偏移Offset 大小Size 名称Name 说明Desciption
//
//开头是RIFF 头部：
//
//0   4    ChunkID    “RIFF”（0x52494646 大序）
//
//4   4    ChunkSize   36+子块2大小= 4+（8+子块1大小）+（8+子块2大小）
//
//8   4    Format       “WAVE”（0x57415645 大序）
//
//“WAVE”格式由2个子块组成：”fmt”和”data”：
//
//子块“fmt”描述音频数据的格式：
//
//12   4    Subchunk1ID   “fmt”（0x666d7420 大序）
//
//16   4    Subchunk1Size   16 for PCM
//
//20   2    AudioFormat     PCM = 1 （大于1表示一些压缩格式）
//
//22   2    NumChannels    通道数Mono = 1, Stereo =2 等
//
//24   4    SampleRate      采样率80000，44100等
//
//28   4    ByteRate        = =SampleRate * NumChannels * BitsPerSample / 8
//
//32   2    BlockAlign      = = NumChnnels * BitsPerSample / 8 （包括所有通道）
//
//34    2    BitsPerSample     8 bits = 8, 16 bits =16 等
//
//      2    ExtraParamSize    如果是PCM，那么不存在
//
//      x    ExtraParams       额外参数的空间
//
//子块”data”包括数据大小和实际的声音：
//
//36    4    Subchunk2ID      “data” （0x64617461 大序）
//
//40    4    Subchunk2Size     = = NumSamples * NumChannels * BitsPerSample / 8
//
//44    *    Data              实际的音频数据
//
//
//
//     注意：WAVE文件缺省的字节序为小序，如果使用RIFX替换RIFF，那么字节序为大序。
//
//采样数据结束于偶数字节，如果长度为奇数，补0，长度不变，即不包括0。
//
//8位采样时，范围0~255，而16位采样时，-32768~32768 .
//
//WAVE文件由若干块组成：RIFF WAVE Chunk, Format Chunk, Fact Chunk（可选）, Data Chunk. 
//
