#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>

namespace RenderApp
{
    std::string WStringToUtf8(const std::wstring& ws)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(ws);
    }

    std::string GLSLtoString(std::string_view path)
    {
        std::ifstream FileReader(path.data(), std::ios::binary);
        std::ostringstream Buffer;
        if(FileReader.is_open())
        {
            Buffer << FileReader.rdbuf();
            return Buffer.str();
        }
        else
        {
            std::cerr << "Failed to load GLSL files\n";
            return "";
        }
    }
}
