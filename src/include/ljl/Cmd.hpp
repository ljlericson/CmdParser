// standard libs
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <string_view>
#include <fstream>
#include <type_traits>
// json parser lib
#include <nlohmann/json.hpp>

namespace ljl
{
    class cmdparser
    {    
    public:
        enum class type
        {
            query,
            command
        };

    public:
        cmdparser(int argc, char** argv, const nlohmann::json& json);
        cmdparser(const cmdparser&) = delete;
        cmdparser& operator=(const cmdparser&) = delete;

        void respond();

        bool is(type type);

        bool operator[](const std::string& cmd);

        template<typename _T_>
        _T_ get_value(const std::string& cmd, const std::string& arg);


    private:
        enum class passtype
        {
            explicit_,
            implicit
        };

        std::string m_default_answer;
        std::vector<std::string> m_argv;
        std::unordered_map<std::string, std::string> m_queries;
        std::unordered_map<std::string, passtype> m_cmd_passtype;
        std::map<std::string, std::map<std::string, std::string>> m_cmds;
        bool m_isCmd;
        bool m_checked = false;

        size_t m_global_shift = 0;
    };
}