#ifndef __APP_LOG_H__
#define __APP_LOG_H__

#include <fstream>

class CApp_Log
{
	private:
		std::ofstream fout;
		char * Filename;
		int NumIndents;		//every new line is indented this many tabs

		void Open();
		void Close();
	public:
		CApp_Log(const char *file);
		~CApp_Log();
		void Write(const char *string,...);	//log a string (use like printf())
		void Indent() {NumIndents++;}
		void Unindent(){if(NumIndents > 0)NumIndents--;}
};

#endif



