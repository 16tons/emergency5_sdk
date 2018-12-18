// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"

#include <bitset>


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
	*    Inherited object base class
	*
	*  @remarks
	*    This class is an extension to "qsf::Object", and adds an optional reference to a base object to inherit from.
	*    It also manages the "override" state of properties: if a property is not overridden, it inherits its value from the base object.
	*/
	class QSF_API_EXPORT InheritedObject : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void copyPropertyOverrideStateFrom(const InheritedObject& object, bool add = false);
		void setAllPropertyOverrideFlags(bool value);

		void autoCreatePropertyOverrideState();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Object methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual camp::Value getPropertyDefaultValue(const StringHash& propertyId) const override;

		virtual PropertyOverrideState getPropertyOverrideState(const StringHash& propertyId) const override;
		virtual bool setPropertyOverrideState(const StringHash& propertyId, PropertyOverrideState overrideState) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InheritedObject methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual const Object* getPropertyDefaultsProvider() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*
		*  @note
		*    - There's no point in creating an instance of this base class
		*/
		InheritedObject();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// For property override flags, we use a fixed number of bits
		//  -> Compared to a boost::dynamic_bitset, this is more performant and does not cost more memory
		//  -> The downside is a fixed limit of the number of properties of a CAMP class deriving from "qsf::InheritedObject"
		std::bitset<128> mPropertyOverrideFlags;	///< One bit for each property; if set, the property is overridden


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::InheritedObject)
