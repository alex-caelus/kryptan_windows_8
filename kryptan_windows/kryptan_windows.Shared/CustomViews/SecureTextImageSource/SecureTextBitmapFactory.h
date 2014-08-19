#pragma once

#include "pch.h"

#include "SecureTextImageSourceDrawLayout.h"
#include "SecureTextBitmapSourceD2D.h"

namespace kryptan_windows
{
    class SecureTextBitmapFactory
    {
    public:
        static concurrency::task<Windows::UI::Xaml::Media::ImageSource^> RenderBitmap(int height, int width, SecureTextImageSourceDrawLayout^ options);

    private:

        static SecureTextBitmapSourceD2D^ getImageSource(int height, int width, bool recreate);
        static std::map<std::pair<int, int>, SecureTextBitmapSourceD2D^ > _ImageSourceList;
    };
}