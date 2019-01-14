#include "BuildTimestamp.hh"

#include <string>
#include <sstream>


std::string buildTimestamp() {
    std::stringstream ss;
    ss << __DATE__;
    ss << " ";
    ss << __TIME__;
    return ss.str();
}
