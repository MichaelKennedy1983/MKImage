#pragma once

#include <vector>
#include <cstdint>

namespace MKI
{
    using ImagePixels = std::vector< std::vector<uint8_t> >;

    class ImageFile;

    class Image
    {
    public:
        friend ImageFile;

        const ImagePixels& pixels() const { return m_pixels; } 
        size_t rows() const { return m_rows; }
        size_t columns() const { return m_columns; }
        uint8_t depth() const { return m_depth; }

        void pixels(const ImagePixels& pixels) { m_pixels = pixels; }
        void pixels(const ImagePixels&& pixels) { m_pixels = pixels; }
        void rows(size_t rows) { m_rows = rows; }
        void columns(size_t columns) { m_columns = columns; }
        void depth(uint8_t depth) { m_depth = depth; }

    private:
        ImagePixels m_pixels;
        size_t m_rows;
        size_t m_columns;
        uint8_t m_depth;
    };

}