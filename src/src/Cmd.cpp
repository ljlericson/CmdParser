#include "../include/ljl/cmd.hpp"

ljl::cmd::cmd(int argc, char** argv, std::string_view jsonfp)
{
    for(int i = 0; i < argc; i++)
        m_argv.push_back(std::string(argv[i]));

    std::ifstream file(jsonfp.data());
    if(!file.is_open()) 
    {
        std::cerr << "Error opening JSON file: " << jsonfp << "\n";
    }
    
    nlohmann::json j;
    file >> j;
    if ((j.contains("queries")))
    {
        for (const auto& [cmd, response] : j["queries"].items()) 
        {
            m_queries.insert({
                cmd,
                response.get<std::string>()
            });
        }
    }
    if(j.contains("commands"))
    {
        for (const auto& [cmd_name, cmd_data] : j["commands"].items()) 
        {
            m_cmds.insert({
                cmd_name,
                {}
            });
            for(const auto& arg : cmd_data)
            {
                m_cmds.at(cmd_name).push_back(arg.get<std::string>());
            }
        }
    }
    if(j.contains("default"))
    {
        m_default_answer = j["default"].get<std::string>();
    }
}

bool ljl::cmd::is(CmdType type)
{
    if(m_checked)
        return m_isCmd;

    bool is_cmd = false;
    if(m_argv.size() > 1)
    {
        for(const auto& [cmd, data] : m_cmds)
        {
            if(m_argv[1] == cmd)
            {    
                is_cmd = true;
                break;
            }
        }
    }
    else
    {
        std::cout << "No arguments give\n";
        return false;
    }

    m_isCmd = is_cmd;

    switch(type)
    {
    case CmdType::command:
        m_checked = true;
        if(is_cmd)
            m_queries.clear();
        return is_cmd;
    case CmdType::query:
        m_checked = true;
        if(!is_cmd)
            m_cmds.clear();
        return !is_cmd;
    }

    // unreachable
    return false;
}

void ljl::cmd::respond()
{
    if(!m_isCmd && m_argv.size() == 2)
    {
        bool found_one = false;
        for(const auto& [query, response] : m_queries)
        {
            if(m_argv[1] == query)
            {    
                std::cout << response << '\n';
                found_one = true;
                break;
            }
        }
        if(!found_one)
            std::cout << m_default_answer << '\n';
    }
}

template<typename _T_>
_T_ ljl::cmd::get_value(const std::string& cmd, const std::string& arg)
{
    auto it = std::find(m_cmds.at(cmd).begin(), m_cmds.at(cmd).end(), arg);
    if(it == m_cmds.at(cmd).end())
    {
        std::cout << "Arg not found\n";
        
        std::cout << std::endl;
        return _T_{};
    }

    size_t element_num = std::distance(m_cmds.at(cmd).begin(), it) + 2;

    std::string* value;
    
    try
    {
        value = &m_argv.at(element_num);
    }
    catch(...)
    {
        std::cout << "Missing argument\nArgs for command" << cmd << ':';
        for(const auto& arg : m_cmds.at(cmd))
            std::cout << ", " << arg;
        
        std::cout << std::endl;
        exit(-1);
        return _T_{}; // just for compiler warning sake
    }

    // Convert based on template type
    if constexpr (std::is_same_v<_T_, std::string>) 
    {
        return *value; // just return as-is
    } 
    else if constexpr (std::is_integral_v<_T_>) 
    {
        _T_ val;
        try
        {
            val = static_cast<_T_>(std::stoll(*value)); // handles int, long, etc.
        }
        catch(const std::exception&)
        {
            std::cout << "Invalid argument given\nValid args for command" << cmd << ':';
            for(const auto& arg : m_cmds.at(cmd))
                std::cout << ", " << arg;
            
            std::cout << std::endl;
            exit(-1);
            return _T_{}; // just for compiler warning sake
        }
        
        return val;
    } 
    else if constexpr (std::is_floating_point_v<_T_>) 
    {
        return static_cast<_T_>(std::stod(*value)); // handles float, double
    } 
    else 
    {
        static_assert(false, "Unsupported type passed to get_value()");
    }
}

bool ljl::cmd::operator[](const std::string& cmd)
{
    return m_argv[1] == cmd;
}

template std::string ljl::cmd::get_value<std::string>(const std::string&, const std::string&);
template int ljl::cmd::get_value<int>(const std::string&, const std::string&);
template uint64_t ljl::cmd::get_value<uint64_t>(const std::string&, const std::string&);
