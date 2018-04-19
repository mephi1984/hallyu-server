//#include "include/Utils/Utils.h"
#include "Network.h"
#include "SignalSender.h"
#include "misc.h"

namespace SE
{

void TDataReadSignalMap::AddSlot(const std::string& nodeName, std::function<void(boost::property_tree::ptree)> f)
{
	if (SignalMap.count(nodeName) == 0)
	{
		SignalMap[nodeName] = std::shared_ptr<boost::signals2::signal<void(boost::property_tree::ptree)>>(new boost::signals2::signal<void(boost::property_tree::ptree)>);
		SignalMap[nodeName]->connect(f);
	}
	else
	{
		SignalMap[nodeName]->connect(f);
	}
}

bool TDataReadSignalMap::SignalExists(const std::string& signalName)
{
	return SignalMap.count(signalName) != 0;
}

void TDataReadSignalMap::EmitSignal(const std::string& signalName, const boost::property_tree::ptree& pt)
{
	if (SignalMap.count(signalName) == 0)
	{
		SE::WriteToLog("Error in EmitSignal - signal not exists");
	}
	else
	{
		(*SignalMap[signalName])(pt);
	}
}



void TDataReadSignalMap::Clear()
{
	SignalMap.clear();
}



} //namspace SE