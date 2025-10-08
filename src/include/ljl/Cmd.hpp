// standard libs
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <string_view>
#include <fstream>
#include <type_traits>
// json parser lib
#include <nlohmann/json.hpp>

namespace ljl
{
    class cmd
    {
    private:
        std::string m_default_answer;
        std::vector<std::string> m_argv;
        std::unordered_map<std::string, std::string> m_queries;
        std::unordered_map<std::string, std::vector<std::string>> m_cmds;
        bool m_isCmd;
        bool m_checked = false;
    
    public:
        enum class CmdType
        {
            query,
            command
        };

    public:
        cmd(int argc, char** argv, std::string_view jsonfp);
        cmd(const cmd&) = delete;
        cmd& operator=(const cmd&) = delete;

        void respond();

        bool is(CmdType type);

        bool operator[](const std::string& cmd);

        template<typename _T_>
        _T_ get_value(const std::string& cmd, const std::string& arg);
    };
}