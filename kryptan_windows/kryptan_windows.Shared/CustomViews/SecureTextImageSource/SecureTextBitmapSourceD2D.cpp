/****************************** Module Header ******************************\
* Module Name:    SecureTextBitmapSourceD2D.cpp
* Project:        CppWindowsStoreAppSaveSurfaceImageSource
* Copyright (c) Microsoft Corporation
*
* This class derives from SurfaceImageSource class. It extends SurfaceImagSource
* to save the content to image file.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL.
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\*****************************************************************************/
#include "pch.h" 
#include "SecureTextBitmapSourceD2D.h" 
#include "DirectXSample.h"

#include "Utilities\EncodingHandler.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace concurrency;

using namespace kryptan_windows;

SecureTextBitmapSourceD2D::SecureTextBitmapSourceD2D(UINT pixelHeight, UINT pixelWidth) :
SurfaceImageSource(pixelWidth, pixelHeight, false)
{
    m_width = pixelWidth;
    m_height = pixelHeight;
    previousDrawArgs = nullptr;

    CreateDeviceIndependentResources();
    CreateDeviceResources();
}

// Initialize resources that are independent of hardware. 
void SecureTextBitmapSourceD2D::CreateDeviceIndependentResources()
{
    // Query for ISurfaceImageSourceNative interface. 
    DX::ThrowIfFailed(
        reinterpret_cast<IUnknown*>(this)->QueryInterface(IID_PPV_ARGS(&m_sisNative))
        );

    D2D1_FACTORY_OPTIONS options;

#if defined(_DEBUG)
    // If the project is in a debug build, enable Direct2D debugging via Direct2D SDK layer.
    // Enabling SDK debug layer can help catch coding mistakes such as invalid calls and
    // resource leaking that needs to be fixed during the development cycle.
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    DX::ThrowIfFailed(
        D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory1),
        &options,
        &m_d2dFactory
        )
        );

    DX::ThrowIfFailed(
        CoCreateInstance(
        CLSID_WICImagingFactory, nullptr,
        CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory)
        )
        );

    // Create a DirectWrite factory.
    DX::ThrowIfFailed(
        DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_d2dWriteFactory),
        &m_d2dWriteFactory
        )
        );
}

// Initialize hardware-dependent resources. 
void SecureTextBitmapSourceD2D::CreateDeviceResources()
{
    // This flag adds support for surfaces with a different color channel ordering 
    // than the API default. It is required for compatibility with Direct2D. 
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)     
    // If the project is in a debug build, enable debugging via SDK Layers. 
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

    // This array defines the set of DirectX hardware feature levels this app will support. 
    // Note the ordering should be preserved. 
    // Don't forget to declare your application's minimum required feature level in its 
    // description.  All applications are assumed to support 9.1 unless otherwise stated. 
    const D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the Direct3D 11 API device object. 
    DX::ThrowIfFailed(
        D3D11CreateDevice(
        nullptr,                        // Specify nullptr to use the default adapter. 
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,                  // Set debug and Direct2D compatibility flags. 
        featureLevels,                  // List of feature levels this app can support. 
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,              // Always set this to D3D11_SDK_VERSION for Metro style apps. 
        &m_d3dDevice,                   // Returns the Direct3D device created. 
        nullptr,
        nullptr
        )
        );

    // Get the Direct3D 11.1 API device. 
    ComPtr<IDXGIDevice> dxgiDevice;
    DX::ThrowIfFailed(
        m_d3dDevice.As(&dxgiDevice)
        );

    // Create the Direct2D device object and a corresponding context. 
    DX::ThrowIfFailed(
        D2D1CreateDevice(
        dxgiDevice.Get(),
        nullptr,
        &m_d2dDevice
        )
        );

    DX::ThrowIfFailed(
        m_d2dDevice->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        &m_d2dContext
        )
        );

    // Set DPI to the display's current DPI. 
    SetDpi(Windows::Graphics::Display::DisplayProperties::LogicalDpi);

    // Associate the DXGI device with the SurfaceImageSource. 
    DX::ThrowIfFailed(
        m_sisNative->SetDevice(dxgiDevice.Get())
        );
}

// Sets the current DPI. 
void SecureTextBitmapSourceD2D::SetDpi(float dpi)
{
    // Update Direct2D's stored DPI. 
    m_d2dContext->SetDpi(dpi, dpi);
}

// Begins drawing, allowing updates to content in the specified area. 
void SecureTextBitmapSourceD2D::BeginDraw(Windows::Foundation::Rect updateRect)
{
    POINT offset;
    ComPtr<IDXGISurface> surface;

    // Express target area as a native RECT type. 
    RECT updateRectNative;
    updateRectNative.left = static_cast<LONG>(updateRect.Left);
    updateRectNative.top = static_cast<LONG>(updateRect.Top);
    updateRectNative.right = static_cast<LONG>(updateRect.Right);
    updateRectNative.bottom = static_cast<LONG>(updateRect.Bottom);

    // Begin drawing - returns a target surface and an offset to use as the top left origin when drawing. 
    HRESULT beginDrawHR = m_sisNative->BeginDraw(updateRectNative, &surface, &offset);

    if (beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET)
    {
        // If the device has been removed or reset, attempt to recreate it and continue drawing. 
        CreateDeviceResources();
        BeginDraw(updateRect);
    }
    else
    {
        // Notify the caller by throwing an exception if any other error was encountered. 
        DX::ThrowIfFailed(beginDrawHR);
    }

    // Create render target. 
    ComPtr<ID2D1Bitmap1> bitmap;
    DX::ThrowIfFailed(
        m_d2dContext->CreateBitmapFromDxgiSurface(
        surface.Get(),
        nullptr,
        &bitmap
        )
        );

    // Set context's render target. 
    m_d2dContext->SetTarget(bitmap.Get());

    // Begin drawing using D2D context. 
    m_d2dContext->BeginDraw();

    // Apply a clip and transform to constrain updates to the target update area. 
    // This is required to ensure coordinates within the target surface remain 
    // consistent by taking into account the offset returned by BeginDraw, and 
    // can also improve performance by optimizing the area that is drawn by D2D. 
    // Apps should always account for the offset output parameter returned by  
    // BeginDraw, since it may not match the passed updateRect input parameter's location. 
    m_d2dContext->PushAxisAlignedClip(
        D2D1::RectF(
        static_cast<float>(offset.x),
        static_cast<float>(offset.y),
        static_cast<float>(offset.x + updateRect.Width),
        static_cast<float>(offset.y + updateRect.Height)
        ),
        D2D1_ANTIALIAS_MODE_ALIASED
        );

    m_d2dContext->SetTransform(
        D2D1::Matrix3x2F::Translation(
        static_cast<float>(offset.x),
        static_cast<float>(offset.y)
        )
        );

    // Create wic bitmap
    WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;

    DX::ThrowIfFailed(
        m_wicFactory->CreateBitmap(static_cast<UINT>(updateRect.Width), static_cast<UINT>(updateRect.Height),
        format, WICBitmapCacheOnLoad, &m_wicBitmap)
        );

    // Create wic render target
    D2D1_RENDER_TARGET_PROPERTIES RTProps = D2D1::RenderTargetProperties(
        );
    RTProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    DX::ThrowIfFailed(
        m_d2dFactory->CreateWicBitmapRenderTarget(m_wicBitmap.Get(), &RTProps, &m_wicBitmapRenderTarget)
        );

    // Begin drawing using WIC Bitmap RenderTarget.
    m_wicBitmapRenderTarget->BeginDraw();
}

// Ends drawing updates started by a previous BeginDraw call. 
void SecureTextBitmapSourceD2D::EndDraw()
{
    DX::ThrowIfFailed(
        m_wicBitmapRenderTarget->EndDraw()
        );

    // Get D2D bitmap from wic bitmap
    DX::ThrowIfFailed(
        m_d2dContext->CreateBitmapFromWicBitmap(m_wicBitmap.Get(), &m_d2dBitmap)
        );

    // Draw D2D bitmap on DXGI surface.
    m_d2dContext->DrawBitmap(m_d2dBitmap.Get(), NULL, 1.0, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC, NULL);

    // Remove the transform and clip applied in BeginDraw since 
    // the target area can change on every update. 
    m_d2dContext->SetTransform(D2D1::IdentityMatrix());
    m_d2dContext->PopAxisAlignedClip();

    // Remove the render target and end drawing. 
    DX::ThrowIfFailed(
        m_d2dContext->EndDraw()
        );

    m_d2dContext->SetTarget(nullptr);

    DX::ThrowIfFailed(
        m_sisNative->EndDraw()
        );
}

// Clears the background with the given color. 
void SecureTextBitmapSourceD2D::Clear(Windows::UI::Color color)
{
    m_wicBitmapRenderTarget->Clear(DX::ConvertToColorF(color));
}

// Draws a filled rectangle with the given color and position. 
// NOTE: For saving the content of SurfaceImageSource to image file, we first create an
// off-screen WIC bitmap, then draw to this WIC Bitmap, and then we draw the wic bitmap to d2d surface.
void SecureTextBitmapSourceD2D::FillSolidRect(Windows::UI::Color color, Windows::Foundation::Rect rect)
{
    // Create a solid color D2D brush. 
    ComPtr<ID2D1SolidColorBrush> brush;
    DX::ThrowIfFailed(
        m_wicBitmapRenderTarget->CreateSolidColorBrush(
        DX::ConvertToColorF(color),
        &brush
        )
        );

    // Draw a filled rectangle. 
    m_wicBitmapRenderTarget->FillRectangle(DX::ConvertToRectF(rect), brush.Get());
}


// Save the content of SurfaceImageSource to an image file.
void SecureTextBitmapSourceD2D::SaveSurfaceImageToFile()
{
    // Prepare a file picker for customers to input image file name.
    Pickers::FileSavePicker^ savePicker = ref new Pickers::FileSavePicker();
    auto pngExtensions = ref new Platform::Collections::Vector<Platform::String^>();
    pngExtensions->Append(".png");
    savePicker->FileTypeChoices->Insert("PNG file", pngExtensions);
    auto jpgExtensions = ref new Platform::Collections::Vector<Platform::String^>();
    jpgExtensions->Append(".jpg");
    savePicker->FileTypeChoices->Insert("JPEG file", jpgExtensions);
    auto bmpExtensions = ref new Platform::Collections::Vector<Platform::String^>();
    bmpExtensions->Append(".bmp");
    savePicker->FileTypeChoices->Insert("BMP file", bmpExtensions);
    savePicker->DefaultFileExtension = ".png";
    savePicker->SuggestedFileName = "image";
    savePicker->SuggestedStartLocation = Pickers::PickerLocationId::PicturesLibrary;

    std::shared_ptr<GUID> wicFormat = std::make_shared<GUID>(GUID_ContainerFormatBmp);

    create_task(savePicker->PickSaveFileAsync()).then([=](StorageFile^ file)
    {
        if (file == nullptr)
        {
            // If user clicks "Cancel", reset the saving state, then cancel the current task.
            //m_screenSavingState = ScreenSavingState::NotSaved;
            cancel_current_task();
        }

        if (file->FileType == ".bmp")
        {
            *wicFormat = GUID_ContainerFormatBmp;
        }
        else if (file->FileType == ".jpg")
        {
            *wicFormat = GUID_ContainerFormatJpeg;
        }
        return file->OpenAsync(FileAccessMode::ReadWrite);

    }).then([=](Streams::IRandomAccessStream^ randomAccessStream)
    {
        // Convert the RandomAccessStream to an IStream.
        ComPtr<IStream> stream;
        DX::ThrowIfFailed(
            CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(&stream))
            );

        SaveBitmapToStream(m_wicBitmap, m_wicFactory, *wicFormat, stream.Get());
    });
}

// Save WIC bitmap to a stream.
void SecureTextBitmapSourceD2D::SaveBitmapToStream(
    _In_ ComPtr<IWICBitmap> wicBitmap,
    _In_ ComPtr<IWICImagingFactory2> wicFactory2,
    _In_ REFGUID wicFormat,
    _In_ IStream* stream
    )
{
    // Create and initialize WIC Bitmap Encoder.
    ComPtr<IWICBitmapEncoder> wicBitmapEncoder;
    DX::ThrowIfFailed(
        wicFactory2->CreateEncoder(
        wicFormat,
        nullptr,    // No preferred codec vendor.
        &wicBitmapEncoder
        )
        );

    DX::ThrowIfFailed(
        wicBitmapEncoder->Initialize(
        stream,
        WICBitmapEncoderNoCache
        )
        );

    // Create and initialize WIC Frame Encoder.
    ComPtr<IWICBitmapFrameEncode> wicFrameEncode;
    DX::ThrowIfFailed(
        wicBitmapEncoder->CreateNewFrame(
        &wicFrameEncode,
        nullptr     // No encoder options.
        )
        );

    DX::ThrowIfFailed(
        wicFrameEncode->Initialize(nullptr)
        );

    WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;

    DX::ThrowIfFailed(
        wicFrameEncode->SetPixelFormat(&format)
        );

    DX::ThrowIfFailed(
        wicFrameEncode->WriteSource(wicBitmap.Get(), nullptr)
        );

    DX::ThrowIfFailed(
        wicFrameEncode->Commit()
        );

    DX::ThrowIfFailed(
        wicBitmapEncoder->Commit()
        );

    // Flush all memory buffers to the next-level storage object.
    DX::ThrowIfFailed(
        stream->Commit(STGC_DEFAULT)
        );
}

Streams::IRandomAccessStream^ SecureTextBitmapSourceD2D::Draw(SecureTextImageSourceDrawLayout^ args)
{
    if (m_width && m_height && args)
    {
        BeginDraw(Windows::Foundation::Rect(0, 0, (float)m_width, (float)m_height));

        Clear(args->BackroundColor);

        if (!args->Equals(previousDrawArgs, false))
        {
            CreateTextFormatResource(args->FontName->Data(), args->FontSize, args->TextAlignHorizontal, args->TextAlignVertical);
        }

        ComPtr<ID2D1SolidColorBrush> brush;
        DX::ThrowIfFailed(
            m_wicBitmapRenderTarget->CreateSolidColorBrush(
            DX::ConvertToColorF(args->TextColor),
            &brush
            )
            );

        D2D_RECT_F rect = D2D1::RectF(0, 0, (float)m_width, (float)m_height);

        auto w = EncodingHandler::makeWideString(args->Text.getUnsecureString());
        args->Text.UnsecuredStringFinished();
        int wlen = wcslen(w);

        m_wicBitmapRenderTarget->DrawText(w, wlen, m_d2dWriteTextFormat.Get(), &rect, brush.Get());

        memset(w, 0, wlen);
        delete[] w;

        EndDraw();

        previousDrawArgs = args;

        //format
        std::shared_ptr<GUID> wicFormat = std::make_shared<GUID>(GUID_ContainerFormatPng);

        //create stream
        Streams::InMemoryRandomAccessStream^ randomAccessStream = ref new Streams::InMemoryRandomAccessStream();

        // Convert the RandomAccessStream to an IStream.
        ComPtr<IStream> stream;
        DX::ThrowIfFailed(
            CreateStreamOverRandomAccessStream(randomAccessStream, IID_PPV_ARGS(&stream))
            );

        SaveBitmapToStream(m_wicBitmap, m_wicFactory, *wicFormat, stream.Get());

        return randomAccessStream;
    }
    else
    {
        return nullptr;
    }
}


void SecureTextBitmapSourceD2D::CreateTextFormatResource(const WCHAR msc_fontName[], const FLOAT msc_fontSize, SecureTextHorizontalAlign hAlign, SecureTextVerticalAlign vAlign)
{
    // Create a DirectWrite text format object.
    DX::ThrowIfFailed(
        m_d2dWriteFactory->CreateTextFormat(
        msc_fontName,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        msc_fontSize,
        L"", //locale
        &m_d2dWriteTextFormat
        )
        );

    DWRITE_TEXT_ALIGNMENT dhAlign;
    DWRITE_PARAGRAPH_ALIGNMENT dvAlign;

    switch (hAlign)
    {
    case SecureTextHorizontalAlign::LEFT:
        dhAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
        break;
    case SecureTextHorizontalAlign::CENTER:
        dhAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
        break;
    case SecureTextHorizontalAlign::RIGHT:
        dhAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
        break;
    }
    switch (vAlign)
    {
    case SecureTextVerticalAlign::TOP:
        dvAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
        break;
    case SecureTextVerticalAlign::MIDDLE:
        dvAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
        break;
    case SecureTextVerticalAlign::BOTTOM:
        dvAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
        break;
    }


    // Center the text horizontally and vertically.
    DX::ThrowIfFailed(
        m_d2dWriteTextFormat->SetTextAlignment(dhAlign)
        );

    DX::ThrowIfFailed(
        m_d2dWriteTextFormat->SetParagraphAlignment(dvAlign)
        );

}
