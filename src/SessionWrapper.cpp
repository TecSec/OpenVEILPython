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

#pragma region Session Wrapper class
SessionWrapper::SessionWrapper()
{
}
SessionWrapper::SessionWrapper(std::shared_ptr<IKeyVEILSession> _sess) : _session(_sess)
{
}
SessionWrapper::~SessionWrapper()
{
	release();
}
void SessionWrapper::release()
{
	_session.reset();
}
void SessionWrapper::close()
{
	if (!!_session)
	{
		_session->Close();
	}
}
bool SessionWrapper::isReady()
{
	return !!_session;
}
LoginStatus SessionWrapper::Login(const std::string& pin)
{
	if (!isReady())
		return loginStatus_NoServer;
	return _session->Login(pin.c_str());
}
bool SessionWrapper::IsLoggedIn()
{
	if (!isReady())
		return false;
	return _session->IsLoggedIn();
}
bool SessionWrapper::Logout()
{
	if (!isReady())
		return false;
	return _session->Logout();
}
//bool SessionWrapper::GenerateWorkingKey(Asn1::CTS::CkmCombineParameters& params, std::function<bool(Asn1::CTS::CkmCombineParameters&, tsCryptoData&)> headerCallback, tsCryptoData &WorkingKey)
//{
//}
//bool SessionWrapper::RegenerateWorkingKey(Asn1::CTS::CkmCombineParameters& params, tsCryptoData &WorkingKey)
//{
//}

static void JSONToDictionary(const JSONObject& obj, py::dict& d);
static void JSONArrayToList(const JSONFieldList& ary, py::list& r)
{
	std::for_each(ary->begin(), ary->end(), [&r](const JSONField& fld) {
		switch (fld.Type())
		{
		case JSONField::jsonNull:
			r.append(py::cast(nullptr));
			break;
		case JSONField::jsonString:
			r.append(py::cast(fld.AsString().c_str()));
			break;
		case JSONField::jsonBool:
			r.append(py::cast(fld.AsBool()));
			break;
		case JSONField::jsonNumber:
			r.append(py::cast(fld.AsNumber()));
			break;
		case JSONField::jsonObject:
		{
			py::dict d1;

			JSONToDictionary(fld.AsObject(), d1);
			r.append(d1);
		}
		break;
		case JSONField::jsonArray:
		{
			py::list r1;
			JSONArrayToList(fld.AsArray(), r1);
			r.append(r1);
		}
		break;
		}

	});
}
static void JSONToDictionary(const JSONObject& obj, py::dict& d)
{
	std::for_each(obj.Fields()->begin(), obj.Fields()->end(), [&d](const JSONField& fld) {
		switch (fld.Type())
		{
		case JSONField::jsonNull:
			d[fld.Name().c_str()] = nullptr;
			break;
		case JSONField::jsonString:
			d[fld.Name().c_str()] = py::cast(fld.AsString().c_str());
			break;
		case JSONField::jsonBool:
			d[fld.Name().c_str()] = py::cast(fld.AsBool());
			break;
		case JSONField::jsonNumber:
			d[fld.Name().c_str()] = py::cast(fld.AsNumber());
			break;
		case JSONField::jsonObject:
		{
			py::dict d1;

			JSONToDictionary(fld.AsObject(), d1);
			d[fld.Name().c_str()] = d1;
		}
			break;
		case JSONField::jsonArray:
		{
			py::list r;
			JSONArrayToList(fld.AsArray(), r);
			d[fld.Name().c_str()] = r;
		}
		break;
		}
	});
}
py::dict SessionWrapper::getProfile()
{
	py::dict d;

	if (!isReady())
		return d;

	JSONObject obj = _session->GetProfile()->toJSON();
	JSONToDictionary(obj, d);
	return d;
}
bool SessionWrapper::IsLocked()
{
	if (!isReady())
		return false;
	return _session->IsLocked();
}
size_t SessionWrapper::retriesLeft()
{
	if (!isReady())
		return 0;
	return _session->retriesLeft();
}
bool SessionWrapper::IsValid()
{
	if (!isReady())
	{
		return false;
	}
	return _session->IsValid();
}
SessionWrapper SessionWrapper::Duplicate()
{
	if (!isReady())
		return SessionWrapper();
	return SessionWrapper(_session->Duplicate());
}

bool SessionWrapper::decryptFile(const std::string& encryptedFile, const std::string& decryptedFile)
{
	if (!isReady())
		return false;

	if (!InitializeCmsHeader())
		return false;

	std::shared_ptr<IFileVEILOperations> fileOps;
	std::shared_ptr<IFileVEILOperationStatus> status;
	tsCryptoString inputFile(encryptedFile.c_str());
	tsCryptoString outputFile(decryptedFile.c_str());

	if (xp_GetFileAttributes(inputFile) == XP_INVALID_FILE_ATTRIBUTES || xp_IsDirectory(inputFile))
	{
		PyErr_SetString(PyExc_RuntimeError, (tsCryptoString() << "File -> " << inputFile << " <- does not exist Decrypt operation aborted").c_str());
		throw py::error_already_set();
		return false;
	}

	status = ::TopServiceLocator()->Finish<IFileVEILOperationStatus>(new StatusClass());

	if (!(fileOps = CreateFileVEILOperationsObject()) ||
		!(fileOps->SetStatusInterface(status)) ||
		!(fileOps->SetSession(_session)))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the file decryptor.  The OpenVEIL installation may be damaged.");
		throw py::error_already_set();
		return false;
	}

	if (!fileOps->DecryptFileAndStreams(inputFile, outputFile))
	{
		PyErr_SetString(PyExc_RuntimeError, "Decrypt failed.");
		throw py::error_already_set();
		return false;
	}

	return true;
}
byte_array SessionWrapper::decryptData(const byte_array& encryptedData)
{
	if (!isReady())
		return byte_array();

	if (!InitializeCmsHeader())
		return byte_array();

	std::shared_ptr<IFileVEILOperations> fileOps;
	std::shared_ptr<IFileVEILOperationStatus> status;
	tsCryptoData inData(tsDataFromPyObject(encryptedData));
	tsCryptoData destData;

	status = ::TopServiceLocator()->Finish<IFileVEILOperationStatus>(new StatusClass());

	if (!(fileOps = CreateFileVEILOperationsObject()) ||
		!(fileOps->SetStatusInterface(status)) ||
		!(fileOps->SetSession(_session)))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the file decryptor.  The OpenVEIL may be damaged.");
		throw py::error_already_set();
		return byte_array();
	}

	if (!fileOps->DecryptCryptoData(inData, destData))
	{
		PyErr_SetString(PyExc_RuntimeError, "Decryption failed.");
		throw py::error_already_set();
		return byte_array();
	}

	return tsDataToPyObject(destData);
}
bool SessionWrapper::encryptFileUsingFavorite(FavoriteWrapper fav, const std::string& sourceFile, bool compress, const std::string& encryptedFile)
{
	return fav.encryptFile(*this, sourceFile, compress, encryptedFile);
}
byte_array SessionWrapper::encryptDataUsingFavorite(FavoriteWrapper fav, const byte_array& sourceData, bool compress)
{
	return fav.encryptData(*this, sourceData, compress);
}

#pragma endregion
