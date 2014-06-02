#pragma once

#include "Libs\kryptan_core\SecureString\SecureString.h"

/// <summary>
/// This is a wrapper for the Kryptan::Core::SecureString class
/// </summary>
class SecureStringHandler
{
public:

    /// <summary>
    /// Initializes a new instance of the <see cref="SecureStringHandler"/> class.
    /// Takes ownership of the string, when deleted it
    /// replaces the buffer with garbage
    /// </summary>
    /// <param name="takeOwnerShip">String to take ownership of.</param>
    SecureStringHandler(Platform::String^ takeOwnerShip);

    /// Creates a new empty securestringhandler
    SecureStringHandler();

    /// Destructor
    ~SecureStringHandler();

    /// <summary>
    /// Gets the corresponding secure string.
    /// </summary>
    /// <returns></returns>
    Kryptan::Core::SecureString getCorrespondingSecureString();

    /// <summary>
    /// Imports the corresponding secure string.
    /// </summary>
    /// <param name="import">The import.</param>
    void importCorrespondingSecureString(Kryptan::Core::SecureString& import);

    /// <summary>
    /// Makes a platform string.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <returns></returns>
    static Platform::String^ makePlatformString(const char* str);
    
private:

    /// <summary>
    /// The secure string
    /// </summary>
    Kryptan::Core::SecureString secureString;

};

