// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/batchprocess/BatchJobMigrateSkeletonCoordinateSystem.h"

#include <qsf_editor/asset/AssetEditHelper.h>

#include <qsf/asset/Asset.h>
#include <qsf/asset/AssetProxy.h>
#include <qsf/asset/CachedAsset.h>
#include <qsf/asset/AssetSystem.h>
#include <qsf/asset/project/AssetPackage.h>
#include <qsf/file/FileSystem.h>
#include <qsf/base/StringParser.h>
#include <qsf/localization/LocalizationSystem.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/plugin/QsfAssetTypes.h>
#include <qsf/renderer/resource/OgreResourceGroup.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreSkeleton.h>
#include <OGRE/OgreOldSkeletonManager.h>
#include <OGRE/OgreSkeletonSerializer.h>
#include <OGRE/OgreOldBone.h>
#include <OGRE/OgreKeyFrame.h>

#include <boost/filesystem.hpp>


//[-------------------------------------------------------]
//[ Locally defined helper functions                      ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{

		void convertSkeleton(Ogre::v1::Skeleton& ogreSkeleton, bool inverse)
		{
			const float sign = inverse ? -1.0f : 1.0f;
			const Ogre::Quaternion correction1(Ogre::Radian(Ogre::Degree(sign * 90.0f)), Ogre::Vector3(1.0f, 0.0f, 0.0f));
			const Ogre::Quaternion correction2(Ogre::Radian(Ogre::Degree(sign * -90.0f)), Ogre::Vector3(1.0f, 0.0f, 0.0f));

			// First convert the bones
			{
				static std::vector<Ogre::Vector3> derivedPositions;
				static std::vector<Ogre::Quaternion> derivedOrientations;
				derivedPositions.clear();
				derivedOrientations.clear();

				// Collect original derived transformations
				for (Ogre::v1::OldBone* bone : ogreSkeleton.getBoneIterator())
				{
					Ogre::Vector3 position = bone->getPosition();
					Ogre::Quaternion orientation = bone->getOrientation();

					Ogre::v1::OldNode* parent = bone->getParent();
					if (nullptr != parent)
					{
						position = parent->convertLocalToWorldPosition(position);
						orientation = parent->convertLocalToWorldOrientation(orientation);
					}

					derivedPositions.push_back(position);
					derivedOrientations.push_back(orientation);
				}

				// Rotate orientations by 90° around local x-axis
				//  -> We have to do this for the derived orientations so that their children don't get affected
				for (Ogre::Quaternion& orientation : derivedOrientations)
				{
					orientation = orientation * correction1;
				}

				// Set updated transformations
				size_t index = 0;
				for (Ogre::v1::OldBone* bone : ogreSkeleton.getBoneIterator())
				{
					Ogre::Vector3 position = derivedPositions[index];
					Ogre::Quaternion orientation = derivedOrientations[index];

					Ogre::v1::OldNode* parent = bone->getParent();
					if (nullptr != parent)
					{
						position = parent->convertWorldToLocalPosition(position);
						orientation = parent->convertWorldToLocalOrientation(orientation);
					}

					bone->setPosition(position);
					bone->setOrientation(orientation);

					++index;
				}
			}

			// Now for the animations
			for (unsigned short animationIndex = 0; animationIndex < ogreSkeleton.getNumAnimations(); ++animationIndex)
			{
				Ogre::v1::Animation* animation = ogreSkeleton.getAnimation(animationIndex);

				for (auto pair : animation->getOldNodeTrackIterator())
				{
					Ogre::v1::OldNodeAnimationTrack* ogreNodeAnimationTrack = pair.second;

					// Go through each frame
					const unsigned int numKeyFrames = ogreNodeAnimationTrack->getNumKeyFrames();
					for (unsigned int i = 0; i < numKeyFrames; ++i)
					{
						Ogre::v1::TransformKeyFrame* ogreTransformKeyFrame = ogreNodeAnimationTrack->getNodeKeyFrame(i);

						Ogre::Vector3 axis;
						Ogre::Degree angle;
						ogreTransformKeyFrame->getRotation().ToAngleAxis(angle, axis);

						// Rotation axis of bone animation must be rotated itself by 90° around local x-axis
						const Ogre::Quaternion newRotation(Ogre::Radian(angle), correction2 * axis);

						// TODO(fw): Most probably, offsets have to be translated as well

						ogreTransformKeyFrame->setRotation(newRotation);
					}
				}
			}
		}

	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		// Qt translation needs always an context/scope for the text which should be translatable
		// For the QT_TR_NOOP macro the lupdate tool checks if the class which contains the macro call has an Q_OBJECT definition (if so the class name is used as the scope)
		// With the QT_TRANSLATE_NOOP macro the needed scope is specified to the macro call -> the containing scope/class doesn't need specify/contains the Q_OBJECT macro
		const char BatchJobMigrateSkeletonCoordinateSystem::JOB_TEXT[] = QT_TRANSLATE_NOOP("em5::editor", "ID_BATCH_JOB_MIGRATE_SKELETON_COORDINATE_SYSTEM");

		const std::string BatchJobMigrateSkeletonCoordinateSystem::OLD_SKELETON_TAG = "old_skeleton";
		const std::string BatchJobMigrateSkeletonCoordinateSystem::NEW_SKELETON_TAG = "new_skeleton";


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		BatchJobMigrateSkeletonCoordinateSystem::BatchJobMigrateSkeletonCoordinateSystem(qsf::editor::BatchJobManager* manager) :
			BatchJob(manager),
			mCurrentIndex(0),
			mNumberOfEditedAssets(0),
			mAssetEditHelper(nullptr),
			mInverse(false)
		{
			// Nothing here
		}

		BatchJobMigrateSkeletonCoordinateSystem::~BatchJobMigrateSkeletonCoordinateSystem()
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		std::string BatchJobMigrateSkeletonCoordinateSystem::getText() const
		{
			return QSF_TRANSLATE_CONTEXTSTRING("em5::editor", JOB_TEXT);
		}

		void BatchJobMigrateSkeletonCoordinateSystem::getProgress(uint32& current, uint32& total) const
		{
			current = static_cast<uint32>(mCurrentIndex);
			total = static_cast<uint32>(mGlobalSkeletonAssetIds.size());
		}

		bool BatchJobMigrateSkeletonCoordinateSystem::configure()
		{
			bool result = true;

			// Make sure we have a clean state
			cleanup();

			std::vector<qsf::Asset*> skeletonAssets;
			QSF_ASSET.getAssetsOfType(qsf::QsfAssetTypes::SKELETON, skeletonAssets);
			for (qsf::Asset* asset : skeletonAssets)
			{
				QSF_ASSERT(nullptr != asset, "nullptr != asset", QSF_REACT_THROW);
				if (asset->hasTag(mInverse ? NEW_SKELETON_TAG : OLD_SKELETON_TAG))
				{
					mGlobalSkeletonAssetIds.push_back(asset->getGlobalAssetId());
				}
			}

			mAssetEditHelper.reset(new qsf::editor::AssetEditHelper());

			return result;
		}

		bool BatchJobMigrateSkeletonCoordinateSystem::work()
		{
			bool result = false;

			if (mCurrentIndex < mGlobalSkeletonAssetIds.size())
			{
				const qsf::GlobalAssetId globalAssetId = mGlobalSkeletonAssetIds.at(mCurrentIndex);

				// Don't touch assets with invalid cached assets
				qsf::CachedAsset* cachedAsset = QSF_ASSET.getCachedAssetByGlobalAssetId(globalAssetId);
				if (nullptr != cachedAsset && !cachedAsset->getCachedAssetDataFilename().empty() && QSF_FILE.exists(cachedAsset->getCachedAssetDataFilename()))
				{
					// Try to load the OGRE skeleton
					Ogre::v1::SkeletonPtr ogreSkeletonPtr = Ogre::v1::OldSkeletonManager::getSingleton().load(std::to_string(globalAssetId), qsf::OgreResourceGroup::OGRE_RESOURCE_GROUP).staticCast<Ogre::v1::Skeleton>();

					// Convert
					::detail::convertSkeleton(*ogreSkeletonPtr, mInverse);

					// OGRE skeleton was loaded and converted, so we need to update the skeleton asset
					qsf::Asset* editedSkeletonAsset = mAssetEditHelper->editAsset(globalAssetId);
					++mNumberOfEditedAssets;
					qsf::CachedAsset* cachedSkeletonAsset = mAssetEditHelper->getCachedAsset(globalAssetId);
					if (cachedSkeletonAsset != nullptr)
					{
						try
						{
							// Serialize the modified OGRE skeleton
							// -> Ensure the directory is there, then serialize, else serialize will fail if the directory is not there, yet
							const std::string absoluteDestinationFilename = QSF_FILE.getBaseDirectory() + '/' + mAssetEditHelper->getIntermediateAssetPackage()->getRelativeDirectory() + '/' + qsf::QsfAssetTypes::SKELETON.getName() +  '/' + editedSkeletonAsset->getCategory() + '/' + boost::filesystem::path(cachedSkeletonAsset->getCachedAssetDataFilename()).filename().generic_string();
							boost::filesystem::create_directories(boost::filesystem::path(absoluteDestinationFilename).parent_path());
							Ogre::v1::SkeletonSerializer().exportSkeleton(ogreSkeletonPtr.getPointer(), absoluteDestinationFilename);
							editedSkeletonAsset->setHash(qsf::AssetSystem::calculateHashFromFileByAbsoluteFilename(absoluteDestinationFilename));
							{ // Update tags
								std::string tags = editedSkeletonAsset->getTags();
								qsf::StringParser::removeValueFromCommaSeparatedValuesString(mInverse ? NEW_SKELETON_TAG : OLD_SKELETON_TAG, ';', tags);
								qsf::StringParser::addValueToCommaSeparatedValuesString(mInverse ? OLD_SKELETON_TAG : NEW_SKELETON_TAG, ';', tags);
								editedSkeletonAsset->setTags(tags);
							}
							mAssetEditHelper->setAssetUploadData(globalAssetId, true, true);
						}
						catch (const std::exception& e)
						{
							QSF_ERROR("Unable to load in OGRE skeleton " << std::to_string(globalAssetId) << ": " << e.what(), QSF_REACT_NONE);
						}
					}
				}

				++mCurrentIndex;

				// Important to not finish the loop if there are still assets being uploaded
				result = (mCurrentIndex < mGlobalSkeletonAssetIds.size() && !mAssetEditHelper->isUploading());
			}

			if (result)
			{
				// Time to flush?
				// -> It's no good idea to edit too many assets in a single burst, we might hit the "_setmaxstdio" ( https://msdn.microsoft.com/en-us/library/6e3b887c.aspx ) limit at one point inside the pipeline
				if (mNumberOfEditedAssets > 1)
				{
					// Submit all changes
					mAssetEditHelper->submit();

					// Reset asset edit helper
					mNumberOfEditedAssets = 0;
					mAssetEditHelper.reset(new qsf::editor::AssetEditHelper());
				}
			}
			else
			{
				// Submit all changes
				mAssetEditHelper->submit();
			}

			return result;
		}

		void BatchJobMigrateSkeletonCoordinateSystem::cleanup()
		{
			mGlobalSkeletonAssetIds.clear();
			mCurrentIndex = 0;
			mNumberOfEditedAssets = 0;

			mAssetEditHelper.reset();

			// TODO(co) Crashed for me with OGRE 2.1
			// Remove meshes we have been using alone
			// Ogre::v1::MeshManager::getSingleton().unloadUnreferencedResources();
		}



		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const char BatchJobMigrateSkeletonCoordinateSystemInverse::JOB_TEXT[] = QT_TRANSLATE_NOOP("em5::editor", "ID_BATCH_JOB_MIGRATE_SKELETON_COORDINATE_SYSTEM_INVERSE");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		BatchJobMigrateSkeletonCoordinateSystemInverse::BatchJobMigrateSkeletonCoordinateSystemInverse(qsf::editor::BatchJobManager* manager) :
			BatchJobMigrateSkeletonCoordinateSystem(manager)
		{
			setInverse(true);
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		std::string BatchJobMigrateSkeletonCoordinateSystemInverse::getText() const
		{
			return QSF_TRANSLATE_CONTEXTSTRING("em5::editor", JOB_TEXT);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
