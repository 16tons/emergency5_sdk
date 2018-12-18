// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Tick interpolation helper class
	*/
	class QSF_API_EXPORT TickInterpolationHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct LightweightTransform
		{
			glm::vec3 mPosition;
			glm::quat mRotation;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		TickInterpolationHelper();

		/**
		*  @brief
		*    Destructor
		*/
		~TickInterpolationHelper();

		/**
		*  @brief
		*    Update the simulation transformation
		*
		*  @note
		*    - This should be called once a tick only, when the final transformation for that tick is known
		*/
		void setTransformation(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& linearVelocity, const glm::quat& angularVelocity, bool warped);

		/**
		*  @brief
		*    Calculate an interpolated transformation
		*/
		void interpolateTransformation(float normalizedTimeOffset, glm::vec3& outInterpolatedPosition, glm::quat& outInterpolatedRotation);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void buildInterpolationFunction();
		void evaluateInterpolationFunction(float timeOffset);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Non-interpolated transformation
		LightweightTransform mFormerTransform;			///< Position and rotation of former simulation update
		LightweightTransform mFormerTransformChange;	///< Position and rotation change rate per tick of former simulation update
		LightweightTransform mCurrentTransform;			///< Position and rotation of current simulation update
		LightweightTransform mCurrentTransformChange;	///< Position and rotation change rate per tick of current simulation update

		// Interpolated transformation
		LightweightTransform mInterpolatedTransform;		///< Interpolated animation position and rotation
		float				 mLastInterpolationTimeOffset;	///< Normalized time offset of last interpolation

		struct InterpolationPoint
		{
			float mTimeOffset;
			LightweightTransform mTransform;
		};
		InterpolationPoint mInterpolationStart;
		InterpolationPoint mInterpolationEnd;

		bool mDirtyInterpolationFunction;	///< If "true", the linear interpolation is invalid and has to be re-calculated
		bool mWasWarped;					///< "true" in case the transformation was warped in the last simulation update
		bool mNoInterpolationBefore;		///< If "true", don't interpolate or extrapolate to the current transformation


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
