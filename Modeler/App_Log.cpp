#include "App_Log.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void CApp_Log::Open()
{
	fout.open(Filename,std::ios::app);
}

void CApp_Log::Close()
{
	fout.close();
}

CApp_Log::CApp_Log(const char *file)
{
	Filename = new char[strlen(file)];
	strcpy(Filename,file);
	fout.open(file);	//empty the log file
	fout.close();
}

CApp_Log::~CApp_Log()
{
	Close();		//paranoia
	delete[] Filename;
}

void CApp_Log::Write(const char *text,...)
{
	if (text == NULL)
	return;

	Open();

	char buffer[256];				//buffer overflow!!
	va_list va;

	va_start(va, text);
	vsprintf(buffer,text,va);
	va_end(va);
	for(int i=0;i<NumIndents;i++) fout.put('\t');
	fout << buffer << std::endl;

	Close();
}
