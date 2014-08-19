//
// SecureTextBlock.xaml.h
// Declaration of the SecureTextBlock class
//

#pragma once

#include "CustomViews\SecureTextBlock.g.h"
#include "SecureTextImageSource\SecureTextImageSourceD2D.h"
#include "SecureTextImageSource\SecureTextImageSourceDrawLayout.h"

#include <map>

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
            SecureTextImageSourceDrawLayout^ get()
            { 
                return (SecureTextImageSourceDrawLayout^)this->GetValue(TextOptionsProperty);
            }
            void set(SecureTextImageSourceDrawLayout^ value)
            {
                this->SetValue(TextOptionsProperty, value);
                DrawText();
            }
        }

        static void RegisterDependencyProperties();

    private:


        Caelus::Utilities::SecureString b_text;

        SecureTextImageSourceD2D^ m_ImageSource;

        void SecureTextBlock_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SecureTextBlock_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);


        static property Windows::UI::Xaml::DependencyProperty^ TextOptionsProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get()
            {
                return _TextOptionsProperty;
            }
        }

        static Windows::UI::Xaml::DependencyProperty^ _TextOptionsProperty;
        void SecureTextBlock_DataContextChanged(Windows::UI::Xaml::FrameworkElement^ sender, Windows::UI::Xaml::DataContextChangedEventArgs^ args);

        Caelus::Utilities::SecureString lastDrawnText;
        double lastActualHeight = 0;
        double lastActualWidth = 0;

        SecureTextImageSourceD2D^ m_secureimagesource;
        SecureTextImageSourceD2D^ getImageSource(int height, int width, bool recreate);
        //static SecureTextImageSourceD2D^ getImageSource(int height, int width, bool recreate);

        //static std::map<std::pair<int, int>, SecureTextImageSourceD2D^ > _ImageSourceList;

    };
}
