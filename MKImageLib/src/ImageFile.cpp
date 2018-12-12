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

    bool ImageFile::save(const std::string& file, const std::string& comment) {
        auto start_time = std::chrono::high_resolution_clock::now();

        std::cout << "\nSaving: " << file << ".\n";

        FS::path out_path(m_path.parent_path());
        out_path.append(Globals::default_image_out_dir);

        if (!FS::exists(out_path)) {
            FS::create_directory(out_path);
        }

        out_path.append(file);

        if (m_filetype == FileType::FType::P2) {
            saveText(out_path, comment);
        } else if (m_filetype == FileType::FType::P5) {
            saveBin(out_path, comment);
        } else {
            std::cout << file << " failed to save. Unsupported file type.\n";
            return false;
        }

        std::chrono::duration<double> duration =
            std::chrono::high_resolution_clock::now() - start_time;
        std::cout << file << " saved successfully in " << duration.count()
            << " seconds.\n";
        
        return true;
    }

    bool ImageFile::saveCopy(const std::string& comment)
    {
        std::string out_name(m_path.filename().generic_string());
        size_t pos = out_name.find_last_of('.');

        if (pos != std::string::npos) {
            out_name.insert(pos, "_COPY");
        } else {
            out_name.append("_COPY.pgm");
        }

        return save(out_name, comment);
    }

    /*
        #################### Private Methods #################### 
    */

    bool ImageFile::readHeader(std::ifstream& in)
    {
        if (in.is_open()) {
            in >> m_filetype;
            skipComment(in);
            in >> m_images.back().m_columns;
            in >> m_images.back().m_rows;
            int depth;
            in >> depth;
            m_images.back().m_depth = depth;

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
            std::vector<Pixel>(m_images.back().columns()));

        for (auto& column : pixels) {
            for (auto& pix : column) {
                in.read(reinterpret_cast<char*>(&pix), sizeof(Pixel));
            }
        }

        return true;
    }

    bool ImageFile::loadText(std::ifstream& in)
    {
        ImagePixels& pixels = m_images.back().m_pixels;
        pixels = ImagePixels(m_images.back().rows(), 
            std::vector<Pixel>(m_images.back().columns()));

        for (auto& column : pixels) {
            for (auto& pix : column) {
                int temp;
                in >> temp;
                pix = temp;
            }
        }

        return true;
    }

    bool ImageFile::saveBin(const FS::path& file, const std::string& comment)
    {
        std::ofstream out;

        out.open(file, std::ios::binary);

        if (out.is_open()) {
            out << m_filetype << '\n';
            if (comment.length() > 0) {
                out << "# " << comment << '\n';
            }
            out << image().m_columns << ' ' << image().m_rows << '\n';
            out << static_cast<int>(image().m_depth) << '\n';

            for (const auto& column : image().m_pixels) {
                for (auto pix : column) {
                    out.write(reinterpret_cast<char*>(&pix), sizeof(Pixel));
                }
            }
        }

        return true;
    }

    bool ImageFile::saveText(const FS::path& file, const std::string& comment)
    {
        std::ofstream out;

        out.open(file);

        if (out.is_open()) {
            out << m_filetype << '\n';
            if (comment.length() > 0) {
                out << "# " << comment << '\n';
            }
            out << image().m_columns << ' ' << image().m_rows << '\n';
            out << static_cast<int>(image().m_depth) << '\n';

            for (const auto& column : image().m_pixels) {
                for (int pix : column) {
                    out << pix << ' ';
                }
                out << '\n';
            }
        }

        return true;
    }
}