// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/EnumBitmask.h"

#include <camp/tagholder.hpp>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
namespace camp
{
	class Class;
	class Value;
	class Property;
	class UserObject;
}
namespace qsf
{
	class Object;
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
	*    Static CAMP helper class
	*
	*  @remarks
	*    CAMP is using exceptions while other used libraries don't use exceptions. The purpose this
	*    CAMP helper class exists in the first place is to hide details from the user. So, no exceptions
	*    are used for most methods and in case of errors default values like an empty string will be
	*    used which is sufficient for most use-cases. See method documentation which methods will throw
	*    exceptions.
	*
	*    This CAMP helper class does not provide deserializing from string because this will get difficult
	*    and inefficient for the parser. Do only serialize to string for debugging, showing values e.g. to
	*    a GUI user as string and so on.
	*/
	class QSF_API_EXPORT CampHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum CampValueSerializationType
		{
			SERIALIZATION_NOTHING,	///< CAMP value is empty
			SERIALIZATION_PTREE,	///< CAMP value is a Boost property tree
			SERIALIZATION_COMPACT,	///< CAMP value can be serialized in a "compact" way, i.e. as a string
			SERIALIZATION_ROLLOUT	///< CAMP value is a CAMP user object, it has a set of properties to be serialized
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return whether or not a CAMP value will be serialized using a compact representation
		*
		*  @param[in] campValue
		*    CAMP value to check
		*
		*  @return
		*    "true" if the CAMP value will be serialized using a compact representation, else "false"
		*/
		static bool isCampValueCompact(const camp::Value& campValue);

		/**
		*  @brief
		*    Return whether a CAMP value is a Boost ptree
		*
		*  @param[in] campValue
		*    CAMP value to check
		*
		*  @return
		*    "true" if the CAMP value is of type "boost::property_tree::ptree" and will be serialized by copying the ptree
		*/
		static bool isCampValuePTree(const camp::Value& campValue);

		/**
		*  @brief
		*    Determine how to serialize or deserialize a given CAMP value
		*
		*  @param[in] campValue
		*    CAMP value to check
		*
		*  @return
		*    The serialization type to use for this value
		*/
		static CampValueSerializationType getSerializationType(const camp::Value& campValue);

		/**
		*  @brief
		*    Check whether two CAMP values are equal
		*
		*  @param[in] campValue1
		*    First CAMP value to compare
		*  @param[in] campValue2
		*    Second CAMP value to compare
		*
		*  @return
		*    "true" in case both are equal, "false" if there is any difference in the CAMP values' contents
		*/
		static bool areCampValuesEqual(const camp::Value& campValue1, const camp::Value& campValue2);

		/**
		*  @brief
		*    Check whether two CAMP properties are equal
		*
		*  @param[in] campValue1
		*    First CAMP value to compare
		*  @param[in] campValue2
		*    Second CAMP value to compare
		*
		*  @return
		*    "true" in case both are equal, "false" if there is any difference in the CAMP properties' contents
		*
		*  @note
		*    - In contrast to "areCampValuesEqual", this method also handles CAMP arrays and checks them for exact match
		*/
		static bool areCampPropertyValuesEqual(const camp::Property& campProperty, const camp::UserObject& campUserObject1, const camp::UserObject& campUserObject2);

		static bool isPropertyValueDefault(const camp::Property& campProperty, const camp::UserObject& campUserObject);

		// Note: We can mark the parameter newCampUserObject const because all methods used inside this method can be called on const instances
		static void copyCampUserObjectProperty(const camp::Property& campProperty, const camp::UserObject& newCampUserObject, const camp::UserObject& originalCampUserObject);
		// Note: We can mark the parameter newCampUserObject const because all methods used inside this method can be called on const instances
		static void copyCampUserObjectProperties(const camp::UserObject& newCampUserObject, const camp::UserObject& originalCampUserObject);

		/**
		*  @brief
		*    Return the number of CAMP class properties which should be shown to the user
		*
		*  @param[in] campClass
		*    CAMP class to return the number of visible properties for
		*  @param[in] showHiddenProperties
		*    Show hidden properties as well?
		*
		*  @return
		*    The number of CAMP class properties which should be shown to the user
		*/
		static uint32 getNumberOfVisibleClassProperties(const camp::Class& campClass, bool showHiddenProperties);

		//[-------------------------------------------------------]
		//[ Value from/to string                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Convert a CAMP value to a string
		*
		*  @param[in] campValue
		*    CAMP value to use
		*
		*  @return
		*    The CAMP value as string
		*/
		static std::string campValueToString(const camp::Value& campValue);

		/**
		*  @brief
		*    Return a CAMP property value as string
		*
		*  @param[in] campUserObject
		*    CAMP user object to use
		*  @param[in] property
		*    CAMP property to return the value from as string
		*
		*  @return
		*    The CAMP property value as string
		*/
		static std::string campPropertyValueAsString(const camp::UserObject& campUserObject, const camp::Property& campProperty);

		/**
		*  @brief
		*    Return a CAMP property value as string
		*
		*  @param[in] campUserObject
		*    CAMP user object to use
		*  @param[in] propertyId
		*    Identifier (result of tag name string hashing) of the property to get, no reference by intent
		*
		*  @return
		*    The CAMP property value as string
		*/
		static std::string campPropertyValueAsString(const camp::UserObject& campUserObject, camp::StringId propertyId);

		//[-------------------------------------------------------]
		//[ CAMP property or value from/to Boost ptree            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize a CAMP property value into a Boost ptree
		*
		*  @param[in] campValue
		*    CAMP value to store
		*  @param[out] pTree
		*    Boost ptree to serialize the CAMP property value into
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*
		*  @note
		*   - The output ptree will directly hold the property's value, and not have any child (in contrast to e.g. campPropertyValueToBoostPTree)
		*/
		static void campValueToBoostPTree(const camp::Value& campValue, boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Serialize a CAMP property value into a Boost ptree
		*
		*  @param[in] campUserObject
		*    CAMP user object to use
		*  @param[in] property
		*    CAMP property to serialize the value from
		*  @param[out] pTree
		*    Boost ptree to serialize the CAMP property value into
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*
		*  @note
		*   - The output ptree will consist of a single child named as the property and holding the property's value
		*/
		static void campPropertyValueToBoostPTree(const camp::UserObject& campUserObject, const camp::Property& campProperty, boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Deserialize a CAMP property from a Boost ptree
		*
		*  @param[in] campUserObject
		*    CAMP user object whose property is to be deserialized
		*  @param[in] campProperty
		*    CAMP property to deserialize
		*  @param[in] pTree
		*    Boost ptree to deserialize the CAMP user object from
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*/
		// Note: We can mark the parameter campUserObject const because all methods used inside this method can be called on const instances
		static void boostPTreeToCampPropertyValue(const camp::UserObject& campUserObject, const camp::Property& campProperty, const boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Deserialize a single CAMP property from a Boost ptree
		*
		*  @param[in] campUserObject
		*    CAMP user object whose property is to be deserialized
		*  @param[in] propertyName
		*    Name of the CAMP property
		*  @param[in] pTree
		*    Boost ptree to deserialize the CAMP user object from
		*
		*  @note
		*    This is a convenience method to need to provide only the property name instead of a CAMP property object
		*/
		// Note: We can mark the parameter campUserObject const because all methods used inside this method can be called on const instances
		static void boostPTreeToSingleCampProperty(const camp::UserObject& campUserObject, const std::string& propertyName, const boost::property_tree::ptree& pTree);

		//[-------------------------------------------------------]
		//[ CAMP user object from/to Boost ptree                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize a CAMP user object into a Boost ptree
		*
		*  @param[in] campUserObject
		*    CAMP user object to serialize
		*  @param[in] pTree
		*    Boost ptree to serialize the CAMP user object into
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*/
		static void campUserObjectToBoostPTree(const camp::UserObject& campUserObject, boost::property_tree::ptree& pTree);

		/**
		*  @brief
		*    Deserialize a CAMP user object from a Boost ptree
		*
		*  @param[in] campUserObject
		*    CAMP user object to deserialize
		*  @param[in] pTree
		*    Boost ptree to deserialize the CAMP user object from
		*
		*  @throw
		*   - Throws an exception in case of an error to be compatible with the external Boost library
		*/
		// Note: We can mark the parameter campUserObject const because all methods used inside this method can be called on const instances
		static void boostPTreeToCampUserObject(const camp::UserObject& campUserObject, const boost::property_tree::ptree& pTree);

		//[-------------------------------------------------------]
		//[ Tag                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return CAMP tag value
		*
		*  @param[in] campTagHolder
		*    CAMP tag holder to use
		*  @param[in] tagId
		*    Identifier (result of tag name string hashing) of the tag to get, no reference by intent
		*  @param[in] defaultValue
		*    Default value of the CAMP tag
		*
		*  @return
		*    The CAMP tag value
		*/
		template <typename T>
		static T getCampTagValue(const camp::TagHolder& campTagHolder, camp::StringId tagId, const T& defaultValue);

		/**
		*  @brief
		*    Return CAMP tag value as string
		*
		*  @param[in] campTagHolder
		*    CAMP tag holder to use
		*  @param[in] tagId
		*    Identifier (result of tag name string hashing) of the tag to get, no reference by intent
		*  @param[in] defaultValue
		*    Default value of the CAMP tag
		*
		*  @return
		*    The CAMP tag value as string
		*
		*  @note
		*    - No specialized version of the template method "qsf::CampHelper::getCampTagValue()", since that would violate standard C++ => just a plain overload that gets the job done instead
		*/
		static std::string getCampTagValue(const camp::TagHolder& campTagHolder, camp::StringId tagId, const std::string& defaultValue);

		static bool boostPTreeToJsonString(const boost::property_tree::ptree& pTree, std::string& string);
		static bool jsonStringToBoostPTree(boost::property_tree::ptree& pTree, const std::string& string);

		//[-------------------------------------------------------]
		//[ Enum-based bitmask                                    ]
		//[-------------------------------------------------------]
		template<typename STORAGE, typename ENUM>
		static std::string enumBitmaskToString(EnumBitmask<STORAGE, ENUM> bits);

		template<typename STORAGE, typename ENUM>
		static void enumBitmaskFromString(const std::string& input, EnumBitmask<STORAGE, ENUM>& enumBitmask);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/reflection/CampHelper-inl.h"
