#pragma once

#include "pch.h"

#include "SecureTextImageSourceDrawLayout.h"

using namespace Microsoft::WRL;

namespace kryptan_windows
{
    public ref class SecureTextBitmapSourceD2D sealed : Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
    {
    public:
        SecureTextBitmapSourceD2D(UINT pixelHeight, UINT pixelWidth);

        Windows::Storage::Streams::IRandomAccessStream^ Draw(SecureTextImageSourceDrawLayout^ args);

    protected:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

    private:

        void BeginDraw(Windows::Foundation::Rect updateRect);
        void BeginDraw()    { BeginDraw(Windows::Foundation::Rect(0, 0, (float)m_width, (float)m_height)); }
        void EndDraw();

        void SetDpi(float dpi);

        void Clear(Windows::UI::Color color);
        void FillSolidRect(Windows::UI::Color color, Windows::Foundation::Rect rect);
        void SaveSurfaceImageToFile();

        void SaveBitmapToStream(
            _In_ ComPtr<IWICBitmap> wicBitmap,
            _In_ ComPtr<IWICImagingFactory2> wicFactory2,
            _In_ REFGUID wicFormat,
            _In_ IStream* stream
            );

        ComPtr<IDWriteTextFormat> m_d2dWriteTextFormat;
        void CreateTextFormatResource(const WCHAR msc_fontName[], const FLOAT msc_fontSize, SecureTextHorizontalAlign hAlign, SecureTextVerticalAlign vAlign);

        ComPtr<ISurfaceImageSourceNative>   m_sisNative;

        // Direct3D device 
        ComPtr<ID3D11Device>                m_d3dDevice;

        // Direct2D objects 
        ComPtr<ID2D1Device>                 m_d2dDevice;
        ComPtr<ID2D1DeviceContext>          m_d2dContext;

        ComPtr<ID2D1Factory1>								m_d2dFactory;
        ComPtr<IWICImagingFactory2>							m_wicFactory;
        ComPtr<IWICBitmap>                                  m_wicBitmap;
        ComPtr<ID2D1RenderTarget>							m_wicBitmapRenderTarget;
        ComPtr<ID2D1Bitmap1>								m_d2dBitmap;
        UINT                                                m_width;
        UINT                                                m_height;
        ComPtr<IDWriteFactory>                              m_d2dWriteFactory;

        SecureTextImageSourceDrawLayout^ previousDrawArgs;
    };
}


