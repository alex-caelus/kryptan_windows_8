//
// KeyboardPopup.xaml.cpp
// Implementation of the KeyboardPopup class
//

#include "pch.h"
#include "KeyboardPopup.xaml.h"

using namespace kryptan_windows;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

KeyboardPopup::KeyboardPopup()
{
	InitializeComponent();
}

float KeyboardPopup::KeyboardWidth::get()
{
    return m_width;
}
float KeyboardPopup::KeyboardHeight::get()
{
    return m_height;
}

void KeyboardPopup::KeyboardWidth::set(float value)
{
    MasterGrid->Width = value;
    m_width = value;
}
void KeyboardPopup::KeyboardHeight::set(float value)
{
    MasterGrid->Height = value;
    m_height = value;
}
Windows::UI::Xaml::Controls::Primitives::Popup^ KeyboardPopup::NewPopup(Page^ page)
{
    auto content = ref new KeyboardPopup();

    content->KeyboardHeight = Window::Current->Bounds.Height;
    content->KeyboardWidth = Window::Current->Bounds.Width;

    //content->MaxWidth = Window::Current->Bounds.Width;
    //content-> = ref new SolidColorBrush(Colors::LightGray);

    auto pop = ref new Popup();
    pop->Child = content;
    //pop->VerticalOffset = 10;
    //pop->HorizontalOffset = 10;
    pop->Width = 30;
    pop->Height = 50;
    pop->IsOpen = true;
    pop->MaxWidth = 200;


    return pop;
}

void kryptan_windows::KeyboardPopup::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Button^ button = (Button^)sender;
    String^ text = (String^)button->Content;

}


double KeyboardPopup::PasteButtonLeftPos::get()
{
    return textContainer->ActualWidth - pasteButton->ActualWidth;
}
