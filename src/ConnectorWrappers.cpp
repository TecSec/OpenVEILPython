//	Copyright (c) 2015, TecSec, Inc.
//
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//	
//		* Redistributions of source code must retain the above copyright
//		  notice, this list of conditions and the following disclaimer.
//		* Redistributions in binary form must reproduce the above copyright
//		  notice, this list of conditions and the following disclaimer in the
//		  documentation and/or other materials provided with the distribution.
//		* Neither the name of TecSec nor the names of the contributors may be
//		  used to endorse or promote products derived from this software 
//		  without specific prior written permission.
//		 
//	ALTERNATIVELY, provided that this notice is retained in full, this product
//	may be distributed under the terms of the GNU General Public License (GPL),
//	in which case the provisions of the GPL apply INSTEAD OF those given above.
//		 
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED.  IN NO EVENT SHALL TECSEC BE LIABLE FOR ANY 
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "OpenVEILWrapper.h"

#pragma region Connector base class
Connector::Connector()
{
	// try_get_instance can return nullptr
	conn = ::ServiceLocator()->try_get_instance<IKeyVEILConnector>("/KeyVEILConnector");

	// get_instance throws exception
	//conn = ::ServiceLocator()->get_instance<IKeyVEILConnector>("/KeyVEILConnector");	
}

Connector::~Connector() {}

void Connector::release()
{
	conn.reset();
}
bool Connector::isReady()
{
	return !!conn;
}

ConnectionStatus Connector::__genericConnectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	if (!isReady())
	{
		return connStatus_NoServer;
	}
	return conn->genericConnectToServer(url.c_str(), username.c_str(), password.c_str());
}

ConnectionStatus Connector::__connectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	if (!isReady())
	{
		return connStatus_NoServer;
	}
	return conn->connect(url.c_str(), username.c_str(), password.c_str());
}

void Connector::disconnect()
{
	if (isConnected())
	{
		conn->disconnect();
	}
}

bool Connector::isConnected()
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

py::tuple Connector::sendJsonRequest(const std::string& verb, const std::string& cmd, const std::string& inData)
{
	if (!isReady())
	{
		return py::make_tuple(false, "Connection object is not ready.", 1000);
	}

	JSONObject inDataTmp;
	JSONObject outDataTmp;
	int status = 0;

	if (inDataTmp.FromJSON(inData.c_str()) <= 0)
	{
		return py::make_tuple(false, "", 0);
	}

	if (!conn->sendJsonRequest(verb.c_str(), cmd.c_str(), inDataTmp, outDataTmp, status))
	{
		return py::make_tuple(false, outDataTmp.ToJSON().c_str(), status);
	}

	return py::make_tuple(true, outDataTmp.ToJSON().c_str(), status);
}

py::tuple Connector::sendBase64Request(const std::string& verb, const std::string& cmd, const byte_array& inData)
{
	if (!isReady())
	{
		return py::make_tuple(false, "Connection object is not ready.", 1000);
	}

	tsData inDataTmp(tsDataFromPyObject(inData));
	tsData outDataTmp;
	int status = 0;

	if (!conn->sendRequest(verb.c_str(), cmd.c_str(), inDataTmp, outDataTmp, status))
	{
		return py::make_tuple(false, outDataTmp.ToBase64().c_str(), status);
	}

	return py::make_tuple(true, outDataTmp.ToBase64().c_str(), status);
}
#pragma endregion

#pragma region KeyVEILConnector Wrapper
KeyVEILConnectorWrapper::KeyVEILConnectorWrapper()
{}
KeyVEILConnectorWrapper::~KeyVEILConnectorWrapper()
{}
ConnectionStatus KeyVEILConnectorWrapper::connectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	return __connectToServer(url, username, password);
}
bool KeyVEILConnectorWrapper::refresh()
{
	if (!isReady())
		return false;
	return conn->refresh();
}
size_t KeyVEILConnectorWrapper::tokenCount()
{
	if (!isReady())
		return 0;
	return conn->tokenCount();
}
TokenWrapper KeyVEILConnectorWrapper::tokenByIndex(size_t index)
{
	if (!isReady())
		return TokenWrapper();
	return TokenWrapper(conn->token(index));
}
TokenWrapper KeyVEILConnectorWrapper::tokenByName(const std::string& tokenName)
{
	if (!isReady())
		return TokenWrapper();
	return TokenWrapper(conn->token(tsAscii(tokenName.c_str())));
}
TokenWrapper KeyVEILConnectorWrapper::tokenBySerialNumber(const byte_array& serialNumber)
{
	if (!isReady())
		return TokenWrapper();
	tsData serial(tsDataFromPyObject(serialNumber));

	return TokenWrapper(conn->token(serial));
}
TokenWrapper KeyVEILConnectorWrapper::tokenById(const std::string& id)
{
	if (!isReady())
		return TokenWrapper();
	return TokenWrapper(conn->token(ToGuid()(tsAscii(id.c_str()))));
}
size_t KeyVEILConnectorWrapper::favoriteCount()
{
	if (!isReady())
		return 0;
	return conn->favoriteCount();
}
FavoriteWrapper KeyVEILConnectorWrapper::favoriteByIndex(size_t index)
{
	if (!isReady())
		return FavoriteWrapper();
	return FavoriteWrapper(conn->favorite(index));
}
FavoriteWrapper KeyVEILConnectorWrapper::favoriteByName(const std::string& name)
{
	if (!isReady())
		return FavoriteWrapper();
	return FavoriteWrapper(conn->favorite(tsAscii(name.c_str())));
}
FavoriteWrapper KeyVEILConnectorWrapper::favoriteById(const std::string& id)
{
	if (!isReady())
		return FavoriteWrapper();
	return FavoriteWrapper(conn->favorite(ToGuid()(tsAscii(id.c_str()))));
}
//GUID CreateFavorite(TokenWrapper token, const tsData& headerData, const tsAscii& name);
//GUID CreateFavorite(const std::string& tokenId, const tsData& headerData, const tsAscii& name);
//GUID CreateFavorite(const tsData& tokenSerial, const tsData& headerData, const tsAscii& name);
bool KeyVEILConnectorWrapper::DeleteFavorite(const std::string& id)
{
	if (!isReady())
		return false;
	return conn->DeleteFavorite(ToGuid()(tsAscii(id.c_str())));
}
bool KeyVEILConnectorWrapper::UpdateFavoriteName(const std::string& id, const std::string& name)
{
	if (!isReady())
		return false;
	return conn->UpdateFavoriteName(ToGuid()(tsAscii(id.c_str())), name.c_str());
}
//bool UpdateFavorite(const std::string& id, const tsData& setTo);
size_t KeyVEILConnectorWrapper::tokenCountForEnterpriseId(const std::string& enterpriseId)
{
	if (!isReady())
		return 0;
	return conn->tokenCountForEnterprise(ToGuid()(tsAscii(enterpriseId.c_str())));
}
TokenWrapper KeyVEILConnectorWrapper::tokenForEnterprise(const std::string& enterpriseId, size_t index)
{
	if (!isReady())
		return TokenWrapper();
	return TokenWrapper(conn->tokenForEnterprise(ToGuid()(tsAscii(enterpriseId.c_str())), index));
}
size_t KeyVEILConnectorWrapper::favoriteCountForEnterprise(const std::string& enterpriseId)
{
	if (!isReady())
		return 0;
	return conn->favoriteCountForEnterprise(ToGuid()(tsAscii(enterpriseId.c_str())));
}
FavoriteWrapper KeyVEILConnectorWrapper::favoriteForEnterprise(const std::string& enterpriseId, size_t index)
{
	if (!isReady())
		return FavoriteWrapper();
	return FavoriteWrapper(conn->favoriteForEnterprise(ToGuid()(tsAscii(enterpriseId.c_str())), index));
}
#pragma endregion

#pragma region GenericConnector Wrapper
GenericConnectorWrapper::GenericConnectorWrapper()
{}
GenericConnectorWrapper::~GenericConnectorWrapper()
{}
ConnectionStatus GenericConnectorWrapper::connectToServer(const std::string& url, const std::string& username, const std::string& password)
{
	return __genericConnectToServer(url, username, password);
}
#pragma endregion
