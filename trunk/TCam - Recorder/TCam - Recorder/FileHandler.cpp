#include "FileHandler.h"
#include "Tibia.h"

CTibia FTibia;

CFileHandler::CFileHandler()
{
}

CFileHandler::~CFileHandler()
{
}

void CFileHandler::Open(char *TfileName)
{
	ZeroMemory(fileName,sizeof(fileName));
	memcpy(fileName,TfileName,MAX_PATH);
	
	offset = 0;
	data = (char *)malloc(300000);
}

bool CFileHandler::Write(char *buf, int nSize)
{
	if(offset > 204800)
	{
		Export();
	}
	memcpy(&data[offset],buf,nSize);
	offset += nSize;
	return true;
}

bool CFileHandler::Write(int buf, int nSize)
{
	if(offset > 204800)
	{
		Export();
	}
	memcpy(&data[offset],&buf,nSize);
	offset += nSize;
	return true;
}

void CFileHandler::Export()
{
	myRecording.open(fileName, ios::out | ios::binary | ios::app);
	myRecording.write(data,offset);
	myRecording.close();
	offset = 0;
}

void CFileHandler::GenerateHeader(int mode)
{
	
}