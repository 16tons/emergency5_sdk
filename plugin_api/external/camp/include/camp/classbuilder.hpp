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


#include <camp/type.hpp>
#include <camp/classbuilderbase.hpp>
#include <camp/classget.hpp>
#include <camp/detail/functionimpl.hpp>
#include <camp/detail/functiontraits.hpp>
#include <camp/detail/constructorimpl.hpp>
#include <camp/detail/propertyfactory.hpp>
#include <boost/mpl/if.hpp>
#include <boost/function_types/function_type.hpp>
#include <cassert>


namespace camp
{
/**
 * \brief Proxy class which fills a metaclass with its members
 *
 * This class is returned by Class::declare<T> in order construct a
 * new metaclass. It contains functions to declare and bind metaproperties,
 * metafunctions, base metaclasses, metaconstructors, etc. with many overloads
 * in order to accept as many types of binds as possible.
 *
 * ClassBuilder also contains functions to set attributes of metafunctions
 * and metaproperties.
 *
 * This class should never be explicitely instanciated, unless you
 * need to split the metaclass creation in multiple parts.
 */
template <typename T>
class ClassBuilder : public ClassBuilderBase
{
public:

    /**
     * \brief Construct the builder with a target metaclass to fill
     *
     * \param target Metaclass to build
     */
    explicit ClassBuilder(Class& target);

    /**
     * \brief Declare a base metaclass
     *
     * The template parameter U is the C++ base class of T.
     *
     * This function makes the target metaclass inherit of all the metaproperties and
     * metafunctions of the given base metaclass.
     *
     * \return Reference to this, in order to chain other calls
     *
     * \throw ClassNotFound no metaclass is bound to U
     */
    template <typename U>
    ClassBuilder<T>& base();

    /**
     * \brief Declare a new property from a single accessor
     *
     * The accessor argument can be a getter of any valid type, or a direct
     * pointer-to-member (which is considered both a getter and a setter)
     *
     * Example:
     *
     * \code
     * struct Point
     * {
     *     float x;
     *     float y;
     *
     *     float length() const;
     * };
     *
     * camp::Class::declare<Point>()
     *     .property("x",      &Point::x);      // getter + setter
     *     .property("y",      &Point::y);      // getter + setter
     *     .property("length", &Point::length); // getter only
     * \endcode

     * \param name Name of the property (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param accessor Accessor to the C++ implementation of the property
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& property(const char* name, F accessor);

    /**
     * \brief Declare a new property from a pair of accessors
     *
     * The accessor1 and accessor2 arguments can be a pair of getter/setter, or
     * two getters which must be composed to form a single getter.
     * If F1 is a direct pointer-to-member, it is considered both a getter and a setter.
     *
     * Having two getters allows to expose a property which requires an extra level of indirection to be accessed
     * (for example, a property of a member of the class instead of a property of the class itself).
     *
     * Example:
     *
     * \code
     * struct Point {float x, y;};
     * 
     * class Entity
     * {
     * public:
     *
     *     Point p;
     * };
     *
     * camp::Class::declare<Entity>()
     *     .property("x", &Point::x, &Entity::p); // will internally resolve to e.p.x
     *     .property("y", &Point::y, &Entity::p); // will internally resolve to e.p.y
     * \endcode
     *
     * \param name Name of the property (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param accessor1 First accessor to the C++ implementation of the property (getter)
     * \param accessor2 Second accessor to the C++ implementation of the property (setter or getter to compose)
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F1, typename F2>
    ClassBuilder<T>& property(const char* name, F1 accessor1, F2 accessor2);

    /**
     * \brief Declare a new property from three accessors
     *
     * The accessor1 and accessor2 arguments are a pair of getter/setter, accessor3 is
     * an accessor to compose to accessor1 and accessor2 to get the final accessors.
     *
     * This allows to expose a property which requires an extra level of indirection to be accessed
     * (for example, a property of a member of the class instead of a property of the class itself).
     *
     * Example:
     *
     * \code
     * struct Point
     * {
     *     float getX() const;
     *     void setX(float);
     *
     *     float getY() const;
     *     void setY(float);
     * };
     * 
     * class Entity
     * {
     * public:
     *
     *     Point p;
     * };
     *
     * camp::Class::declare<Entity>()
     *     .property("x", &Point::getX, &Point::setX, &Entity::p); // will internally resolve to e.p.get/setX()
     *     .property("y", &Point::getY, &Point::setY, &Entity::p); // will internally resolve to e.p.get/setY()
     * \endcode
     *
     * \param name Name of the property (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param accessor1 First accessor (getter)
     * \param accessor2 Second accessor (setter)
     * \param accessor3 Third accessor (getter)
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F1, typename F2, typename F3>
    ClassBuilder<T>& property(const char* name, F1 accessor1, F2 accessor2, F3 accessor3);

    /**
     * \brief Declare a new function from any bindable type
     *
     * The function argument can be any valid type: a non-member function,
     * a member function, const, non-const, etc.
     *
     * \param name Name of the function (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param function C++ callable entity to bind to the function
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& function(const char* name, F function);

    /**
     * \brief Declare a new function from a boost::function
     *
     * Overload handling functions of type boost::function.
     *
     * \param name Name of the function (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param function Instance of boost::function to bind to the function
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& function(const char* name, boost::function<F> function);

    /**
     * \brief Declare a new function from two functions to compose
     *
     * This allows to expose a function which requires an extra level of indirection to be accessed
     * (for example, a function of a member of the class instead of a function of the class itself).
     *
     * Example:
     *
     * \code
     * struct Point
     * {
     *     void move(float x, float y);
     * };
     * 
     * class Entity
     * {
     * public:
     *
     *     Point p;
     * };
     *
     * camp::Class::declare<Entity>()
     *     .function("move", &Point::move, &Entity::p); // will internally resolve to e.p.move()
     * \endcode
     *
     * \param name Name of the function (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param function1 C++ callable entity to bind to the function
     * \param function2 Accessor returning the member to apply to function1
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F1, typename F2>
    ClassBuilder<T>& function(const char* name, F1 function1, F2 function2);

    /**
     * \brief Declare a new static tag
     *
     * \param name Name of the tag (must be unique within the metaclass), must stay valid as long as this instance exists
     *
     * \return Reference to this, in order to chain other calls
     */
    ClassBuilder<T>& tag(const char* name);

    /**
     * \brief Declare a new tag with value
     *
     * \param name Name of the tag (must be unique within the metaclass), must stay valid as long as this instance exists
     * \param value Value associated to the tag. It can be a static value, or a function
     *              which will be evaluated each time the tag will be requested.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename U>
    ClassBuilder<T>& tag(const char* name, const U& value);

    /**
     * \brief Set the readable state of the current property with a static value
     *
     * \param value True to set the current property as readable, false otherwise
     *
     * \return Reference to this, in order to chain other calls
     */
    ClassBuilder<T>& readable(bool value);

    /**
     * \brief Set the readable state of the current property with a dynamic value
     *
     * function can be any C++ callable type, and will be called to return the
     * readable state of the property each time it is requested. This way, the readable
     * state of a property can depend on metaclass instances.
     *
     * \param function Function to call to get the readable state of the property
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& readable(F function);

    /**
     * \brief Set the writable state of the current property with a static value
     *
     * \param value True to set the current property as writable, false otherwise
     *
     * \return Reference to this, in order to chain other calls
     */
    ClassBuilder<T>& writable(bool value);

    /**
     * \brief Set the writable state of the current property with a dynamic value
     *
     * function can be any C++ callable type, and will be called to return the
     * writable state of the property each time it is requested. This way, the writable
     * state of a property can depend on metaclass instances.
     *
     * \param function Function to call to get the writable state of the property
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& writable(F function);

    /**
     * \brief Set the callable state of the current function with a static value
     *
     * \param value True to set the current function as callable, false otherwise
     *
     * \return Reference to this, in order to chain other calls
     */
    ClassBuilder<T>& callable(bool value);

    /**
     * \brief Set the callable state of the current function with a dynamic value
     *
     * function can be any C++ callable type, and will be called to return the
     * callable state of the function each time it is requested. This way, the callable
     * state of a function can depend on metaclass instances.
     *
     * \param function Function to call to get the callable state of the function
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename F>
    ClassBuilder<T>& callable(F function);

    /**
     * \brief Declare a default constructor for the metaclass
     *
     * \return Reference to this, in order to chain other calls
     */
    ClassBuilder<T>& constructor0();

    /**
     * \brief Declare a constructor with 1 parameter for the metaclass
     *
     * A0 is the type of the parameter
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename A0>
    ClassBuilder<T>& constructor1();

    /**
     * \brief Declare a constructor with 2 parameters for the metaclass
     *
     * A0 is the type of the first parameter.
     * A1 is the type of the second parameter.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename A0, typename A1>
    ClassBuilder<T>& constructor2();

    /**
     * \brief Declare a constructor with 3 parameters for the metaclass
     *
     * A0 is the type of the first parameter.
     * A1 is the type of the second parameter.
     * A2 is the type of the third parameter.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename A0, typename A1, typename A2>
    ClassBuilder<T>& constructor3();

    /**
     * \brief Declare a constructor with 4 parameters for the metaclass
     *
     * A0 is the type of the first parameter.
     * A1 is the type of the second parameter.
     * A2 is the type of the third parameter.
     * A3 is the type of the fourth parameter.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename A0, typename A1, typename A2, typename A3>
    ClassBuilder<T>& constructor4();

    /**
     * \brief Declare a constructor with 5 parameters for the metaclass
     *
     * A0 is the type of the first parameter.
     * A1 is the type of the second parameter.
     * A2 is the type of the third parameter.
     * A3 is the type of the fourth parameter.
     * A4 is the type of the fifth parameter.
     *
     * \return Reference to this, in order to chain other calls
     */
    template <typename A0, typename A1, typename A2, typename A3, typename A4>
    ClassBuilder<T>& constructor5();

    /**
     * \brief Add properties and/or functions from an external source
     *
     * The purpose of this function is to allow the binding of classes
     * that already use a similar system of metaproperties and metafunctions,
     * with a direct mapping from external attributes to CAMP ones.
     *
     * The mapping process must be done in a specific mapper class (see below), thus avoiding
     * to manually write the mapping for every class.
     *
     * The mapper class must accept a template parameter (which is the target C++ class)
     * and be compatible with the following interface:
     *
     * \code
     * template <typename T>
     * class MyClassMapper
     * {
     * public:
     *
     *     MyClassMapper();
     *
     *     size_t propertyCount();
     *     camp::Property* property(size_t index);
     *
     *     size_t functionCount();
     *     camp::Function* function(size_t index);
     * };
     * \endcode
     *
     * Example of usage:
     *
     * \code
     * camp::Class::declare<MyClass>()
     *     .external<MyClassMapper>()
     *     ...
     * \endcode
     *
     * \return Reference to this, in order to chain other calls
     */
    template <template <typename> class U>
    ClassBuilder<T>& external();
};

} // namespace camp

#include <camp/classbuilder.inl>
