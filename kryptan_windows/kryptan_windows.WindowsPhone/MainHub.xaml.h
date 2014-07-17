//
// MainHub.xaml.h
// Declaration of the MainHub class
//

#pragma once

#include "MainHub.g.h"

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
	};
}
