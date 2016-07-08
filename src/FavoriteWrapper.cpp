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

#pragma region Favorite Wrapper class
FavoriteWrapper::FavoriteWrapper()
{}
FavoriteWrapper::FavoriteWrapper(std::shared_ptr<IFavorite> _fav) : _favorite(_fav)
{}
FavoriteWrapper::~FavoriteWrapper()
{
	release();
}
void FavoriteWrapper::release()
{
	_favorite.reset();
}
bool FavoriteWrapper::isReady()
{
	return !!_favorite;
}
std::string FavoriteWrapper::getFavoriteId()
{
	if (!isReady())
		return "";
	return ToString()(_favorite->favoriteId()).c_str();
}
void FavoriteWrapper::setFavoriteId(const std::string& setTo)
{
	if (!isReady())
		return;
	_favorite->favoriteId(ToGuid()(setTo.c_str()));
}
std::string FavoriteWrapper::getEnterpriseId()
{
	if (!isReady())
		return "";
	return ToString()(_favorite->enterpriseId()).c_str();
}
void FavoriteWrapper::setEnterpriseId(const std::string& setTo)
{
	if (!isReady())
		return;
	_favorite->enterpriseId(ToGuid()(setTo.c_str()));
}
std::string FavoriteWrapper::getFavoriteName()
{
	if (!isReady())
		return "";
	return _favorite->favoriteName().c_str();
}
void FavoriteWrapper::setFavoriteName(const std::string& setTo)
{
	if (!isReady())
		return;
	_favorite->favoriteName(setTo.c_str());
}
std::string FavoriteWrapper::getTokenSerialNumber()
{
	if (!isReady())
		return "";
	return _favorite->tokenSerialNumber().ToHexString().c_str();
}
void FavoriteWrapper::setTokenSerialNumber(const std::string& setTo)
{
	if (!isReady())
		return;
	_favorite->tokenSerialNumber((tsCryptoString(setTo.c_str())).HexToData());
}
byte_array FavoriteWrapper::getHeaderData()
{
	if (!isReady())
		return byte_array();
	tsCryptoData data(_favorite->headerData());

	return tsDataToPyObject(data);
}
void FavoriteWrapper::setHeaderData(const byte_array& setTo)
{
	if (!isReady())
		return;
	_favorite->headerData(tsDataFromPyObject(setTo));
}
bool FavoriteWrapper::encryptFile(SessionWrapper session, const std::string& sourceFile, bool compress, const std::string& encryptedFile)
{
	std::shared_ptr<IFileVEILOperations> fileOps;
	std::shared_ptr<ICmsHeader> header;
	std::shared_ptr<IFileVEILOperationStatus> status;
	tsCryptoString inputFile(sourceFile.c_str());
	tsCryptoString outputFile(encryptedFile.c_str());

	if (!isReady())
		return false;

	if (!InitializeCmsHeader())
		return false;

	if (xp_GetFileAttributes(inputFile) == XP_INVALID_FILE_ATTRIBUTES || xp_IsDirectory(inputFile))
	{
		PyErr_SetString(PyExc_RuntimeError, (tsCryptoString() << "File -> " << inputFile.c_str() << " <- does not exist Encrypt operation aborted").c_str());
		throw py::error_already_set();
		return false;
	}

	status = ::TopServiceLocator()->Finish<IFileVEILOperationStatus>(new StatusClass());

	if (!(fileOps = CreateFileVEILOperationsObject()) ||
		!(fileOps->SetStatusInterface(status)) ||
		!(fileOps->SetSession(session.handle())))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the file encryptor.  The CKM Runtime may be damaged.");
		throw py::error_already_set();
		return false;
	}

	// Create output file name based on the input file name
	if (outputFile.size() == 0)
	{
		outputFile = inputFile;
		outputFile += ".ckm";
	}
	if (!(header = ::TopServiceLocator()->get_instance<ICmsHeader>("/CmsHeader")) || !header->FromBytes(_favorite->headerData()))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the encryption header.");
		throw py::error_already_set();
		return false;
	}

	// Indicate compression is desired.
	if (compress)
	{
		header->SetCompressionType(ct_zLib);
	}
	else
	{
		header->SetCompressionType(ct_None);
	}
	if (header->GetEncryptionAlgorithmID() == tscrypto::_TS_ALG_ID::TS_ALG_INVALID)
		header->SetEncryptionAlgorithmID(tscrypto::_TS_ALG_ID::TS_ALG_AES_GCM_256);

	if (!(fileOps->EncryptFileAndStreams(inputFile.c_str(), outputFile.c_str(), header, compress ? ct_zLib : ct_None,
		header->GetEncryptionAlgorithmID(), OIDtoID(header->GetDataHashOID().ToOIDString().c_str()),
		header->HasHeaderSigningPublicKey(), true,
		(Alg2Mode(header->GetEncryptionAlgorithmID()) == tscrypto::_SymmetricMode::CKM_SymMode_GCM ||
			Alg2Mode(header->GetEncryptionAlgorithmID()) == tscrypto::_SymmetricMode::CKM_SymMode_CCM) ?
		TS_FORMAT_CMS_ENC_AUTH : TS_FORMAT_CMS_CT_HASHED,
		false, header->GetPaddingType(), 5000000)))
	{
		PyErr_SetString(PyExc_RuntimeError, "Encrypt failed.");
		throw py::error_already_set();
		return false;
	}
	return true;
}

byte_array FavoriteWrapper::encryptData(SessionWrapper session, const byte_array& sourceData, bool compress)
{
	if (!isReady())
		return byte_array();

	tsCryptoData inData(tsDataFromPyObject(sourceData));
	tsCryptoData encData;

	if (inData.size() == 0)
	{
		return byte_array();
	}

	if (!InitializeCmsHeader())
		return byte_array();

	std::shared_ptr<IFileVEILOperations> fileOps;
	std::shared_ptr<IFileVEILOperationStatus> status;
	std::shared_ptr<ICmsHeader> header;

	if (!session.handle())
	{
		PyErr_SetString(PyExc_RuntimeError, "Session not valid.");
		throw py::error_already_set();
		return byte_array();
	}

	status = ::TopServiceLocator()->Finish<IFileVEILOperationStatus>(new StatusClass());

	if (!(fileOps = CreateFileVEILOperationsObject()) ||
		!(fileOps->SetStatusInterface(status)) ||
		!(fileOps->SetSession(session.handle())))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the file encryptor.  The CKM Runtime may be damaged.");
		throw py::error_already_set();
		return byte_array();
	}
	if (!(header = ::TopServiceLocator()->get_instance<ICmsHeader>("/CmsHeader")) || !header->FromBytes(_favorite->headerData()))
	{
		PyErr_SetString(PyExc_RuntimeError, "An error occurred while building the encryption header.");
		throw py::error_already_set();
		return byte_array();
	}

	if (!header)
	{
		PyErr_SetString(PyExc_RuntimeError, "The favorite is invalid or incomplete.");
		throw py::error_already_set();
		return byte_array();
	}

	// Indicate compression is desired.
	if (compress)
	{
		header->SetCompressionType(ct_zLib);
	}
	else
	{
		header->SetCompressionType(ct_None);
	}
	if (header->GetEncryptionAlgorithmID() == tscrypto::_TS_ALG_ID::TS_ALG_INVALID)
		header->SetEncryptionAlgorithmID(tscrypto::_TS_ALG_ID::TS_ALG_AES_GCM_256);

	if (!(fileOps->EncryptCryptoData(inData, encData, header, compress ? ct_zLib : ct_None,
		header->GetEncryptionAlgorithmID(), OIDtoID(header->GetDataHashOID().ToOIDString().c_str()),
		header->HasHeaderSigningPublicKey(), true,
		(Alg2Mode(header->GetEncryptionAlgorithmID()) == tscrypto::_SymmetricMode::CKM_SymMode_GCM ||
			Alg2Mode(header->GetEncryptionAlgorithmID()) == tscrypto::_SymmetricMode::CKM_SymMode_CCM) ?
		TS_FORMAT_CMS_ENC_AUTH : TS_FORMAT_CMS_CT_HASHED,
		false, header->GetPaddingType(), 5000000)))
	{
		PyErr_SetString(PyExc_RuntimeError, "Encrypt failed.");
		throw py::error_already_set();
		return byte_array();
	}
	return tsDataToPyObject(encData);
}
#pragma endregion
