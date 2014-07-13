//
// SecureTextEdit.xaml.h
// Declaration of the SecureTextEdit class
//

#pragma once


#include "CustomViews\SecureTextEdit.g.h"
#include "SecureTextImageSource\SecureTextImageSourceD2D.h"
#include "SecureTextImageSource\SecureTextImageSourceDrawLayout.h"

namespace kryptan_windows
{
    public ref class SecureStringContainer sealed
    {
    internal:
        SecureStringContainer(Caelus::Utilities::SecureString str)
        {
            this->str = str;
        };

        Caelus::Utilities::SecureString& get(){ return str; };
    private:
        Caelus::Utilities::SecureString str;

        SecureStringContainer(const SecureStringContainer ^obj)
        {
            this->str = obj->str;
        };

        SecureStringContainer(){};
    };

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SecureTextEdit sealed
    {
    public:
        SecureTextEdit();
        void DrawText();

        property SecureTextImageSourceDrawLayout^ TextOptions
        {
            SecureTextImageSourceDrawLayout^ get(){ return m_TextOptions; }
        }

        SecureStringContainer^ getSecurePasswordContainer();

        property bool IsPassword
        {
            bool get()
            {
                return (Platform::Boolean)this->GetValue(IsPasswordProperty);
            }
            void set(bool value)
            {
                SetValue(IsPasswordProperty, value);
            }
        }

        property Platform::String^ Hint
        {
            Platform::String^ get()
            {
                return (Platform::String^)this->GetValue(HintProperty);
            }
            void set(Platform::String^ value)
            {
                SetValue(HintProperty, value);
            }
        }

        property bool Editable
        {
            bool get()
            {
                return (Platform::Boolean)this->GetValue(EditableProperty);
            }
            void set(bool value)
            {
                SetValue(EditableProperty, value);
            }
        }

        static void RegisterDependencyProperties();

    internal:
        Caelus::Utilities::SecureString getSecurePassword();

    private:

        Caelus::Utilities::SecureString b_text;

        SecureTextImageSourceD2D^ m_ImageSource;

        SecureTextImageSourceDrawLayout^ m_TextOptions;
        void UserControl_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
        void Control_Clicked(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);

        static property Windows::UI::Xaml::DependencyProperty^ IsPasswordProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get()
            {
                return _IsPasswordProperty;
            }
        }
        static property Windows::UI::Xaml::DependencyProperty^ HintProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get()
            {
                return _HintProperty;
            }
        }
        static property Windows::UI::Xaml::DependencyProperty^ EditableProperty
        {
            Windows::UI::Xaml::DependencyProperty^ get()
            {
                return _EditableProperty;
            }
        }

        static Windows::UI::Xaml::DependencyProperty^ _IsPasswordProperty;
        static Windows::UI::Xaml::DependencyProperty^ _HintProperty;
        static Windows::UI::Xaml::DependencyProperty^ _EditableProperty;
        void SecureTextEdit_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void SecureTextEdit_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
    };
}
