#pragma once

#include "Libs\kryptan_core\SecureString\SecureString.h"

/// <summary>
/// This is a wrapper for the Kryptan::Core::SecureString class
/// </summary>
class EncodingHandler
{
public:

    /// <summary>
    /// Makes a platform string.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <returns></returns>
    static Platform::String^ makePlatformString(const char* str);

    /// <summary>
    /// Makes a platform string.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <returns></returns>
    static wchar_t* makeWideString(const char* str);
    
    /// <summary>
    /// Extracts utf-8 encoded data from a platform string.
    /// </summary>
    /// <param name="str">The string.</param>
    /// <returns></returns>
    static char* extractFromPlatformString(Platform::String^ str);

private:

};

