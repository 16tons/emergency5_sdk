// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/asset/AssetProxy.h>
#include <qsf/component/Component.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Activity;
	class ActivityPlayer;
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
	*    Activity component class
	*/
	class EM5_API_EXPORT ActivityComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::ActivityComponent" unique component ID
		static const uint32 ACTIVITY;			///< "Activity" unique class property ID inside the class


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
		inline explicit ActivityComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ActivityComponent();

		/**
		*  @brief
		*    Return the activity asset to use
		*
		*  @return
		*    The activity asset to use (e.g. "sample/activity/default/factoryworker"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const qsf::AssetProxy& getActivityAsset() const;

		/**
		*  @brief
		*    Set the activity asset to use
		*
		*  @param[in] asset
		*    The activity asset to use (e.g. "sample/activity/default/factoryworker")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setActivityAsset(const qsf::AssetProxy& asset);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startActivity(Activity& activity);
		void stopActivity();
		void updateSimulation(const qsf::JobArguments& jobArgs);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP system
		qsf::AssetProxy mActivityAsset;

		// Internal
		ActivityPlayer* mActivityPlayer;
		qsf::JobProxy	mSimulationJobProxy;
		bool			mWasMoveToDespawnActive;

		// Debug draw
		qsf::DebugDrawProxy mDebugDrawProxy;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/activity/ActivityComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ActivityComponent)
