//
// SecureTextBlock.xaml.cpp
// Implementation of the SecureTextBlock class
//

#include "pch.h"
#include "SecureTextBlock.xaml.h"

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

using namespace Caelus::Utilities;
using namespace kryptan_windows;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

SecureTextBlock::SecureTextBlock() : m_TextOptions(ref new SecureTextImageSourceDrawLayout())
{
    InitializeComponent();

    TextOptions->BackroundColor = Windows::UI::Colors::Transparent;
    TextOptions->TextColor = Windows::UI::Colors::White;
    TextOptions->FontName = L"Verdana";
    TextOptions->FontSize = 16;
    TextOptions->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
    TextOptions->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    TextOptions->Text = SecureString();
}

void SecureTextBlock::DrawText()
{
    if (this->ActualHeight > 0 && this->ActualWidth > 0 )
    {
        m_ImageSource = ref new SecureTextImageSourceD2D((int)this->ActualWidth, (int)this->ActualHeight);
        Image1->Source = m_ImageSource;

        m_ImageSource->Draw(TextOptions);
    }
}

void kryptan_windows::SecureTextBlock::SecureTextBlock_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    DrawText();
}


void kryptan_windows::SecureTextBlock::SecureTextBlock_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
    DrawText();
}
