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
    enum class KeyboardCloseReason{Cancel, Done};

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class KeyboardPopup sealed
	{
    internal:
        typedef std::function<void(KeyboardCloseReason, Caelus::Utilities::SecureString)> CloseCallbackFunction;
        static Windows::UI::Xaml::Controls::Primitives::Popup^ NewPopup(CloseCallbackFunction closeCallback, Caelus::Utilities::SecureString initialValue = Caelus::Utilities::SecureString(), bool isPassword = false, Platform::String^ hint = nullptr);

	public:
        KeyboardPopup();

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

        property CloseCallbackFunction closeCallback;
        property Caelus::Utilities::SecureString initialValue;
        property Windows::UI::Xaml::Controls::Primitives::Popup^ popupParent;

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
        void Back_Holding(Platform::Object^ sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs^ e);
    };
}
