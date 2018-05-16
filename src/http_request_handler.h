#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <string>
#include <sstream>
#include <fstream>

#include "http_request.h"
#include "http_reply.h"

#include <set>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include "LuaHelper.h"

namespace http {
namespace server {

class request_handler
{
public:
	request_handler(const request_handler&) = delete;
	request_handler& operator=(const request_handler&) = delete;

	explicit request_handler(LH::LuaHelper& iluaHelper);

	void handle_request(const request& req, reply& rep);
	
	// Request/Reply methods
	boost::property_tree::ptree http_receive_RequestWordTranslation(boost::property_tree::ptree propertyTree);
	boost::property_tree::ptree http_send_RequestWordTranslation(std::string wordToTranslate);
	boost::property_tree::ptree http_receive_RequestCard(boost::property_tree::ptree propertyTree);
	boost::property_tree::ptree http_send_OnRequestCard(size_t wordCount);
	boost::property_tree::ptree http_receive_RequestChineseNumberRecognize(boost::property_tree::ptree propertyTree);
	boost::property_tree::ptree http_send_OnRequestChineseNumberRecognize(int maxDigits);

	// ptree send methods
	void reply_store_PropertyTree(reply& rep, boost::property_tree::ptree pTree);
	void reply_store_ErrorPt(reply& rep, std::string err_s);

	//THallyuHttpServer& HallyuHttpServer;
	LH::LuaHelper& luaHelper;

private:
	std::string root_dir;

	static bool url_decode(const std::string& in, std::string& out);
};

}
}

#endif