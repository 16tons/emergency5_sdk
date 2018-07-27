// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/network/BitStream.h>

#include <qsf/map/Entity.h>
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/time/clock/Clock.h>


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
		template<class TrackedComponentType>
		inline DataCacheComponentExistenceTrackingBase<TrackedComponentType>::DataCacheComponentExistenceTrackingBase(const qsf::Entity& entity, bool noDestruction) :
			DataCacheBase(entity),
			mComponentChangeCounter(0),
			mNoDestruction(noDestruction),
			mEntityHasComponentFromStream(false),
			mEntityHasComponent(false),
			mComponentExistenceChanged(false)
		{
			mComponentChangeCounter = entity.getComponentChangesCount();
			mComponent = entity.getComponent<TrackedComponentType>();
			mEntityHasComponent = mComponent.valid();
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		template<class TrackedComponentType>
		bool DataCacheComponentExistenceTrackingBase<TrackedComponentType>::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponentChangeCounter != targetEntity.getComponentChangesCount())
			{
				mComponentChangeCounter = targetEntity.getComponentChangesCount();
				// TODO(sw) Could we avoid to call getComponent here? Because this part is called in every tick
				//			The case when an existing component gets destroy can be handled via checking the weakptr.
				//          But how do the check in a better way (performance wise) to find out if a component got created in the entity?
				TrackedComponentType* component = targetEntity.getComponent<TrackedComponentType>();

				if (nullptr == component && mEntityHasComponent)
				{
					// The entity lost its component since last update
					mComponentExistenceChanged = true;
					mEntityHasComponent = false;
				}
				else if (nullptr != component && !mEntityHasComponent)
				{
					// The entity got its component since last update
					mEntityHasComponent = true;
					mComponentExistenceChanged = true;
				}
				else
				{
					// Noting has changed so save that
					mComponentExistenceChanged = false;
					mEntityHasComponent = nullptr != component;
				}

				mComponent = component;
			}
			else
			{
				mComponentExistenceChanged = false;
			}

			return mComponentExistenceChanged;
		}

		template<class TrackedComponentType>
		void DataCacheComponentExistenceTrackingBase<TrackedComponentType>::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// Write if the component existence has changed
			bitStream.write(mComponentExistenceChanged);

			// Write component existence. This values is always needed for the client side, when it reads the data from the stream
			bitStream.write(mEntityHasComponent);
		}

		template<class TrackedComponentType>
		void DataCacheComponentExistenceTrackingBase<TrackedComponentType>::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// When this method is called at least one tracked data by this class has changed on the host side

			// Read if the component existence has changed
			if (!bitStream.read(mComponentExistenceChanged))
			{
				QSF_ERROR("DataCacheComponentExistenceTrackingBase: Could not read the component existence state for the entity \"" << mEntityId << "\" from the data stream", return);
			}

			// Read component existence
			if (!bitStream.read(mEntityHasComponentFromStream))
			{
				QSF_ERROR("DataCacheComponentExistenceTrackingBase: Could not read if component exists for the entity \"" << mEntityId << "\" from the data stream", return);
			}

			if (mComponentExistenceChanged)
			{
				HistoryEntry entry;
				entry.tickCount = receivedHostTickCount;
				entry.mEntityHasComponent = mEntityHasComponentFromStream;
				entry.mComponentExistenceChanged = mComponentExistenceChanged;
				mHistoryList.emplace_back(entry);
			}
		}

		template<class TrackedComponentType>
		void DataCacheComponentExistenceTrackingBase<TrackedComponentType>::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					const HistoryEntry& entry = *iterator;
					if (entry.tickCount == clock.getSignalCounter())
					{
						// Store the value in a additional variable, because the derived class needs to access this value
						mEntityHasComponent = entry.mEntityHasComponent;
						mComponentExistenceChanged = entry.mComponentExistenceChanged;

						if (entry.mEntityHasComponent)
						{
							// The stream says that the entity should have the component now -> make it happen
							mComponent = targetEntity.getOrCreateComponent<TrackedComponentType>();
						}
						else if (!mNoDestruction)
						{
							// The stream says that the entity should not have the component now -> make it happen
							targetEntity.destroyComponent<TrackedComponentType>();
							mComponent.clear();
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


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		template<class TrackedComponentType>
		inline TrackedComponentType* DataCacheComponentExistenceTrackingBase<TrackedComponentType>::getComponent(const qsf::Entity& entity)
		{
			return mComponent.get();
		}

		template<class TrackedComponentType>
		inline bool DataCacheComponentExistenceTrackingBase<TrackedComponentType>::hasEntityComponentFromStream()
		{
			return mEntityHasComponentFromStream;
		}

		template<class TrackedComponentType>
		inline bool DataCacheComponentExistenceTrackingBase<TrackedComponentType>::hasEntityComponent()
		{
			return mEntityHasComponent;
		}

		template<class TrackedComponentType>
		inline bool DataCacheComponentExistenceTrackingBase<TrackedComponentType>::hasComponentExistenceChanged()
		{
			return mComponentExistenceChanged;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
