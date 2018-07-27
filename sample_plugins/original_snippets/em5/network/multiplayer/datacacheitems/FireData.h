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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireComponent;
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
		*    EMERGENCY 5 entity with a fire component data cache for a single entity for multiplayer
		*/
		class FireData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			FireData(const qsf::Entity& targetEntity);
			virtual ~FireData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible) override;
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<FireComponent> mComponent;
			bool	mActive;
			uint8	mFireState;	///< The fire state of the fire component
			bool	mFireStateChanged;
			float	mFireEnergy;	///< The current fire energy of the component
			bool	mFireEnergyChanged;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool	mActive;
				uint8	mFireState;	///< The fire state of the fire component
				bool	mFireStateChanged;
				float	mFireEnergy;	///< The current fire energy of the component
				bool	mFireEnergyChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
