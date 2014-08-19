//
// SecureTextBlock.xaml.cpp
// Implementation of the SecureTextBlock class
//

#include "pch.h"
#include "SecureTextBlock.xaml.h"
#include "SecureTextImageSource\SecureTextBitmapFactory.h"

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
//std::map<std::pair<int, int>, SecureTextImageSourceD2D^ > SecureTextBlock::_ImageSourceList;

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
    //auto t_opt = TextOptions;
    //auto a_height = 10;
    //auto a_width = 10;
    //if (t_opt && a_height > 0 && a_width > 0)
    //{
    //    if (!t_opt->Text.equals(lastDrawnText))
    //    {
    //        m_ImageSource = getImageSource((int)a_width, (int)a_height, false);
    //        Image1->Source = m_ImageSource;
    //        try{
    //            //m_ImageSource->Draw(t_opt);
    //        }
    //        catch (std::exception &)
    //        {
    //            //we recreate the drawing surface and try again.
    //            m_ImageSource = getImageSource((int)a_width, (int)a_height, true);
    //            Image1->Source = m_ImageSource;
    //            //m_ImageSource->Draw(t_opt);
    //        }
    //    }
    //    lastDrawnText = t_opt->Text;
    //    lastActualHeight = a_height;
    //    lastActualWidth = a_width;
    //}

    SecureTextBitmapFactory::RenderBitmap((int)this->ActualHeight, (int)this->ActualWidth, TextOptions)
        .then([this](Windows::UI::Xaml::Media::ImageSource^ image)
        {
            this->Image1->Source = image;
            this->Image1->Stretch = Windows::UI::Xaml::Media::Stretch::None;
        }, 
        concurrency::task_continuation_context::use_current());
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

SecureTextImageSourceD2D^ SecureTextBlock::getImageSource(int height, int width, bool recreate)
{
    if (!recreate && m_secureimagesource != nullptr)
    {
        return m_secureimagesource;
    }
    return ref new SecureTextImageSourceD2D(height, width);
}

//SecureTextImageSourceD2D^ SecureTextBlock::getImageSource(int height, int width, bool recreate)
//{
//    std::pair<int, int> key(height, width);
//    if (!recreate)
//    {
//        auto it = _ImageSourceList.find(key);
//        if (it != _ImageSourceList.end())
//        {
//            return it->second;
//        }
//    }
//    auto newSource = ref new SecureTextImageSourceD2D(height, width);
//
//    _ImageSourceList[key] = newSource;
//
//    return newSource;
//}