// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/logic/action/ActionComponent.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class ActionComponent;
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
		*    EMERGENCY 5 entity with a action component data cache for a single entity for multiplayer
		*
		*  @note
		*    This tracks only the current action priority
		*/
		class ActionData: public DataCacheComponentExistenceTrackingBase<qsf::ActionComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			ActionData(const qsf::Entity& targetEntity);
			virtual ~ActionData();


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
			qsf::action::Priority	mCurrentPriority;
			bool					mCurrentPriorityChanged;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				qsf::action::Priority	mCurrentPriority;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
