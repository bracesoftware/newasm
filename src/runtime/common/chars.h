// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace chars
    {
        const std::string empty = "0c0"; //empty
        const std::string endl = "0c1"; //end of line
        const std::string carret = "0c2"; //carriage return
        const std::string tab = "0c3"; //tab
        const std::string alert = "0c4"; //alert
        const std::string backspace = "0c5"; //backspace

        const std::unordered_map<std::string, std::string> map = {
            {empty, ""},
            {endl, "\n"},
            {carret, "\r"},
            {tab, "\t"},
            {alert, "\a"},
            {backspace, "\b"}
        };
    }
}