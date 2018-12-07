#include "FileType.h"

namespace MKI
{
    const std::unordered_map<FileType::FType, std::string> FileType::ftype_to_string
    {
        {FileType::FType::Unknown, "Unkown"},
        {FileType::FType::P2, "P2"},
        {FileType::FType::P5, "P5"}
    };

    const std::unordered_map<std::string, FileType::FType> FileType::string_to_ftype
    {
        {"Unknown", FileType::FType::Unknown},
        {"P2", FileType::FType::P2},
        {"P5", FileType::FType::P5}
    };

    FileType::FileType(const std::string& type_str)
        : m_file_type(string_to_ftype.at(type_str))
    {
        
    }

    std::string FileType::toString() const
    {
        return ftype_to_string.at(m_file_type);
    }

    bool FileType::operator ==(const FileType& rhs) const
    {
        return m_file_type == rhs.m_file_type;
    }

    bool FileType::operator ==(const std::string& rhs) const
    {
        return m_file_type == string_to_ftype.at(rhs);
    }

    bool FileType::operator ==(const FType rhs) const
    {
        return m_file_type == rhs;
    }

    std::ostream& operator <<(std::ostream& out, const FileType& ft)
    {
        out << ft.toString();
        return out;
    }

    std::istream& operator >>(std::istream& in, FileType& ft)
    {
        std::string temp;

        in >> temp;
        ft.m_file_type = FileType::string_to_ftype.at(temp);
        return in;
    }
}