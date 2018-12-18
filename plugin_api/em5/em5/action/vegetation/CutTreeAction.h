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
	class PropertyDictionary;
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
	*    EMERGENCY 5 cut tree action
	*/
	class EM5_API_EXPORT CutTreeAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::CutTreeAction" unique action identifier

		// Key definitions for use with the generic object constructor
		static const qsf::NamedIdentifier PARAMETER_TARGET_ENTITYID;	///< "targetEntityId" action parameter

		enum State
		{
			STATE_INIT,
			STATE_USE_SAW,
			STATE_CUT_TREE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CutTreeAction();

		/**
		*  @brief
		*    Constructor with parameter, used for creating action via script
		*/
		explicit CutTreeAction(qsf::PropertyDictionary* parameters);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CutTreeAction();

		/**
		*  @brief
		*    Initialize the action with values
		*/
		void init(qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Initialize the action with values, give data as pointer, used for creating action via script
		*
		*  @note
		*    - Function is registert in CAMP with the name "init" to replace the original CutTreeAction::init()
		*
		*  TODO(mk) Remove this function in case we support uint64 data types in LUA
		*/
		void initByPointer(qsf::Entity* target);

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
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void playBeginUseChainSawAnimation();
		void playUseChainSawAnimation();
		void playEndUseChainSawAnimation();
		void startFallTreeEffect();
		void startChainsawEffect();
		void stopChainsawEffect();
		void unburyTarget();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::Time					mTimeRunning;
		State						mState;
		qsf::WeakPtr<qsf::Entity>	mParticleEffectEntity;
		qsf::WeakPtr<qsf::Entity>	mParticleSmokeEffectEntity;
		AudioProxy					mAudioProxy;
		bool						mReactivateFireComponent;	// "true" if we have to reactivate the fire component


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CutTreeAction)
