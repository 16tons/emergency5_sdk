// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/NavigationData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf_game/network/BitStream.h>

#include <qsf/log/LogSystem.h>


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
		NavigationData::NavigationData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mForwardSpeed(0.0f),
			mForwardSpeedChanged(false)
		{
			const qsf::ai::NavigationComponent* navigationComponent = targetEntity.getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				mForwardSpeed = navigationComponent->getMaxForwardSpeed();
			}
			else
			{
				QSF_ERROR("NavigationData: The entity \""<<targetEntity.getId()<<"\" doesn't have a ai navigation component for caching the data by this class", QSF_REACT_THROW);
			}
		}

		NavigationData::~NavigationData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool NavigationData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			const qsf::ai::NavigationComponent* navigationComponent = targetEntity.getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				mForwardSpeedChanged = navigationComponent->getNativeMaxForwardSpeed().getValue() != mForwardSpeed;
				if (mForwardSpeedChanged)
				{
					mForwardSpeed = navigationComponent->getNativeMaxForwardSpeed().getValue();
				}

				return mForwardSpeedChanged;
			}

			return false;
		}

		void NavigationData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif

			detail::writeChanges(mForwardSpeedChanged, mForwardSpeed, bitStream);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "NavigationData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void NavigationData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			// TODO(sw) This tracking might not be needed anymore!
			//qsf::ai::NavigationComponent* navigationComponent = targetEntity.getComponent<qsf::ai::NavigationComponent>();
			//// Additional check if the target entity has a movable component
			//if (nullptr != navigationComponent)
			//{
			//	// Yep the target has also the movable component as on the host side
			//	// Read the data from the stream and apply it to the target

			//	if (detail::checkChangedAndRead(bitStream, mForwardSpeed))
			//	{
			//		navigationComponent->setNativeMaxForwardSpeed(mForwardSpeed);
			//	}
			//}

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "NavigationData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void NavigationData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			// Nothing to do here currently
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
