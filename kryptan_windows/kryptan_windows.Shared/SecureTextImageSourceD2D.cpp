#include "pch.h"
#include "SecureTextImageSourceD2D.h"

#include "DirectXSample.h"

using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::Graphics::Display;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;

using namespace Caelus::Utilities;

using namespace kryptan_windows;

bool SecureTextImageSourceD2D::m_deviceIndependentResourcesCreated = false;
Microsoft::WRL::ComPtr<ID2D1Factory> SecureTextImageSourceD2D::m_d2dFactory;
Microsoft::WRL::ComPtr<IDWriteFactory> SecureTextImageSourceD2D::m_d2dWriteFactory;

SecureTextImageSourceD2D::SecureTextImageSourceD2D(int width, int height)
    : SurfaceImageSource(width, height, false)
{
    m_height = height;
    m_width = width;
    previousDrawArgs = nullptr;

    CreateDeviceIndependentResources();

    CreateDeviceResources();

    Application::Current->Suspending += ref new SuspendingEventHandler(this, &SecureTextImageSourceD2D::OnSuspending);
}


SecureTextImageSourceD2D::~SecureTextImageSourceD2D()
{
}

void SecureTextImageSourceD2D::CreateDeviceResources()
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
        D3D11_SDK_VERSION,              // Always set this to D3D11_SDK_VERSION for Windows Store apps.
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

    // Query for ISurfaceImageSourceNative interface.
    Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
    DX::ThrowIfFailed(
        reinterpret_cast<IUnknown*>(this)->QueryInterface(IID_PPV_ARGS(&sisNative))
        );

    // Associate the DXGI device with the SurfaceImageSource.
    DX::ThrowIfFailed(
        sisNative->SetDevice(dxgiDevice.Get())
        );
}

void SecureTextImageSourceD2D::CreateDeviceIndependentResources()
{
    if (!m_deviceIndependentResourcesCreated)
    {
        m_deviceIndependentResourcesCreated = true;
        // Create a Direct2D factory.
        DX::ThrowIfFailed(
            D2D1CreateFactory<ID2D1Factory>(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory
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
}

void SecureTextImageSourceD2D::CreateTextFormatResource(const WCHAR msc_fontName[], const FLOAT msc_fontSize, SecureTextHorizontalAlign hAlign, SecureTextVerticalAlign vAlign)
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


void SecureTextImageSourceD2D::BeginDraw(Windows::Foundation::Rect updateRect)
{
    POINT offset;
    ComPtr<IDXGISurface> surface;

    // Express target area as a native RECT type.
    RECT updateRectNative;
    updateRectNative.left = static_cast<LONG>(updateRect.Left);
    updateRectNative.top = static_cast<LONG>(updateRect.Top);
    updateRectNative.right = static_cast<LONG>(updateRect.Right);
    updateRectNative.bottom = static_cast<LONG>(updateRect.Bottom);

    // Query for ISurfaceImageSourceNative interface.
    Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
    DX::ThrowIfFailed(
        reinterpret_cast<IUnknown*>(this)->QueryInterface(IID_PPV_ARGS(&sisNative))
        );

    // Begin drawing - returns a target surface and an offset to use as the top left origin when drawing.
    HRESULT beginDrawHR = sisNative->BeginDraw(updateRectNative, &surface, &offset);

    if (SUCCEEDED(beginDrawHR))
    {
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
    }
    else if (beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET)
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
}


// Ends drawing updates started by a previous BeginDraw call.
void SecureTextImageSourceD2D::EndDraw()
{
    // Remove the transform and clip applied in BeginDraw since
    // the target area can change on every update.
    m_d2dContext->SetTransform(D2D1::IdentityMatrix());
    m_d2dContext->PopAxisAlignedClip();

    // Remove the render target and end drawing.
    DX::ThrowIfFailed(
        m_d2dContext->EndDraw()
        );

    m_d2dContext->SetTarget(nullptr);

    // Query for ISurfaceImageSourceNative interface.
    Microsoft::WRL::ComPtr<ISurfaceImageSourceNative> sisNative;
    DX::ThrowIfFailed(
        reinterpret_cast<IUnknown*>(this)->QueryInterface(IID_PPV_ARGS(&sisNative))
        );

    DX::ThrowIfFailed(
        sisNative->EndDraw()
        );
}

// Clears the background with the given color.
void SecureTextImageSourceD2D::Clear(Windows::UI::Color color)
{
    m_d2dContext->Clear(DX::ConvertToColorF(color));
}

void SecureTextImageSourceD2D::OnSuspending(Object ^sender, SuspendingEventArgs ^e)
{
    ComPtr<IDXGIDevice3> dxgiDevice;
    m_d3dDevice.As(&dxgiDevice);

    // Hints to the driver that the app is entering an idle state and that its memory can be used temporarily for other apps.
    dxgiDevice->Trim();
}

void SecureTextImageSourceD2D::Draw(SecureTextImageSourceDrawLayout^ args)
{
    BeginDraw(Windows::Foundation::Rect(0, 0, (float)m_width, (float)m_height));

    Clear(args->BackroundColor);

    if (!args->Equals(previousDrawArgs, false))
    {
        CreateTextFormatResource(args->FontName->Data(), args->FontSize, args->TextAlignHorizontal, args->TextAlignVertical);
    }

    ComPtr<ID2D1SolidColorBrush> brush;
    DX::ThrowIfFailed(
        m_d2dContext->CreateSolidColorBrush(
        DX::ConvertToColorF(args->TextColor),
        &brush
        )
        );

    D2D_RECT_F rect = D2D1::RectF(0, 0, (float)m_width, (float)m_height);

    m_d2dContext->DrawText((WCHAR*)args->Text.getUnsecureString(), args->Text.length()/2, m_d2dWriteTextFormat.Get(), &rect, brush.Get());
    args->Text.UnsecuredStringFinished();

    EndDraw();
}