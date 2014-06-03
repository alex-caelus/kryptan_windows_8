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
    TextOptions->FontSize = 12;
    TextOptions->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
    TextOptions->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    wchar_t* ptr = L"This is a test.";
    TextOptions->Text = SecureString((char*)ptr, wcslen(ptr) * 2, false, true);
}

void SecureTextBlock::DrawText()
{
    m_ImageSource = ref new SecureTextImageSourceD2D((int)this->ActualWidth, (int)this->ActualHeight);
    Image1->Source = m_ImageSource;

    m_ImageSource->Draw(TextOptions);
}