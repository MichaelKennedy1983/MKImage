#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <string>

namespace MKI {

    class Image;
    using ImagePixels = std::vector< std::vector<uint8_t> >;

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
        std::function<uint8_t(size_t, size_t, double, double)> getAlgorithm();

        uint8_t rangeCheckedPixelVal(size_t row, size_t column);
        double cubicHermite(double a, double b, double c, double d, double t);

        uint8_t nearestNeighbor(size_t row, size_t column, double ratio_height, double ratio_width);
        uint8_t bilinear(size_t row, size_t column, double ratio_height, double ratio_width);
        uint8_t bicubic(size_t row, size_t column, double ratio_height, double ratio_width);

    private:
        const Image* m_in_image;
        Image* m_out_image;
        ImagePixels::iterator m_begin;
        ImagePixels::iterator m_end;
        Algorithm m_algorithm;
    };

}