#include "pch.h"
#include "SecureTextBitmapFactory.h"


using namespace kryptan_windows;
using namespace concurrency;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Media;

std::map<std::pair<int, int>, SecureTextBitmapSourceD2D^ > SecureTextBitmapFactory::_ImageSourceList;

concurrency::task<ImageSource^> SecureTextBitmapFactory::RenderBitmap(int height, int width, SecureTextImageSourceDrawLayout^ options)
{
    return create_task([](){}).then([height, width, options](){
        //source
        auto iSource = getImageSource(height, width, false);
        
        //destination
        auto image = ref new BitmapImage();

        //draw
        auto stream = iSource->Draw(options);

        //Fill image
        if (stream != nullptr){
            image->SetSourceAsync(stream);
        }

        //return result
        return (ImageSource^)image;
    }, Concurrency::task_continuation_context::use_current());
}

SecureTextBitmapSourceD2D^ SecureTextBitmapFactory::getImageSource(int height, int width, bool recreate)
{
    std::pair<int, int> key(height, width);
    if (!recreate)
    {
        auto it = _ImageSourceList.find(key);
        if (it != _ImageSourceList.end())
        {
            return it->second;
        }
    }
    auto newSource = ref new SecureTextBitmapSourceD2D(height, width);

    _ImageSourceList[key] = newSource;

    return newSource;
}