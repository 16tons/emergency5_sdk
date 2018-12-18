// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MeshData.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Math.h>
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
		MeshData::MeshData(const qsf::Entity& targetEntity) :
			DataCacheBase(targetEntity),
			mCustomBoundingBoxSet(false),
			mCustomBoundingBoxChanged(false),
			mCustomBoundingBoxExtendsChanged(false),
			mTintPaletteLocalAssetId(qsf::getUninitialized<uint32>()),
			mTintPaletteLocalAssetIdChanged(false),
			mTintPaletteIndex(qsf::getUninitialized<int32>()),
			mTintPaletteIndexChanged(false),
			mMeshLocalAssetId(qsf::getUninitialized<uint32>()),
			mMeshLocalAssetIdChanged(false)
		{
			mComponent = targetEntity.getComponent<qsf::MeshComponent>();
			if (mComponent.valid() && (mComponent->getId() == qsf::MeshComponent::COMPONENT_ID || mComponent->getId() == qsf::compositing::TintableMeshComponent::COMPONENT_ID ))
			{
				mCustomBoundingBoxSet = mComponent->isCustomBoundingBoxSet();
				mComponent->getOgreEntityBoundingBox(mCustomBoundBoxMin, mCustomBoundBoxMax);
				mCustomBoundingBoxExtends = mComponent->getOgreEntityBoundingBox();
				mMeshLocalAssetId = mComponent->getMesh().getLocalAssetId();

				if (mComponent->getId() == qsf::compositing::TintableMeshComponent::COMPONENT_ID)
				{
					qsf::compositing::TintableMeshComponent& tintableMeshComponent = static_cast<qsf::compositing::TintableMeshComponent&>(mComponent.getSafe());
					const qsf::AssetProxy& assetProxy = tintableMeshComponent.getTintPalette();
					mTintPaletteLocalAssetId = assetProxy.getLocalAssetId();
					mTintPaletteIndex = static_cast<int32>(tintableMeshComponent.getTintPaletteIndex());
				}
			}
			else
			{
				QSF_ERROR("MeshData: The entity \""<<targetEntity.getId()<<"\" doesn't have a MeshComponent or TintableMeshComponent for caching the data by this class", QSF_REACT_THROW);
			}
		}

		MeshData::~MeshData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		bool MeshData::includeForPrepare(const int32 currentTickCount, bool entityIsVisible)
		{
			// Do only checks each 3. tick
			return currentTickCount % 3 == 0;
		}

		bool MeshData::prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock)
		{
			if (mComponent.valid())
			{
				bool customBoundingBoxSetChanged = mCustomBoundingBoxSet != mComponent->isCustomBoundingBoxSet();
				mCustomBoundingBoxSet = mComponent->isCustomBoundingBoxSet();

				if (mCustomBoundingBoxSet)
				{
					{ // Custom bounding box
						glm::vec3 minimum;
						glm::vec3 maximum;
						mComponent->getOgreEntityBoundingBox(minimum, maximum);
						bool minimumChanged = detail::checkForChanges(minimum, mCustomBoundBoxMin);
						bool maximumChanged = detail::checkForChanges(maximum, mCustomBoundBoxMax);
						mCustomBoundingBoxChanged = minimumChanged || maximumChanged;
					}
					mCustomBoundingBoxExtendsChanged = detail::checkForChanges(mComponent->getOgreEntityBoundingBox(), mCustomBoundingBoxExtends);
				}
				else
				{
					// The custom bounding box is unset -> no need to check/transfer the bounding box values
					mCustomBoundBoxMax = qsf::Math::GLM_VEC3_ZERO;
					mCustomBoundBoxMin = qsf::Math::GLM_VEC3_ZERO;
					mCustomBoundingBoxExtends = qsf::Math::GLM_VEC3_ZERO;
					mCustomBoundingBoxChanged = false;
					mCustomBoundingBoxExtendsChanged = false;
				}

				mMeshLocalAssetIdChanged = detail::checkForChanges(mComponent->getMesh().getLocalAssetId(), mMeshLocalAssetId);

				if (mComponent->getId() == qsf::compositing::TintableMeshComponent::COMPONENT_ID)
				{
					qsf::compositing::TintableMeshComponent& tintableMeshComponent = static_cast<qsf::compositing::TintableMeshComponent&>(mComponent.getSafe());
					const qsf::AssetProxy& assetProxy = tintableMeshComponent.getTintPalette();
					uint32 localAssetId = assetProxy.getLocalAssetId();

					mTintPaletteLocalAssetIdChanged = detail::checkForChanges(localAssetId, mTintPaletteLocalAssetId);
					mTintPaletteIndexChanged = detail::checkForChanges(static_cast<int32>(tintableMeshComponent.getTintPaletteIndex()), mTintPaletteIndex);
				}
				else
				{
					qsf::setUninitialized(mTintPaletteLocalAssetId);
					mTintPaletteLocalAssetIdChanged = false;
					qsf::setUninitialized(mTintPaletteIndex);
					mTintPaletteIndexChanged = false;
				}

				return mMeshLocalAssetIdChanged || mTintPaletteIndexChanged || mTintPaletteLocalAssetIdChanged || customBoundingBoxSetChanged || mCustomBoundingBoxChanged || mCustomBoundingBoxExtendsChanged;
			}
			else
			{
				// The entity has no movable component anymore -> This shouldn't never happen. An instance of MeshData for such an entity should never be created
				QSF_ERROR("MeshData: The entity \""<<targetEntity.getId()<<"\" lost its mesh components this instance is not longer usable for that entity", QSF_REACT_NONE);
			}

			return false;
		}

		void MeshData::updateData(qsf::game::BitStream& bitStream, bool force)
		{
			// When this method is called at least one tracked data by this class has changed
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getBitLength();
#endif
			bitStream.write(mCustomBoundingBoxSet);
			if (mCustomBoundingBoxSet)
			{
				// Write information if we have an extends(true) or a complete bounding box(false)
				bitStream.write(mCustomBoundingBoxExtends != qsf::Math::GLM_VEC3_ZERO);

				// Only if a custom bounding box is set check and transfer the bounding box data
				if (mCustomBoundingBoxExtends != qsf::Math::GLM_VEC3_ZERO)
				{
					// Bounding box extend is set
					// Write data about the extends values
					bitStream.write(mCustomBoundingBoxExtendsChanged);
					if (mCustomBoundingBoxExtendsChanged)
					{
						writeBoundingBoxPartToStream(bitStream, mCustomBoundingBoxExtends);
					}
				}
				else
				{
					// Write data about that bounding box values
					bitStream.write(mCustomBoundingBoxChanged);
					if (mCustomBoundingBoxChanged)
					{
						// Write the bounding box values
						writeBoundingBoxPartToStream(bitStream, mCustomBoundBoxMin);
						writeBoundingBoxPartToStream(bitStream, mCustomBoundBoxMax);
					}
				}
			}

			detail::writeChanges(mMeshLocalAssetIdChanged, mMeshLocalAssetId, bitStream);

			detail::writeChanges(mTintPaletteLocalAssetIdChanged, mTintPaletteLocalAssetId, bitStream);
			detail::writeChanges(mTintPaletteIndexChanged, mTintPaletteIndex, bitStream);


#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getBitLength();
			QSF_LOG_PRINTS(INFO, "MeshData: written bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MeshData::setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount)
		{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t init = bitStream.getReadPosition();
#endif
			HistoryEntry entry;
			entry.tickCount = receivedHostTickCount;

			// Read if a custom bounding box is set
			bitStream.read(entry.mCustomBoundingBoxSet);

			if (entry.mCustomBoundingBoxSet)
			{
				// Read which type of bounding box we have (true = extends, false = full bounding box )
				bool haveBoundingBoxExtend;
				if (!bitStream.read(haveBoundingBoxExtend))
				{
					QSF_ERROR("MeshData Could not read data from stream", QSF_REACT_THROW);
				}

				if (haveBoundingBoxExtend)
				{
					// We have a custom bounding box set read the values
					// First check for bounding box extend changes
					bitStream.read(entry.mCustomBoundingBoxExtendsChanged);
					if (entry.mCustomBoundingBoxExtendsChanged)
					{
						// We have data about a extend of the bounding box
						readBoundingBoxPartFromStream(bitStream, entry.mCustomBoundingBoxExtends);
					}

					entry.mCustomBoundingBoxChanged = false;
				}
				else
				{
					entry.mCustomBoundingBoxExtendsChanged = false;

					// Check for bounding box data changes
					bitStream.read(entry.mCustomBoundingBoxChanged);
					if (entry.mCustomBoundingBoxChanged)
					{
						// We have data about a bounding box
						readBoundingBoxPartFromStream(bitStream, entry.mCustomBoundBoxMin);
						readBoundingBoxPartFromStream(bitStream, entry.mCustomBoundBoxMax);
					}
				}
			}

			entry.mMeshLocalAssetIdChanged = detail::checkChangedAndRead(bitStream, entry.mMeshLocalAssetId);

			entry.mTintPaletteLocalAssetIdChanged = detail::checkChangedAndRead(bitStream, entry.mTintPaletteLocalAssetId);
			entry.mTintPaletteIndexChanged =detail::checkChangedAndRead(bitStream, entry.mTintPaletteIndex);

			mHistoryList.emplace_back(entry);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			size_t end = bitStream.getReadPosition();
			QSF_LOG_PRINTS(INFO, "MeshData: read bitcount for entity \""<< mEntityId <<"\": "<<end-init);
#endif
		}

		void MeshData::interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount)
		{
			if (!mHistoryList.empty() && mComponent.valid())
			{
				auto iterator = mHistoryList.begin();
				while (iterator != mHistoryList.end())
				{
					if (iterator->tickCount == clock.getSignalCounter())
					{
						const HistoryEntry& historyEntry = *iterator;

						// Apply the read data
						if (historyEntry.mCustomBoundingBoxSet)
						{
							if (historyEntry.mCustomBoundingBoxExtendsChanged)
							{
								mComponent->setOgreEntityBoundingBox(historyEntry.mCustomBoundingBoxExtends);
							}
							else if (historyEntry.mCustomBoundingBoxChanged)
							{
								mComponent->setOgreEntityBoundingBox(historyEntry.mCustomBoundBoxMin, historyEntry.mCustomBoundBoxMax);
							}
						}
						else
						{
							mComponent->resetCustomBoundingBox();
						}

						if (historyEntry.mMeshLocalAssetIdChanged)
						{
							// Set the new mesh
							mComponent->setMesh(qsf::AssetProxy(historyEntry.mMeshLocalAssetId));
						}

						if (historyEntry.mTintPaletteIndexChanged || historyEntry.mTintPaletteLocalAssetIdChanged)
						{
							if (mComponent->getId() == qsf::compositing::TintableMeshComponent::COMPONENT_ID)
							{
								qsf::compositing::TintableMeshComponent& tintableMeshComponent = static_cast<qsf::compositing::TintableMeshComponent&>(mComponent.getSafe());
								if (historyEntry.mTintPaletteLocalAssetIdChanged)
								{
									tintableMeshComponent.setTintPalette(qsf::AssetProxy(historyEntry.mTintPaletteLocalAssetId));
								}

								if (historyEntry.mTintPaletteIndexChanged)
								{
									tintableMeshComponent.setTintPaletteIndex(historyEntry.mTintPaletteIndex);
								}
							}
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
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void MeshData::writeBoundingBoxPartToStream(qsf::game::BitStream& bitStream, const glm::vec3& boundingBoxPart)
		{
			// We have for the custom bounding boxes currently only a value range from -1.1f to 2.0f
			// We can send it as 8 bit signed value (-11 to 20)
			int8 xValue = static_cast<int8>(boundingBoxPart.x*10);
			int8 yValue = static_cast<int8>(boundingBoxPart.y*10);
			int8 zValue = static_cast<int8>(boundingBoxPart.z*10);

			bitStream.write(xValue);
			bitStream.write(yValue);
			bitStream.write(zValue);
		}

		void MeshData::readBoundingBoxPartFromStream(const qsf::game::BitStream& bitStream, glm::vec3& boundingBoxPart)
		{
			// We have for the custom bounding boxes currently only a value range from -1.1f to 2.0f
			// We can send it as 8 bit signed value (-11 to 20)
			int8 xValue;
			if (!bitStream.read(xValue))
			{
				QSF_ERROR("MeshData Could not read data from stream", QSF_REACT_THROW);
			}
			int8 yValue;
			if (!bitStream.read(yValue))
			{
				QSF_ERROR("MeshData Could not read data from stream", QSF_REACT_THROW);
			}
			int8 zValue;
			if (!bitStream.read(zValue))
			{
				QSF_ERROR("MeshData Could not read data from stream", QSF_REACT_THROW);
			}

			boundingBoxPart.x = xValue/10.0f;
			boundingBoxPart.y = yValue/10.0f;
			boundingBoxPart.z = zValue/10.0f;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
