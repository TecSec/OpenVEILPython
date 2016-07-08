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

#pragma once
#include "VEIL.h"
using namespace tscrypto;

#include "VEILCmsHeader.h"
#include "VEILFileSupport.h"
#undef min
#undef max
#include <pybind11/pybind11.h>
#include <deque>
#include <vector>

typedef uint8_t byte;
namespace py = pybind11;
typedef py::object byte_array;

class Environment
{
public:
	Environment();
	~Environment();

	void DispatchEvents(); // Call this in the main thread to receive queued up events
	bool InitializeVEIL(bool initiateChangeMonitoring);
	bool TerminateVEIL();
private:
	//std::deque<QueuedVEILEvent> _events;
};

class Connector
{
public:
	Connector();
	Connector(Connector&& obj) : conn(std::move(obj.conn)) {}
	Connector(const Connector& obj) : conn(obj.conn) {}
	virtual ~Connector();
	Connector& operator=(Connector&& obj) {
		if (this != &obj) { conn = std::move(obj.conn); }
	}
	virtual void release();
	virtual void disconnect();
	virtual bool isConnected();
	virtual py::tuple sendJsonRequest(const std::string& verb, const std::string& cmd, const std::string& inData);
	virtual py::tuple sendBase64Request(const std::string& verb, const std::string& cmd, const py::object& inData);
protected:
	std::shared_ptr<IKeyVEILConnector> conn;
	bool isReady();
	ConnectionStatus __genericConnectToServer(const std::string& url, const std::string& username, const std::string& password);
	ConnectionStatus __connectToServer(const std::string& url, const std::string& username, const std::string& password);
};

class FavoriteWrapper;

class SessionWrapper
{
public:
	SessionWrapper();
	SessionWrapper(std::shared_ptr<IKeyVEILSession> _sess);
	SessionWrapper(SessionWrapper&& obj) : _session(std::move(obj._session)) {}
	SessionWrapper(const SessionWrapper& obj) : _session(obj._session) {}
	~SessionWrapper();
	SessionWrapper& operator=(SessionWrapper&& obj) {
		if (this != &obj) { _session = std::move(obj._session); }
	}

	void release();
	void close();
	LoginStatus Login(const std::string& pin);
	bool IsLoggedIn();
	bool Logout();
	//bool GenerateWorkingKey(Asn1::CTS::CkmCombineParameters& params, std::function<bool(Asn1::CTS::CkmCombineParameters&, tsCryptoData&)> headerCallback, tsCryptoData &WorkingKey);
	//bool RegenerateWorkingKey(Asn1::CTS::CkmCombineParameters& params, tsCryptoData &WorkingKey);
	py::dict getProfile();
	bool IsLocked();
	size_t retriesLeft();
	bool IsValid();
	SessionWrapper Duplicate();
	bool encryptFileUsingFavorite(FavoriteWrapper fav, const std::string& sourceFile, bool compress, const std::string& encryptedFile);
	bool decryptFile(const std::string& encryptedFile, const std::string& decryptedFile);
	byte_array encryptDataUsingFavorite(FavoriteWrapper fav, const py::object& sourceData, bool compress);
	byte_array decryptData(const py::object& encryptedData);

	std::shared_ptr<IKeyVEILSession> handle() { return _session; }

protected:
	std::shared_ptr<IKeyVEILSession> _session;

	bool isReady();
};

class FavoriteWrapper
{
public:
	FavoriteWrapper();
	FavoriteWrapper(std::shared_ptr<IFavorite> _fav);
	FavoriteWrapper(FavoriteWrapper&& obj) : _favorite(std::move(obj._favorite)) {}
	FavoriteWrapper(const FavoriteWrapper& obj) : _favorite(obj._favorite) {}
	~FavoriteWrapper();
	FavoriteWrapper& operator=(FavoriteWrapper&& obj) {
		if (this != &obj) { _favorite = std::move(obj._favorite); }
	}

	void release();

	std::string getFavoriteId();
	void setFavoriteId(const std::string& setTo);

	std::string getEnterpriseId();
	void setEnterpriseId(const std::string& setTo);

	std::string getFavoriteName();
	void setFavoriteName(const std::string& setTo);

	std::string getTokenSerialNumber();
	void setTokenSerialNumber(const std::string& setTo);

	byte_array getHeaderData();
	void setHeaderData(const py::object& setTo);

	bool encryptFile(SessionWrapper session, const std::string& sourceFile, bool compress, const std::string& encryptedFile);
	byte_array encryptData(SessionWrapper session, const py::object& sourceData, bool compress);

protected:
	std::shared_ptr<IFavorite> _favorite;

	bool isReady();
};

class TokenWrapper
{
public:
	TokenWrapper();
	TokenWrapper(std::shared_ptr<IToken> _tok);
	TokenWrapper(TokenWrapper&& obj) : _token(std::move(obj._token)) {}
	TokenWrapper(const TokenWrapper& obj) : _token(obj._token) {}
	~TokenWrapper();
	TokenWrapper& operator=(TokenWrapper&& obj) {
		if (this != &obj) { _token = std::move(obj._token); }
	}

	void release();

	std::string getTokenName();
	bool setTokenName(const std::string& setTo);
	byte_array serialNumber();
	std::string id();
	std::string enterpriseName();
	std::string memberName();
	std::string tokenType();
	std::string enterpriseId();
	std::string memberId();

	SessionWrapper openSession();

protected:
	std::shared_ptr<IToken> _token;

	bool isReady();
};

class KeyVEILConnectorWrapper : public Connector
{
public:
	KeyVEILConnectorWrapper();
	virtual ~KeyVEILConnectorWrapper();
	ConnectionStatus connectToServer(const std::string& url, const std::string& username, const std::string& password);
	bool refresh();
	size_t tokenCount();
	TokenWrapper tokenByIndex(size_t index);
	TokenWrapper tokenByName(const std::string& tokenName);
	TokenWrapper tokenBySerialNumber(const py::object& serialNumber);
	TokenWrapper tokenById(const std::string& id);

	size_t favoriteCount();
	FavoriteWrapper favoriteByIndex(size_t index);
	FavoriteWrapper favoriteByName(const std::string& name);
	FavoriteWrapper favoriteById(const std::string& id);
	//std::string CreateFavorite(TokenWrapper token, const std::string& headerData, const std::string& name);
	//std::string CreateFavorite(const std::string& tokenId, const std::string& headerData, const std::string& name);
	//std::string CreateFavorite(const tsCryptoData& tokenSerial, const std::string& headerData, const std::string& name);
	bool DeleteFavorite(const std::string& id);
	bool UpdateFavoriteName(const std::string& id, const std::string& name);
	//bool UpdateFavorite(const std::string& id, const tsCryptoData& setTo);
	size_t tokenCountForEnterpriseId(const std::string& enterpriseId);
	TokenWrapper tokenForEnterprise(const std::string& enterpriseId, size_t index);
	size_t favoriteCountForEnterprise(const std::string& enterpriseId);
	FavoriteWrapper favoriteForEnterprise(const std::string& enterpriseId, size_t index);
};

class GenericConnectorWrapper : public Connector
{
public:
	GenericConnectorWrapper();
	virtual ~GenericConnectorWrapper();
	ConnectionStatus connectToServer(const std::string& url, const std::string& username, const std::string& password);
};



// Used in the file encrypt and decrypt routines
class StatusClass : public IFileVEILOperationStatus, public tsmod::IObject
{
public:
	StatusClass() {}
	virtual bool Status(const tsCryptoString& taskName, int taskNumber, int ofTaskCount, int taskPercentageDone)
	{
		//if (g_doStatus)
		//{
		//	ts_out << "Task " << taskNumber << " of " << ofTaskCount << " " << taskName << " " << taskPercentageDone << "%" << endl;
		//}
		return true;
	}
	virtual void    FailureReason(const tsCryptoString&failureText)
	{
		//ERROR(failureText);
	}

private:
	virtual ~StatusClass() {}
};

extern tsCryptoData tsDataFromPyObject(const py::object& obj);
extern py::object tsDataToPyObject(const tsCryptoData& data);
