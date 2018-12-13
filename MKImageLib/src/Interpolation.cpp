#include "Interpolation.h"

#include "Image.h"

#include <cmath>
#include <algorithm>

namespace MKI::Interpolation
{
    const std::unordered_map<ScalingFunct::Algorithm, std::string>
    ScalingFunct::algorithm_to_string
    {
        {Algorithm::unknown, "unkown"},
        {Algorithm::nearest_neighbor, "nearest neightbor"},
        {Algorithm::bilinear, "bilinear"},
        {Algorithm::bicubic, "bicubic"}
    };

    ScalingFunct::ScalingFunct(const Image* in_image,
                               Image* out_image,
                               ImagePixels::iterator begin,
                               ImagePixels::iterator end,
                               Algorithm algorithm)
        : m_in_image(in_image),
          m_out_image(out_image),
          m_begin(begin),
          m_end(end),
          m_algorithm(algorithm)
    {

    }

    void ScalingFunct::operator()(double ratio_height, double ratio_width)
    {
        auto interpolation = getAlgorithm();
        ImagePixels& out_pixels = m_out_image->mutablePixels();

        size_t row_count = m_begin - out_pixels.begin();
        for (; m_begin != m_end; ++m_begin) {
            for (size_t col_count = 0; col_count < m_out_image->columns(); ++col_count) {
                out_pixels.at(row_count).at(col_count) =
                    interpolation(row_count, col_count, ratio_height, ratio_width);
            }
            ++row_count;
        }
    }

    /*
        #################### Private Methods #################### 
    */

    std::function<Pixel(size_t, size_t, double, double)> ScalingFunct::getAlgorithm()
    {
        switch (m_algorithm) {
        case Algorithm::nearest_neighbor:
            return [this](size_t row, size_t column, double ratio_height, double ratio_width)
                -> Pixel
            {
                size_t row_index = std::floor(row * ratio_height);
                size_t column_index = std::floor(column * ratio_width);

                Pixel val = m_in_image->pixels().at(row_index).at(column_index);
                return val;
            };
        case Algorithm::bilinear:
            return [this](size_t row, size_t column, double ratio_height, double ratio_width)
                -> Pixel
            {
                double row_floating = row * ratio_height;
                double column_floating = column * ratio_width;
                size_t row_index = std::floor(row_floating);
                size_t column_index = std::floor(column_floating);
                double row_offset = row_floating - row_index;
                double column_offset = column_floating - column_index;

                Pixel a = rangeCheckedPixelVal(row_index, column_index);
                Pixel b = rangeCheckedPixelVal(row_index, column_index + 1);
                Pixel c = rangeCheckedPixelVal(row_index + 1, column_index);
                Pixel d = rangeCheckedPixelVal(row_index + 1, column_index + 1);

                double val =
                    a * (1 - row_offset) * (1 - column_offset) +
                    b * (1 - row_offset) * column_offset +
                    c * row_offset * (1 - column_offset) +
                    d * row_offset * column_offset;

                return std::floor(val);
            };
        case Algorithm::bicubic:
            return [this](size_t row, size_t column, double ratio_height, double ratio_width)
                -> Pixel
            {
                double row_floating = row * ratio_height;
                double column_floating = column * ratio_width;
                size_t row_index = std::floor(row_floating);
                size_t column_index = std::floor(column_floating);
                double row_offset = row_floating - row_index;
                double column_offset = column_floating - column_index;

                Pixel p00 = rangeCheckedPixelVal(row_index - 1, column_index - 1);
                Pixel p01 = rangeCheckedPixelVal(row_index - 1, column_index + 0);
                Pixel p02 = rangeCheckedPixelVal(row_index - 1, column_index + 1);
                Pixel p03 = rangeCheckedPixelVal(row_index - 1, column_index + 2);

                Pixel p10 = rangeCheckedPixelVal(row_index + 0, column_index - 1);
                Pixel p11 = rangeCheckedPixelVal(row_index + 0, column_index + 0);
                Pixel p12 = rangeCheckedPixelVal(row_index + 0, column_index + 1);
                Pixel p13 = rangeCheckedPixelVal(row_index + 0, column_index + 2);

                Pixel p20 = rangeCheckedPixelVal(row_index + 1, column_index - 1);
                Pixel p21 = rangeCheckedPixelVal(row_index + 1, column_index + 0);
                Pixel p22 = rangeCheckedPixelVal(row_index + 1, column_index + 1);
                Pixel p23 = rangeCheckedPixelVal(row_index + 1, column_index + 2);

                Pixel p30 = rangeCheckedPixelVal(row_index + 2, column_index - 1);
                Pixel p31 = rangeCheckedPixelVal(row_index + 2, column_index + 0);
                Pixel p32 = rangeCheckedPixelVal(row_index + 2, column_index + 1);
                Pixel p33 = rangeCheckedPixelVal(row_index + 2, column_index + 2);

                double row1 = cubicHermite(p00, p01, p02, p03, column_offset);
                double row2 = cubicHermite(p10, p11, p12, p13, column_offset);
                double row3 = cubicHermite(p20, p21, p22, p23, column_offset);
                double row4 = cubicHermite(p30, p31, p32, p33, column_offset);

                double val = cubicHermite(row1, row2, row3, row4, row_offset);
                val = std::clamp(val, 0.0, static_cast<double>(m_in_image->depth()));

                return std::floor(val);
            };
        case Algorithm::unknown:
            return [](size_t, size_t, double, double)
                -> Pixel
            {
                return Pixel();
            };
        }
    }

    Pixel ScalingFunct::rangeCheckedPixelVal(size_t row, size_t column)
    {
        size_t row_clamped = std::clamp(row, size_t(0), m_in_image->rows() - 1);
        size_t column_clamped = std::clamp(column, size_t(0) , m_in_image->columns() - 1);

        return m_in_image->pixels().at(row_clamped).at(column_clamped);
    }

    double ScalingFunct::cubicHermite(double a, double b, double c, double d, double t)
    {
        double aa = -a/2.0 + (3.0*b)/2.0 - (3.0*c)/2.0 + d/2.0;
        double bb = a - (5.0*b)/2.0 + 2.0*c - d/2.0;
        double cc = -a/2.0 + c/2.0;
        double dd = b;

        return aa*t*t*t + bb*t*t + cc*t + dd;
    }
}