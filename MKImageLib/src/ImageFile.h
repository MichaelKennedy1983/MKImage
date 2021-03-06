#pragma once

#include "Image.h"
#include "FileType.h"

#include <filesystem>
#include <string>
#include <vector>
#include <type_traits>

namespace MKI
{
    namespace FS = std::filesystem;

    class ImageFile
    {
    public:
        explicit ImageFile(const std::string& file);

        Image& mutableImage() { return m_images.back(); }
        const Image& image() const { return m_images.back(); }
        const Image& originImage() const { return m_images.front(); }
        FS::path path() const { return m_path; };
        FileType filetype() const { return m_filetype; }

        void add(Image& image) { m_images.push_back(image); }
        void add(Image&& image) { m_images.push_back(image); }

        bool load(const std::string& file);
        bool save(const std::string& file, const std::string& comment = "");
        bool saveCopy(const std::string& comment = "");

    private:
        bool readHeader(std::ifstream& in);
        void skipHeader(std::ifstream& in) const;
        void skipComment(std::ifstream& in) const;

        bool loadBin(std::ifstream& in);
        bool loadText(std::ifstream& in);

        bool saveBin(const FS::path& file, const std::string& comment);
        bool saveText(const FS::path& file, const std::string& comment);

    private:
        std::vector<Image> m_images;
        FS::path m_path;
        FileType m_filetype;
    };
}