#pragma once

#include "Interpolation.h"

#include <vector>
#include <cstdint>

namespace MKI
{
    using ImagePixels = std::vector< std::vector<uint8_t> >;
    using ResizeAlgorithm = Interpolation::ScalingFunct::Algorithm;

    class ImageFile;

    class Image
    {
    public:
        friend ImageFile;

        Image() = default;
        Image(ImagePixels&& pixels, size_t rows, size_t columns, uint8_t depth);

        ImagePixels& mutablePixels() { return m_pixels; }
        const ImagePixels& pixels() const { return m_pixels; } 
        size_t rows() const { return m_rows; }
        size_t columns() const { return m_columns; }
        uint8_t depth() const { return m_depth; }

        void pixels(const ImagePixels& pixels) { m_pixels = pixels; }
        void pixels(const ImagePixels&& pixels) { m_pixels = pixels; }
        void rows(size_t rows) { m_rows = rows; }
        void columns(size_t columns) { m_columns = columns; }
        void depth(uint8_t depth) { m_depth = depth; }

        Image resize(size_t new_height, size_t new_width,
                     ResizeAlgorithm algorithm) const;
        Image singleResize(size_t new_height, size_t new_width,
                           ResizeAlgorithm algorithm) const;

    private:
        ImagePixels m_pixels;
        size_t m_rows;
        size_t m_columns;
        uint8_t m_depth;
    };

}