#include "ImageFile.h"

#include "Globals.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <limits>
#include <cstdint>

namespace MKI
{
    ImageFile::ImageFile(const std::string& file)
        : m_images(), m_path(), m_filetype()
    {
        load(file);
    }

    bool ImageFile::load(const std::string& file)
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        std::cout << "\nLoading: " << file << ".\n";

        m_path.assign(FS::current_path());
        m_path.append(file);
        if (!FS::exists(m_path)) {
            m_path.assign(FS::current_path());
            m_path.append(Globals::default_image_dir);
            m_path.append(file);
        }
        if (!FS::exists(m_path)) {
            std::cout << "Image failed to load.\n";
            return false;
        }

        m_images.emplace_back(Image());

        std::ifstream in(m_path);
        readHeader(in);
        
        if (m_filetype == FileType::FType::P2) {
            loadText(in);
        } else if (m_filetype == FileType::FType::P5) {
            in.close();
            in.clear();
            in.open(m_path, std::ios::binary);
            skipHeader(in);
            loadBin(in);
        } else {
            std::cout << "Image in unsupported format.\n";
            return false;
        }

        std::chrono::duration<double> duration = 
            std::chrono::high_resolution_clock::now() - start_time;
        std::cout << file << " loaded successfully in " << duration.count()
            << " seconds.\n";

        return true;
    }

    bool save(const std::string& file, const std::string& comment = "");
    bool saveCopy(const std::string& comment = "");

    bool ImageFile::readHeader(std::ifstream& in)
    {
        if (in.is_open()) {
            in >> m_filetype;
            skipComment(in);
            in >> m_images.back().m_columns;
            in >> m_images.back().m_rows;
            in >> m_images.back().m_depth;

            return true;
        }

        return false;
    }

    void ImageFile::skipHeader(std::ifstream& in) const
    {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        skipComment(in);

        int a, b, c;

        in >> a;
        in >> b;
        in >> c;
    }

    void ImageFile::skipComment(std::ifstream& in) const
    {
        char next;
        in.get(next);

        while (next == '\n' || next == '#') {
            if (next == '#') {
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            in.get(next);
        }
        in.unget();
    }

    bool ImageFile::loadBin(std::ifstream& in)
    {
        ImagePixels& pixels = m_images.back().m_pixels;
        pixels = ImagePixels(m_images.back().rows(), 
            std::vector<uint8_t>(m_images.back().columns()));

        for (auto& columns : pixels) {
            for (auto& pix : columns) {
                in.read(reinterpret_cast<char*>(&pix), sizeof(uint8_t));
            }
        }

        return true;
    }

    bool ImageFile::loadText(std::ifstream& in)
    {
        ImagePixels& pixels = m_images.back().m_pixels;
        pixels = ImagePixels(m_images.back().rows(), 
            std::vector<uint8_t>(m_images.back().columns()));

        for (auto& columns : pixels) {
            for (auto& pix : columns) {
                in >> pix;
            }
        }

        return true;
    }

    bool saveBin();
    bool saveText();
}