#ifndef IMAGE_DOWNLOADER_H_INCLUDED
#define IMAGE_DOWNLOADER_H_INCLUDED

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <fstream>

#include "boost/algorithm/string.hpp"
#include "boost/asio.hpp"

#include "SE/misc.h"

class TImageDownloader
{
public:

	TImageDownloader(boost::asio::io_service& ioService);

	void DownloadImageToFile(std::string url, const std::string& fileName);

	static std::string getFilePath();

protected:
	boost::asio::io_service& IoService;
};



#endif
