// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(KRNLINFO, {
    //setup goes here
});

namespace newasm
{
    struct pair_hash
    {
        std::size_t operator()(const std::pair<int, int>& p) const
        {
            std::hash<int> hash_int;
            return hash_int(p.first) ^ (hash_int(p.second) << 1);
        }
    };

    namespace kernel
    {
        const std::unordered_map<std::pair<int, int>, std::string, newasm::pair_hash> syscalls = {
            {{newasm::core::lang_inf::refs::ios, 1}, "sys_write_text"},
            {{newasm::core::lang_inf::refs::ios, 2}, "sys_write_number"},
            {{newasm::core::lang_inf::refs::ios, 3}, "sys_input_text"},
            {{newasm::core::lang_inf::refs::ios, 4}, "sys_input_number"},
            {{newasm::core::lang_inf::refs::ios, 5}, "sys__write__"},
            {{newasm::core::lang_inf::refs::ios, 6}, "sys_write_ref"},
            {{newasm::core::lang_inf::refs::ios, 7}, "sys_write_char"},
            {{newasm::core::lang_inf::refs::ios, 8}, "sys_input_char"},

            {{newasm::core::lang_inf::refs::fs, 1}, "sys_mkdir"},
            {{newasm::core::lang_inf::refs::fs, 2}, "sys_rmdir"},
            {{newasm::core::lang_inf::refs::fs, 3}, "sys_mkfile"},
            {{newasm::core::lang_inf::refs::fs, 4}, "sys_rmfile"},
            {{newasm::core::lang_inf::refs::fs, 5}, "sys_overwrite"},
            {{newasm::core::lang_inf::refs::fs, 6}, "sys_append"},
            {{newasm::core::lang_inf::refs::fs, 7}, "sys_clear"},
            {{newasm::core::lang_inf::refs::fs, 8}, "sys_readline"},

            {{newasm::core::lang_inf::refs::thread, 1}, "sys_cout_thread"},
            {{newasm::core::lang_inf::refs::thread, 2}, "sys_fetch_retf"},

            {{newasm::core::lang_inf::refs::chrono, 1}, "sys_getyear"},
            {{newasm::core::lang_inf::refs::chrono, 2}, "sys_getmonth"},
            {{newasm::core::lang_inf::refs::chrono, 3}, "sys_getday"},
            {{newasm::core::lang_inf::refs::chrono, 4}, "sys_gethour"},
            {{newasm::core::lang_inf::refs::chrono, 5}, "sys_getminute"},
            {{newasm::core::lang_inf::refs::chrono, 6}, "sys_getsecond"},

            //{{newasm::core::lang_inf::refs::ext, 1}, "sys_"},

            {{newasm::core::lang_inf::refs::net, 1}, "sys_download"},

            {{newasm::core::lang_inf::refs::mem, 1}, "sys_const"},

            {{newasm::core::lang_inf::refs::txtop, 1}, "sys_txt_concat"},
            {{newasm::core::lang_inf::refs::txtop, 2}, "sys_txt_trim"},

            {{newasm::core::lang_inf::refs::cmanip, 1}, "sys_bitarr_clear"},
            {{newasm::core::lang_inf::refs::cmanip, 2}, "sys_bitarr_flip"},
            {{newasm::core::lang_inf::refs::cmanip, 3}, "sys_bitarr_reverse"},
            {{newasm::core::lang_inf::refs::cmanip, 4}, "sys_bitarr_setat"},
            {{newasm::core::lang_inf::refs::cmanip, 5}, "sys_bitatt_getat"},
            {{newasm::core::lang_inf::refs::cmanip, 6}, "sys_bintree_setatparentof"},
            {{newasm::core::lang_inf::refs::cmanip, 7}, "sys_bintree_setatrightchildof"},
            {{newasm::core::lang_inf::refs::cmanip, 8}, "sys_bintree_setatleftchildof"},
            {{newasm::core::lang_inf::refs::cmanip, 9}, "sys_bintree_getat"},

            {{newasm::core::lang_inf::refs::tuple, 1}, "sys_tuple_size"},

            {{newasm::core::lang_inf::refs::tcp, 1}, "sys_tcp_send"},
            {{newasm::core::lang_inf::refs::tcp, 2}, "sys_tcp_recv"},

            {{newasm::core::lang_inf::refs::http, 1}, "sys_http_get"},
            {{newasm::core::lang_inf::refs::http, 2}, "sys_http_post"},

            {{newasm::core::lang_inf::refs::math, 1}, "sys_math_sin"},
            {{newasm::core::lang_inf::refs::math, 2}, "sys_math_cos"},
            {{newasm::core::lang_inf::refs::math, 3}, "sys_math_tan"},
            {{newasm::core::lang_inf::refs::math, 4}, "sys_math_sqrt"},
            {{newasm::core::lang_inf::refs::math, 5}, "sys_math_abs"},
            {{newasm::core::lang_inf::refs::math, 6}, "sys_math_cbrt"},

            {{newasm::core::lang_inf::refs::misc, 1}, "sys_misc_rand"},

            {{newasm::core::lang_inf::refs::crypto, 1}, "sys_crypto_sha256"},
            {{newasm::core::lang_inf::refs::crypto, 2}, "sys_crypto_xor_enc"},
            {{newasm::core::lang_inf::refs::crypto, 3}, "sys_crypto_xor_dec"}
        };
    }
}