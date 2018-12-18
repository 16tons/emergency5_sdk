// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BuildingOverlayData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		BuildingOverlayData::BuildingOverlayData(const qsf::Entity& targetEntity) :
			DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>(targetEntity),
			mRegisteredProvidersChanged(false),
			mNumberOfRequiredProvidersChanged(false),
			mNumberOfRequiredProviders(0)
		{
			const BuildingOverlayComponent* buildingOverlayComponent = getComponent(targetEntity);
			if (nullptr != buildingOverlayComponent)
			{
				for (const auto& registeredProvider : buildingOverlayComponent->getRegisteredProviders())
				{
					Provider provider;
					provider.mEntityId = registeredProvider.entityId;
					provider.mProgress = static_cast<uint8>(registeredProvider.progress * 100);
					mRegisteredProviders.emplace_back(provider);
				}

				mNumberOfRequiredProviders = static_cast<uint8>(buildingOverlayComponent->getNumberOfRequiredProviders());
			}
		}

		BuildingOverlayData::~BuildingOverlayData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool BuildingOverlayData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			// Call base implementation
			const bool componentExistenceChanged = DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>::prepareForUpdate(targetEntity, clock);

			// Check for component data changes, when component exists
			const BuildingOverlayComponent* buildingOverlayComponent = getComponent(targetEntity);
			if (nullptr != buildingOverlayComponent)
			{
				const std::vector<BuildingOverlayProvider>& registeredProviders = buildingOverlayComponent->getRegisteredProviders();
				mRegisteredProvidersChanged = (registeredProviders.size() != mRegisteredProviders.size());

				{ // First remove all known providers that seem to have been removed in the meantime
					mUnregisteredProviderEntityIds.clear();

					auto mineIterator = mRegisteredProviders.begin();
					auto theirsIterator = registeredProviders.begin();
					while (mineIterator != mRegisteredProviders.end())
					{
						if (registeredProviders.end() == theirsIterator || theirsIterator->entityId != mineIterator->mEntityId)
						{
							mUnregisteredProviderEntityIds.push_back(mineIterator->mEntityId);
							mineIterator = mRegisteredProviders.erase(mineIterator);
							mRegisteredProvidersChanged = true;
						}
						else
						{
							++mineIterator;
							++theirsIterator;
						}
					}
				}

				// Now add/update entries to match the current state
				mRegisteredProviders.resize(registeredProviders.size());
				for (size_t i = 0; i < registeredProviders.size(); ++i)
				{
					const uint64 providerEntityId = registeredProviders[i].entityId;
					const uint8 providerProgress = static_cast<uint8>(registeredProviders[i].progress * 100);

					mRegisteredProvidersChanged |= mRegisteredProviders[i].mEntityIdChanged = detail::checkForChanges(providerEntityId, mRegisteredProviders[i].mEntityId);
					mRegisteredProvidersChanged |= mRegisteredProviders[i].mProgressChanged = detail::checkForChanges(providerProgress, mRegisteredProviders[i].mProgress);
				}

				mNumberOfRequiredProvidersChanged = detail::checkForChanges(static_cast<uint8>(buildingOverlayComponent->getNumberOfRequiredProviders()), mNumberOfRequiredProviders);

				return mNumberOfRequiredProvidersChanged || mRegisteredProvidersChanged;
			}
			else if (componentExistenceChanged)
			{
				// Only do an reset if the component existence has changed and the component was destroyed
				// Reset cached data
				mRegisteredProvidersChanged = false;
				mUnregisteredProviderEntityIds.clear();
				mRegisteredProviders.clear();
				mNumberOfRequiredProvidersChanged = false;
				mNumberOfRequiredProviders = 0;
			}

			return componentExistenceChanged;
		}

		void BuildingOverlayData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>::updateData(bitStream, force);

			// Only write component data when the entity has the component
			if (hasEntityComponent())
			{
				bitStream.write(mRegisteredProvidersChanged);
				if (mRegisteredProvidersChanged)
				{
					// Support up to 15 unregistered providers
					bitStream.write(static_cast<uint8>(mUnregisteredProviderEntityIds.size()), 4);
					for (const uint64 unregisteredProviderEntityId : mUnregisteredProviderEntityIds)
					{
						bitStream.write(unregisteredProviderEntityId);
					}

					// Support up to 15 registered providers
					bitStream.write(static_cast<uint8>(mRegisteredProviders.size()), 4);
					for (const auto& registeredProvider : mRegisteredProviders)
					{
						detail::writeChanges(registeredProvider.mEntityIdChanged, registeredProvider.mEntityId, bitStream);

						// We only support mProgress values in the range [0...100] (which amount to values 0.00 to 1.00 in the BuildingOverlayComponent then)
						detail::writeChanges(registeredProvider.mProgressChanged, registeredProvider.mProgress, 7, bitStream);
					}
				}

				// Support up to 15 registered providers
				detail::writeChanges(mNumberOfRequiredProvidersChanged, mNumberOfRequiredProviders, 4, bitStream);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "BuildingOverlayData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BuildingOverlayData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif

			// Call base implementation
			DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>::setData(bitStream, receivedHostTickCount);

			// Read the component data when the component exists
			if (hasEntityComponentFromStream())
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;

				bitStream.read(entry.mRegisteredProvidersChanged);
				if (entry.mRegisteredProvidersChanged)
				{
					// Support up to 15 unregistered providers
					uint8 numberOfUnregisteredProviders = 0;
					bitStream.read(numberOfUnregisteredProviders, 4);

					entry.mUnregisteredProviderEntityIds.resize(numberOfUnregisteredProviders);
					for (uint64& unregisteredProviderEntityId : entry.mUnregisteredProviderEntityIds)
					{
						bitStream.read(unregisteredProviderEntityId);
					}

					// Support up to 15 registered providers
					uint8 numberOfRegisteredProviders = 0;
					bitStream.read(numberOfRegisteredProviders, 4);

					entry.mRegisteredProviders.resize(numberOfRegisteredProviders);
					for (auto& registeredProvider : entry.mRegisteredProviders)
					{
						registeredProvider.mEntityIdChanged = detail::checkChangedAndRead(bitStream, registeredProvider.mEntityId);

						// We only support mProgress values in the range [0...100] (which amount to values 0.00 to 1.00 in the BuildingOverlayComponent then)
						registeredProvider.mProgressChanged = detail::checkChangedAndRead(bitStream, 7, registeredProvider.mProgress);
					}
				}

				// Support up to 15 registered providers
				entry.mNumberOfRequiredProvidersChanged = detail::checkChangedAndRead(bitStream, 4, entry.mNumberOfRequiredProviders);

				mHistoryList.emplace_back(entry);
			}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "BuildingOverlayData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void BuildingOverlayData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Call base implementation
			DataCacheComponentExistenceTrackingBase<BuildingOverlayComponent>::interpolate(targetEntity, clock, receivedHostTickCount);

			if (!mHistoryList.empty())
			{
				BuildingOverlayComponent* buildingOverlayComponent = getComponent(targetEntity);
				if (nullptr != buildingOverlayComponent)
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						const HistoryEntry& entry = *iterator;
						if (entry.tickCount == clock.getSignalCounter())
						{
							if (entry.mRegisteredProvidersChanged)
							{
								{ // First remove all known providers that seem to have been removed in the meantime
									for (const uint64 unregisteredProvider : entry.mUnregisteredProviderEntityIds)
									{
										buildingOverlayComponent->unregisterProvider(unregisteredProvider);
									}
								}

								// Now add/update entries to match the current state
								const std::vector<BuildingOverlayProvider>& registeredProviders = buildingOverlayComponent->getRegisteredProviders();
								for (size_t i = 0; i < entry.mRegisteredProviders.size(); ++i)
								{
									if (entry.mRegisteredProviders[i].mEntityIdChanged)
									{
										buildingOverlayComponent->registerOrUpdateProvider(entry.mRegisteredProviders[i].mEntityId, static_cast<float>(entry.mRegisteredProviders[i].mProgress) / 100.0f);
									}
									else if (entry.mRegisteredProviders[i].mProgressChanged)
									{
										QSF_CHECK(i < registeredProviders.size(), "Multiplayer synchronization error in the building overlay component of the entity with the id \"" << buildingOverlayComponent->getEntityId() << "\"", continue);
										buildingOverlayComponent->registerOrUpdateProvider(registeredProviders[i].entityId, static_cast<float>(entry.mRegisteredProviders[i].mProgress) / 100.0f);
									}
								}
							}

							if (entry.mNumberOfRequiredProvidersChanged)
							{
								buildingOverlayComponent->setNumberOfRequiredProviders(entry.mNumberOfRequiredProviders);
							}

							iterator = mHistoryList.erase(iterator);
						}
						else
						{
							// The entry is not for the current tick -> no further processing possible
							return;
						}
					}
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
