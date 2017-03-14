#include "helpers.h"

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size()>=suffix.size()
        && str.compare(str.size()-suffix.size(), suffix.size(), suffix)==0;
}
