#pragma once

#include <string>

namespace MKI
{
    class Globals
    {
    public:
        static std::string default_image_dir;
        static std::string default_image_out_dir;
        static size_t threads_used;
    };  
}