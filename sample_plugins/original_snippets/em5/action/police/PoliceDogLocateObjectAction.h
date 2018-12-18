// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/ambulance/RescueDogLocateVictimAction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 police dog locate object action
	*
	*  @note
	*    - Basically the same as rescue dog's search action, so we inherit from this base class
	*/
	class EM5_API_EXPORT PoliceDogLocateObjectAction : public RescueDogLocateVictimAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PoliceDogLocateObjectAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PoliceDogLocateObjectAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PoliceDogLocateObjectAction();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::RescueDogLocateVictimAction methods ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getFoundMessageId() const override;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PoliceDogLocateObjectAction)
