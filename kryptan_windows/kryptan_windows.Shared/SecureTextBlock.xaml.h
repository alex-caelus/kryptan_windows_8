//
// SecureTextBlock.xaml.h
// Declaration of the SecureTextBlock class
//

#pragma once

#include "SecureTextBlock.g.h"

namespace kryptan_windows
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SecureTextBlock sealed
	{
	public:
		SecureTextBlock();

        void SetSecureString(Kryptan::Core::SecureString text);
        Kryptan::Core::SecureString GetSecureString();

    private:

        void UpdateImageSource();

	};
}
