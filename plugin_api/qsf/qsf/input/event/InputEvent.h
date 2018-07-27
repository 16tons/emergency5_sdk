// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/manager/FastAllocator.h"


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
	*    Input event base class
	*/
	class QSF_API_EXPORT InputEvent : public FastAllocator
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline InputEvent(InputDevice& device);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputEvent();

		/**
		*  @brief
		*    Return the associated device
		*/
		inline InputDevice& getDevice() const;

		/**
		*  @brief
		*    Return type ID, which is the string hash of the event class name
		*/
		virtual uint32 getTypeId() const = 0;

		/**
		*  @brief
		*    Called right before event is inserted into the input system's event queue
		*/
		inline virtual void onAddToQueue() const;

		/**
		*  @brief
		*    Apply event to the associated device by changing its state
		*/
		inline virtual void applyToDevice() const;

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
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void addFineGranularEvent(InputEvent* inputEvent) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InputDevice& mDevice;	///< Reference to the device the event is associated with; do not destroy


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/event/InputEvent-inl.h"
