// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/reflection/type/CampQsfTime.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
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
	*    EMERGENCY 5 bomb component
	*/
	class EM5_API_EXPORT BombComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BombComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit BombComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BombComponent();

		qsf::Time getTimeRunning() const;
		void setTimeRunning(qsf::Time timeRunning);

		qsf::Time getTimeToDefuse() const;
		void setTimeToDefuse(qsf::Time timeToDefuse);

		// "true" if the bomb is currently defused (de: wird entschärft)
		bool getIsDefuseBombActive() const;
		void setIsDefuseBombActive(bool isDefuseBomb);

		bool isBombActive() const;
		bool isBombDefused() const;
		bool isBombIntact() const;

		// "true" if the defusing process can continue, "false" if bomb is exploded or defused
		bool canBeDefused() const;

		/**
		*  @brief
		*    Starting the bomb timer; when it reaches zero, the bomb explodes
		*/
		void startBombTimer(FreeplayEvent& freeplayEvent, bool playTicking = true);

		/**
		*  @brief
		*    Let the bomb explode right now
		*/
		void explodeBomb();

		/**
		*  @brief
		*    Defuse the bomb
		*/
		void defuseBomb();

		void enableTicking(bool enable);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);

		void damageObjectsAroundBomb();
		void drawDebugDisplay();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		enum BombState
		{
			STATE_INACTIVE,
			STATE_ACTIVE,
			STATE_DEFUSED,
			STATE_EXPLODED
		};

		// Internal
		qsf::JobProxy		mUpdateJobProxy;
		qsf::Time			mTimeRunning;
		qsf::Time			mTimeToDefuse;
		qsf::Time			mCurrentTimeDefuse;
		BombState			mState;
		bool				mIsDefuseActive;

		qsf::DebugDrawProxy	mDebugDrawProxy;	///< Only for debug
		AudioProxy			mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BombComponent)
