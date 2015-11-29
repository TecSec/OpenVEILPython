import OpenVEIL

kvConn = OpenVEIL.KeyVEILConnector.KeyVEILConnector();
kvConn.connectToServer('http://localhost:8125', 'user1', '11111111')

for i in range(kvConn.tokenCount):
	token = kvConn.tokenByIndex(i)
	print("\nToken")
	print("  Name:            ", token.tokenName)
	print("  Type:            ", token.tokenType)
	print("  serialNumber:    ", token.serialNumber)
	print("  id:              ", token.id)
	print("  Enterprise name: ", token.enterpriseName)
	print("  Enterprise ID:   ", token.enterpriseId)
	print("  Member Name:     ", token.memberName)
	print("  Member ID:       ", token.memberId)

for i in range(kvConn.favoriteCount):
	fav = kvConn.favoriteByIndex(i)
	print("\nFavorite")
	print("  Name:         ", fav.favoriteName)
	print("  ID:           ", fav.favoriteId)
	print("  Enterprise:   ", fav.enterpriseId)
	print("  Token Serial: ", fav.tokenSerialNumber)

	
session = kvConn.tokenBySerialNumber(bytearray.fromhex('906845AEC554109D')).openSession()

print("SESSION")
print("Is Valid:      ", session.isValid)
print("Is logged in:  ", session.isLoggedIn)

if not session.isLoggedIn:
	print("  login returned:  ", session.login('11111111'))
	print("  Is logged in:  ", session.isLoggedIn)


inData = bytes([1,2,3,4,5])
print ("Original data: ", inData)

outData = kvConn.favoriteByName("Staff").encryptData(session, inData, True)

print("Encrypted data: ", outData)

newSrc = session.decryptData(outData)

print ("Decrypted data: ", newSrc)


#
# Now try file encryption using the same encryption information
#
print("File encrypt returned", kvConn.favoriteByName("Staff").encryptFile(session, 'tokenList.py', True, 'tokenList.py.ckm'))
print("File decrypt returned", session.decryptFile('tokenList.py.ckm', 'tokenList2.py'))