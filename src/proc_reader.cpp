#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/types.h>

class Process
{
public:
    long get_mem_kb(int pid)
    {
        std::ifstream file("/proc/" + std::to_string(pid) + "/status");
        std::string line;

        while (std::getline(file, line))
        {
            if (line.rfind("VmRSS:", 0) == 0)
            {
                std::istringstream iss(line);
                std::string key;
                long value;
                std::string unit;

                iss >> key >> value >> unit;
                return value;
            }
        }
        return 0;
    }
    std::string get_name(int pid)
    {
        std::ifstream file("/proc/" + std::to_string(pid) + "/status");
        std::string line;

        while (std::getline(file, line))
        {
            if (line.rfind("Name:", 0) == 0)
            {
                const char *p = line.c_str();

                while (*p != ':')
                    ++p;

                if (*p == ':')
                    ++p;

                while (*p == ' ' || *p == '\t')
                    ++p;

                return std::string(p);
            }
        }
        return "";
    }
};

int main()
{
    Process process_obj;
    std::string::difference_type max_length_name = 0;

    int counter = 0;
    int limit = 2000;

    std::cout << std::left
              << std::setw(10) << "PID"
              << std::setw(25) << "NAME"
              << std::setw(11) << "MEM(kB)"
              << "\n\n";

    for (const auto &entry : std::filesystem::directory_iterator("/proc"))
    {
        if (counter >= limit)
            break;

        const auto name = entry.path().filename().string();
        if (std::all_of(name.begin(), name.end(), ::isdigit))
        {
            int pid = std::stoi(name);
            long mem_kb = process_obj.get_mem_kb(pid);
            std::string name_pid = process_obj.get_name(pid);
            auto name_len = static_cast<std::string::difference_type>(name_pid.size());
            max_length_name = std::max(max_length_name, name_len);

            if (mem_kb == 0)
                continue;

            std::cout << std::left
                      << std::setw(10) << pid
                      << std::setw(25) << name_pid
                      << std::setw(11) << mem_kb
                      << "\n";
            counter++;
        }
    }
}
