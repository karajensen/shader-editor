////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ptree_utilities.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/lexical_cast.hpp"
#include "boost/property_tree/ptree.hpp"
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
    template<typename T> 
    T GetValueOptional(boost::property_tree::ptree::iterator& itr, 
                       T defaultValue, 
                       const std::string& name)
    {
        return itr->second.count(name) > 0 ?
            boost::lexical_cast<T>(itr->second.get_child(name).data()) :
            defaultValue;
    }

    /**
    * Gets the value of the node
    * @param node Node from the tree to check 
    * @param name The name of the node to search
    * @return the value of the node
    */
    template<typename T> 
    T GetValue(boost::property_tree::ptree& node, const std::string& name)
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
    template<typename T> 
    T GetValue(boost::property_tree::ptree::iterator& itr, const std::string& name)
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
    template<typename T> 
    T GetAttribute(boost::property_tree::ptree::iterator& itr, const char* name, const char* attr)
    {
        return boost::lexical_cast<T>(itr->second.get_child(
            name).get_child("<xmlattr>").get_child(attr).data());
    }
}
