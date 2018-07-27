// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkComponent;
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
	*  @remarks
	*    EMERGENCY 5 helicopter search light component, this can also be used for the helicopter camera
	*/
	class EM5_API_EXPORT HelicopterSearchLightComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::HelicopterSearchLightComponent" unique component ID


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
		explicit HelicopterSearchLightComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HelicopterSearchLightComponent();

		void startFollowTarget();
		void stopFollowTarget();

		uint64 getSearchLightEntityId() const;
		void setSearchLightEntityId(uint64 cannonEntityId);
		bool getIsSearchLight() const;
		void setIsSearchLight(bool isSearchlight);
		void setTargetEntityId(uint64 targetEntityId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void onSetSimulating(bool simulating) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void deactivateJob();
		void updateJob(const qsf::JobArguments& jobArguments);
		void startupLights(const qsf::JobArguments& jobArguments);
		void calculateFinalAngles();
		void calculateCurrentAngles(float passedTime);
		void calculateFinalRandomAngles();
		void calculateCurrentRandomAngles(float passedTime);

		void setLocalRotation(float yaw, float pitch);

		void switchLight(bool active);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mSearchLightEntityId;	// The id of the child entity of base part
		bool	mIsSearchLight;			// Is this a searchlight or should it be used as camera

		qsf::WeakPtr<qsf::Entity>	mSearchLightEntity;
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::WeakPtr<qsf::LinkComponent> mBaseLinkComponent;
		qsf::WeakPtr<qsf::LinkComponent> mSearchLightLinkComponent;

		// Internal
		float mCurrentYaw;
		float mCurrentPitch;
		float mFinalYaw;
		float mFinalPitch;

		float mCurrentRandYaw;
		float mCurrentRandPitch;
		float mFinalRandYaw;
		float mFinalRandPitch;

		bool mIsFollowingTarget;
		bool mIsSearchLightInFinalRotation;
		bool mIsRandomInZeroRotation;
		qsf::JobProxy	mUpdateJobProxy;
		qsf::JobProxy	mStartupJobProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HelicopterSearchLightComponent)
