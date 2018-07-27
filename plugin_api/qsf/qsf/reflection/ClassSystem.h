// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/base/StringHash.h"

#include <boost/signals2.hpp>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
	class Property;
	class UserObject;
}
namespace qsf
{
	class ObjectManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Class system class
	*
	*  @remarks
	*    The purpose of this class manager is to extend the CAMP reflection system interface, not to totally replace it. So, use
	*    "camp::detail::ClassManager" to e.g. get a list of all available CAMP classes.
	*
	*    Class: Each class connected to the CAMP reflection system should have the following tags:
	*    - "Name":        Human readable class name, should have a similar name as the real C++ class name,
	*                     but shouldn't be to technical because it will e.g. be used inside the editor (type: "std::string" with UTF-8 encoding)
	*                     In case the name is composed by multiple words (keep it short!), write each major word in capital letters.
	*                     (Examples: "Ambient Polygon", "Properties", "View Manager", "The Test was Successful",
	*                     See e.g. "Headings and publication titles" at http://en.wikipedia.org/wiki/Letter_case#Headings_and_publication_titles as hint.)
	*    - "Description": A short description what the class is about (type: "std::string" with UTF-8 encoding)
	*
	*    Property: Each property connected to the CAMP reflection system should have the following tags:
	*    (- No human readable property name as tag required, the CAMP reflection property name is already another one as the one of the real C++ member variable)
	*    - "Description": A short description what the property is about (type: "std::string" with UTF-8 encoding)
	*    - "Default":     Default value of the property
	*
	*    Property: Further optional CAMP reflection system property tags:
	*    - "Serializable": Use this to give the system a hint whether or not this property should be serialized (load/save), the default setting is "true" and data type is boolean
	*    - "Compact":      If true, the owner of the tag will be serialized in a compact way, if "false" it will be fully unrolled. Default is "false". Data type is boolean. An example were compact is proffered are e.g. vectors. Only CAMP user types with more than one properties can be none compact.
	*    - "Hidden":       Do not show this property within e.g. the editor, data type is boolean
	*    - "Editable":     Use this to give e.g. the editor a hint whether or not it should be possible to edit the property via editor, the default setting is "true" and data type is boolean
	*    - "RangeMinimum": Use this to give e.g. the editor some hints about the minimum of a decent interval. Example: "0", data type is floating point.
	*    - "RangeMaximum": Use this to give e.g. the editor some hints about the maximum of a decent interval. Example: "1", data type is floating point.
	*    - "StepSize":     Use this to give e.g. the editor some hints about a decent step size to use e.g. within a spinner control. Example: "0.1", data type is floating point.
	*    - "Semantic":     Semantic of the data, use this to give e.g. the editor some hints what the property is about. Example: "Filename", data type is string.
	*    - "AssetType":    Asset type. Example: "texture", "mesh", "prefab". Usually used in case the property data type is "qsf::AssetProxy".
	*    - "FileFilter":   File filter using the syntax of Qt's QFileDialog. Example: "All Files (*);;Map (*.map *.MAP);;Script (*.lua *.LUA)", data type is string. Usually used in case "Semantic" is "Filename".
	*    - "Group":        Optional name of the parameter group the parameter is in. Example: "Atmosphere". The editor can use this information to visually group parameters. Data type is string.
	*
	*  @note
	*    - The external library "CAMP" ( https://projects.tegesoft.com/pm/projects/camp/ ) is used
	*    - Lookout! We don't recommend to use "camp::Observer" to get informed for example in case new CAMP classes are registered. "camp::Observer"
	*      will be informed instantly after metaclass creation, meaning before it's filled with it's concrete data. Use "qsf::ClassSystem::NewCampClass" instead.
	*/
	class QSF_API_EXPORT ClassSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Plugin;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const camp::Class&)> NewCampClass;	///< This Boost signal is emitted as soon as a new CAMP class was registered, CAMP class as first parameter


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ClassSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ClassSystem();

		/**
		*  @brief
		*    Return the object manager instance
		*
		*  @return
		*    The object manager instance, do not destroy the instance
		*/
		inline ObjectManager& getObjectManager() const;

		/**
		*  @brief
		*    Check for class name string hash collision
		*
		*  @remarks
		*    At multiple places, a string hash of RTTI class names will be calculated for more
		*    effective processing. This task is always done by "qsf::StringHash" which is using
		*    FNV-1a as hash function. By using hash values there's always the risk of a collision,
		*    meaning that multiple strings result in one and the same hash value. While the probability
		*    for this is low, you might want to call this function during development to be informed
		*    when such a collision occurs.
		*
		*  @return
		*    "true" if all went fine and there was no collision, else "false" in case at least one collision occurred
		*
		*  @note
		*    - In case there's a string hash collision, a log message will be written
		*    - Also checks inside classes whether or not there are property name collisions
		*/
		bool checkForClassStringHashCollision() const;

		/**
		*  @brief
		*    Dump plugin and CAMP class meta information to a json file.
		*
		*  @remarks
		*    Output file name is currently hard-coded: "classes_dump.json" inside the User Data Directory.
		*/
		void dumpToFile() const;

		/**
		*  @brief
		*    Check for class property name string hash collision
		*
		*  @param[in] campClass
		*    CAMP class to check
		*
		*  @remarks
		*    At multiple places, a string hash of RTTI class property names will be calculated for more
		*    effective processing. This task is always done by "qsf::StringHash" which is using
		*    FNV-1a as hash function. By using hash values there's always the risk of a collision,
		*    meaning that multiple strings result in one and the same hash value. While the probability
		*    for this is low, you might want to call this function during development to be informed
		*    when such a collision occurs.
		*
		*  @return
		*    "true" if all went fine and there was no collision, else "false" in case at least one collision occurred
		*
		*  @note
		*    - In case there's a string hash collision, a log message will be written
		*/
		bool checkForPropertyStringHashCollision(const camp::Class& campClass) const;

		/**
		*  @brief
		*    Return a CAMP class by using its unique identifier
		*
		*  @param[in] id
		*    The unique class identifier
		*  @param[in] allowForwarding
		*    If "true", consider class forwarding as well; note that the resulting class may have a different identifier than "id"
		*
		*  @return
		*    The requested CAMP class instance, do no destroy the returned instance
		*
		*  @throw
		*   - Throws the exception "camp::ClassNotFound" in case the given unique identifier is unknown
		*
		*  @note
		*    - The unique class identifier is generated by "qsf::StringHash" by using the class name
		*      (e.g. qsf::StringHash("qsf::MetadataComponent") generating the hash value 483975945)
		*/
		const camp::Class& getCampClassById(uint32 id, bool allowForwarding = false) const;

		/**
		*  @brief
		*    Return a CAMP class by using its unique identifier
		*
		*  @param[in] id
		*    The unique class identifier
		*  @param[in] allowForwarding
		*    If "true", consider class forwarding as well; note that the resulting class may have a different identifier than "id"
		*
		*  @return
		*    The requested CAMP class instance, do no destroy the returned instance, or a null pointer in case none was found
		*/
		const camp::Class* tryGetCampClassById(uint32 id, bool allowForwarding = false) const;

		/**
		*  @brief
		*    Return a CAMP class name by using its unique identifier
		*
		*  @param[in] id
		*    The unique class identifier
		*  @param[in] allowForwarding
		*    If "true", consider class forwarding as well; note that the resulting class may have a different identifier than "id"
		*
		*  @return
		*    The requested CAMP class name, ID as string in case of an unknown class
		*
		*  @note
		*    - Ease-of-use method which can be used e.g. for log messages
		*/
		std::string getCampClassNameById(uint32 id, bool allowForwarding = false) const;

		/**
		*  @brief
		*    Check whether or not a given CAMP class is derived from another given CAMP base class
		*
		*  @param[in] campClass
		*    The CAMP class to check
		*  @param[in] campBaseClass
		*    The CAMP base class
		*
		*  @return
		*    "true" if the given CAMP class is derived from the given CAMP base class, else "false"
		*/
		bool isCampClassDerivedFrom(const camp::Class& campClass, const camp::Class& campBaseClass) const;

		/**
		*  @brief
		*    Check whether or not a given CAMP class is equal or derived from another given CAMP class
		*
		*  @param[in] campClass
		*    The CAMP class to check
		*  @param[in] campOtherClass
		*    The other CAMP class
		*
		*  @return
		*    "true" if the given CAMP class is equal or derived from the given other CAMP class, else "false"
		*/
		bool isCampClassEqualOrDerived(const camp::Class& campClass, const camp::Class& campOtherClass) const;

		/**
		*  @brief
		*    Return a CAMP property by using its unique identifier
		*
		*  @param[in] propertyId
		*    The unique property identifier
		*  @param[in] classId
		*    The unique class identifier
		*
		*  @return
		*    The requested CAMP property instance, do no destroy the returned instance
		*
		*  @throw
		*    - Throws the exception "camp::ClassNotFound" in case the given unique class identifier is unknown
		*    - Throws the exception "camp::PropertyNotFound" in case the given unique property identifier is unknown
		*
		*  @note
		*    - The unique identifiers are generated by "qsf::StringHash" by using the class or property name, respectively
		*/
		const camp::Property& getCampPropertyById(uint32 propertyId, uint32 classId) const;

		/**
		*  @brief
		*    Return a CAMP property's index by using its unique identifier
		*
		*  @param[in] propertyId
		*    The unique property identifier
		*  @param[in] classId
		*    The unique class identifier
		*
		*  @return
		*    The CAMP property's index in the CAMP's class's list of properties
		*
		*  @throw
		*    - Throws the exception "camp::ClassNotFound" in case the given unique class identifier is unknown
		*    - Throws the exception "camp::PropertyNotFound" in case the given unique property identifier is unknown
		*
		*  @note
		*    - The unique identifiers are generated by "qsf::StringHash" by using the class or property name, respectively
		*/
		int getCampPropertyIndexById(uint32 propertyId, uint32 classId) const;

		/**
		*  @brief
		*    Check whether or not a given CAMP user object is derived from a given CAMP base class
		*
		*  @param[in] campUserObject
		*    The CAMP user object to check
		*  @param[in] campBaseClass
		*    The CAMP base class
		*
		*  @return
		*    "true" if the given CAMP user object is derived from the given CAMP base class, else "false"
		*/
		bool isCampUserObjectDerivedFrom(const camp::UserObject& campUserObject, const camp::Class& campBaseClass) const;

		/**
		*  @brief
		*    Check whether or not a given CAMP user object is equal or derived from a given CAMP class
		*
		*  @param[in] campUserObject
		*    The CAMP user object to check
		*  @param[in] campClass
		*    The CAMP class
		*
		*  @return
		*    "true" if the given CAMP user object is equal or derived from the given CAMP class, else "false"
		*/
		bool isCampUserObjectEqualOrDerived(const camp::UserObject& campUserObject, const camp::Class& campClass) const;

		//[-------------------------------------------------------]
		//[ Forwarding & Deprecation                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add a class identifier forwarding, so that asking for the old class ID will return the new class
		*
		*  @param[in] oldClassId
		*    The old unique class identifier
		*  @param[in] newClassId
		*    The new unique class identifier
		*/
		void addClassForwarding(uint32 oldClassId, uint32 newClassId);

		/**
		*  @brief
		*    Check if the given class is known to be deprecated
		*
		*  @param[in] classId
		*    The unique class identifier
		*
		*  @return
		*    "true" if class is deprecated, "false" otherwise (which does not mean it actually exists)
		*/
		bool isClassDeprecated(uint32 classId) const;

		/**
		*  @brief
		*    Check if the given class property is known to be deprecated
		*
		*  @param[in] classId
		*    The unique class identifier
		*  @param[in] propertyId
		*    The unique property identifier inside the class
		*
		*  @return
		*    "true" if property is deprecated, "false" otherwise (which does not mean it actually exists)
		*/
		bool isClassPropertyDeprecated(uint32 classId, uint32 propertyId) const;

		/**
		*  @brief
		*    Add a class identifier to be known as deprecated
		*
		*  @param[in] classId
		*    The unique class identifier
		*/
		void addDeprecatedClass(uint32 classId);

		/**
		*  @brief
		*    Add a class identifier to be known as renamed, with the new class identifier
		*
		*  @param[in] classId
		*    The old unique class identifier
		*  @param[in] newClassId
		*    The new unique class identifier
		*/
		void addDeprecatedClassWithNewId(uint32 classId, uint32 newClassId);

		/**
		*  @brief
		*    Add a class property identifier to be known as deprecated
		*
		*  @param[in] classId
		*    The unique class identifier
		*  @param[in] propertyId
		*    The unique property identifier inside the class
		*/
		void addDeprecatedClassProperty(uint32 classId, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/** Property data storage struct */
		struct PropertyData
		{
			const camp::Property*	mCampProperty;
			int						mIndex;

			PropertyData() : mCampProperty(nullptr), mIndex(-1) {}
		};
		typedef boost::container::flat_map<uint32, PropertyData> PropertyDataMap;
		typedef boost::container::flat_set<uint32>				 DeprecatedPropertySet;

		/** Class data storage struct */
		struct ClassData
		{
			const camp::Class&		mCampClass;
			ClassData*				mForwardedClassData;
			mutable PropertyDataMap	mPropertyDataMap;
			DeprecatedPropertySet	mDeprecatedPropertySet;

			explicit ClassData(const camp::Class& campClass) : mCampClass(campClass), mForwardedClassData(nullptr) {}
		};
		typedef std::unordered_map<uint32, ClassData>	   ClassDataMap;
		typedef boost::container::flat_map<uint32, uint32> DeprecatedClassMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add a CAMP class
		*
		*  @param[in] campClass
		*    CAMP class, must stay valid as long as this class system instance exists
		*
		*  @note
		*    - Usually only called by "qsf::Plugin::addCampClass()"
		*/
		void addCampClass(const camp::Class& campClass);

		/**
		*  @brief
		*    Return a property data structure by using the property's unique identifier
		*
		*  @param[in] propertyId
		*    The unique property identifier
		*  @param[in] classId
		*    The unique class identifier
		*
		*  @return
		*    The requested property data structure, do no destroy the returned instance
		*
		*  @throw
		*    - Throws the exception "camp::ClassNotFound" in case the given unique class identifier is unknown
		*    - Throws the exception "camp::PropertyNotFound" in case the given unique property identifier is unknown
		*
		*  @note
		*    - The unique identifiers are generated by "qsf::StringHash" by using the class or property name, respectively
		*/
		const PropertyData& getPropertyDataById(uint32 propertyId, uint32 classId) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ClassDataMap	   mClassDataMap;			///< Class data map with unique component identifier as key and a "ClassData" instance as value
		DeprecatedClassMap mDeprecatedClassMap;		///< Maps class unique IDs to either "qsf::getUninitialized<uint32>()" or a new ID in case the class was renamed
		ObjectManager*	   mObjectManager;			///< Object manager instance, always valid


	};


	/**
	*  @brief
	*    Small helper class that derives the ClassId from a given typename
	*/
	template<typename T>
	class QsfCampClassId
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static inline unsigned int get()
		{
			return StringHash(camp::detail::StaticTypeId<T>::get());
		}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/reflection/ClassSystem-inl.h"
