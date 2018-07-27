// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_ai/navigation/MoveAction.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameComponent;
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
	*    EMERGENCY 5 move action
	*/
	class EM5_API_EXPORT MoveAction : public qsf::ai::MoveAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::MoveAction" unique action identifier
		static const float MIN_DISTANCE_FOR_RETRY;		///< Minimal distance for retrying the pathfinding


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MoveAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MoveAction();

		/**
		*  @brief
		*    Initialization
		*
		*  @param[in] goal
		*    Navigation goal with its configurations for this movement, should only be null if the path parameter is not null
		*  @param[in] movementModeId
		*    Movement mode identifier, choose movement modes from "em5/ai/MovementModes.h", usually used to run or use different navigation maps
		*  @param[in] path
		*    Navigation path if we already have a path we follow, for example from "AiPathComponent::createAiPathSmoothed()"
		*  @param[in] maxMoveTime
		*    Maximal time before we stop the movement and finish the action. If time is zero we will run as long as it needs to reach the goal or fail
		*/
		void init(qsf::ai::NavigationGoal* goal, uint32 movementModeId = qsf::getUninitialized(static_cast<uint32>(0)), qsf::ai::Path* path = nullptr, qsf::Time maxMoveTime = qsf::Time::ZERO);

		/**
		*  @brief
		*    Calculate the length of the navigation path.
		*	 TAKE CARE: This will only return values > 0 once the path exists. Which is after the MoveAction has been updated once.
		*/
		float getPathLength() const;

		/**
		*  @brief
		*    Return the target entity which is followed; "qsf::getUninitialized<uint64>()" if nothing is followed
		*/
		uint64 getFollowedEntity() const;

		/**
		*  @brief
		*    Force a followed entity to wait when we reach a certain distance
		*
		*  @param[in] distance
		*    Distance at which we force the target to wait
		*  @param[in] waitTime
		*    Time we force a target to wait
		*/
		void forceFollowedEntityToWait(float distance, qsf::Time waitTime);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum TraceNodeAddMode
		{
			ADD_NEW_NODE,
			UPDATE_EXISTING_NODE,
			FINISH_OFF_TRACE
		};

		enum class StuckCheckMode
		{
			NONE = 0,
			CIVIL_VEHICLES,
			SQUADS				// Also gangsters
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/*
		* Returns whether the entity is currently on emergency operations that is used to trigger blue lights and sirens.
		* This is currently decided based on the AI movement mode ID.
		*/
		bool isOnEmergencyOperations() const;

		uint32 getMovementMode(uint32 movementModeId) const;
		bool checkAllowUpdate() const;
		bool checkPositionStuck();
		bool checkIfStuck(const qsf::Time& timePassed);
		void checkForSirenActivationOnStartMoving();
		void addTraceNode(const glm::vec3& position, TraceNodeAddMode addMode = ADD_NEW_NODE);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		bool		mIsPerson;
		bool		mIsCivilPerson;
		bool		mIsCivilRoadVehicle;
		bool		mIsSquadVehicle;
		bool		mDidStartMoveCheckForBluelightAndSiren;
		qsf::WeakPtr<GameComponent> mGameComponent;

		// Stuck check
		StuckCheckMode mStuckCheckMode;
		qsf::Time	mStuckTime;
		glm::vec3	mOldPosition;

		// Trace ground
		glm::vec3	mPreviousTracePosition;
		float		mPreviousTracePointPosition;
		bool		mIsOnTraceGround;
		qsf::WeakPtr<qsf::Entity> mCurrentTraceEntity;

		qsf::Time	mMaxMoveTime;
		qsf::Time	mTimePassed;

		float		mForcedWaitSquaredDistance;
		qsf::Time	mForcedWaitTime;
		uint32		mNumberOfRetries;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MoveAction)
