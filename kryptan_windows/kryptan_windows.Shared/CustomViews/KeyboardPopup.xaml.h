//
// KeyboardPopup.xaml.h
// Declaration of the KeyboardPopup class
//

#pragma once

#include "CustomViews\KeyboardPopup.g.h"

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

        property double PasteButtonLeftPos
        {
            double get();
        }

    private:
        void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);


        float m_width;
        float m_height;
    };
}
