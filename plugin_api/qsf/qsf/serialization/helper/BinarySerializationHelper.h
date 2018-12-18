// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BinarySerializer.h"
#include "qsf/reflection/object/Object.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class UserObject;
	class Property;
	class ArrayProperty;
	class Value;
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
	*    Binary serialization helper class
	*/
	class QSF_API_EXPORT BinarySerializationHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		BinarySerializationHelper(BinarySerializer& serializer, Object::SerializationMode mode = Object::MODE_MINIMAL, Object::SerializationMethod method = Object::SERIALIZE_FLAT);

		/**
		*  @brief
		*    Destructor
		*/
		~BinarySerializationHelper();

		/**
		*  @brief
		*    Serialize or deserialize a CAMP user object using a binary serializer
		*
		*  @param[in] campUserObject
		*    CAMP user object to load or store
		*/
		void serializeCampUserObject(const camp::UserObject& campUserObject);

		/**
		*  @brief
		*    Serialize or deserialize a CAMP property value using a binary serializer
		*
		*  @param[in] campUserObject
		*    CAMP user object holding the property value
		*  @param[in] campProperty
		*    CAMP property to serialize or deserialize
		*/
		void serializeCampPropertyValue(const camp::UserObject& campUserObject, const camp::Property& campProperty);

		/**
		*  @brief
		*    Serialize or deserialize a CAMP array property value using a binary serializer
		*
		*  @param[in] campUserObject
		*    CAMP user object holding the property value
		*  @param[in] campArrayProperty
		*    CAMP array property to serialize or deserialize
		*/
		void serializeCampArrayPropertyValue(const camp::UserObject& campUserObject, const camp::ArrayProperty& campArrayProperty);

		/**
		*  @brief
		*    Serialize or deserialize a CAMP value using a binary serializer
		*
		*  @param[in] campValue
		*    CAMP value to load or store; has to be already filled with the correct type
		*/
		void serializeCampValue(camp::Value& campValue);

		/**
		*  @brief
		*    Serialize or deserialize a boost property tree object using a binary serializer
		*
		*  @param[in] ptree
		*    boost property tree object to load or store
		*/
		void serializeBoostPtree(boost::property_tree::ptree& ptree);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BinarySerializer&			mSerializer;	///< The binary serializer instance to use for input / output
		Object::SerializationMode	mMode;			///< Serialization mode setting
		Object::SerializationMethod mMethod;		///< Serialization method setting


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
