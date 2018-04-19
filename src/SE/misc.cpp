#include "misc.h"



#include "boost/locale.hpp"
#include <locale>
#include <fstream>
#include "boost/thread.hpp"
#include "boost/date_time.hpp"


namespace SE
{

	boost::mutex logMutex;

	void WriteToLog(const std::string& msg)
	{
		boost::mutex::scoped_lock l(logMutex);

		std::ofstream ofs;
#ifdef _WIN32
		ofs.open("C:/Workplace/Projects/hallyu/hallyu_server/hallyu_log2.txt", std::ofstream::out | std::ofstream::app);
#else
		ofs.open("/home/devuser/hallyu_log2.txt", std::ofstream::out | std::ofstream::app);
#endif

		boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();

		ofs << to_simple_string(now) << " " << msg << std::endl;

		ofs.close();

	}

	ErrorToLog::ErrorToLog()
	{
	}
	
	ErrorToLog::ErrorToLog(const std::string& msg)
	{
		WriteToLog(msg);
		//std::cout << msg << std::endl;
	}


	std::string wstring_to_string(std::wstring in)
	{
		
		return boost::locale::conv::utf_to_utf<char>(in);

	

	}

	std::wstring string_to_wstring(std::string in)
	{
		return boost::locale::conv::utf_to_utf<wchar_t>(in);

		
	}
}