// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/InputListener.h"
#include "qsf/reflection/CampClass.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Time;
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
	*    Input handler base class
	*/
	class QSF_API_EXPORT InputHandler : public InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] id
		*    Input handler ID
		*/
		inline explicit InputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputHandler();

		/**
		*  @brief
		*    Return the handler's ID
		*/
		inline uint32 getId();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputHandler methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Gets called after the input handler was created -> This is needed to support special initialization in derived classes
		*/
		inline virtual void initialize();

		/**
		*  @brief
		*    Update the input handler
		*/
		inline virtual void updateInputHandler(const Time& timePassed);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    React to input events
		*/
		inline virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/handler/InputHandler-inl.h"
