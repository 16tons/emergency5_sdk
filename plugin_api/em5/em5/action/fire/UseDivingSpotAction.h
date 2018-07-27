// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>


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
	*    EMERGENCY 5 use diving spot action
	*/
	class EM5_API_EXPORT UseDivingSpotAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::UseDivingSpotAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		UseDivingSpotAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UseDivingSpotAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity);

		uint64 getTargetEntityId() const;


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
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,			///< The action starts here
			STATE_DIVE_DOWN,	///< Animation to dive down playing
			STATE_DIVING,		///< Under water out of sight
			STATE_DIVING_END
		};

		enum LootType
		{
			LOOT_NONE,
			LOOT_CAR,
			LOOT_PERSON
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void handleDivingState();
		void handleResultNone();
		qsf::Entity* spawnBuoyEntity() const;
		qsf::Entity* spawnUnderWaterTarget();
		void handleUnderWaterTarget(qsf::Entity& underWaterTarget);
		void createMessage(const qsf::Entity& underWaterTarget);
		void flagDivingSpotAsUsed();

		// Overlay has to be attached to some visible part of the diving-spot
		qsf::Entity* getVisibleDivingSpotEntity() const;
		void showHint() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64			mTargetEntityId;
		State			mState;
		float			mDivingTimeMax;		///< Time to stay down in seconds
		qsf::Time		mDiveTime;
		uint64			mParticlesId;
		qsf::WeakPtr<qsf::Entity> mUnderwaterEntity;	///< Entity which is recovered
		LootType		mLootType;
		bool			mWasInvincible;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::UseDivingSpotAction)
