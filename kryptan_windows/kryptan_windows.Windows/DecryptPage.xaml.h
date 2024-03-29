﻿//
// DecryptPage.xaml.h
// Declaration of the DecryptPage class
//

#pragma once

#include "DecryptPage.g.h"
#include "Common\NavigationHelper.h"
#include "PageModels\DecryptModel.h"
#include "CustomViews\SecureTextBlock.xaml.h"

namespace kryptan_windows
{
	/// <summary>
	/// A basic page that provides characteristics common to most applications.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DecryptPage sealed
	{
	public:
		DecryptPage();

		/// <summary>
		/// This can be changed to a strongly typed view model.
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}

		/// <summary>
		/// NavigationHelper is used on each page to aid in navigation and 
		/// process lifetime management
		/// </summary>
		property Common::NavigationHelper^ NavigationHelper
		{
			Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e);
		void SaveState(Platform::Object^ sender, Common::SaveStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
        void goButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        DecryptModel pageModel;
    };
}
