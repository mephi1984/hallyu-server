#include "ImageDownloader.h"


TImageDownloader::TImageDownloader(boost::asio::io_service& ioService)
	: IoService(ioService)
{
	SE::WriteToLog("TImageDownloader::TImageDownloader");
}

std::string TImageDownloader::getFilePath()
{
#ifdef _WIN32
	std::string filePath = "./";
#else
	std::string filePath = "/home/hallyu-http/hallyu.ru/http/";
#endif

	return filePath;
}


void TImageDownloader::DownloadImageToFile(std::string url, const std::string& fileName)
{
	try
	{

		using boost::asio::ip::tcp;

		/*
		if (argc != 3)
		{
		std::cout << "Usage: sync_client <server> <path>\n";
		std::cout << "Example:\n";
		std::cout << "  sync_client www.boost.org /LICENSE_1_0.txt\n";
		return 1;
		}
		*/

		if (boost::starts_with(url, "https://"))
		{
			url.erase(0, 8);
		}
		if (boost::starts_with(url, "http://"))
		{
			url.erase(0, 7);
		}



		size_t p = url.find_first_of('/');

		std::string server(url.begin(), url.begin() + p);

		url.erase(0, p);

		std::string path = url;

		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(server, "http");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << server << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			//std::cout << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			//std::cout << "Response returned with status code " << status_code << "\n";
			return;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		{
			//std::cout << header << "\n";
		}

		//std::cout << "\n";

		std::ofstream f;

		std::string filePath = getFilePath();

		f.open(filePath + fileName, std::ofstream::binary);

		// Write whatever content we already have to output.
		if (response.size() > 0)
		{
			//std::cout << &response;
			f << &response;
		}



		// Read until EOF, writing data to output as we go.
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
		{
			//std::cout << &response;
			f << &response;
		}

		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
	}
	catch (std::exception&)
	{
		SE::WriteToLog("Exception in TImageDownloader::DownloadImageToFile");
		//std::cout << "Exception: " << e.what() << "\n";
	}

}