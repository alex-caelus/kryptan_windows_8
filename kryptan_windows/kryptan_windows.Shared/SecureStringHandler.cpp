#include "pch.h"
#include "SecureStringHandler.h"


/// <summary>
/// Initializes a new instance of the <see cref="SecureStringHandler" /> class.
/// Takes ownership of the string, when deleted it
/// replaces the buffer with garbage
/// </summary>
/// <param name="takeOwnerShip">String to take ownership of.</param>
SecureStringHandler::SecureStringHandler(Platform::String^ takeOwnerShip)
{

}

/// <summary>
/// Initializes a new instance of the <see cref="SecureStringHandler"/> class.
/// </summary>
SecureStringHandler::SecureStringHandler()
{
}


/// <summary>
/// Finalizes an instance of the <see cref="SecureStringHandler"/> class.
/// </summary>
/// Destructor
SecureStringHandler::~SecureStringHandler()
{
}

/// <summary>
/// Gets the corresponding secure string.
/// </summary>
/// <returns></returns>
Kryptan::Core::SecureString SecureStringHandler::getCorrespondingSecureString()
{
    return secureString;
}

/// <summary>
/// Imports the corresponding secure string.
/// </summary>
/// <param name="import">The import.</param>
void SecureStringHandler::importCorrespondingSecureString(Kryptan::Core::SecureString& import)
{

}

/// <summary>
/// Makes a platform string.
/// </summary>
/// <param name="str">The string.</param>
/// <returns></returns>
Platform::String^ SecureStringHandler::makePlatformString(const char* str)
{
    std::string strA = str;
    std::wstring strW(strA.begin(), strA.end());
    return ref new Platform::String(strW.c_str());
}
