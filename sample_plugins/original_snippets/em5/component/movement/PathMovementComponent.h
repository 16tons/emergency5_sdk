// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/math/path/PathSegmenter.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PathComponent;
	class TransformComponent;
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
	*    EMERGENCY 5 move entity along a given path component
	*
	*  @todo
	*    This component is currently purly designed for ME12
	*/
	class EM5_API_EXPORT PathMovementComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::PathMovementComponent" unique component ID


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
		explicit PathMovementComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PathMovementComponent();

		void setPath(qsf::PathComponent& pathComponent, bool activate = true);
		void setPath(const qsf::Entity& entityContainingPath, bool activate = true);

		void setSpeed(float speed);
		void setSpeedByTime(const qsf::Time& time);

		void initializeConfiguration(float speed, float maximumAcceleration, float maximumDeceleration, float mCurveSpeedReduction);

		bool isLooped() const;
		void setLooped(bool looped);

		/**
		*  @brief
		*    Returns how much of the path is already reached (in percentage)
		*/
		float getPathProgressPercentage() const;

		// Getters and setters for single entity slots
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PathToFollowId, uint64, mPathEntityId);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);
		void internalActivate(bool activate);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy		mUpdateJobProxy;
		qsf::PathSegmenter	mPathSegmenter;
		qsf::WeakPtr<qsf::TransformComponent> mPathComponent;

		// Configuration
		float				mMaximumSpeed;
		float				mMaximumAcceleration;
		float				mMaximumDeceleration;
		float				mCurveSpeedReduction;
		bool				mLooped;

		float				mCurrentSpeed;
		float				mCurrentPathOffset;
		uint64				mPathEntityId;
		bool				mAiNavigationWasActiveBefore;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PathMovementComponent)
