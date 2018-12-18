// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>
#include <qsf/math/Transform.h>
#include <qsf/reflection/type/CampGlmQuat.h>


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
	*    EMERGENCY 5 unload person from vehicle action
	*/
	class EM5_API_EXPORT UnloadPersonFromVehicleAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::UnloadPersonFromVehicleAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_START_EFFECT,
			STATE_END_EFFECT
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		UnloadPersonFromVehicleAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UnloadPersonFromVehicleAction();

		void init(qsf::Entity& exitVehicle, uint32 doorType);
		void init(qsf::Entity& exitVehicle, const glm::vec3& placePosition, const glm::quat& placeRotation, qsf::Time spawnDelay = qsf::Time::ZERO, bool pushAnimation = true, bool enforceUnloading = false);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity>	mVehicleEntity;					///< The entity of the vehicle the caller is exit
		qsf::Transform				mPlaceTransform;
		bool						mPlaceTransformInitialized;
		bool						mPushAnimation;					///< Indicates if for persons a idle animation should be pushed after unload
		bool						mEnforceUnloading;				///< If set, person must get unloaded at any cost (when it's important for the logic for some reason)
		qsf::Time					mSpawnDelay;
		uint32						mDoorType;
		State						mState;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::UnloadPersonFromVehicleAction)
