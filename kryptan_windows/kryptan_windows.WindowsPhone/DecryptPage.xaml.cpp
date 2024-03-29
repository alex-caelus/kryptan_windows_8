﻿//
// DecryptPage.xaml.cpp
// Implementation of the DecryptPage class
//

#include "pch.h"
#include "DecryptPage.xaml.h"
//#include "Utilities\SecureStringHandler.h"
#include "Libs\kryptan_core\core.h"
#include "CustomViews\KeyboardPopup.xaml.h"
#include "MainHub.xaml.h"

using namespace kryptan_windows;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556

DecryptPage::DecryptPage()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Platform::Collections::Map<String^, Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &DecryptPage::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &DecryptPage::SaveState);
    auto options = ref new SecureTextImageSourceDrawLayout();
    options->Text = SecureString("This is a test");
    options->BackroundColor = Windows::UI::Colors::Transparent;
    options->TextColor = Windows::UI::Colors::White;
    options->FontName = L"Verdana";
    options->FontSize = 16;
    options->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
    options->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
    test->TextOptions = options;
}

DependencyProperty^ DecryptPage::_defaultViewModelProperty =
DependencyProperty::Register("DefaultViewModel",
TypeName(IObservableMap<String^, Object^>::typeid), TypeName(DecryptPage::typeid), nullptr);

/// <summary>
/// Used as a trivial view model.
/// </summary>
IObservableMap<String^, Object^>^ DecryptPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ DecryptPage::_navigationHelperProperty =
DependencyProperty::Register("NavigationHelper",
TypeName(Common::NavigationHelper::typeid), TypeName(DecryptPage::typeid), nullptr);

/// <summary>
/// Gets an implementation of <see cref="NavigationHelper"/> designed to be
/// used as a trivial view model.
/// </summary>
Common::NavigationHelper^ DecryptPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// The methods provided in this section are simply used to allow
/// NavigationHelper to respond to the page's navigation methods.
/// 
/// Page specific logic should be placed in event handlers for the  
/// <see cref="NavigationHelper::LoadState"/>
/// and <see cref="NavigationHelper::SaveState"/>.
/// The navigation parameter is available in the LoadState method 
/// in addition to page state preserved during an earlier session.

void DecryptPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

/// <summary>
/// Called when navigated from.
/// </summary>
/// <param name="e">The e.</param>
void DecryptPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// Populates the page with content passed during navigation. Any saved state is also
/// provided when recreating a page from a prior session.
/// </summary>
/// <param name="sender">
/// The source of the event; typically <see cref="NavigationHelper"/>
/// </param>
/// <param name="e">Event data that provides both the navigation parameter passed to
/// <see cref="Frame::Navigate(Type, Object)"/> when this page was initially requested and
/// a dictionary of state preserved by this page during an earlier
/// session. The state will be null the first time a page is visited.</param>
void DecryptPage::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter
}

/// <summary>
/// Preserves state associated with this page in case the application is suspended or the
/// page is discarded from the navigation cache.  Values must conform to the serialization
/// requirements of <see cref="SuspensionManager::SessionState"/>.
/// </summary>
/// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
/// <param name="e">Event data that provides an empty dictionary to be populated with
/// serializable state.</param>
void DecryptPage::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// Unused parameter
	(void) e; // Unused parameter
}


void kryptan_windows::DecryptPage::goButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto statusbar = Windows::UI::ViewManagement::StatusBar::GetForCurrentView();
    statusbar->ProgressIndicator->ShowAsync();

    pageModel.decryptButtonClicked(masterkeyBox->getSecureStringContainer()).then([this, statusbar](DecryptModel::DecryptResult result) {
            resultTextBlock->Text = result.statusString;

            switch (result.status)
            {
            case DecryptModel::DecryptResult::FAILED:
                resultTextBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::DarkRed);
                masterkeyBox->clearSecureString();
                break;
            case DecryptModel::DecryptResult::CONFIRM:
                resultTextBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::DarkGreen);
                masterkeyBox->clearSecureString();
                break;
            case DecryptModel::DecryptResult::SUCCESS:
                resultTextBlock->Foreground = ref new SolidColorBrush(Windows::UI::Colors::DarkGreen);
                this->Frame->Navigate(MainHub::typeid);
                break;
            default:
                throw ref new Exception(-1, L"Impossible error: default DecryptResult enumeration.");
                break;
            }

            statusbar->ProgressIndicator->HideAsync();
        }, task_continuation_context::use_current());
}
