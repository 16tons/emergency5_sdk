// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DebugDrawProxy;
	class BinarySerializer;
}


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
	*    Ground map level class
	*/
	class QSF_API_EXPORT GroundMapLevel
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GroundMap;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef uint16 SampleType;
		static const SampleType INVALID_VALUE = 0;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline GroundMapLevel();

		inline SampleType* operator*();
		inline const SampleType* operator*() const;

		inline bool empty() const;
		inline size_t size() const;

		void create(uint32 width, uint32 height);
		void optimize();
		void setRangeY(float minimumY, float rangeY);

		inline uint32 getWidth() const;
		inline uint32 getHeight() const;
		inline uint32 getWalkableLevel() const;
		inline float getHeightForSample(SampleType sample) const;

		float sampleBilinear(float normalizedLocalX, float normalizedLocalZ) const;
		bool sampleBilinearFloating(float normalizedLocalX, float normalizedLocalZ, float& outHeight) const;

		void debugDrawAt(float normalizedLocalX, float normalizedLocalZ, float radius, bool floating, DebugDrawProxy& debugDrawProxy) const;

		bool serialize(BinarySerializer& binarySerializer, bool convertEndianness);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32					mWidth;				///< Width of the height map in samples (along world space x-axis)
		uint32					mHeight;			///< Height of the height map in samples (along world space z-axis)
		float					mMinimumY;			///< World space y-position associated with minimum sample value
		float					mRangeY;			///< World space y-position range associated with sample value range (usually negative, because higher sample values represent a lower y-height)
		float					mRangeYMultiplier;
		std::vector<SampleType>	mSamples;			///< The sampled values as 2D array in row-major order
		std::vector<bool>		mValidBlocks;
		uint32					mValidBlocksStride;
		bool					mActive;			///< If "false", this level won't get sampled
		uint32					mWalkableLevel;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/ground/GroundMapLevel-inl.h"
