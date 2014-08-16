//
// SecureTextEdit.xaml.cpp
// Implementation of the SecureTextEdit class
//

#include "pch.h"
#include "SecureTextEdit.xaml.h"
#include "Utilities\EncodingHandler.h"
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

using namespace concurrency;

using namespace Caelus::Utilities;
using namespace kryptan_windows;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

Windows::UI::Xaml::DependencyProperty^ SecureTextEdit::_IsPasswordProperty = nullptr;
Windows::UI::Xaml::DependencyProperty^ SecureTextEdit::_HintProperty = nullptr;
Windows::UI::Xaml::DependencyProperty^ SecureTextEdit::_EditableProperty = nullptr;

SecureTextEdit::SecureTextEdit() : m_TextOptions(ref new SecureTextImageSourceDrawLayout())
{
    InitializeComponent();

    TextOptions->BackroundColor = Windows::UI::Colors::Transparent;
    TextOptions->TextColor = Windows::UI::Colors::Black;
    TextOptions->FontName = L"Verdana";
    TextOptions->FontSize = 20;
    TextOptions->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
    TextOptions->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    TextOptions->Text = SecureString();
}

void SecureTextEdit::RegisterDependencyProperties()
{
    if (_IsPasswordProperty == nullptr)
    {
        _IsPasswordProperty = DependencyProperty::RegisterAttached("IsPassword", Platform::Boolean::typeid, SecureTextEdit::typeid, nullptr);
        _HintProperty = DependencyProperty::RegisterAttached("Hint", Platform::String::typeid, SecureTextEdit::typeid, nullptr);
        _EditableProperty = DependencyProperty::RegisterAttached("Editable", Platform::Boolean::typeid, SecureTextEdit::typeid, ref new PropertyMetadata(true));
    }
}

void SecureTextEdit::DrawText()
{
    if (this->ActualHeight > 0 && this->ActualWidth > 0)
    {
        m_ImageSource = ref new SecureTextImageSourceD2D((int)this->ActualWidth, (int)this->ActualHeight);
        Image1->Source = m_ImageSource;

        if ((Hint != nullptr && TextOptions->Text.length() == 0))
        {
            //display hint
            SecureString tmp = TextOptions->Text;
            Windows::UI::Color tmpColor = TextOptions->TextColor;
            
            TextOptions->Text = SecureString(EncodingHandler::extractFromPlatformString(Hint));
            TextOptions->TextColor = Windows::UI::Colors::Gray;
            m_ImageSource->Draw(TextOptions);
            TextOptions->Text = tmp;
            TextOptions->TextColor = tmpColor;
        }
        else if (this->IsPassword)
        {
            SecureString tmp = TextOptions->Text;
            Windows::UI::Color tmpColor = TextOptions->TextColor;

            int nchars = tmp.length();
            SecureString bullets;
            for (int i = 0; i < nchars; i++)
            {
                bullets.append("*");
            }
            TextOptions->Text = bullets;

            m_ImageSource->Draw(TextOptions);
            TextOptions->Text = tmp;
            TextOptions->TextColor = tmpColor;
        }
        else
        {
            m_ImageSource->Draw(TextOptions);
        }
    }
}

SecureString SecureTextEdit::getSecurePassword()
{
    return b_text;
}

SecureStringContainer^ SecureTextEdit::getSecureStringContainer()
{
    return ref new SecureStringContainer(b_text);
}

void kryptan_windows::SecureTextEdit::Control_Clicked(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
    if (Editable)
    {
        auto currentBackground = border->Background;
        border->Background = ref new SolidColorBrush(Windows::UI::Colors::Gray);

        Dispatcher->RunIdleAsync(ref new Windows::UI::Core::IdleDispatchedHandler([this, currentBackground](Windows::UI::Core::IdleDispatchedHandlerArgs^ args){
            auto pop = KeyboardPopup::NewPopup([this, currentBackground](KeyboardCloseReason reason, SecureString text)
            {
                //this lambda is called when the keyboard closes
                if (reason == KeyboardCloseReason::Done)
                {
                    this->b_text = text;
                    this->TextOptions->Text = text;
                    this->DrawText();
                }
                this->border->Background = currentBackground;
            }, this->b_text, IsPassword, Hint);
        }));
    }
}

void kryptan_windows::SecureTextEdit::SecureTextEdit_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    DrawText();
}

void kryptan_windows::SecureTextEdit::SecureTextEdit_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
    DrawText();
}


void SecureTextEdit::clearSecureString()
{
    b_text = SecureString();
    TextOptions->Text = b_text;
    DrawText();
}