//
// KeyboardPopup.xaml.h
// Declaration of the KeyboardPopup class
//

#pragma once

#include "CustomViews\KeyboardPopup.g.h"
#include "kryptan_core\SecureString\SecureString.h"

#include <ctime>

namespace kryptan_windows
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class KeyboardPopup sealed
	{
	public:
        KeyboardPopup();

        static Windows::UI::Xaml::Controls::Primitives::Popup^ NewPopup(Windows::UI::Xaml::Controls::Page^ page);

        property float KeyboardWidth
        {
            float get();
            void set(float value);
        }

        property float KeyboardHeight
        {
            float get();
            void set(float value);
        }

    private:
        void Letter_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        Caelus::Utilities::SecureString currentText;
        float m_width;
        float m_height;

        bool shiftDown = false;
        bool shiftKeepUp = false;
        clock_t shiftPressedTime = 0;

        void doToggleShift();
        void currentTextChanged();

        void keyboardPopup_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Shift_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Back_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Cancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Space_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Done_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void Paste_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
