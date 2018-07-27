// Copyright (C) 2012-2017 Promotion Software GmbH


// This lightweight header contains everything we need to connect new classes inside headers to the CAMP reflection system.
// Without this header you would have to include
//    #include <camp/camptype.hpp>		// For "CAMP_TYPE_NONCOPYABLE()"
//    #include <camp/detail/typeid.hpp>	// Required to ensure "CAMP_RTTI()" will cause no linker issues (which may not happen in one situation, but in other situations, so, just include this to be on the safe side)
// which would slow down the build process. ("camp/camptype.hpp" is lightweight while "camp/detail/typeid.hpp" has quite complex header dependencies)


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/stringid.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace camp
{
	namespace detail
	{
		// From "camp/detail/rawtype.hpp" (names changed to avoid collisions)
		template <typename T, typename E = void>
		struct QsfRawType
		{
			typedef T Type;
		};
		template <typename T> struct QsfRawType<const T>
		{
			typedef typename QsfRawType<T>::Type Type;
		};
		template <typename T> struct QsfRawType<T&>
		{
			typedef typename QsfRawType<T>::Type Type;
		};
		template <typename T>
		struct QsfRawType<T*>
		{
			typedef typename QsfRawType<T>::Type Type;
		};

		// From "camp/camptype.hpp" (names changed to avoid collisions)
		template <typename T> struct StaticTypeId;
		template <typename T> uint32_t QsfStaticTypeId(const T&) {return StaticTypeId<typename QsfRawType<T>::Type>::get();}
		template <typename T> struct StaticTypeName;
		template <typename T> const char* QsfStaticTypeName(const T&) {return StaticTypeName<typename QsfRawType<T>::Type>::get();}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // detail
} // camp


//[-------------------------------------------------------]
//[ QSF macros as direct replacement of CAMP macros       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Only adds the virtual method "campClassId()", nothing more
*
*  @note
*    - See "CAMP_RTTI"-documentation for details.
*    - From "camp/camptype.hpp" (names changed to avoid collisions)
*/
#define QSF_CAMP_RTTI() \
	public: virtual uint32_t campClassId() const {return camp::detail::QsfStaticTypeId(this);} \
	public: virtual const char* campClassName() const {return camp::detail::QsfStaticTypeName(this);} \
	private:

/**
*  @brief
*    See "CAMP_TYPE" for details
*
*  @note
*    - From "camp/camptype.hpp" (names changed to avoid collisions)
*/
#define QSF_CAMP_TYPE(type) \
	namespace camp \
	{ \
		namespace detail \
		{ \
			template <> struct StaticTypeId<type> \
			{ \
				static uint32_t get(bool = true) {return StringId(#type);} \
				enum {defined = true, copyable = true}; \
			}; \
			template <> struct StaticTypeName<type> \
			{ \
				static const char* get(bool = true) {return #type;} \
				enum {defined = true, copyable = true}; \
			}; \
		} \
	}

/**
*  @brief
*    See "CAMP_TYPE_NONCOPYABLE" for details
*
*  @note
*    - From "camp/camptype.hpp" (names changed to avoid collisions)
*/
#define QSF_CAMP_TYPE_NONCOPYABLE(type) \
	namespace camp \
	{ \
		namespace detail \
		{ \
			template <> struct StaticTypeId<type> \
			{ \
				static uint32_t get(bool = true) {return StringId(#type);} \
				enum {defined = true, copyable = false}; \
			}; \
			template <> struct StaticTypeName<type> \
			{ \
				static const char* get(bool = true) {return #type;} \
				enum {defined = true, copyable = false}; \
			}; \
		} \
	}


//[-------------------------------------------------------]
//[ QSF CAMP extension macros                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Define an CAMP array property wrapper for redirecting array requests into the C++ class interface which needs to perform additional work per request
*
*  @param[in] propertyName
*    Name of the CAMP array property to add to the given class, e.g. "Nodes"
*  @param[in] className
*    Name of the class without namespace to add the CAMP array property to, e.g. "NodesComponent"
*
*  @remarks
*    The CAMP reflection system supports arrays as class properties, but unlike elementary class properties it's no good idea
*    to access those array properties by using getter and setter methods. This will lead to massive coping array instances around
*    because returning array properties as references or pointers is not possible. For the compiler, a reference is a reference and no array.
*
*    The only decent way to access C++ arrays via the CAMP reflection system in general is to make the C++ array member variables public.
*    In QSF, our convention is to declare CAMP metaclasses only inside "qsf::Plugin::onInstall()". In case you don't want to make the C++ array
*    member variables public for the rest of the world while still being able to directly plugin the array property into CAMP without
*    additional wrappers, it's valid to make your plugin class to a friend of the C++ class containing the array member variable.
*    Please be warned that CAMP is now able to work without any security measurements directly on the array member variable. While this might
*    be fine for several use-cases, there are situations were additional security checks or additional deferred work needs to be done as soon
*    as there are changes inside the array. In case you're in such a situation, please continue reading, else you can stop reading right now
*    and choose the simple direct approach.
*
*    Our solution is to add an C++ interface member variable which is used by a "camp_ext::ArrayMapper"-specialization in order to be able
*    to let the CAMP reflection system interact with a certain C++ class interface. With this solution one is able to use the C++ class interface
*    as usual and ignoring the CAMP reflection system if does not bring any benefit in certain use cases. On the other hand it's also possible
*    to access an array by using the generic CAMP reflection system.
*
*    1. The following example shows how to add an CAMP array property to your class. At first, use the "QSF_CAMP_ARRAY_PROPERTY()"-macro inside
*    your C++ header below the "QSF_CAMP_RTTI()"-macro in order to define the CAMP array property:
*    @code
*    QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
*    QSF_CAMP_ARRAY_PROPERTY(Nodes, NodesComponent, glm::vec3)	// Adds public member variable "Nodes" which is the middle part of the node array <-> CAMP interconnection
*    @endcode
*
*    2. Inside the initializer list of your class constructor, you need to initialize the automatically added public member variable by using "*this" as seen in the following example:
*    @code
*    NodesComponent::NodesComponent(Prototype* prototype) :
*        Component(prototype),
*        Nodes(*this),	// Initialize the automatically added public member variable
*    @endcode
*
*    3. When declaring the CAMP metaclass, just declare our CAMP array property as you would do when connecting a public C++ class member variable
*    directly to the CAMP reflection system:
*    @code
*    camp::Class::declare<NodesComponent>()
*        .base<Component>()
*        .property("Nodes", &NodesComponent::Nodes)
*    );
*    @endcode
*    "NodesComponent::Nodes" is in fact the public C++ class member variable you defined above by using the "QSF_CAMP_ARRAY_PROPERTY()"-macro.
*
*    4. Next implement the CAMP array property interface methods for the class implicitly defined by step 1:
*    @code
*    inline std::size_t NodesComponent::NodesInterface::size() const
*    {
*        return getOwner().getNodes().size();
*    }
*    inline const glm::vec3& NodesComponent::NodesInterface::get(std::size_t index) const
*    {
*        return getOwner().getNodes().at(index);
*    }
*    inline void NodesComponent::NodesInterface::set(std::size_t index, const glm::vec3& value)
*    {
*        getOwner().setNode(index, value);
*    }
*    inline void NodesComponent::NodesInterface::insert(std::size_t before, const glm::vec3& value)
*    {
*        getOwner().insertNode(before, value);
*    }
*    inline void NodesComponent::NodesInterface::remove(std::size_t index)
*    {
*        getOwner().removeNode(index);
*    }
*    @endcode
*    You can do this either in your C++ source code file (not inlined then, of course), or when using simple wrapper methods like those in the sample code,
*    inside your C++ class header or "-inl.h"-file in case you're using one.
*
*    5. Finally, you need to tell the CAMP reflection system how to interact with the array by defining a CAMP array mapper class:
*    @code
*    QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::NodesComponent::NodesInterface)
*    @endcode
*    Do this inside your C++ class header, outside of any namespaces, preferably at the end of the header file.
*    For this to work without compiler errors, you will need to include the CAMP array mapper definition into the C++ class header as well:
*    @code
*    #include <camp/arraymapper.hpp>
*    @endcode
*/

#define QSF_CAMP_ARRAY_INTERFACE_CLASS(interfaceClassName, ownerType, elementType) \
	class interfaceClassName \
	{ \
	public: \
		typedef elementType ElementType; \
		inline interfaceClassName(ownerType& owner) : mOwner(&owner) {} \
		inline ownerType& getOwner() { return *mOwner; } \
		inline const ownerType& getOwner() const { return *mOwner; } \
		std::size_t size() const; \
		const elementType& get(std::size_t index) const; \
		void set(std::size_t index, const elementType& value); \
		void insert(std::size_t before, const elementType& value); \
		void remove(std::size_t index); \
	private: \
		ownerType* mOwner; \
	};


#define QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(interfaceClassName, ownerType, variableType, elementType) \
	class interfaceClassName \
	{ \
	public: \
		typedef elementType ElementType; \
		inline interfaceClassName(ownerType& owner, variableType& variable) : mOwner(&owner), mVariable(&variable) {} \
		inline ownerType& getOwner() { return *mOwner; } \
		inline const ownerType& getOwner() const { return *mOwner; } \
		inline variableType& getVariable() { return *mVariable; } \
		inline const variableType& getVariable() const { return *mVariable; } \
		std::size_t size() const; \
		const elementType& get(std::size_t index) const; \
		void set(std::size_t index, const elementType& value); \
		void insert(std::size_t before, const elementType& value); \
		void remove(std::size_t index); \
	private: \
		ownerType* mOwner; \
		variableType* mVariable; \
	};


#define QSF_CAMP_ARRAY_PROPERTY(propertyName, ownerType, elementType) \
	public: \
		QSF_CAMP_ARRAY_INTERFACE_CLASS(propertyName##Interface, ownerType, elementType); \
		propertyName##Interface propertyName; \
	private:


#define QSF_CAMP_ARRAY_MAPPER_DEFINITION(interfaceClassName) \
namespace camp_ext \
{ \
	template <> \
	struct ArrayMapper<interfaceClassName> \
	{ \
		typedef interfaceClassName::ElementType ElementType; \
		static bool dynamic() \
		{ \
			return true; \
		} \
		static std::size_t size(const interfaceClassName& arrayInterface) \
		{ \
			return arrayInterface.size(); \
		} \
		static const ElementType& get(const interfaceClassName& arrayInterface, std::size_t index) \
		{ \
			return arrayInterface.get(index); \
		} \
		static void set(interfaceClassName& arrayInterface, std::size_t index, const ElementType& value) \
		{ \
			arrayInterface.set(index, value); \
		} \
		static void insert(interfaceClassName& arrayInterface, std::size_t before, const ElementType& value) \
		{ \
			arrayInterface.insert(before, value); \
		} \
		static void remove(interfaceClassName& arrayInterface, std::size_t index) \
		{ \
			arrayInterface.remove(index); \
		} \
	}; \
}
