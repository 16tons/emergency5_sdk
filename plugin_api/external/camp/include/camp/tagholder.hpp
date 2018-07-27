/****************************************************************************
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Contact: Tegesoft Information (contact@tegesoft.com)
**
** This file is part of the CAMP library.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#pragma once


#include <camp/detail/getter.hpp>
#include <camp/value.hpp>
#include <vector>


namespace camp
{
class UserObject;

/**
 * \brief Defines objects that hold a set of tags
 *
 * TagHolder defines an interface for accessing custom tags stored into instances.
 *
 * A tag is an identifier.
 * It can also be associated to a value, either constant (static tags) or evaluated each
 * time the tag is requested (dynamic tags).
 *
 * \code
 * // Check tag existence
 * bool scriptable = tagHolder.hasTag("scriptable");
 *
 * // Retrieve static tag
 * int category = tagHolder.tag("category");
 *
 * // Retrieve dynamic tag whose value depends on an object's state
 * bool hidden = tagHolder.tag("hidden", object);
 * \endcode
 */
class CAMP_API TagHolder
{
public:

    struct TagEntry
    {
        StringId id; ///< The ID (result of tag name string hashing) of the metaclass
        const char* name; ///< Name of the metatag, must stay valid as long as this instance exists
        detail::Getter<Value> value;
        TagEntry(StringId _id, const char* _name, const detail::Getter<Value>& _value) :
            id(_id),
            name(_name),
            value(_value)
        {}
    };

    /**
     * \brief Destructor
     */
    virtual ~TagHolder();

    /**
     * \brief Get the total number of tags stored in the object
     *
     * \return Number of tags
     */
    std::size_t tagCount() const;

    /**
     * \brief Get a tag entry by its index
     *
     * \param index Index of the tag to retrieve, please note that the tag order does not match the metatag declaration order
     *
     * \return index-th tag
     *
     * \throw OutOfRange index is out of range
     */
    const TagEntry& getTagEntryByIndex(std::size_t index) const;

    /**
     * \brief Check the existence of a tag
     *
     * \param id Identifier (result of tag name string hashing) of the tag to check, no reference by intent
     *
     * \return True if the tag exists, false otherwise
     */
    bool hasTag(StringId id) const;

    /**
     * \brief Get the value associated to a tag
     *
     * \param id Identifier (result of tag name string hashing) of the tag to get, no reference by intent
     *
     * \return Value associated to the tag, or Value::nothing if the tag doesn't exist
     */
    const Value& tag(StringId id) const;

    /**
     * \brief Get the value associated to a dynamic tag
     *
     * \param id Identifier (result of tag name string hashing) of the tag to get, no reference by intent
     * \param object User object for which to get the tag value 
     *
     * \return Value associated to the tag for \a object, or Value::nothing if the tag doesn't exist
     */
    Value tag(StringId id, const UserObject& object) const;

    /**
     * \brief Try to get the value associated to a tag
     *
     * \param id Identifier (result of tag name string hashing) of the tag to get, no reference by intent
     *
     * \return Value associated to the tag, or null pointer if the tag doesn't exist
     */
    const Value* tryGetTagById(StringId id) const;

protected:

    /**
     * \brief Default constructor
     */
    TagHolder();

private:

    friend class ClassBuilderBase;

    struct OrderByTagId
    {
        inline bool operator()(const TagHolder::TagEntry& left, uint32_t right) const
        {
            return (left.id < right);
        }
    };

    typedef std::vector<TagEntry> SortedTagVector; ///< Tag ID sorted vector storing tags
    SortedTagVector m_tags;
};

} // namespace camp
