// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"


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
		*    EMERGENCY 5 entity with a building overlay component data cache for a single entity for multiplayer
		*/
		class BuildingOverlayData: public DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			BuildingOverlayData(const qsf::Entity& targetEntity);
			virtual ~BuildingOverlayData();


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
			struct Provider
			{
				bool mEntityIdChanged;
				uint64 mEntityId;
				bool mProgressChanged;
				uint8 mProgress;

				inline Provider() :
					mEntityIdChanged(false),
					mEntityId(qsf::getUninitialized<uint64>()),
					mProgressChanged(false),
					mProgress(0)
				{
					// Nothing to do in here
				}
			};

			bool mRegisteredProvidersChanged;
			std::vector<uint64> mUnregisteredProviderEntityIds;
			std::vector<Provider> mRegisteredProviders;
			bool mNumberOfRequiredProvidersChanged;
			uint8 mNumberOfRequiredProviders;

			struct HistoryEntry
			{
				int32 tickCount;
				bool mRegisteredProvidersChanged;
				std::vector<uint64> mUnregisteredProviderEntityIds;
				std::vector<Provider> mRegisteredProviders;
				bool mNumberOfRequiredProvidersChanged;
				uint8 mNumberOfRequiredProviders;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
