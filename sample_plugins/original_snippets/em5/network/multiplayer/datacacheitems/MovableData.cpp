// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MovableData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/time/clock/Clock.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>

// TODO(sw) This is only for debugging the prediction system
#ifndef ENDUSER
	#include <qsf/QsfHelper.h>
	#include <qsf/debug/DebugDrawProxy.h>
	#include <qsf/debug/request/CircleDebugDrawRequest.h>
#endif

// TODO(sw) Only for testing
//static std::set<uint64> mMovingEntities;


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
		MovableData::MovableData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mInitialTransfer(true),
			mForceTransfer(false),
			mUnuseable(false),
			mEntityVisible(false),
			mIsUnit(false),
			mVelocityChanged(false),
			mPositionChanged(false),
			mRotationChanged(false),
			mChangedRotation(0),
			mContainerEntityChanged(false),
			mContainerCategory(0)
		{
			mIsUnit = nullptr != targetEntity.getComponent<CommandableComponent>();
			mComponent = targetEntity.getComponent<qsf::MovableComponent>();
			if (mComponent.valid())
			{
				mLastRotation = mComponent->getRotation();
				mLastPosition = mComponent->getPosition();
				mContainerEntityId = mComponent->getContainerEntityId();

				const qsf::ContainerLink* containerLink = mComponent->getContainerLink();
				if (nullptr != containerLink)
				{
					mContainerCategory = static_cast<uint8>(containerLink->mContainerCategory);
				}

				mIsMoving = mComponent->isMoving();
			}
			else
			{
				QSF_ERROR("MovableData: The entity \""<<targetEntity.getId()<<"\" doesn't have a MovableComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		MovableData::~MovableData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool MovableData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// The reduced update of movement data is only done for civilians
			if (!mIsUnit && !mInitialTransfer && !mEntityVisible && entityIsVisible)
			{
				mForceTransfer = true;
			}
			mEntityVisible = entityIsVisible;

			// Don't include me when I'm unusable e.g. the entity lost its movable component
			return !mUnuseable;
		}

		bool MovableData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				mContainerEntityChanged = detail::checkForChanges(mComponent->getContainerEntityId(), mContainerEntityId);
				const bool isMovingChanged = detail::checkForChanges(mComponent->isMoving(), mIsMoving);

				//// TODO(sw) Only for testing
				//if (mIsMoving && isMovingChanged)
				//{
				//	if (mMovingEntities.find(mEntityId) == mMovingEntities.end())
				//	{
				//		mMovingEntities.emplace(mEntityId);
				//		QSF_LOG_PRINTS(INFO, "MovableData entity moving: "<<mEntityId);
				//	}
				//}

				const qsf::ContainerLink* containerLink = mComponent->getContainerLink();
				if (nullptr != containerLink)
				{
					mContainerCategory = static_cast<uint8>(containerLink->mContainerCategory);
				}

				const bool isLinked = isEntityLinked(targetEntity);

				if (isLinked)
				{
					// No force transfer for linked items
					mForceTransfer = false;
				}

				if (mInitialTransfer || mForceTransfer)
				{
					mVelocityChanged = prepareVelocityForUpdate(*mComponent);
					mPositionChanged = preparePositionForUpdate(*mComponent, mInitialTransfer || mForceTransfer, false);
					mRotationChanged = prepareRotationForUpdate(*mComponent, mForceTransfer);
					mInitialTransfer = false;
					mForceTransfer = false;

					// The component was created the first time -> send initial transform information
					return mPositionChanged || mRotationChanged || mVelocityChanged || mContainerEntityChanged || isMovingChanged;
				}

				if (isLinked)
				{
					if (mContainerEntityChanged)
					{
						// If we have a change to linked status send once the position and rotation
						mPositionChanged = preparePositionForUpdate(*mComponent, true);
						mRotationChanged = prepareRotationForUpdate(*mComponent);
					}
					else
					{
						mPositionChanged = false;
						mRotationChanged = false;
						mVelocityChanged = false;
					}

					// Do nothing when entity is linked but only when the container entity has not changed
					return mContainerEntityChanged || mPositionChanged || mRotationChanged || isMovingChanged;
				}

				// The move data (position, rotation, velocity) is for civilians (person/vehicle) only updates every 1000 ms when they are not visible to any player
				// This reduces the needed bandwidth each second, but can cause jumps of such civilian entities on the mutliplayer client side, when they get visible to that players camera (Mostly caused, when the player jumps directly to a position)
				if (!mIsUnit && !mEntityVisible && clock.getSignalCounter() % 10 != 0)
				{
					mPositionChanged = false;
					mRotationChanged = false;
					mVelocityChanged = false;
					return mContainerEntityChanged || isMovingChanged;
				}

				mRotationChanged = prepareRotationForUpdate(*mComponent);

				{ // Do the same calculation as the client. Check the result against the real position and when the difference is too huge -> full position transfer
					glm::vec3 newPosition = calculateNewPosition(mLastPosition, mLastVelocity, clock.getTimePassed().getSeconds());
					mLastPosition = newPosition;

// TODO(sw) This is only for debugging the prediction system
#if 0 && !defined(ENDUSER)
					if (mLastVelocity != qsf::Math::GLM_VEC3_ZERO)
					{
						qsf::DebugDrawProxy ddp;
						ddp.registerAt(QSF_DEBUGDRAW);
						ddp.addRequest(qsf::CircleDebugDrawRequest(newPosition, glm::vec3(0, 1, 0), 0.4f, qsf::Color4::GREEN, true));
						ddp.setLifetimeInSeconds(10.0f);
						ddp.detachRequests();
					}
#endif

					glm::vec3 originalPosition = mComponent->getPosition();
					glm::vec3 diff = glm::abs(originalPosition - newPosition);
					if (originalPosition != newPosition)
					{
						static const float limit = 0.05f;
						// Check how big the difference is -> ignore here the y-axis this is modified by placement
						// For the first test we do an correction when on the x/z axis an difference >= 0.05 occurs
						// TODO(sw) Currently we ignore the y value. For helicopters also the y value must be taken into account somehow
						if (diff.x >= limit || diff.z >= limit)
						{
							mPositionChanged = preparePositionForUpdate(*mComponent, true);

							// Only update the velocity when the difference exceeds the limit and it changes
							mVelocityChanged = prepareVelocityForUpdate(*mComponent);

// TODO(sw) This is only for debugging the prediction system
#if 0 && !defined(ENDUSER)
							originalPosition.y += 1.0f;
							qsf::DebugDrawProxy ddp2;
							ddp2.registerAt(QSF_DEBUGDRAW);
							ddp2.addRequest(qsf::CircleDebugDrawRequest(originalPosition, glm::vec3(0, 1, 0), 0.4f, qsf::Color4::RED, true));
							ddp2.setLifetimeInSeconds(5.0f);
							ddp2.detachRequests();
#endif
						}
						else
						{
							mVelocityChanged = false;
							mPositionChanged = false;
						}
					}
					else
					{
						mVelocityChanged = false;
						mPositionChanged = false;
					}
				}

				return mVelocityChanged || mPositionChanged || mRotationChanged || mContainerEntityChanged || isMovingChanged;
			}
			else
			{
				// The entity has no movable component anymore -> This shouldn't never happen. An instance of MovableData for such an entity should never be created
				QSF_ERROR("MovableData: The entity \""<<targetEntity.getId()<<"\" lost its movable components this instance is not longer usable for that entity", QSF_REACT_NONE);
				mUnuseable = true;
			}

			return false;
		}

		void MovableData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			//// TODO(sw) Only for testing
			//if (mMovingEntities.find(mEntityId) == mMovingEntities.end())
			//{
			//	mMovingEntities.emplace(mEntityId);
			//	QSF_LOG_PRINTS(INFO, "MovableData entity writing: "<<mEntityId);
			//}

			detail::writeChanges(mContainerEntityChanged, mContainerEntityId, bitStream);
			if (mContainerEntityChanged)
			{
				bitStream.write(mContainerCategory, 3);
			}

			writeVelocityToStream(bitStream);

			writePositionToStream(bitStream);

			writeRotationToStream(bitStream);

			// Write is moving flag
			bitStream.write(mIsMoving);


#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "MovableData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MovableData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
			// TODO(sw) make it similiar to the other cache items which tracks component existence
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			entry.containerEntityChanged = detail::checkChangedAndRead(bitStream, entry.containerEntityId);

			if (entry.containerEntityChanged)
			{
				if (!bitStream.read(entry.mContainerCategory, 3))
				{
					QSF_ERROR("MovableData: Could not read value from stream", QSF_REACT_THROW);
				}
			}

			entry.velocityChanged = readVelocityFromStream(bitStream, entry.velocity);

			entry.positionChanged = readPositionFromStream(bitStream, entry.position);

			entry.rotationChanged = readRotationFromStream(bitStream, entry.rotationAroundYAxis);

			// Read is moving flag
			if (!bitStream.read(entry.mIsMoving))
			{
				QSF_ERROR("MovableData: Could not read is moving flag from data stream", QSF_REACT_THROW);
			}

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "MovableData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MovableData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (mComponent.valid())
			{
				bool doPositionPrediction = !isEntityLinked(targetEntity);
				bool rotationHasChanged = false;
				if (!mHistoryList.empty())
				{
					auto iterator = mHistoryList.begin();
					while (iterator != mHistoryList.end())
					{
						if (iterator->tickCount == clock.getSignalCounter())
						{
							const HistoryEntry& historyEntry = *iterator;

							if (mIsMoving != historyEntry.mIsMoving)
							{
								mIsMoving = historyEntry.mIsMoving;
							}

							if (historyEntry.containerEntityChanged)
							{
								EntityHelper entityHelper(targetEntity);

								if (qsf::isInitialized(historyEntry.containerEntityId))
								{
									qsf::Entity* containerEntity = targetEntity.getMap().getEntityById(historyEntry.containerEntityId);
									QSF_CHECK(nullptr != containerEntity, "MovableData: The given container entity doesn't exists", QSF_REACT_NONE);

									if (nullptr != containerEntity)
									{
										// The hidden flag which is internally set by the enterContainer method is separately tracked -> tell the enterContainer method don't hide the entity
										entityHelper.enterContainer(*containerEntity, historyEntry.mContainerCategory, false);
										doPositionPrediction = false;
									}
								}
								else
								{
									// Unhiding the entity is done via tracking the hidden component
									entityHelper.leaveContainer(false);
								}
							}

							if (historyEntry.rotationChanged)
							{
								rotationHasChanged = true;
								mLastRotation = qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::radians(historyEntry.rotationAroundYAxis));
							}

							// Apply the read data
							if (historyEntry.positionChanged )
							{
								doPositionPrediction = false;
								if (mInitialTransfer)
								{
									mComponent->warpToPositionAndRotation(historyEntry.position, mLastRotation);
									mInitialTransfer = false;
								}
								else
								{
									mComponent->moveToPositionAndRotation(historyEntry.position, mLastRotation);
								}
								mLastPosition = mComponent->getPosition();
							}

							if (historyEntry.velocityChanged)
							{
								mCurrentVelocity = historyEntry.velocity;
							}

							iterator = mHistoryList.erase(iterator);
						}
						else
						{
							// The entry is not for the current tick -> no further processing possible
							break;
						}
					}
				}

				if (mIsMoving && doPositionPrediction && mCurrentVelocity != qsf::Math::GLM_VEC3_ZERO)
				{
					glm::vec3 newPosition = calculateNewPosition(mLastPosition, mCurrentVelocity, clock.getTimePassed().getSeconds());
					if (newPosition != mLastPosition)
					{
						mComponent->moveToPositionAndRotation(newPosition, mLastRotation);
						mLastPosition = mComponent->getPosition();
					}
					else if (rotationHasChanged)
					{
						// When the entity position hasn't changed but the rotation has changed apply it
						mComponent->moveToRotation(mLastRotation);
					}
				}
				else if (rotationHasChanged)
				{
					// When the entity is not moving but the rotation has changed apply it
					mComponent->moveToRotation(mLastRotation);
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		bool MovableData::isEntityLinked(const qsf::Entity& entity) const
		{
			qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
			return (nullptr != linkComponent && qsf::isInitialized(linkComponent->getParentId()));
		}

		bool MovableData::preparePositionForUpdate(const qsf::MovableComponent& movableComponent, bool includeHeight, bool force)
		{
			// We support up to +- 3 km (+-3,000.0f) with accuracy of three decimal places (+-3,000,000.0f)
			// Thus we need only 23 bits to represent the value (in non negative way)
			// To support negative values we use the 23th bit to encode if the value is negative
			// On the y-axis we have a range of +-250m (+-250.0f) with accuracy of three decimal places (+-250,000.0f)
			// We can use here a 18 bit value (in non negative way)
			// To support negative values we use the 18th bit to encode if the value is negative
			// First reset changed member
			mChangedPositionChanged[0] = false;
			mChangedPositionChanged[1] = false;
			mChangedPositionChanged[2] = false;

			const glm::vec3& newValue = movableComponent.getPosition();

			// Three decimal places accuracy
			int32 newValueX = static_cast<int32>(newValue.x*1000);
			int32 newValueY = static_cast<int32>(newValue.y*1000);
			int32 newValueZ = static_cast<int32>(newValue.z*1000);

			int32 currentValueX = static_cast<int32>(mLastPosition.x*1000);
			int32 currentValueY = static_cast<int32>(mLastPosition.y*1000);
			int32 currentValueZ = static_cast<int32>(mLastPosition.z*1000);

			bool somethingChanged = false;

			if (currentValueX != newValueX || force)
			{
				mChangedPositionChanged[0] = true;
				somethingChanged = true;

				if (newValueX < 0)
				{
					// Clamp the position value to a 23 bit unsigned value and use the 24th bit as negative value indecator
					mChangedPosition[0] = (static_cast<uint32>((newValueX * -1) & 0x7FFFFF)) + 0x800000;
				}
				else
				{
					// Clamp the position value to a 23 bit unsigned value and use the 24th bit as negative value indecator
					mChangedPosition[0] = static_cast<uint32>((newValueX) & 0x7FFFFF);
				}
			}

			if (includeHeight && ( currentValueY != newValueY || force))
			{
				// Special handling y-position ( max/min = 250.0f/-250.0f => +-250,000 => 18bit unsigned value and use the 18th bit as negative value indector ===> max value +-262.143f)
				mChangedPositionChanged[1] = true;
				somethingChanged = true;

				if (newValueY < 0)
				{
					// Clamp the position value to a 18 bit unsigned value and use the 18th bit as negative value indecator
					mChangedPosition[1] = (static_cast<uint32>((newValueY * -1) & 0x3FFFF)) + 0x40000;
				}
				else
				{
					// Clamp the position value to a 18 bit unsigned value and use the 18th bit as negative value indecator
					mChangedPosition[1] = static_cast<uint32>((newValueY) & 0x3FFFF);
				}
			}

			if (currentValueZ != newValueZ || force)
			{
				mChangedPositionChanged[2] = true;
				somethingChanged = true;
				if (newValueZ < 0)
				{
					// Clamp the position value to a 23 bit unsigned value and use the 24th bit as negative value indecator
					mChangedPosition[2] = (static_cast<uint32>((newValueZ * -1) & 0x7FFFFF)) + 0x800000;
				}
				else
				{
					// Clamp the position value to a 23 bit unsigned value and use the 24th bit as negative value indecator
					mChangedPosition[2] = static_cast<uint32>((newValueZ) & 0x7FFFFF);
				}
			}

			mLastPosition = newValue;

			return somethingChanged;
		}

		void MovableData::writePositionToStream(qsf::game::BitStream& bitStream)
		{
			bitStream.write(mPositionChanged);
			if (mPositionChanged)
			{
				for (size_t index = 0; index < 3; ++index)
				{
					bitStream.write(mChangedPositionChanged[index]);
					if (mChangedPositionChanged[index])
					{
						if (index == 1)
						{
							// Special handling y-position ( max = 250.0f => 250,000 ==> uint32 but with 19bits)
							bitStream.write(mChangedPosition[index], 19);
						}
						else
						{
							bitStream.write(mChangedPosition[index], 24);
						}
					}
				}
			}
		}

		bool MovableData::readPositionFromStream(const qsf::game::BitStream& bitStream, glm::vec3& position)
		{
			if (detail::checkChanged(bitStream))
			{
				// We have a position update
				// We encoded the values as follows:
				// - For x/z: The position value with 3 decimal places accuracy as an 24bit unsigned value, the bits 0-22 are the position value and the 23th bit is used as indicator if the value is negative
				// - For y: On y direction we have one a range of +-250.0f thus we can encode the value as an 19bit unsigned value, the bits 0-17 are the position value and the 18th bit is used as indicator if the value is negative
				// Check if the x value has changed and read the value
				uint32 streamValue;
				if (detail::checkChangedAndRead(bitStream, 24, streamValue))
				{
					bool isNegative = (streamValue & 0x800000) > 0;
					mLastPosition.x = (streamValue & 0x7FFFFF)/1000.0f;
					if (isNegative)
					{
						mLastPosition.x *= -1;
					}
				}

				// Check if the y value has changed and read the value
				// Special handling y-position ( max = 250.0f => 250,000 ==> uint32) but only 19 bits
				uint32 yStreamValue;
				if (detail::checkChangedAndRead(bitStream, 19, yStreamValue))
				{
					bool isNegative = (yStreamValue & 0x40000) > 0;
					mLastPosition.y = (yStreamValue & 0x3FFFF)/1000.0f;
					if (isNegative)
					{
						mLastPosition.y *= -1;
					}
				}

				// Check if the z value has changed and read the value
				if (detail::checkChangedAndRead(bitStream, 24, streamValue))
				{
					bool isNegative = (streamValue & 0x800000) > 0;
					mLastPosition.z = (streamValue & 0x7FFFFF)/1000.0f;
					if (isNegative)
					{
						mLastPosition.z *= -1;
					}
				}

				position = mLastPosition;
				return true;
			}

			return false;
		}

		bool MovableData::prepareRotationForUpdate(const qsf::MovableComponent& movableComponent, bool force)
		{
			const glm::quat& newValue = movableComponent.getRotation();

			// Rotation are stored as 8bit signed values (as euler angles, -180° - 180°)
			glm::vec3 newEulerAngles = glm::degrees(qsf::EulerAngles::quaternionToEuler(newValue));
			glm::vec3 currentEulerAngles = glm::degrees(qsf::EulerAngles::quaternionToEuler(mLastRotation));

			int8 newValueX = detail::wrapEulerAngle(newEulerAngles.x);
			int8 currentValueX = detail::wrapEulerAngle(currentEulerAngles.x);

			bool somethingChanged = force || newValueX != currentValueX;

			if (somethingChanged)
			{
				// Write changed value
				mChangedRotation = newValueX;
			}

			mLastRotation = newValue;

			return somethingChanged;
		}

		void MovableData::writeRotationToStream(qsf::game::BitStream& bitStream)
		{
			detail::writeChanges(mRotationChanged, mChangedRotation, bitStream);
		}

		bool MovableData::readRotationFromStream(const qsf::game::BitStream& bitStream, float& rotation)
		{
			int8 rotationX;
			if (detail::checkChangedAndRead(bitStream, rotationX))
			{
				rotation = detail::unwrapEulerAngle(rotationX);
				return true;
			}

			return false;
		}

		bool MovableData::prepareVelocityForUpdate(const qsf::MovableComponent& movableComponent)
		{
			const glm::vec3 newValue = mComponent->getVelocity();

			// Velocity is only in x/z direction != 0
			// For the velocity we have min max range of -50 - 50 and we use a max accuracy of 3 values after the decimal point (+- 50,000.0f)
			// We can use here a 16 bit value (in non negative way)
			// To support negative values we use the 16th bit to encode if the value is negative (The whole value uses 17 bits)

			// First reset changed member
			mChangedVelocityChanged[0] = false;
			mChangedVelocityChanged[1] = false;

			// Three decimal places accuracy
			int32 newValueX = static_cast<int32>(newValue.x*1000);
			int32 newValueZ = static_cast<int32>(newValue.z*1000);

			int32 currentValueX = static_cast<int32>(mLastVelocity.x*1000);
			int32 currentValueZ = static_cast<int32>(mLastVelocity.z*1000);

			bool somethingChanged = false;

			if (currentValueX != newValueX)
			{
				mChangedVelocityChanged[0] = true;
				somethingChanged = true;

				if (newValueX < 0)
				{
					// Clamp the position value to a 16 bit unsigned value and use the 16th bit as negative value indecator
					mChangedVelocity[0] = static_cast<uint16>(newValueX * -1) + 0x10000;
				}
				else
				{
					// Clamp the position value to a 16 bit unsigned value and use the 16th bit as negative value indecator
					mChangedVelocity[0] = static_cast<uint16>(newValueX);
				}
			}

			if (currentValueZ != newValueZ)
			{
				mChangedVelocityChanged[1] = true;
				somethingChanged = true;

				if (newValueZ < 0)
				{
					// Clamp the position value to a 16 bit unsigned value and use the 16th bit as negative value indecator
					mChangedVelocity[1] = static_cast<uint16>(newValueZ * -1) + 0x10000;
				}
				else
				{
					// Clamp the position value to a 16 bit unsigned value and use the 16th bit as negative value indecator
					mChangedVelocity[1] = static_cast<uint16>(newValueZ);
				}
			}

			// To have for the local prediction check the same values as the client do the conversion here too
			mLastVelocity.x = static_cast<int32>(newValue.x * 1000) / 1000.0f;
			mLastVelocity.z = static_cast<int32>(newValue.z * 1000) / 1000.0f;

			return somethingChanged;
		}

		void MovableData::writeVelocityToStream(qsf::game::BitStream& bitStream)
		{
			bitStream.write(mVelocityChanged);
			if (mVelocityChanged)
			{
				// Velocity is only in x/z direction != 0
				// For the velocity we have min max range of -50 - 50 and we use a max accuracy of 3 values after the decimal point
				for (size_t index = 0; index < 2; ++index)
				{
					bitStream.write(mChangedVelocityChanged[index]);

					if (mChangedVelocityChanged[index])
					{
						bitStream.write(mChangedVelocity[index], 17);
					}
				}
			}
		}

		bool MovableData::readVelocityFromStream(const qsf::game::BitStream& bitStream, glm::vec3& velocity)
		{
			if (detail::checkChanged(bitStream))
			{
				// Velocity is only in x/z direction != 0
				// For the velocity we have min max range of -50 - 50 and we use a max accuracy of 3 values after the decimal point
				uint32 streamValue;
				if (detail::checkChangedAndRead(bitStream, 17, streamValue))
				{
					bool isNegative = (streamValue & 0x10000) > 0;
					mLastVelocity.x = static_cast<uint16>(streamValue)/1000.0f;
					if (isNegative)
					{
						mLastVelocity.x *= -1;
					}
				}

				if (detail::checkChangedAndRead(bitStream, 17, streamValue))
				{
					bool isNegative = (streamValue & 0x10000) > 0;
					mLastVelocity.z = static_cast<uint16>(streamValue)/1000.0f;
					if (isNegative)
					{
						mLastVelocity.z *= -1;
					}
				}

				velocity = mLastVelocity;
				return true;
			}

			return false;
		}

		glm::vec3 MovableData::calculateNewPosition(const glm::vec3& position, const glm::vec3& velocity, float timePassedInSeconds)
		{
			// Extrapolation for vectors is easy: Just add the right portion of the change rate
			return position + velocity * timePassedInSeconds;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
