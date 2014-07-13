#include "pch.h"
#include "SecureTextImageSourceDrawLayout.h"


using namespace kryptan_windows;


bool SecureTextImageSourceDrawLayout::Equals(SecureTextImageSourceDrawLayout^ other, bool checkText)
{
    if (other == nullptr) return false;

    if(this->FontSize != other->FontSize) return false;

    if (this->BackroundColor.A != other->BackroundColor.A) return false;
    if (this->BackroundColor.R != other->BackroundColor.R) return false;
    if (this->BackroundColor.G != other->BackroundColor.G) return false;
    if (this->BackroundColor.B != other->BackroundColor.B) return false;

    if (this->TextColor.A != other->TextColor.A) return false;
    if (this->TextColor.R != other->TextColor.R) return false;
    if (this->TextColor.G != other->TextColor.G) return false;
    if (this->TextColor.B != other->TextColor.B) return false;

    if (this->TextAlignHorizontal != other->TextAlignHorizontal) return false;
    if (this->TextAlignVertical != other->TextAlignVertical) return false;

    if (checkText && !this->Text.equals(other->Text)) return false;

    return true;
}