#include "../include/ljl/cmd.hpp"

ljl::cmd::cmd(int argc, char** argv, const nlohmann::json& json)
{
    for(int i = 0; i < argc; i++)
        m_argv.push_back(std::string(argv[i]));
    
    if ((json.contains("queries")))
    {
        for (const auto& [cmd, response] : json["queries"].items()) 
        {
            m_queries.insert({
                cmd,
                response.get<std::string>()
            });
        }
    }
    if(json.contains("commands"))
    {
        for (const auto& [cmd_name, cmd_data] : json["commands"].items()) 
        {
            m_cmds.insert({
                cmd_name,
                {}
            });
            for(const auto& [arg, type] : cmd_data.items())
            {
                m_cmds.at(cmd_name).insert(std::pair{arg, type.get<std::string>()});
            }
        }
    }
    if(json.contains("default"))
    {
        m_default_answer = json["default"].get<std::string>();
    }
}

bool ljl::cmd::is(cmd::type type)
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
    case type::command:

        m_checked = true;

        if(is_cmd)
            m_queries.clear();

        return is_cmd;
        
    case type::query:

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
    if(!m_isCmd && m_checked && m_argv.size() == 2)
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
    auto it = m_cmds.at(cmd).find(arg);
    if(it == m_cmds.at(cmd).end())
    {
        std::cout << "PROGRAMMER ERROR: Arg " << arg << " not found\n";

        return _T_{};
    }
    // else get type:
    const std::string& type = m_cmds.at(cmd).at(arg);

    size_t element_num = std::distance(m_cmds.at(cmd).begin(), it) + 2;

    std::string* value;
    
    try
    {
        value = &m_argv.at(element_num);
    }
    catch(...)
    {
        std::cout << "|====| Missing argument(s) |====|\nArgs for command " << cmd << ":\n";
        for(const auto& [arg, type] : m_cmds.at(cmd))
            std::cout << arg << " | Type: " << type << std::endl;
        
        exit(-1);
        return _T_{}; // just for compiler warning sake
    }

    // Convert based on template type
    if constexpr (std::is_same_v<_T_, std::string>)
    {
        // have to nest 2nd if because it is evaluated at runtime
        if(type == "string")
            return *value; // just return as-is
    } 
    else if constexpr (std::is_integral_v<_T_>)
    {
        if(type == "integer")
        {        
            _T_ val;
            try
            {
                val = static_cast<_T_>(std::stoll(*value)); // handles int, long, etc.
            }
            catch(const std::exception&)
            {
                std::cout << "|====| Invalid argument(s) |====|\nArgs for command " << cmd << ":\n";
                for(const auto& [arg, type] : m_cmds.at(cmd))
                    std::cout << arg << " | Type: " << type << std::endl;

                
                std::cout << std::endl;
                exit(-1);
                return _T_{}; // just for compiler warning sake
            }
            
            return val;
        }
    } 
    else if constexpr (std::is_floating_point_v<_T_>) 
    {
        if(type == "decimal")
        {
            _T_ val;
            try
            {
                val = static_cast<_T_>(std::stod(*value)); // handles float, double
            }
            catch(...)
            {
                std::cout << "|====| Invalid argument(s) |====|\nArgs for command " << cmd << ":\n";
                for(const auto& [arg, type] : m_cmds.at(cmd))
                    std::cout << arg << " | Type: " << type << std::endl;

                
                std::cout << std::endl;
                exit(-1);
                return _T_{}; // just for compiler warning sake
            }

            return val;
        }
    } 

    std::cout << "PROGRAMMER ERROR: Type mismatch or unsupported type\n";
    exit(-1);
    return _T_{};
}

bool ljl::cmd::operator[](const std::string& cmd)
{
    return m_argv[1] == cmd;
}

// string
template std::string ljl::cmd::get_value<std::string>(const std::string&, const std::string&);
// integer types
template int8_t ljl::cmd::get_value<int8_t>(const std::string&, const std::string&);
template int16_t ljl::cmd::get_value<int16_t>(const std::string&, const std::string&);
template int32_t ljl::cmd::get_value<int32_t>(const std::string&, const std::string&);
template int64_t ljl::cmd::get_value<int64_t>(const std::string&, const std::string&);
// unsinged integer types
template uint8_t ljl::cmd::get_value<uint8_t>(const std::string&, const std::string&);
template uint16_t ljl::cmd::get_value<uint16_t>(const std::string&, const std::string&);
template uint32_t ljl::cmd::get_value<uint32_t>(const std::string&, const std::string&);
template uint64_t ljl::cmd::get_value<uint64_t>(const std::string&, const std::string&);
// decimal types
template float ljl::cmd::get_value<float>(const std::string&, const std::string&);
template double ljl::cmd::get_value<double>(const std::string&, const std::string&);
