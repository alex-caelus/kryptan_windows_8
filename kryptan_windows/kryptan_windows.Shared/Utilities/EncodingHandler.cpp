#include "pch.h"
#include "EncodingHandler.h"

#include <string>

using namespace Caelus::Utilities;

/// <summary>
/// Makes a platform string.
/// </summary>
/// <param name="str">The string.</param>
/// <returns></returns>
Platform::String^ EncodingHandler::makePlatformString(const char* str)
{
    auto w = makeWideString(str);
    auto p = ref new Platform::String(w);
    delete[] w;
    return p;
}

wchar_t* EncodingHandler::makeWideString(const char* str)
{
    if (!str) return 0;
    int srcLen = strlen(str);
    if (!srcLen)
    {
        wchar_t *w = new wchar_t[1];
        w[0] = 0;
        return w;
    }

    int requiredSize = MultiByteToWideChar(CP_UTF8,
        0,
        str, srcLen, 0, 0);

    if (!requiredSize)
    {
        return 0;
    }

    wchar_t *w = new wchar_t[requiredSize + 1];
    w[requiredSize] = 0;

    int retval = MultiByteToWideChar(CP_UTF8,
        0,
        str, srcLen, w, requiredSize);
    if (!retval)
    {
        delete[] w;
        return 0;
    }

    return w;
}


char* EncodingHandler::extractFromPlatformString(Platform::String^ str)
{
    std::wstring wstr(str->Data());
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';

    return buffer;
}