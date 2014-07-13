//
// SecureTextBlock.xaml.h
// Declaration of the SecureTextBlock class
//

#pragma once

#include "CustomViews\SecureTextBlock.g.h"
#include "SecureTextImageSource\SecureTextImageSourceD2D.h"
#include "SecureTextImageSource\SecureTextImageSourceDrawLayout.h"

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
        void SecureTextBlock_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SecureTextBlock_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
    };
}
