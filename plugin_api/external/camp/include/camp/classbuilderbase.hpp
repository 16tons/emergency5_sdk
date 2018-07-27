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


#include <camp/value.hpp>
#include <camp/detail/getter.hpp>


namespace camp
{
class Class;
class Property;
class Function;
class TagHolder;
class Constructor;

/**
 * \brief Class builder base class
 */
class CAMP_API ClassBuilderBase
{
public:

    /**
     * \brief Construct the builder with a target metaclass to fill
     *
     * \param target Metaclass to build
     */
    explicit ClassBuilderBase(Class& target);

    /**
    * \brief Return the target metaclass to fill
    *
    * \return Reference to the target metaclass to fill
    */
    Class& getClass();
protected:

    /**
     * \brief Add a new base class to the target class
     *
     * \param baseClass Base class to add
     * \param offset Offset of the base class to add
     */
    void addBase(Class& baseClass, int offset);

    /**
     * \brief Add a new constructor to the target class
     *
     * \param constructor Constructor to add
     */
    void addConstructor(Constructor* constructor);

    /**
     * \brief Add a new property to the target class
     *
     * \param property Property to add
     */
    void addProperty(Property* property);

    /**
     * \brief Add a new function to the target class
     *
     * \param function Function to add
     */
    void addFunction(Function* function);

    /**
     * \brief Add a new tag to the target class
     *
     * \param name Name of the tag (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param value Tag value
     */
    void addTag(const char* name, const detail::Getter<Value>& value);

    Class* m_target; ///< Target metaclass to fill
    TagHolder* m_currentTagHolder; ///< Last tag holder which has been declared
    Property* m_currentProperty; ///< Last metaproperty which has been declared
    Function* m_currentFunction; ///< Last function which has been declared
};

} // namespace camp
