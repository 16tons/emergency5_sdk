// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/move/escape/EscapeAction.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class PersonSpawnPointComponent;
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
	*    EMERGENCY 5 escape foot long action
	*/
	class EM5_API_EXPORT EscapeFootLongAction : public EscapeAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::EscapeFootLongAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EscapeFootLongAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EscapeFootLongAction();

		/**
		*  @brief
		*    Initialization
		*/
		void init(bool movementModeRun = true);
		void init(const qsf::Time& escapeLength, bool movementModeRun = true);
		void init(const std::string& escapeTargetTag, bool movementModeRun = true);

		/**
		*  @brief
		*    This entity got catched
		*/
		virtual void catched() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void runTowardsClosestEscapePoint();
		void runTowardsDistantPosition();
		void runTowardsPosition(const glm::vec3& position);

		bool collectDespawnPoints(std::vector<PersonSpawnPointComponent*>& outPoints, bool onlyOutsideMapBoundaries);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time mTimeRunning;
		qsf::Time mTimeUntilNextPause;
		qsf::Time mMaximumTime;
		glm::vec3 mCurrentTargetPosition;
		bool	  mMovementModeRun;

		enum class EscapeMode
		{
			FIXED,	///< Run to fixed target and disappear there
			LONG,	///< Run to random distant targets until time is nearly over
			SHORT	///< Run to closest escape point and disappear there
		};
		EscapeMode mEscapeMode;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EscapeFootLongAction)
