// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class HelicopterHatchComponent;
}


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
	*    EMERGENCY 5 place person by flying heli action
	*/
	class EM5_API_EXPORT PlacePersonByFlyingHeliAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PlacePersonByFlyingHeliAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_LOWER_STRETCHER,
			STATE_WAIT,
			STATE_RAISE_STRETCHER,
			STATE_FINISH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlacePersonByFlyingHeliAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlacePersonByFlyingHeliAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const glm::vec3& targetPosition);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool checkActionCondition(qsf::Entity& targetEntity);
		void placePersonOnGround();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		State		mState;
		glm::vec3	mTargetPosition;

		AudioProxy	mAudioProxy;

		qsf::WeakPtr<HelicopterHatchComponent> mHelicopterHatchComponent;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlacePersonByFlyingHeliAction)