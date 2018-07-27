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


#include <camp/enumbuilder.hpp>
#include <camp/enumget.hpp>
#include <camp/detail/typename.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>


namespace bm = boost::multi_index;

namespace camp
{
/**
 * \brief camp::Enum represents a metaenum composed of <name, value> pairs
 *
 * Enums are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * enum MyEnum {one = 1, two = 2, ten = 10};
 *
 * camp::Enum::declare<MyEnum>()
 *     .value("one", one)
 *     .value("two", two)
 *     .value("ten", ten);
 * \endcode
 *
 * It then provides a set of accessors to retrieve names, values and pairs contained in it.
 *
 * \code
 * const camp::Enum& metaenum = camp::enumByType<MyEnum>();
 *
 * bool b1 = metaenum.hasName("one");     // b1 == true
 * bool b2 = metaenum.hasValue(5);        // b2 == false
 *
 * const char* s = metaenum.name(10);     // s == "ten"
 * long l = metaenum.value("two");        // l == 2
 *
 * camp::Enum::Pair p = metaenum.pair(0); // p == {"one", one}
 * \endcode
 *
 * \remark All values and names are unique within the metaenum.
 *
 * \sa Class, EnumBuilder
 */
class CAMP_API Enum : public boost::noncopyable
{
public:

    /**
     * \brief Structure defining the <name, value> pairs stored in metaenums
     */
    struct Pair
    {
        std::string name; ///< Name of the pair
        long value; ///< Value of the pair
    };

public:

    /**
     * \brief Declare a new metaenum
     *
     * This is the function to call to create a new metaenum. The template
     * parameter T is the C++ enum type that will be bound to the metaclass.
     *
     * \return A EnumBuilder object that will provide functions
     *         to fill the new metaenum with values.
     */
    template <typename T>
    static EnumBuilder declare();

public:

    /**
     * \brief Get the ID of the metaenum
     *
     * \return ID (result of "camp::StringId(camp::Enum::name())") of the metaenum
     */
    uint32_t id() const;

    /**
     * \brief Return the name of the metaenum
     *
     * \return String containing the name of the metaenum, always valid
     */
    const char* name() const;

    /**
     * \brief Return the size of the metaenum
     *
     * \return Total number of values contained in the metaenum
     */
    std::size_t size() const;

    /**
     * \brief Get a pair by its index
     *
     * \param index Index of the pair to get
     *
     * \return index-th pair
     *
     * \throw OutOfRange index is out of range
     */
    const Pair& pair(std::size_t index) const;

    /**
     * \brief Check if the enum contains a name
     *
     * \param name Name to check
     *
     * \return True if the metaenum contains a pair whose name is \a name
     */
    bool hasName(const std::string& name) const;

    /**
     * \brief Check if the enum contains a value
     *
     * \param value Value to check
     *
     * \return True if the metaenum contains a pair whose value is \a value
     */
    bool hasValue(long value) const;

    /**
     * \brief Return the name corresponding to given a value
     *
     * \param value Value to get
     *
     * \return Name of the requested value
     *
     * \throw InvalidEnumValue value doesn't exist in the metaenum
     */
    const std::string& name(long value) const;

    /**
     * \brief Return the value corresponding to given a name
     *
     * \param name Name to get
     *
     * \return Value of the requested name
     *
     * \throw InvalidEnumName name doesn't exist in the metaenum
     */
    long value(const std::string& name) const;

    /**
     * \brief Operator == to check equality between two metaenums
     *
     * Two metaenums are equal if their name is the same.
     *
     * \param other Metaenum to compare with this
     *
     * \return True if both metaenums are the same, false otherwise
     */
    bool operator==(const Enum& other) const;

    /**
     * \brief Operator != to check inequality between two metaenums
     *
     * \param other Metaenum to compare with this
     *
     * \return True if metaenums are different, false if they are equal
     */
    bool operator!=(const Enum& other) const;

private:

    friend class EnumBuilder;
    friend class detail::EnumManager;

    /**
     * \brief Construct the metaenum from its name
     *
     * \param id ID (result of "camp::StringId(camp::Enum::name())") of the metaenum, no reference by intent
     * \param name Name of the metaenum, must stay valid as long as this instance exists
     */
    explicit Enum(StringId id, const char* name);

    struct Id;
    struct Val;
    struct Name;

    typedef boost::multi_index_container<Pair,
        bm::indexed_by<bm::random_access<bm::tag<Id> >,
                       bm::ordered_unique<bm::tag<Val>, bm::member<Pair, long, &Pair::value> >,
                       bm::ordered_unique<bm::tag<Name>, bm::member<Pair, std::string, &Pair::name> >
        >
    > PairTable;

    typedef PairTable::index<Val>::type ValueIndex;
    typedef PairTable::index<Name>::type NameIndex;

    StringId m_id; ///< ID (result of "camp::StringId(camp::Enum::name())") of the metaenum
    const char* m_name; ///< Name of the metaenum, must stay valid as long as this instance exists
    PairTable m_pairs; ///< Table of pairs, indexed by their value and name
};

} // namespace camp

#include <camp/enum.inl>
