// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase.h"

#include <qsf/base/WeakPtr.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class SwingComponent;
}
namespace qsf
{
	class Entity;
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 entity with a swing component data cache for a single entity for multiplayer
		*/
		class SwingData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SwingData(const qsf::Entity& targetEntity);
			virtual ~SwingData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<SwingComponent> mComponent;
			bool	mIsActive;
			uint8	mAnimationType;				// Current value range 0-2
			bool	mAnimationTypeChanged;
			uint8	mAnimationCylce;			// Currently two values are used: 30.0f and 40.0f
			bool	mAnimationCylceChanged;
			uint8	mAnimationIntensity;		// Currently only one value is used: 4.0f
			bool	mAnimationIntensityChanged;
			int8	mVelocitySkew;				// Currently only two values are used: -0.15f, 0.15f
			bool	mVelocitySkewChanged;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool	mIsActive;
				uint8	mAnimationType;
				bool	mAnimationTypeChanged;
				uint8	mAnimationCylce;
				bool	mAnimationCylceChanged;
				uint8	mAnimationIntensity;
				bool	mAnimationIntensityChanged;
				int8	mVelocitySkew;
				bool	mVelocitySkewChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
