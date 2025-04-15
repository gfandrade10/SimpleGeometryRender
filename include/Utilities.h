#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <string_view>

namespace RenderApp
{
    std::string WStringToUtf8(const std::wstring& wstr);
    std::string GLSLtoString(std::string_view path);
}

#endif
