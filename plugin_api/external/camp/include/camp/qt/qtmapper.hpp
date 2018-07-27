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


#include <camp/qt/qtsimpleproperty.hpp>
#include <camp/qt/qtfunction.hpp>
#include <camp/function.hpp>
#include <QMetaObject>
#include <QMetaMethod>


namespace camp_ext
{
/**
 * \brief Mapper for Qt properties
 *
 * This class tells CAMP how to build metaproperties and metafunctions from Qt's metatypes.
 *
 * This mapper is not complete due to some limitations of either CAMP or Qt:
 * \li enum properties are not mapped using camp::EnumProperty, but camp::SimpleProperty
 * \li arrays and user defined classes are not supported for meta-properties
 * \li arrays, user defined enums and classes are not supported for meta-functions
 */
template <typename T>
class QtMapper
{
public:

    /**
     * \brief Return the number of properties to map
     *
     * \return Number of properties
     */
    static std::size_t propertyCount()
    {
        int count = 0;
        for (int i = 0; i < T::staticMetaObject.propertyCount(); ++i)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.property(i)))
                count++;
        }

        return count;
    }

    /**
     * \brief Map a property
     *
     * \param index Index of the property to map
     *
     * \return index-th property mapped to a camp::Property
     */
    static camp::Property* property(std::size_t index)
    {
        int prop = 0;
        int count = 0;
        for (prop = 0; count <= static_cast<int>(index); ++prop)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.property(prop)))
                count++;
        }

        return new QtSimpleProperty<T>(T::staticMetaObject.property(prop - 1));
    }

    /**
     * \brief Return the number of functions to map
     *
     * \return Number of functions
     */
    static std::size_t functionCount()
    {
        int count = 0;
        for (int i = 0; i < T::staticMetaObject.methodCount(); ++i)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.method(i)))
                count++;
        }

        return count;
    }

    /**
     * \brief Map a function
     *
     * \param index Index of the function to map
     *
     * \return index-th function mapped to a camp::function
     */
    static camp::Function* function(std::size_t index)
    {
        int func = 0;
        int count = 0;
        for (func = 0; count <= static_cast<int>(index); ++func)
        {
            // Discard incompatible types
            if (filter(T::staticMetaObject.method(func)))
                count++;
        }

        return new QtFunction<T>(T::staticMetaObject.method(func - 1));
    }

private:

    /**
     * \brief Check if a property is mappable to CAMP
     *
     * \param property Property to check
     *
     * \return True if the property can be bound to CAMP
     */
    static bool filter(const QMetaProperty& property)
    {
        return QtHelper::type(property.type()) != camp::noType
               || property.isEnumType();
    }

    /**
     * \brief Check if a function is mappable to CAMP
     *
     * \param function Function to check
     *
     * \return True if the function can be bound to CAMP
     */
    static bool filter(const QMetaMethod& function)
    {
        // Check return type
        if ((QtHelper::type(function.typeName()) == camp::noType)
            && QMetaType::type(function.typeName()) != QMetaType::Void) // void maps to camp::noType but is a valid return type
        {
            return false;
        }

        // Check parameters types
        QList<QByteArray> args = function.parameterTypes();
        Q_FOREACH(QByteArray arg, args)
        {
            if (QtHelper::type(arg) == camp::noType)
                return false;
        }

        return true;
    }
};

} // namespace camp_ext
