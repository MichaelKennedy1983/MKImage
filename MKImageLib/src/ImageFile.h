#pragma once

#include "Image.h"
#include "FileType.h"

#include <filesystem>
#include <string>
#include <vector>

namespace MKI
{
    namespace FS = std::filesystem;

    class ImageFile
    {
    public:
        explicit ImageFile(const std::string& file);

        bool load(const std::string& file);
        bool save(const std::string& file, const std::string& comment = "");
        bool saveCopy(const std::string& comment = "");

    private:
        bool readHeader(std::ifstream& in);
        void skipHeader(std::ifstream& in) const;
        void skipComment(std::ifstream& in) const;

        bool loadBin(std::ifstream& in);
        bool loadText(std::ifstream& in);

        bool saveBin();
        bool saveText();

    private:
        std::vector<Image> m_images;
        FS::path m_path;
        FileType m_filetype;
    };
}