#include "pch.h"
#include "MainHubModel.h"

using namespace kryptan_windows;
using namespace Platform::Collections;

MainHubModel::MainHubModel()
{
}


MainHubModel::~MainHubModel()
{
}

Windows::Foundation::Collections::IObservableVector<SecureTextImageSourceDrawLayout^>^ MainHubModel::GetLabels()
{
    auto list = ref new Platform::Collections::Vector<SecureTextImageSourceDrawLayout^>();

    for (auto label : this->pwdFile->GetPasswordList()->AllLabels())
    {
        auto labelControl = ref new SecureTextImageSourceDrawLayout();
        labelControl->Text = label;
        labelControl->BackroundColor = Windows::UI::Colors::Transparent;
        labelControl->TextColor = Windows::UI::Colors::White;
        labelControl->FontName = L"Verdana";
        labelControl->FontSize = 16;
        labelControl->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
        labelControl->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
        list->Append(labelControl);
    }

    return list;
}

Windows::Foundation::Collections::IObservableVector<SecureTextImageSourceDrawLayout^>^ MainHubModel::GetPasswords()
{
    auto list = ref new Platform::Collections::Vector<SecureTextImageSourceDrawLayout^>();

    for (auto pwd : this->pwdFile->GetPasswordList()->All())
    {
        auto pwdControl = ref new SecureTextImageSourceDrawLayout();
        pwdControl->Text = pwd->GetDescription();
        pwdControl->BackroundColor = Windows::UI::Colors::Transparent;
        pwdControl->TextColor = Windows::UI::Colors::White;
        pwdControl->FontName = L"Verdana";
        pwdControl->FontSize = 16;
        pwdControl->TextAlignHorizontal = SecureTextHorizontalAlign::CENTER;
        pwdControl->TextAlignVertical = SecureTextVerticalAlign::MIDDLE;
        list->Append(pwdControl);
    }

    return list;
}