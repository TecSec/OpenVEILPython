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

Environment::Environment()
{

}
Environment::~Environment()
{

}
void Environment::DispatchEvents()
{

}
bool Environment::InitializeVEIL(bool initiateChangeMonitoring)
{
	// Forces the core system to initialize
	if (!::ServiceLocator())
		return false;

	if (initiateChangeMonitoring)
	{

	}
	return true;
}
bool Environment::TerminateVEIL()
{
	TerminateVEILSystem();
	return true;
}


// Expose methods to BOOST Python
using namespace boost::python;

ConnectionStatus identity_(ConnectionStatus x) { return x; }

//PyObject* boost::python::type_into_python<byte_array>::convert(byte_array const&ba)
//{
//	const char* src = ba.empty() ? "" : reinterpret_cast<const char*>(&ba.front;());
//	return PyBytes_FromStringAndSize(src, ba.size());
//}
//
//void* boost::python::type_from_python<byte_array>::convertible(PyObject* obj)
//{
//	return PyBytes_Check(obj) ? obj : nullptr;
//}
//
//void boost::python::type_from_python<byte_array>::construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data)
//{
//	auto storage = reinterpret_cast< converter::rvalue_from_python_storage<byte_array>* >(data)->storage.bytes;
//	byte* dest; Py_ssize_t len;
//	PyBytes_AsStringAndSize(obj, reinterpret_cast<char**>(&dest; ), &len; );
//	new(storage) byte_array(dest, dest + len);
//	data->convertible = storage;
//}

BOOST_PYTHON_MODULE(OpenVEIL)
{
	object package = scope();
	package.attr("__path__") = "OpenVEIL";

	//to_python_converter< byte_array, boost::python::type_into_python<byte_array> >();
	//type_from_python< byte_array >();

	object class_Env = class_<Environment>("Environment")
		.def("DispatchEvents", &Environment::DispatchEvents)
		.def("InitializeVEIL", &Environment::InitializeVEIL)
		.def("TerminateVEIL", &Environment::TerminateVEIL)
	;
	object Env = class_Env();

	scope().attr("Environment") = Env;

	enum_<ConnectionStatus>("ConnectionStatus")
		.value("Connected", connStatus_Connected)
		.value("NoServer", connStatus_NoServer)
		.value("BadAuth", connStatus_BadAuth)
		.value("WrongProtocol", connStatus_WrongProtocol)
		.value("UrlBad", connStatus_UrlBad)
		;

	enum_<LoginStatus>("LoginStatus")
		.value("Connected", loginStatus_Connected)
		.value("NoServer", loginStatus_NoServer)
		.value("BadAuth", loginStatus_BadAuth)
		;

	{
		// map the Util namespace to a sub-module
		// make "from mypackage.Util import <whatever>" work
		object KeyVEILConnectorModule(handle<>(borrowed(PyImport_AddModule("OpenVEIL.KeyVEILConnector"))));
		// make "from mypackage import Util" work
		scope().attr("KeyVEILConnector") = KeyVEILConnectorModule;
		// set the current scope to the new sub-module
		scope util_scope = KeyVEILConnectorModule;

		class_<KeyVEILConnectorWrapper>("KeyVEILConnector")
			.def("release", &KeyVEILConnectorWrapper::release)
			.def("connectToServer", &KeyVEILConnectorWrapper::connectToServer)
			.def("disconnect", &KeyVEILConnectorWrapper::disconnect)
			.def_readonly("isConnected", &KeyVEILConnectorWrapper::isConnected)
			.def("sendJsonRequest", &KeyVEILConnectorWrapper::sendJsonRequest)
			.def("sendBase64Request", &KeyVEILConnectorWrapper::sendBase64Request)
			.def("refresh", &KeyVEILConnectorWrapper::refresh)
			.def_readonly("tokenCount", &KeyVEILConnectorWrapper::tokenCount)
			.def("tokenByIndex", &KeyVEILConnectorWrapper::tokenByIndex)
			.def("tokenByName", &KeyVEILConnectorWrapper::tokenByName)
			.def("tokenBySerialNumber", &KeyVEILConnectorWrapper::tokenBySerialNumber)
			.def("tokenById", &KeyVEILConnectorWrapper::tokenById)
			.def_readonly("favoriteCount", &KeyVEILConnectorWrapper::favoriteCount)
			.def("favoriteByIndex", &KeyVEILConnectorWrapper::favoriteByIndex)
			.def("favoriteByName", &KeyVEILConnectorWrapper::favoriteByName)
			.def("favoriteById", &KeyVEILConnectorWrapper::favoriteById)
			.def("deleteFavorite", &KeyVEILConnectorWrapper::DeleteFavorite)
			.def("updateFavoriteName", &KeyVEILConnectorWrapper::UpdateFavoriteName)
			.def("tokenCountForEnterpriseId", &KeyVEILConnectorWrapper::tokenCountForEnterpriseId)
			.def("tokenForEnterprise", &KeyVEILConnectorWrapper::tokenForEnterprise)
			.def("favoriteCountForEnterpriseId", &KeyVEILConnectorWrapper::favoriteCountForEnterprise)
			.def("favoriteForEnterprise", &KeyVEILConnectorWrapper::favoriteForEnterprise)
			;

		class_<TokenWrapper>("Token")
			.def("release", &TokenWrapper::release)
			.add_property("tokenName", &TokenWrapper::getTokenName, &TokenWrapper::setTokenName)
			.def_readonly("serialNumber", &TokenWrapper::serialNumber)
			.def_readonly("id", &TokenWrapper::id)
			.def_readonly("enterpriseName", &TokenWrapper::enterpriseName)
			.def_readonly("memberName", &TokenWrapper::memberName)
			.def_readonly("tokenType", &TokenWrapper::tokenType)
			.def_readonly("enterpriseId", &TokenWrapper::enterpriseId)
			.def_readonly("memberId", &TokenWrapper::memberId)
			.def("openSession", &TokenWrapper::openSession)
			;

		class_<SessionWrapper>("Session")
			.def("release", &SessionWrapper::release)
			.def("close", &SessionWrapper::close)
			.def("login", &SessionWrapper::Login)
			.def_readonly("isLoggedIn", &SessionWrapper::IsLoggedIn)
			.def("logout", &SessionWrapper::Logout)
			//.def("GenerateWorkingKey", &SessionWrapper::GenerateWorkingKey)
			//.def("RegenerateWorkingKey", &SessionWrapper::RegenerateWorkingKey)
			.def("getProfile", &SessionWrapper::getProfile)
			.def_readonly("isLocked", &SessionWrapper::IsLocked)
			.def_readonly("retriesLeft", &SessionWrapper::retriesLeft)
			.def_readonly("isValid", &SessionWrapper::IsValid)
			.def("duplicate", &SessionWrapper::Duplicate)
			.def("encryptFileUsingFavorite", &SessionWrapper::encryptFileUsingFavorite)
			.def("decryptFile", &SessionWrapper::decryptFile)
			.def("encryptDataUsingFavorite", &SessionWrapper::encryptDataUsingFavorite)
			.def("decryptData", &SessionWrapper::decryptData)
			;

		class_<FavoriteWrapper>("Favorite")
			.def("release", &FavoriteWrapper::release)
			.def_readonly("favoriteId", &FavoriteWrapper::getFavoriteId)
			.def_readonly("enterpriseId", &FavoriteWrapper::getEnterpriseId)
			.add_property("favoriteName", &FavoriteWrapper::getFavoriteName, &FavoriteWrapper::setFavoriteName)
			.add_property("tokenSerialNumber", &FavoriteWrapper::getTokenSerialNumber, &FavoriteWrapper::setTokenSerialNumber)
			.add_property("headerData", &FavoriteWrapper::getHeaderData, &FavoriteWrapper::setHeaderData)
			.def("encryptFile", &FavoriteWrapper::encryptFile)
			.def("encryptData", &FavoriteWrapper::encryptData)
			;
	}


	{
		// map the Util namespace to a sub-module
		// make "from mypackage.Util import <whatever>" work
		object GenericConnectorWrapperModule(handle<>(borrowed(PyImport_AddModule("OpenVEIL.GenericConnector"))));
		// make "from mypackage import Util" work
		scope().attr("GenericConnector") = GenericConnectorWrapperModule;
		// set the current scope to the new sub-module
		scope util_scope = GenericConnectorWrapperModule;


		class_<GenericConnectorWrapper>("GenericConnector")
			.def("release", &GenericConnectorWrapper::release)
			.def("connectToServer", &GenericConnectorWrapper::connectToServer)
			.def("disconnect", &GenericConnectorWrapper::disconnect)
			.def_readonly("isConnected", &GenericConnectorWrapper::isConnected)
			.def("sendJsonRequest", &GenericConnectorWrapper::sendJsonRequest)
			.def("sendBase64Request", &GenericConnectorWrapper::sendBase64Request)
			;
	}
}


tsData tsDataFromPyObject(const boost::python::object& obj)
{
	tsData tmp;
	char *data;
	Py_ssize_t size;
	PyObject* py_buffer = obj.ptr();

	if (PyByteArray_Check(py_buffer)) {
		size = PyByteArray_GET_SIZE(py_buffer);
		data = PyByteArray_AS_STRING(py_buffer);
		tmp.assign((uint8_t*)data, size);
	}
	else if (PyBytes_Check(py_buffer)) {
		size = PyBytes_GET_SIZE(py_buffer);
		data = PyBytes_AS_STRING(py_buffer);
		tmp.assign((uint8_t*)data, size);
	}
#ifdef Py_USING_UNICODE
	else if (PyUnicode_Check(py_buffer)) {
		size = PyUnicode_GET_SIZE(py_buffer);
		tsAscii str(PyUnicode_AS_UNICODE(py_buffer), size);
		tmp = str.ToUTF8Data();
	}
#endif
#ifdef PyString_AS_STRING
	else if (PyString_Check(py_buffer)) {
		size = PyString_GET_SIZE(py_buffer);
		if (size == 1) {
			data = PyString_AS_STRING(py_buffer);
			return PyInt_FromLong(ord);
		}
	}
#endif

	return tmp;
}

boost::python::object tsDataToPyObject(const tsData& data)
{
	boost::python::object o(boost::python::handle<>(PyByteArray_FromStringAndSize((const char *)data.c_str(), data.size())));
	return o;
}