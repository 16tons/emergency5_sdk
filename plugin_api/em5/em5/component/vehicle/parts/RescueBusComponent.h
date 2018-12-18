// Copyright (C) 2012-2018 Promotion Software GmbH


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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		class RescueBusData;
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
	*    Rescue bus component is for the expandable part of the IMT rescue bus.
	*/
	class EM5_API_EXPORT RescueBusComponent : public qsf::Component
	{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class multiplayer::RescueBusData; // Needed for multiplay


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::RescueBusComponent" unique component ID


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
		explicit RescueBusComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RescueBusComponent();

		bool isHidden() const;
		bool isAnimationPlaying() const;
		bool isExtended() const;

		void startExtending();
		void hideExtending();

		const glm::vec3& getCollisionExtended() const;
		void setCollisionExtended(const glm::vec3& collisionExtentExtended);

		bool checkCollisionForExtending();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);
		void registerRescueBusParts();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum RescueBusState
		{
			RESCUEBUSSTATE_HIDDEN,
			RESCUEBUSSTATE_EXTENDING_ANIMATION,
			RESCUEBUSSTATE_EXTENDED,
			RESCUEBUSSTATE_HIDDING_ANIMATION
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void enableCollisionShapeExtended();
		void disableCollisionShapeExtended();
		void enableNavigationComponent(bool enable);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RescueBusState			  mRescueBusState;
		qsf::WeakPtr<qsf::Entity> mRescueBusSide;
		glm::vec3				  mDeployedCollisionExtend;

		// Internal
		qsf::JobProxy		mUpdateJobProxy;
		AudioProxy			mAudioProxy;
		qsf::DebugDrawProxy	mDebugDrawProxy;		///< Only for debug


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RescueBusComponent)
