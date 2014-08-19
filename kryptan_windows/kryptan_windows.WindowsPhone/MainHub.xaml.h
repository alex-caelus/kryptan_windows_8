//
// MainHub.xaml.h
// Declaration of the MainHub class
//

#pragma once

#include "MainHub.g.h"

#include "PageModels\MainHubModel.h"

namespace kryptan_windows
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainHub sealed
	{
	public:
		MainHub();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
    private:
        void LabelList_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

        MainHubModel pageModel;

        Windows::Foundation::Collections::IObservableVector<SecureTextImageSourceDrawLayout^>^ labels;
        Windows::Foundation::Collections::IObservableVector<SecureTextImageSourceDrawLayout^>^ pwds;

        void Hub_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
        void LabelList_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

        void LabelTapped(Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void PasswordList_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void PasswordList_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
        void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    };
}
