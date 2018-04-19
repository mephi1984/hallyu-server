#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "boost/shared_array.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"

#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/signals2.hpp"
#include "boost/variant.hpp"

#include "SignalSender.h"


namespace SE
{

const int CONST_CONNECTION_TIMEOUT_SECONDS = 300;


class TDataReadSignalMap
{
protected:
	std::map<std::string, std::shared_ptr<boost::signals2::signal<void(boost::property_tree::ptree)>>> SignalMap;
public:

	void AddSlot(const std::string& nodeName, std::function<void(boost::property_tree::ptree)> f);

	bool SignalExists(const std::string& signalName);
	
	void EmitSignal(const std::string& signalName, const boost::property_tree::ptree& pt);

	void Clear();

};


} //namespace SE


#endif
