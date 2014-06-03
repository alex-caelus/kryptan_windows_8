#pragma once

#include "pch.h"

#include "SecureTextImageSourceDrawLayout.h"

namespace kryptan_windows
{

    public ref class SecureTextImageSourceD2D sealed : public Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
    {
    public:
        SecureTextImageSourceD2D(int width, int height);
        virtual ~SecureTextImageSourceD2D();

        void Draw(SecureTextImageSourceDrawLayout^ args);

    protected:
        void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^e);
        void CreateDeviceResources();

    private:

        void BeginDraw(Windows::Foundation::Rect updateRect);
        void EndDraw();
        void Clear(Windows::UI::Color color);

        // Direct3D device
        Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;

        // Direct2D objects
        Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext;

        static void CreateDeviceIndependentResources();

        //Device independent resoures
        static bool m_deviceIndependentResourcesCreated;
        static Microsoft::WRL::ComPtr<ID2D1Factory> m_d2dFactory;
        static Microsoft::WRL::ComPtr<IDWriteFactory> m_d2dWriteFactory;

        void CreateTextFormatResource(const WCHAR msc_fontName[], const FLOAT msc_fontSize, SecureTextHorizontalAlign hAlign, SecureTextVerticalAlign vAlign);
        Microsoft::WRL::ComPtr<IDWriteTextFormat> m_d2dWriteTextFormat;

        SecureTextImageSourceDrawLayout^ previousDrawArgs;

        int m_width;
        int m_height;
    };

}