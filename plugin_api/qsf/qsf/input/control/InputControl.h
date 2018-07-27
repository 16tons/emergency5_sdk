// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputDevice;
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
	*    Input control base class
	*
	*  @remarks
	*    A control is part of an input device and can be e.g. a button or an axis.
	*/
	class QSF_API_EXPORT InputControl
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] device
		*    Owning device
		*  @param[in] typeId
		*    Control type ID
		*  @param[in] name
		*    Control name
		*  @param[in] description
		*    Control description
		*/
		InputControl(InputDevice& device, uint32 typeId, const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputControl();

		/**
		*  @brief
		*    Get device
		*
		*  @return
		*    Reference to device that owns the control
		*/
		inline InputDevice& getDevice() const;

		/**
		*  @brief
		*    Get control type ID
		*
		*  @return
		*    Type ID of control, which is the string-hashed class name
		*/
		inline uint32 getTypeId() const;

		/**
		*  @brief
		*    Get control ID
		*
		*  @return
		*    The ID, which is simply the string-hashed name
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Get control name
		*
		*  @return
		*    Name
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Get control description
		*
		*  @return
		*    Description
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Checks if the input control has the given name
		*/
		inline bool hasName(const StringHash& name) const;

		/**
		*  @brief
		*    Compare control name to a given name
		*
		*  @remarks
		*    This is just an ease-of-use operator, it is identical to the hasName() method
		*/
		inline bool operator==(const StringHash& name) const;

		/**
		*  @brief
		*    Compare control references
		*
		*  @remarks
		*    This is just an ease-of-use operator
		*/
		inline bool operator==(const InputControl& control) const;

		/**
		*  @brief
		*    Check if this event is of the given type.
		*
		*  @remarks
		*    Subclasses are not recognized, it is only checked if the type is exactly the given type.
		*/
		template<typename TYPE>
		inline bool isOfType() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputControl methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Reset state of this control back to default
		*/
		inline virtual void reset();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Control data
		InputDevice* mDevice;		///< Owning device; always valid
		uint32		 mTypeId;		///< Type ID of the specific control type, generated as the string-hashed class name
		uint32		 mId;			///< Control ID, simply the string-hashed name
		std::string	 mName;			///< Control name
		std::string	 mDescription;	///< Control description


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/control/InputControl-inl.h"
