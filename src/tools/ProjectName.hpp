#include <string>

#ifndef _PROJECTNAME
#define _PROJECTNAME

class ProjectName
{
        std::string original;
        std::string log_file;
        std::string db_file;
        std::string out_file;

public:
        ProjectName(){};
        ProjectName(std::string new_original) { setOriginal(new_original); };
        void setOriginal(std::string new_original)
        {
                original = new_original;
                std::string cutted_original = original;
                if (cutted_original.substr(cutted_original.find_last_of(".") + 1) == "tme" || cutted_original.substr(cutted_original.find_last_of(".") + 1) == "txt")
                {
                        cutted_original.pop_back();
                        cutted_original.pop_back();
                        cutted_original.pop_back();
                        cutted_original.pop_back();
                }
                log_file = cutted_original;
                log_file.append("_log.txt");
                db_file = cutted_original;
                db_file.append(".db");
                out_file = cutted_original;
                out_file.append("_out.txt");
        };
        std::string getOriginal() { return original; };
        std::string getLogFile() { return log_file; };
        std::string getDBFile() { return db_file; };
        std::string getOutFile() { return out_file; };
        bool empty() { return original.empty(); };
        void clear()
        {
                original.clear();
                log_file.clear();
                db_file.clear();
                out_file.clear();
        };
};

#endif