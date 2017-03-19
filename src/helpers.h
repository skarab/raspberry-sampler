#ifndef __SAMPLER_HELPERS__
#define __SAMPLER_HELPERS__

#include "includes.h"

bool has_suffix(const std::string &str, const std::string &suffix);

#define TO_STRING( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

void XML_Write(pugi::xml_node& node, string name, string value);
void XML_Write(pugi::xml_node& node, string name, int value);
bool XML_Read(pugi::xml_node& node, string name, string& value);
bool XML_Read(pugi::xml_node& node, string name, int& value);

#endif

