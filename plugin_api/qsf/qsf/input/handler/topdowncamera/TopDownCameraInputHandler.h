// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/handler/freemove/FreeMoveInputHandler.h"


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
	*    Top down camera input handler class
	*/
	class QSF_API_EXPORT TopDownCameraInputHandler : public FreeMoveInputHandler
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
		explicit TopDownCameraInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TopDownCameraInputHandler();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputHandler methods           ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateInputHandler(const Time& timePassed) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TopDownCameraInputHandler)
