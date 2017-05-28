#include "helpers.h"

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size()>=suffix.size()
        && str.compare(str.size()-suffix.size(), suffix.size(), suffix)==0;
}

void XML_Write(pugi::xml_node& node, string name, string value)
{
    pugi::xml_node xml_node = node.append_child(name.c_str());
    pugi::xml_node xml_data = xml_node.append_child(pugi::node_pcdata);
    xml_data.set_value(value.c_str());
}

void XML_Write(pugi::xml_node& node, string name, int value)
{
    pugi::xml_node xml_node = node.append_child(name.c_str());
    pugi::xml_node xml_data = xml_node.append_child(pugi::node_pcdata);
    xml_data.set_value(TO_STRING(value).c_str());
}

bool XML_Read(const pugi::xml_node& node, string name, string& value)
{
    pugi::xml_node xml_node = node.child(name.c_str()).first_child();
    if (xml_node)
    {
        value = xml_node.value();
        return true;
    }
    return false;
}

bool XML_Read(const pugi::xml_node& node, string name, int& value)
{
    pugi::xml_node xml_node = node.child(name.c_str()).first_child();
    if (xml_node)
    {
        value = atoi(xml_node.value());
        return true;
    }
    return false;
}
