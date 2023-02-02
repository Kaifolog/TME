#ifndef _PROJECTNAME
#define _PROJECTNAME

#include <string>
#include <utility>

namespace tools
{

class ProjectName
{
    std::string _original;
    std::string _log_file;
    std::string _db_file;
    std::string _out_file;

  public:
    ProjectName() = default;
    ProjectName(std::string new_original)
    {
        setOriginal(std::move(new_original));
    };
    void setOriginal(std::string new_original)
    {
        _original = std::move(new_original);
        std::string cutted_original = _original;
        if (cutted_original.substr(cutted_original.find_last_of('.') + 1) == "tme" ||
            cutted_original.substr(cutted_original.find_last_of('.') + 1) == "txt")
        {
            cutted_original.pop_back();
            cutted_original.pop_back();
            cutted_original.pop_back();
            cutted_original.pop_back();
        }
        _log_file = cutted_original;
        _log_file.append("_log.txt");
        _db_file = cutted_original;
        _db_file.append(".db");
        _out_file = cutted_original;
        _out_file.append("_out.txt");
    };
    std::string getOriginal()
    {
        return _original;
    };
    std::string getLogFile()
    {
        return _log_file;
    };
    std::string getDBFile()
    {
        return _db_file;
    };
    std::string getOutFile()
    {
        return _out_file;
    };
    bool empty()
    {
        return _original.empty();
    };
    void clear()
    {
        _original.clear();
        _log_file.clear();
        _db_file.clear();
        _out_file.clear();
    };
};

} // namespace tools

#endif