// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/action/Action.h>
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
	*    EMERGENCY 5 pickup vehicle by towcar action
	*/
	class EM5_API_EXPORT PickupVehicleByTowcarAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PickupVehicleByTowcarAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_SUPPORTLEGS_EXPAND,
			STATE_CRANE_ALIGN_ARM,
			STATE_CRANE_LINK_CARGO,
			STATE_CRANE_LOAD_CARGO,
			STATE_CRANE_WAIT_BEFORE_DROP,
			STATE_SUPPORTLEGS_HIDE,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PickupVehicleByTowcarAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PickupVehicleByTowcarAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity);

		uint64 getTargetId() const;


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
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint64 getCoverEntityId();
		void linkWreckToCraneEntity(qsf::Entity& wreckEntity);
		void unLinkWreckEntity(qsf::Entity& wreckEntity);
		void exchangeTargetWithCover(qsf::Entity& wreckEntity, qsf::Entity& coverEntity);
		void addToContainer(qsf::Entity& wreckEntity);
		void addChildsSafeToContainer(qsf::Entity& mainChildEntity);

		qsf::Entity* getFirstChildEntity(qsf::Entity* entity);
		float towcarToWreckYaw(const qsf::Transform& towcarTransform, const qsf::Transform& wreckTransform);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64		mTargetEntityId;
		uint64		mCoverEntityId;
		State		mState;
		glm::quat	mDefaultRotation;
		bool		mUseCover;
		float		mSecondsPassed;
		glm::vec3	mTargetPosition;

		AudioProxy	mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PickupVehicleByTowcarAction)
