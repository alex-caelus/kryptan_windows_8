//
// SecureTextBlock.xaml.h
// Declaration of the SecureTextBlock class
//

#pragma once

#include "SecureTextBlock.g.h"
#include "SecureTextImageSourceD2D.h"
#include "SecureTextImageSourceDrawLayout.h"

namespace kryptan_windows
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SecureTextBlock sealed
	{
	public:
        SecureTextBlock();
        void DrawText();

        property SecureTextImageSourceDrawLayout^ TextOptions
        {
            SecureTextImageSourceDrawLayout^ get(){ return m_TextOptions; }
        }
    private:


        Caelus::Utilities::SecureString b_text;

        SecureTextImageSourceD2D^ m_ImageSource;

        SecureTextImageSourceDrawLayout^ m_TextOptions;
	};
}
