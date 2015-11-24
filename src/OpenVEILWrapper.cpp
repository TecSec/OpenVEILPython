#include "OpenVEILWrapper.h"

OpenVEILWrapper::OpenVEILWrapper()
{
	// try_get_instance can return nullptr
	conn = ::ServiceLocator()->try_get_instance<IKeyVEILConnector>("/KeyVEILConnector");
	
	// get_instance throws exception
	//conn = ::ServiceLocator()->get_instance<IKeyVEILConnector>("/KeyVEILConnector");	
}

OpenVEILWrapper::~OpenVEILWrapper() {}

bool OpenVEILWrapper::isReady()
{
	return !!conn;
}

int OpenVEILWrapper::genericConnectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	if (!isReady())
	{
		return connStatus_NoServer;
	}
	return conn->genericConnectToServer(url.c_str(), username.c_str(), password.c_str());
}

int OpenVEILWrapper::connectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	if (!isReady())
	{
		return connStatus_NoServer;
	}
	return conn->connect(url.c_str(), username.c_str(), password.c_str());
}

void OpenVEILWrapper::disconnect()
{
	if (isConnected())
	{
		conn->disconnect();
	}
}

bool OpenVEILWrapper::isConnected()
{
	if (isReady())
	{
		return conn->isConnected();
	}
	else
	{
		return false;
	}
}

boost::python::tuple OpenVEILWrapper::sendJsonRequest(const std::string& verb, const std::string& cmd, const std::string& inData)
{
	if (!isReady())
	{
		return boost::python::make_tuple(false, "Connection object is not ready.", 1000);
	}

	JSONObject inDataTmp;
	JSONObject outDataTmp;
	int status = 0;

	if (inDataTmp.FromJSON(inData.c_str()) <= 0)
	{
		return boost::python::make_tuple(false, "", 0);
	}

	if (!conn->sendJsonRequest(verb.c_str(), cmd.c_str(), inDataTmp, outDataTmp, status))
	{
		return boost::python::make_tuple(false, outDataTmp.ToJSON().c_str(), status);
	}

	return boost::python::make_tuple(true, outDataTmp.ToJSON().c_str(), status);
}

boost::python::tuple OpenVEILWrapper::sendBase64Request(const std::string& verb, const std::string& cmd, const std::string& inData)
{
	if (!isReady())
	{
		return boost::python::make_tuple(false, "Connection object is not ready.", 1000);
	}

	tsData inDataTmp(inData.c_str(), tsData::BASE64);
	tsData outDataTmp;
	int status = 0;

	if (!conn->sendRequest(verb.c_str(), cmd.c_str(), inDataTmp, outDataTmp, status))
	{
		return boost::python::make_tuple(false, outDataTmp.ToBase64().c_str(), status);
	}

	return boost::python::make_tuple(true, outDataTmp.ToBase64().c_str(), status);
}

// Expose methods to BOOST Python
using namespace boost::python;

ConnectionStatus identity_(ConnectionStatus x) { return x; }

BOOST_PYTHON_MODULE(OpenVEILWrapper)
{
	class_<OpenVEILWrapper>("OpenVEILWrapper")
		.def("genericConnectToServer", &OpenVEILWrapper::genericConnectToServer)
		.def("connectToServer", &OpenVEILWrapper::connectToServer)
		.def("disconnect", &OpenVEILWrapper::disconnect)
		.def("isConnected", &OpenVEILWrapper::isConnected)
		.def("sendJsonRequest", &OpenVEILWrapper::sendJsonRequest)
		.def("sendBase64Request", &OpenVEILWrapper::sendBase64Request)
		;
}
