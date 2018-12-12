#pragma once

#include "ImageDefs.h"

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <string>

namespace MKI {
    class Image;
}

namespace MKI::Interpolation
{

    class ScalingFunct
    {
    public:
        enum class Algorithm { unknown = 0, nearest_neighbor, bilinear, bicubic };
        static const std::unordered_map<Algorithm, std::string> algorithm_to_string;

    public:
        ScalingFunct(const Image* in_image, Image* out_image, ImagePixels::iterator begin,
                     ImagePixels::iterator end, Algorithm algorithm);

        void operator()(double ratio_height, double ratio_width);

    private:
        std::function<Pixel(size_t, size_t, double, double)> getAlgorithm();

        Pixel rangeCheckedPixelVal(size_t row, size_t column);
        double cubicHermite(double a, double b, double c, double d, double t);

        Pixel nearestNeighbor(size_t row, size_t column, double ratio_height, double ratio_width);
        Pixel bilinear(size_t row, size_t column, double ratio_height, double ratio_width);
        Pixel bicubic(size_t row, size_t column, double ratio_height, double ratio_width);

    private:
        const Image* m_in_image;
        Image* m_out_image;
        ImagePixels::iterator m_begin;
        ImagePixels::iterator m_end;
        Algorithm m_algorithm;
    };

}