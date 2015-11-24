#include "OpenVEIL.h"
#include <boost/python.hpp>

#pragma once
class OpenVEILWrapper
{
public:
	OpenVEILWrapper();
	~OpenVEILWrapper();
	int genericConnectToServer(const std::string& url, const std::string& username, const std::string& password);
	int connectToServer(const std::string& url, const std::string& username, const std::string& password);
	void disconnect();
	bool isConnected();
	boost::python::tuple sendJsonRequest(const std::string& verb, const std::string& cmd, const std::string& inData);
	boost::python::tuple sendBase64Request(const std::string& verb, const std::string& cmd, const std::string& inData);
protected:
	std::shared_ptr<IKeyVEILConnector> conn;
private:
	bool isReady();
};

