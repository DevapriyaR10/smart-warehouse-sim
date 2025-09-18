#pragma once
#include <iostream>
#include <string>
inline void log_info(const std::string &s) { std::cout << "[INFO] " << s << std::endl; }
inline void log_warn(const std::string &s) { std::cout << "[WARN] " << s << std::endl; }
inline void log_err(const std::string &s) { std::cerr << "[ERROR] " << s << std::endl; }
