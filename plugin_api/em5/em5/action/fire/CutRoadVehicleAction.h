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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MeshAnimationChannel;
	namespace game
	{
		class PrototypeContainer;
	}
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
	*    EMERGENCY 5 cut car action
	*
	*  @note
	*    - We make a difference between vehicle, helicopter and ships. This class works for road vehicles.
	*/
	class EM5_API_EXPORT CutRoadVehicleAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::CutRoadVehicleAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CutRoadVehicleAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CutRoadVehicleAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity);

		uint64 getTargetEntityId() const;

		bool isDone() const;


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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,							///< The action starts here
			STATE_CREATE_COMPRESSOR,
			STATE_PLACE_CUTTER,
			STATE_PICKUP_CUTTER,
			STATE_START_CUTTING,
			STATE_LOOP_CUTTING,					///< Special animation state
			STATE_END_CUTTING,
			STATE_PLACE_CUTTER_AGAIN,
			STATE_UNLOAD,						///< Unload all persons from vehicle
			STATE_FINALLY_START_PICKUP_CUTTER,	///< Last step, take cutter and finish action
			STATE_FINALLY_PICKUP_CUTTER,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void createCompressorPump();
		void linkCutterToCompressor() const;
		void pickupCutterFromCompressor() const;
		uint32 unloadPersonFromVehicle(const qsf::game::PrototypeContainer& roadVehicleComponent);
		void pushPullingPersonFromVehicleActions() const;
		void pushFallbackAnimationsAndPlaceRescuedPerson() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64									mTargetEntityId;
		State									mState;
		qsf::Time								mTime;
		qsf::WeakPtr<qsf::Entity>				mCompressorPumpEntity;
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mMeshAnimationChannel;
		qsf::WeakPtr<qsf::Entity>				mRescuedPerson;
		AudioProxy								mAudioProxy;
		AudioProxy								mCompressorAudioProxy;
		bool									mPlayAllRescueAnimations;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CutRoadVehicleAction)
