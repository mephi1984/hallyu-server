#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <string>
#include "boost/lexical_cast.hpp"





namespace SE
{


	void WriteToLog(const std::string& msg);

	class ErrorToLog : public std::exception
	{
	public:
		ErrorToLog();
		ErrorToLog(const std::string& msg);
	};

	std::string wstring_to_string(std::wstring in);

	std::wstring string_to_wstring(std::string in);

}


#endif //MISC_H_INCLUDED