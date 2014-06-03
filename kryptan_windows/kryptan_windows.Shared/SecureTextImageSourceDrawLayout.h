#pragma once

#include "pch.h"

namespace kryptan_windows
{
    ref class SecureTextBlock;

    public enum class SecureTextHorizontalAlign
    {
        LEFT,
        CENTER,
        RIGHT
    };
    public enum class SecureTextVerticalAlign
    {
        TOP,
        MIDDLE,
        BOTTOM
    };

    public ref class SecureTextImageSourceDrawLayout sealed
    {
    internal:
        property Caelus::Utilities::SecureString Text;
        SecureTextImageSourceDrawLayout(){};
    public:
        property float FontSize;
        property Platform::String^ FontName;
        property SecureTextHorizontalAlign TextAlignHorizontal;
        property SecureTextVerticalAlign TextAlignVertical;
        property Windows::UI::Color BackroundColor;
        property Windows::UI::Color TextColor;

        bool Equals(SecureTextImageSourceDrawLayout^ other, bool checkText);
        bool Equals(SecureTextImageSourceDrawLayout^ other){ return Equals(other, true); };

        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        bool Equals(Platform::Object^ other){ return Equals((SecureTextImageSourceDrawLayout^)other); }

    };

}