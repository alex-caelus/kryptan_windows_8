#include "pch.h"
#include "SecureTextImageSourceDrawLayout.h"


using namespace Shared;


bool SecureTextImageSourceDrawLayout::Equals(SecureTextImageSourceDrawLayout^ other, bool checkText)
{
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

    if (checkText)
    {
        Kryptan::Core::SecureString* thisStr = (Kryptan::Core::SecureString*)this->Text.ToInt32();
        Kryptan::Core::SecureString* otherStr = (Kryptan::Core::SecureString*)other->Text.ToInt32();

        if (!thisStr->equals(*otherStr)) return false;
    }

    return true;
}