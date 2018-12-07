#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

namespace MKI
{
    class FileType
    {
    public:
        enum class FType { Unknown = 0, P2, P5 };
        
        static const std::unordered_map<FType, std::string> ftype_to_string;
        static const std::unordered_map<std::string, FType> string_to_ftype;

    public:
        FileType() = default;
        explicit FileType(const std::string& type_str);

        std::string toString() const;

        bool operator ==(const FileType& rhs) const;
        bool operator ==(const std::string& rhs) const;
        bool operator ==(const FType rhs) const;

        friend std::ostream& operator <<(std::ostream& out, const FileType& ft);
        friend std::istream& operator >>(std::istream& in, FileType& ft);

    private:
        FType m_file_type;
    };
}