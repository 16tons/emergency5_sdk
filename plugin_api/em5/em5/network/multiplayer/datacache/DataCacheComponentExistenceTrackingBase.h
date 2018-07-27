// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/datacache/DataCacheBase.h"

#include <qsf/base/WeakPtr.h>


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
		*    EMERGENCY 5 multiplayer data cache base class which tracks component existence
		*/
		template<class TrackedComponentType>
		class DataCacheComponentExistenceTrackingBase : public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline DataCacheComponentExistenceTrackingBase(const qsf::Entity& entity, bool noDestruction = false);


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			inline virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			inline virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			inline virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			inline virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline TrackedComponentType* getComponent(const qsf::Entity& entity);
			inline bool hasEntityComponentFromStream();
			inline bool hasEntityComponent();
			inline bool hasComponentExistenceChanged();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<TrackedComponentType> mComponent;
			uint8	mComponentChangeCounter;
			bool	mNoDestruction;
			bool	mEntityHasComponentFromStream;

			bool	mEntityHasComponent;
			bool	mComponentExistenceChanged;

			struct HistoryEntry
			{
				int32 tickCount;
				bool	mEntityHasComponent;
				bool	mComponentExistenceChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase-inl.h"
