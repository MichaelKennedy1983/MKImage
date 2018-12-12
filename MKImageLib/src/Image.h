#pragma once

#include "ImageDefs.h"
#include "Interpolation.h"

#include <vector>
#include <cstdint>

namespace MKI
{
    using ResizeAlgorithm = Interpolation::ScalingFunct::Algorithm;

    class ImageFile;

    class Image
    {
    public:
        friend ImageFile;

        Image() = default;
        Image(ImagePixels&& pixels, size_t rows, size_t columns, Pixel depth);

        ImagePixels& mutablePixels() { return m_pixels; }
        const ImagePixels& pixels() const { return m_pixels; }
        size_t rows() const { return m_rows; }
        size_t columns() const { return m_columns; }
        Pixel depth() const { return m_depth; }

        void pixels(const ImagePixels& pixels) { m_pixels = pixels; }
        void pixels(const ImagePixels&& pixels) { m_pixels = pixels; }
        void rows(size_t rows) { m_rows = rows; }
        void columns(size_t columns) { m_columns = columns; }
        void depth(Pixel depth) { m_depth = depth; }

        Image resize(size_t new_height, size_t new_width,
                     ResizeAlgorithm algorithm) const;
        Image singleResize(size_t new_height, size_t new_width,
                           ResizeAlgorithm algorithm) const;

    private:
        ImagePixels m_pixels;
        size_t m_rows;
        size_t m_columns;
        Pixel m_depth;
    };

}