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


#include <camp/classget.hpp>
#include <camp/classcast.hpp>
#include <camp/property.hpp>
#include <camp/function.hpp>
#include <camp/tagholder.hpp>
#include <camp/userobject.hpp>
#include <camp/detail/typeid.hpp>
#include <camp/detail/typename.hpp>
#include <boost/noncopyable.hpp>

namespace camp
{
template <typename T> class ClassBuilder;
class Constructor;
class Args;
class ClassVisitor;

/**
 * \brief camp::Class represents a metaclass composed of properties and functions
 *
 * camp::Class is the main class of the CAMP API. It defines a metaclass, which
 * is an abstract representation of a C++ class with its own properties,
 * functions, constructors, base classes, etc.
 *
 * Classes are declared, bound to a C++ type and filled with the \c declare
 * template function.
 *
 * \code
 * class MyClass
 * {
 * public:
 *
 *     MyClass();
 *     int getProp() const;
 *     void setProp(int);
 *     const char* func();
 * };
 *
 * camp::Class::declare<MyClass>()
 *     .tag("help", "this is my class")
 *     .constructor0()
 *     .property("prop", &MyClass::getProp, &MyClass::setProp)
 *     .function("func", &MyClass::func);
 * \endcode
 *
 * It then provides a set of accessors to retrieve its member functions and properties.
 *
 * \code
 * const camp::Class& metaclass = camp::classByType<MyClass>();
 *
 * const camp::Property& prop = metaclass.getPropertyById"prop");
 * const camp::Function& func = metaclass.getFunctionById("func");
 * \endcode
 *
 * Another way to inspect a class, which is more type-safe, is to use a ClassVisitor.
 *
 * \code
 * MyVisitor visitor;
 * metaclass.visit(visitor);
 * \endcode
 *
 * It also allows to create and destroy instances of the bound C++ class.
 *
 * \code
 * MyClass* obj = metaclass.construct<MyClass>();
 * metaclass.destroy(obj);
 * \endcode
 *
 * \remark All function and property names are unique within the metaclass.
 *
 * \sa Enum, TagHolder, ClassBuilder, Function, Property
 */
class CAMP_API Class : public TagHolder, public boost::noncopyable
{
public:

    /**
     * \brief Declare a new metaclass
     *
     * This is the function to call to create a new metaclass. The template
     * parameter T is the C++ class that will be bound to the metaclass.
     *
     * \return A ClassBuilder<T> object that will provide functions
     *         to fill the new metaclass with properties, functions, etc.
     */
    template <typename T>
    static ClassBuilder<T> declare();

    typedef std::shared_ptr<Function> FunctionPtr;
    struct FunctionEntry
    {
        uint32_t id;
        FunctionPtr functionPtr;
        FunctionEntry(uint32_t _id, FunctionPtr _functionPtr) :
            id(_id),
            functionPtr(_functionPtr)
        {}
    };

    typedef std::shared_ptr<Property> PropertyPtr;
    struct PropertyEntry
    {
        uint32_t id;
        PropertyPtr propertyPtr;
        PropertyEntry(uint32_t _id, PropertyPtr _propertyPtr) :
            id(_id),
            propertyPtr(_propertyPtr)
        {}
    };

    struct OrderByFunctionId
    {
        inline bool operator()(const FunctionEntry& left, uint32_t right) const
        {
            return (left.id < right);
        }
    };

    struct OrderByPropertyId
    {
        inline bool operator()(const PropertyEntry& left, uint32_t right) const
        {
            return (left.id < right);
        }
    };

public:

    /**
     * \brief Return the ID of the metaclass
     *
     * \return The ID (result of "camp::StringId(camp::Class::name())") of the metaclass
     */
    uint32_t id() const;

    /**
     * \brief Return the name of the metaclass
     *
     * \return String containing the name of the metaclass, always valid, do not destroy the instance
     */
    const char* name() const;

    /**
     * \brief Return the total number of base metaclasses of this metaclass
     *
     * \return Number of base metaclasses
     */
    std::size_t baseCount() const;

    /**
     * \brief Return a base metaclass from its index
     *
     * \param index Index of the base to get
     *
     * \return Reference to the index-th base metaclass of this metaclass
     *
     * \throw OutOfRange index is out of range
     */
    const Class& base(std::size_t index) const;

    /**
     * \brief Return the total number of functions of this metaclass
     *
     * \return Number of functions
     */
    std::size_t functionCount() const;

    /**
     * \brief Check if this metaclass contains the given function
     *
     * \param id ID (result of "camp::StringId(camp::Function::name())") of the function to check, no reference by intent
     *
     * \return True if the function is in the metaclass, false otherwise
     */
    bool hasFunction(StringId id) const;

    /**
     * \brief Get a function from its index in this metaclass
     *
     * \param index Index of the function to get, please note that the function order does not match the metafunction declaration order
     *
     * \return Reference to the function
     *
     * \throw OutOfRange index is out of range
     */
    const Function& getFunctionByIndex(std::size_t index) const;

    /**
     * \brief Get a function from its ID
     *
     * \param id ID (result of "camp::StringId(camp::Function::name())") of the function to get, no reference by intent
     *
     * \return Reference to the function
     *
     * \throw FunctionNotFound \a ID is not a function of the metaclass
     */
    const Function& getFunctionById(StringId id) const;

    /**
     * \brief Try to get a function from its ID
     *
     * \param id ID (result of "camp::StringId(camp::Function::name())") of the function to get, no reference by intent
     *
     * \return Pointer to the function, null pointer if the ID is not a function of the metaclass
     */
    const Function* tryGetFunctionById(StringId id) const;

    /**
     * \brief Return the total number of properties of this metaclass
     *
     * \return Number of properties
     */
    std::size_t propertyCount() const;

    /**
     * \brief Check if this metaclass contains the given property
     *
     * \param id ID (result of "camp::StringId(camp::Property::name())") of the property to check, no reference by intent
     *
     * \return True if the property is in the metaclass, false otherwise
     */
    bool hasProperty(StringId id) const;

    /**
     * \brief Get a property from its index in this metaclass
     *
     * \param index Index of the property to get, please note that the property order matches the metaproperty declaration order
     *
     * \return Reference to the property
     *
     * \throw OutOfRange index is out of range
     */
    const Property& getPropertyByIndex(std::size_t index) const;

    /**
     * \brief Get a property from its ID
     *
     * \param id ID (result of "camp::StringId(camp::Property::name())") of the property to get, no reference by intent
     *
     * \return Reference to the property
     *
     * \throw PropertyNotFound \a ID is not a property of the metaclass
     */
    const Property& getPropertyById(StringId id) const;

    /**
     * \brief Try to get a property from its ID
     *
     * \param id ID (result of "camp::StringId(camp::Property::name())") of the property to get, no reference by intent
     *
     * \return Pointer to the property, null pointer if the ID is not a property of the metaclass
     */
    const Property* tryGetPropertyById(StringId id) const;

    /**
    * \brief Return the total number of constructors of this metaclass
    *
    * \return Number of constructors
    */
    std::size_t constructorCount() const;

    /**
     * \brief Construct a new instance of the C++ class bound to the metaclass
     *
     * If no constructor can match the provided arguments, UserObject::nothing
     * is returned.
     * The new instance is wrapped into a UserObject. It must be destroyed
     * with the Class::destroy function.
     *
     * \param args Arguments to pass to the constructor, for example "camp::Args::empty"
     *
     * \return New instance wrapped into a UserObject, or UserObject::nothing if it failed
     */
    UserObject construct(const Args& args) const;

    /**
     * \brief Destroy an instance of the C++ class bound to the metaclass
     *
     * This function must be called to destroy every instance created with
     * Class::construct.
     *
     * \param object Object to be destroyed
     */
    void destroy(const UserObject& object) const;

    /**
     * \brief Start visitation of a class
     *
     * \param visitor Visitor to use for visitation
     */
    void visit(ClassVisitor& visitor) const;

    /**
     * \brief Convert a pointer to an object to be compatible with a base or derived metaclass
     *
     * The target metaclass may be a base or a derived of this, both cases are properly handled.
     *
     * \param pointer Pointer to convert
     * \param target Target metaclass to convert to
     *
     * \return Converted pointer
     *
     * \throw ClassUnrelated \a target is not a base nor a derived class of this
     */
    void* applyOffset(void* pointer, const Class& target) const;

    /**
     * \brief Operator == to check equality between two metaclasses
     *
     * Two metaclasses are equal if their name is the same.
     *
     * \param other Metaclass to compare with this
     *
     * \return True if both metaclasses are the same, false otherwise
     */
    bool operator==(const Class& other) const;

    /**
     * \brief Operator != to check inequality between two metaclasses
     *
     * \param other Metaclass to compare with this
     *
     * \return True if metaclasses are different, false if they are equal
     */
     bool operator!=(const Class& other) const;

private:

    template <typename T> friend class ClassBuilder;
    friend class ClassBuilderBase;
    friend class detail::ClassManager;

    /**
     * \brief Construct the metaclass from its name
     *
     * \param id ID (result of "camp::StringId(camp::Class::name())") of the metaclass, no reference by intent
     * \param name Name of the metaclass, must stay valid as long as this instance exists
     */
    explicit Class(StringId id, const char* name);

    /**
     * \brief Destructor
     */
    virtual ~Class();

    /**
     * \brief Get the offset of a base metaclass
     *
     * \param base Base metaclass to check
     *
     * \return offset between this and base, or -1 if both classes are unrelated
     */
    int baseOffset(const Class& base) const;

private:

    /**
     * \brief Structure holding informations about a base metaclass
     */
    struct BaseInfo
    {
        const Class* base;
        int offset;
    };

    typedef std::vector<Constructor*> ConstructorVector;
    typedef std::vector<BaseInfo> BaseVector;
    typedef std::vector<FunctionEntry> SortedFunctionVector; ///< Function ID sorted vector storing functions (for CPU cache efficient searches, "camp::Class::FunctionEntry" instead of a direct "camp::Class::FunctionPtr" is used)
    typedef std::vector<PropertyEntry> SortedPropertyVector;
    typedef void (*Destructor)(const UserObject&);

    StringId m_id; ///< The ID (result of "camp::StringId(camp::Class::name())") of the metaclass
    const char* m_name; ///< Name of the metaclass, must stay valid as long as this instance exists
    SortedFunctionVector m_functions;
    SortedPropertyVector m_propertiesByIndex; ///< Property index sorted vector storing properties (for CPU cache efficient searches, "camp::Class::PropertyEntry" instead of a direct "camp::Class::PropertyPtr" is used)
    SortedPropertyVector m_propertiesById;    ///< Property ID sorted vector storing properties (for CPU cache efficient searches, "camp::Class::PropertyEntry" instead of a direct "camp::Class::PropertyPtr" is used)
    BaseVector m_bases; ///< List of base metaclasses
    ConstructorVector m_constructors; ///< List of metaconstructors
    Destructor m_destructor; ///< Destructor (function that is able to delete an abstract object)
};

} // namespace camp

#include <camp/class.inl>
