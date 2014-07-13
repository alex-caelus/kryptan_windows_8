//
// KeyboardPopup.xaml.cpp
// Implementation of the KeyboardPopup class
//

#include "pch.h"
#include "KeyboardPopup.xaml.h"
#include "SecureTextEdit.xaml.h"

#include <cctype>
#include <string>
#include <algorithm>

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
Windows::UI::Xaml::Controls::Primitives::Popup^ KeyboardPopup::NewPopup(KeyboardPopup::CloseCallbackFunction closeCallback, Caelus::Utilities::SecureString initialValue, bool isPassword, Platform::String^ hint)
{
    auto content = ref new KeyboardPopup();

    content->KeyboardHeight = Window::Current->Bounds.Height;
    content->KeyboardWidth = Window::Current->Bounds.Width;
    content->closeCallback = closeCallback;
    content->initialValue = initialValue;
    content->currentText = initialValue;
    content->secureText->TextOptions->Text = content->currentText;
    content->secureText->IsPassword = isPassword;
    content->secureText->Hint = hint;

    //content->MaxWidth = Window::Current->Bounds.Width;
    //content-> = ref new SolidColorBrush(Colors::LightGray);

    auto pop = ref new Popup();
    content->popupParent = pop;
    pop->Child = content;
    //pop->VerticalOffset = 10;
    //pop->HorizontalOffset = 10;
    pop->Width = 30;
    pop->Height = 50;
    pop->IsOpen = true;
    pop->MaxWidth = 200;
    
    return pop;
}

void kryptan_windows::KeyboardPopup::currentTextChanged()
{

    if (shiftDown && !shiftKeepUp)
        doToggleShift();
}

void kryptan_windows::KeyboardPopup::Letter_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    Button^ button = (Button^)sender;
    String^ textBtn = (String^)button->Content;

    const wchar_t* ptr = textBtn->Data();
    currentText.append((char*)ptr, textBtn->Length() * sizeof(wchar_t), false, true);

    secureText->TextOptions->Text = currentText;
    secureText->DrawText();

    currentTextChanged();
}

void kryptan_windows::KeyboardPopup::keyboardPopup_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    //move paste button to right side of the screen
    double rightPadding = 5;
    Canvas::SetLeft(this->pasteButton, textContainer->ActualWidth - pasteButton->ActualWidth - 3);
}

Platform::String^ convertSpecialCharToCase(Platform::String^ str, bool toLowerCase)
{
    if (toLowerCase)
    {
        if (str->Equals(L"Å"))
        {
            return ref new Platform::String(L"å");
        }
        else if (str->Equals(L"Ä"))
        {
            return ref new Platform::String(L"ä");
        }
        else if (str->Equals(L"Ö"))
        {
            return ref new Platform::String(L"ö");
        }
        else if (str->Equals(L"Ø"))
        {
            return ref new Platform::String(L"ø");
        }
        else if (str->Equals(L"Æ"))
        {
            return ref new Platform::String(L"æ");
        }
        else if (str->Equals(L"Ü"))
        {
            return ref new Platform::String(L"ü");
        }
        else if (str->Equals(L"ẞ"))
        {
            return ref new Platform::String(L"ß");
        }
        else if (str->Equals(L"Μ"))
        {
            return ref new Platform::String(L"µ");
        }
        else
        {
            return str;
        }
    }
    else
    {
        if (str->Equals(L"å"))
        {
            return ref new Platform::String(L"Å");
        }
        else if (str->Equals(L"ä"))
        {
            return ref new Platform::String(L"Ä");
        }
        else if (str->Equals(L"ö"))
        {
            return ref new Platform::String(L"Ö");
        }
        else if (str->Equals(L"ø"))
        {
            return ref new Platform::String(L"Ø");
        }
        else if (str->Equals(L"æ"))
        {
            return ref new Platform::String(L"Æ");
        }
        else if (str->Equals(L"ü"))
        {
            return ref new Platform::String(L"Ü");
        }
        else if (str->Equals(L"ß"))
        {
            return ref new Platform::String(L"ẞ");
        }
        else if (str->Equals(L"µ"))
        {
            return ref new Platform::String(L"Μ");
        }
        else
        {
            return str;
        }
    }
}

void kryptan_windows::KeyboardPopup::Shift_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (shiftDown && shiftPressedTime > 0 && (clock() - shiftPressedTime) * 1.0 / CLOCKS_PER_SEC < 0.5)
    {
        shiftKeepUp = true;
        ((Button^)sender)->Background = ref new SolidColorBrush(Colors::DarkGray);
    }
    else
    {
        doToggleShift();
    }
    shiftPressedTime = clock();
}

void kryptan_windows::KeyboardPopup::doToggleShift()
{
    static Brush^ oldColor = shiftButton->Background;
    if (shiftDown)
    {
        shiftButton->Background = oldColor;
    }
    else
    {
        Brush^ currentAccentColor = (Brush^)(Application::Current->Resources->Lookup("PhoneAccentBrush"));
        shiftButton->Background = currentAccentColor;
    }

    std::locale loc("");
    const std::ctype<char>& ct = std::use_facet<std::ctype<char> >(loc);
    auto converter = std::bind1st(std::mem_fun(&std::ctype<char>::toupper), &ct);
    if (shiftDown)
        converter = std::bind1st(std::mem_fun(&std::ctype<char>::tolower), &ct);


    //loop through all letter buttons of the special letter grid
    for (unsigned int i = 0; i < specialLetterGrid->Children->Size; i++)
    {
        auto button = (Button^)specialLetterGrid->Children->GetAt(i);
        button->Content = convertSpecialCharToCase((Platform::String^)button->Content, shiftDown);
    }

    //loop through all letter buttons of the main letter grid
    for (unsigned int i = 0; i < mainLetterGrid->Children->Size; i++)
    {
        auto button = (Button^)mainLetterGrid->Children->GetAt(i);
        std::wstring wstr = ((Platform::String^)button->Content)->Data();
        if (button != shiftButton && button != backButton && button != cancelButton && button != spaceButton && button != doneButton)
        {
            std::transform(wstr.begin(), wstr.end(), wstr.begin(), converter);
            button->Content = ref new Platform::String(wstr.data());
        }
    }

    shiftDown = !shiftDown;
    shiftKeepUp = false;
}


void kryptan_windows::KeyboardPopup::Back_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    wchar_t* str = (wchar_t*)currentText.getUnsecureString();
    str[currentText.length() / sizeof(wchar_t)] = 0;
    int nwchars = wcslen(str);
    
    if (nwchars > 0)
    {
        if (nwchars == 1)
        {
            //reset to zero length
            currentText.assign(Caelus::Utilities::SecureString());
        }
        else
        {
            nwchars--;
            currentText.assign((char*)str, nwchars * sizeof(wchar_t), false, true);
        }

        secureText->TextOptions->Text = currentText;
        secureText->DrawText();

        currentTextChanged();
    }

    currentText.UnsecuredStringFinished();
}


void kryptan_windows::KeyboardPopup::Cancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    popupParent->IsOpen = false;
    closeCallback(KeyboardCloseReason::Cancel, initialValue);
}


void kryptan_windows::KeyboardPopup::Space_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    const wchar_t* ptr = L" ";
    currentText.append((char*)ptr, 1 * sizeof(wchar_t), false, true);

    secureText->TextOptions->Text = currentText;
    secureText->DrawText();

    currentTextChanged();
}


void kryptan_windows::KeyboardPopup::Done_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    popupParent->IsOpen = false;
    closeCallback(KeyboardCloseReason::Done, currentText);
}


void kryptan_windows::KeyboardPopup::Paste_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    currentTextChanged();
}


void kryptan_windows::KeyboardPopup::Back_Holding(Platform::Object^ sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs^ e)
{
    currentText = Caelus::Utilities::SecureString();

    secureText->TextOptions->Text = currentText;
    secureText->DrawText();

    currentTextChanged();
}
