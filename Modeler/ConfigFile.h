#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#include <vector>
#include <string>
#include "Vector.h"

class ConfigFile
{
	private:
		enum DataType
		{
			DT_DOUBLE,
			DT_STRING,
			DT_VECTOR,
			DT_INVALID = -1
		};
		
		struct Entry
		{
			std::string Name;
			DataType Type;
			//union
			//{
				Vector3D<> vVal;
				double dVal;
				std::string sVal;	
			//};
		};
		
		std::vector<Entry> Entries;
		
		Entry GetEntry(std::string name);
		
		void ParseVector(std::string s,Vector3D<> & vec);
		double ParseDouble(std::string s);
	public:
		ConfigFile(const char * file);
		~ConfigFile();
		
		const Vector3D<> & GetVector(std::string name,const Vector3D<> & def);
		const Vector3D<> & GetVector(std::string name){return GetVector(name,Vector3D<>(0,0,0));}
		double GetDouble(std::string name,double def = -1.0);
		const GLubyte * GetColor(std::string name,const GLubyte * def = NULL);
		std::string GetString(std::string name, std::string def = "");
};

#endif
