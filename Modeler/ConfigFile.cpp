#include "ConfigFile.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include "App_Log.h"

extern CApp_Log Log;

void ConfigFile::ParseVector(std::string s, Vector3D<> & vec)
{
	s = s.substr(1);
	vec.V[0] = ParseDouble(s);
	s = s.substr(s.find_first_of(",")+1);
	vec.V[1] = ParseDouble(s);
	s = s.substr(s.find_first_of(",")+1);
	vec.V[2] = ParseDouble(s);
}

double ConfigFile::ParseDouble(std::string s)
{
	int i =0;
	bool Frac = false;
	double tot = 0;
	int places = 1;
	double tmp;
	int Sign = 1;
	if(s[0] == '-')
	{
		Sign = -1;
		i = 1;
	}
	for(;isdigit(s[i]) || s[i] == '.';i++)
	{
		if(!Frac && s[i] != '.')
		{
			tot *= 10;
			tot += s[i] - '0';
		}
		else if(s[i] == '.')
			Frac = true;
		else
		{
			if(s[i] == '.') return tot;
			tmp = s[i] - '0';
			for(int j = 0;j<places;j++)
				tmp /= 10;
			tot += tmp;
			places++;
		}
	}
	return Sign * tot;
}

ConfigFile::Entry ConfigFile::GetEntry(std::string name)
{
	unsigned int i;
	for(i=0;i<Entries.size();i++)
	{
		//Log.Write("checking %s",Entries[i].Name.c_str());
		if(Entries[i].Name == name)
			return Entries[i];
	}
	
	Entry e;
	e.Type = DT_INVALID;
	return e;
}

ConfigFile::ConfigFile(const char * file)
{
	std::ifstream in(file);
	if(!in) return;
	
	enum Mode
	{
		ReadingName,
		ReadingValue
	};
	
	Entry e;
	//e.vVal[0] = 1;
	e.Name = "";
	std::string val = "";
	char ch;	
	Mode mode = ReadingName;
	
	while(!in.eof())
	{
		ch = in.get();
		if(ch == ' ') continue;
		if(mode == ReadingName)
		{
			if(ch == '=')
			{
				mode = ReadingValue;
				continue;
			}
			if(ch == '\n' || ch == '#')
			{
				e.Name = "";
				continue;
			}
			e.Name += ch;
		}
		else
		{
			if(ch == '\n' || in.eof())
			{
				if(val[0] == '<')
				{
					ParseVector(val,e.vVal);
					e.Type = DT_VECTOR;
				}
				else if(isdigit(val[0]))
				{
					e.dVal = ParseDouble(val);
					e.Type = DT_DOUBLE;
				}
				else
				{
					e.sVal = val;
					e.Type = DT_STRING;
				}
				Entries.push_back(e);
/*
				Log.Indent();
				Log.Write("Added %s:",e.Name.c_str());
				switch(e.Type)
				{
					case DT_DOUBLE:
						Log.Write("\t%f",e.dVal);
						break;
					case DT_STRING:
						Log.Write("\t%f",e.sVal.c_str());
						break;
					case DT_VECTOR:
						Log.Write("\t<%f,%f,%f>",e.vVal.V[0],e.vVal.V[1],e.vVal.V[2]);
						break;
					default:
						Log.Write("\tInvalid!");
						break;
				}
				Log.Unindent();
*/
				e.Name = "";
				val = "";
				mode = ReadingName;
			}
			else
				val += ch;
		}
	}
}

ConfigFile::~ConfigFile(){}

std::string ConfigFile::GetString(std::string name, std::string def)
{
	Entry e = GetEntry(name);
	if(e.Type != DT_STRING)
		return def;
	return e.sVal;
}

double ConfigFile::GetDouble(std::string name, double def)
{
	Entry e = GetEntry(name);
	if(e.Type != DT_DOUBLE)
		return def;
	return e.dVal;
}

const Vector3D<> & ConfigFile::GetVector(std::string name, const Vector3D<> & def)
{
	Entry e = GetEntry(name);
	if(e.Type != DT_VECTOR)
		return def;
	return e.vVal;
}

const GLubyte * ConfigFile::GetColor(std::string name,const GLubyte * def)
{
	static GLubyte ret[3];
	Entry e = GetEntry(name);
	if(e.Type != DT_VECTOR)
		return def;
	for(int i=0;i<3;i++)		//for each element
	{
		//if the vVal is <= 1, scale to range 0-255, else keep as is
		ret[i] = (e.vVal.V[i] <= 1)?((GLubyte)(e.vVal.V[i] * 255)):(GLubyte)e.vVal.V[i];
	}
	return ret;
}


