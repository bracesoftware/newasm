// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

__newasm_LOAD_PACKAGE_MODULE(dynlib, {
    //setup goes here
});

namespace newasm
{
    namespace dynlib
    {
        namespace err_codes
        {
            const int labels_unsupported = 1;
            const int unsupported_instruction = 2;

            const std::unordered_map<int, std::string> identifier = {
                {labels_unsupported, "LabelsUnsupported"},
                {unsupported_instruction, "UnsupportedInstruction"}
            };
        }
        namespace mem
        {
            std::vector<std::string> invalid_dynlibs;
        }
        namespace settings
        {
            std::string analyzed_dynlib;
            std::string lastline;
            int lastlinedx;
        }
        namespace functions
        {
            void abort(int error_code)
            {
                newasm::dynlib::mem::invalid_dynlibs.push_back(newasm::dynlib::settings::analyzed_dynlib);
                std::cout << newasm::header::col::reset
                << newasm::header::col::red << "\t  "
                << "Aborted dynamic library implementation.\n"<< "\t  "
                << "Runtime error " << error_code << " \"" << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::err_codes::identifier.at(error_code) << newasm::header::col::reset
                << newasm::header::col::red << "\" in " << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::settings::analyzed_dynlib << ":" << newasm::dynlib::settings::lastlinedx << newasm::header::col::reset
                << newasm::header::col::red << " >> " << newasm::header::col::gray << newasm::dynlib::settings::lastline << "\n";
                return;
            }
        }
    }
}