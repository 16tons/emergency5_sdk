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


#include <camp/stringid.hpp>
#include <camp/detail/observernotifier.hpp>
#include <camp/detail/singleton.hpp>
#include <vector>


namespace camp
{
class Class;

namespace detail
{
/**
 * \brief Manages creation, storage, retrieval and destruction of metaclasses
 *
 * camp::ClassManager is the place where all metaclasses are stored and accessed.
 * It consists of a singleton which is created on first use and destroyed at global exit.
 *
 * \sa Class
 */
class CAMP_API ClassManager : public ObserverNotifier, public Singleton<ClassManager>
{
public:

    /**
     * \brief Get the unique instance of the class
     *
     * \return Reference to the unique instance of ClassManager
     */
    static ClassManager& instance();

    /**
     * \brief Create and register a new metaclass
     *
     * This is the entry point for every metaclass creation. This
     * function also notifies registered observers after successful creations.
     *
     * \param id ID (result of "camp::StringId(camp::Class::name())") of the metaclass, no reference by intent
     * \param name Name of the metaclass to create (must be unique), must stay valid as long as this instance exists
     *
     * \return Reference to the new metaclass
     *
     * \throw ClassAlreadyCreated \a name already exists
     */
    Class& addClass(StringId id, const char* name);

    /**
     * \brief Get the total number of metaclasses
     *
     * \return Number of metaclasses that have been registered
     */
    std::size_t count() const;

    /**
     * \brief Get a metaclass from its global index
     *
     * This function, together with ClassManager::count, provides a way to iterate through
     * all the metaclasses that have been declared.
     *
     * \param index Global index of the metaclass to get
     *
     * \return Reference to the index-th metaclass
     *
     * \throw OutOfRange index is out of range
     */
    const Class& getByIndex(std::size_t index) const;

    /**
     * \brief Get a metaclass from an ID
     *
     * \param id The ID (result of "camp::StringId(camp::Class::name())") of the metaclass, no reference by intent
     *
     * \return Reference to the requested metaclass
     *
     * \throw ClassNotFound id is not the name of an existing metaclass
     */
    const Class& getById(StringId id) const;

    /**
     * \brief Get a metaclass from an ID
     *
     * This version returns a null pointer if no metaclass is found, instead
     * of throwing an exception.
     *
     * \param id The ID (result of "camp::StringId(camp::Class::name())") of the metaclass, no reference by intent
     *
     * \return Pointer to the requested metaclass, or null pointer if not found
     */
    const Class* getByIdSafe(StringId id) const;

    /**
     * \brief Check if a given ID has a metaclass
     *
     * \param id The ID (result of "camp::StringId(camp::Class::name())") of the metaclass, no reference by intent
     *
     * \return True if the class exists, false otherwise
     */
    bool classExists(StringId id) const;

    /**
     * \brief Default constructor
     */
    ClassManager();

    /**
     * \brief Destructor
     *
     * The destructor destroys all the registered metaclasses and notifies the observers.
     */
    virtual ~ClassManager();

private:
    struct ClassEntry
    {
        uint32_t id;
        Class* classPtr;
        ClassEntry(uint32_t _id, Class* _classPtr) :
            id(_id),
            classPtr(_classPtr)
        {}
    };
    struct OrderByClassId
    {
        inline bool operator()(const ClassEntry& left, uint32_t right) const
        {
            return (left.id < right);
        }
    };
    typedef std::vector<ClassEntry> SortedClassVector; ///< Class ID sorted vector storing classes (for CPU cache efficient searches, "camp::ClassManager::ClassEntry" instead of a direct "camp::Class*" is used)
    SortedClassVector m_classes;
};

} // namespace detail

} // namespace camp
