// Copyright (C) 2012-2018 Promotion Software GmbH


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
	class HelicopterComponent;
	class HelicopterHatchComponent;
}
namespace qsf
{
	class Entity;
	class TransformComponent;
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
		*    EMERGENCY 5 entity with a helicopter component data cache for a single entity for multiplayer
		*/
		class HelicopterData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			HelicopterData(const qsf::Entity& targetEntity);
			virtual ~HelicopterData();


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
			qsf::WeakPtr<HelicopterComponent> mComponent;
			qsf::WeakPtr<qsf::TransformComponent> mTransformComponent;
			qsf::WeakPtr<HelicopterHatchComponent> mHelicopterHatchComponent;
			bool	mIsFlying;
			bool	mPositionYChanged;
			float	mPositionY;
			bool	mHatchStateChanged;
			uint8	mHatchState;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool	mIsFlying;
				bool	mPositionYChanged;
				float	mPositionY;
				bool	mHatchStateChanged;
				uint8	mHatchState;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
