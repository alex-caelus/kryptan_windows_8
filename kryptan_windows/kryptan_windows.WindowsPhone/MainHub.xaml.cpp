//
// MainHub.xaml.cpp
// Implementation of the MainHub class
//

#include "pch.h"
#include "MainHub.xaml.h"
#include "CustomViews\SecureTextBlock.xaml.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=390556

MainHub::MainHub()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.
/// This parameter is typically used to configure the page.</param>
void MainHub::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}


void kryptan_windows::MainHub::LabelList_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    //StackPanel^ list = (StackPanel^)sender;
    //
    //labels = pageModel.GetLabels();

    //for (unsigned int i = 0; i < labels->Size; i++)
    //{
    //    SecureTextBlock^ label = labels->GetAt(i);
    //    label->Height = 30;
    //    label->TextOptions->TextAlignHorizontal = SecureTextHorizontalAlign::LEFT;
    //    label->TextOptions->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    //    label->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
    //    label->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;

    //    auto check = ref new CheckBox();
    //    check->Content = label;
    //    check->Tapped += ref new TappedEventHandler(this, &MainHub::LabelTapped);
    //    check->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
    //    check->HorizontalContentAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;

    //    list->Children->Append(check);
    //}
    ListView^ list = (ListView^)sender;

    labels = pageModel.GetLabels();

    for (unsigned int i = 0; i < labels->Size; i++)
    {
        auto pwd = labels->GetAt(i);
        pwd->TextAlignHorizontal = SecureTextHorizontalAlign::LEFT;
        pwd->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    }

    list->ItemsSource = labels;
}

void kryptan_windows::MainHub::PasswordList_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    ListView^ list = (ListView^)sender;

    pwds = pageModel.GetPasswords();

    for (unsigned int i = 0; i < pwds->Size; i++)
    {
        auto pwd = pwds->GetAt(i);
        pwd->TextAlignHorizontal = SecureTextHorizontalAlign::LEFT;
        pwd->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    }

    //list->ItemsSource = pwds;
}


void kryptan_windows::MainHub::Hub_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
    //if (labels != nullptr)
    //{
    //    for (int i = 0; i < labels->Size; i++)
    //    {
    //        labels->GetAt(i)->DrawText();
    //    }
    //}
}


void kryptan_windows::MainHub::LabelList_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
    //if (labels != nullptr)
    //{
    //    for (int i = 0; i < labels->Size; i++)
    //    {
    //        labels->GetAt(i)->DrawText();
    //    }
    //}
}

void kryptan_windows::MainHub::LabelTapped(Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
}


void kryptan_windows::MainHub::PasswordList_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{

}
