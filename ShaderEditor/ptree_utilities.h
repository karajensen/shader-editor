////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ptree_utilities.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/lexical_cast.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/filesystem.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "logger.h"

namespace
{
    /**
    * Gets the value if it exists in the tree or returns defaultValue
    * @param itr Iterator from the tree to check 
    * @param defaultValue The default value to use if name is not found
    * @param name The name of the node to search
    * @return the chosen value from either the tree or defaultValue
    */
    template<typename T> T GetValueOptional(boost::property_tree::ptree::iterator& itr, 
                                            T defaultValue, 
                                            const std::string& name)
    {
        return itr->second.count(name) > 0 ?
            GetValue<T>(itr, name) : defaultValue;
    }

    /**
    * Gets the value of the node
    * @param node Node from the tree to check 
    * @param name The name of the node to search
    * @return the value of the node
    */
    template<typename T> T GetValue(boost::property_tree::ptree& node, 
                                    const std::string& name)
    {
        if (node.count(name) == 0)
        {
            Logger::LogError("Could not find entry for " + name);
            return T();
        }
        return boost::lexical_cast<T>(node.get_child(name).data());
    }

    /**
    * Gets the value of the node
    * @param itr Iterator from the tree to check 
    * @param name The name of the node to search
    * @return the value of the node
    */
    template<typename T> T GetValue(boost::property_tree::ptree::iterator& itr, 
                                    const std::string& name)
    {
        return GetValue<T>(itr->second, name);
    }

    /**
    * Gets the attribute of the node
    * @param itr Iterator from the tree to get 
    * @param name The name of the node to search
    * @param attr The attribute of the nodeto get
    * @return the chosen value from either the tree
    */
    template<typename T> T GetAttribute(boost::property_tree::ptree::iterator& itr, 
                                        const char* name, 
                                        const char* attr)
    {
        return GetAttribute<T>(itr->second, name, attr);
    }

    /**
    * Gets the attribute of the node
    * @param itr Iterator from the tree to get 
    * @param name The name of the node to search
    * @param attr The attribute of the nodeto get
    * @return the chosen value from either the tree
    */
    template<typename T> T GetAttribute(boost::property_tree::ptree& node,
                                        const char* name, 
                                        const char* attr)
    {
        return boost::lexical_cast<T>(node.get_child(
            name).get_child("<xmlattr>").get_child(attr).data());
    }

    /**
    * Gets the attribute of the node optionally
    * @param itr Iterator from the tree to get 
    * @param name The name of the node to search
    * @param attr The attribute of the nodeto get
    * @param defaultValue The value to use if it doesn't exist
    * @return the chosen value from either the tree or optional
    */
    template<typename T> T GetAttributeOptional(boost::property_tree::ptree::iterator& itr, 
                                                const char* name, 
                                                const char* attr, 
                                                T defaultValue)
    {
        return itr->second.count(name) > 0 ?
            GetAttribute<T>(itr, name, attr) : defaultValue;
    }

    /**
    * Adds a value to the node if it doesn't equal the default value
    * @param node The tree to add to
    * @param name The name of the value to add
    * @param value The value to add 
    * @param defaultValue The value to check against
    */
    template<typename T> void AddValueOptional(boost::property_tree::ptree& node,
                                               const char* name,
                                               T value,
                                               T defaultValue)
    {
        if (value != defaultValue)
        {
            node.add(name, value);
        }
    }

    /**
    * Saves the tree to a file
    * @param root The first entry
    * @param tree The data to save
    * @param name The name of the file to save
    * @param path The path to the file
    */
    void SaveXMLFile(boost::property_tree::ptree& root,
                     boost::property_tree::ptree& tree,
                     const std::string& name,
                     const std::string& path)
    {
        root.add_child(name, tree);
        boost::filesystem::path filePath(path);
        boost::property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
        boost::property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
    }

    /**
    * Reads an xml file into a tree
    * @param name The name of the file to read
    * @param path The path to the file
    * @return the tree generated
    */
    boost::property_tree::ptree ReadXMLFile(const std::string& name, 
                                            const std::string& path)
    {
        boost::property_tree::ptree root;
        boost::property_tree::xml_parser::read_xml(path, root, 
            boost::property_tree::xml_parser::trim_whitespace);
        return root.get_child(name);
    }
}
