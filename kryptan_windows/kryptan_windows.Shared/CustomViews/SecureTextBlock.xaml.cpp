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

Windows::UI::Xaml::DependencyProperty^ SecureTextBlock::_TextOptionsProperty = nullptr;

SecureTextBlock::SecureTextBlock()
{
    InitializeComponent();
}

void SecureTextBlock::RegisterDependencyProperties()
{
    if (_TextOptionsProperty == nullptr)
    {
        _TextOptionsProperty = DependencyProperty::RegisterAttached("TextOptions", SecureTextImageSourceDrawLayout::typeid, SecureTextBlock::typeid, nullptr);
    }
}

void SecureTextBlock::DrawText()
{
    auto t_opt = TextOptions;
    auto a_height = this->ActualHeight;
    auto a_width = this->ActualWidth;
    if (t_opt && this->ActualHeight > 0 && this->ActualWidth > 0)
    {
        if (!t_opt->Text.equals(lastDrawnText))
        {
            if (lastActualHeight != a_height || lastActualWidth != a_width)
            {
                m_ImageSource = ref new SecureTextImageSourceD2D((int)this->ActualWidth, (int)this->ActualHeight);
                Image1->Source = m_ImageSource;

            }
            try{
                m_ImageSource->Draw(t_opt);
            }
            catch (std::exception &e)
            {
                //we recreate the drawing surface and try again.
                m_ImageSource = ref new SecureTextImageSourceD2D((int)this->ActualWidth, (int)this->ActualHeight);
                Image1->Source = m_ImageSource;
                m_ImageSource->Draw(t_opt);
            }
        }
        lastDrawnText = t_opt->Text;
        lastActualHeight = a_height;
        lastActualWidth = a_width;
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


void kryptan_windows::SecureTextBlock::SecureTextBlock_DataContextChanged(Windows::UI::Xaml::FrameworkElement^ sender, Windows::UI::Xaml::DataContextChangedEventArgs^ args)
{
    DrawText();
}
