#pragma once

#include <stdlib.h>

namespace NimEditor {
namespace Internal {

enum Format {
    Format_Number = 0,
    Format_String,
    Format_Keyword,
    Format_Type,
    Format_ClassField,
    Format_MagicAttr, // magic class attribute/method, like __name__, __init__
    Format_Operator,
    Format_Comment,
    Format_Doxygen,
    Format_Identifier,
    Format_Whitespace,
    Format_ImportedModule,

    Format_FormatsAmount,
    Format_EndOfBlock
};

class FormatToken
{
public:
    FormatToken() {}

    FormatToken(Format format, size_t position, size_t length)
        :m_format(format)
        ,m_position(int(position))
        ,m_length(int(length))
    {}

    inline Format format() const { return m_format; }
    inline int begin() const { return m_position; }
    inline int end() const { return m_position + m_length; }
    inline int length() const { return m_length; }

private:
    Format m_format;
    int m_position;
    int m_length;
};

} // namespace Internal
} // namespace NimEditor
