// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class AxisAlignedBox;
	class Texture;
	template<typename T> class SharedPtr;
	typedef SharedPtr<Texture> TexturePtr;
}
namespace qsf
{
	class Map;
	class Entity;
	class ParameterGroup;
	class CameraComponent;
	namespace editor
	{
		namespace detail
		{
			class FastLookupLRUCache;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF editor asset preview cache
		*/
		class AssetPreviewCache : public QObject
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			//[-------------------------------------------------------]
			//[ Asset previews                                        ]
			//[-------------------------------------------------------]
			void assetPreviewChanged(GlobalAssetId asset);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetPreviewCache();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetPreviewCache();

			//[-------------------------------------------------------]
			//[ Asset preview                                         ]
			//[-------------------------------------------------------]
			QImage getAssetPreviewQImage(GlobalAssetId forAsset);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onAssetUpdated(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateAssetPreview(GlobalAssetId forAsset);
			std::string getAssetPreviewFileName(GlobalAssetId forAsset);
			std::string computeHashForAssetState(GlobalAssetId forAsset);
			void computeAABBOfObject(Entity* prefabClone, Ogre::AxisAlignedBox& outAABB);
			void previewGenerationJob(const JobArguments& jobArguments);
			void killZombies();

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			void onStartup();
			void onShutdown();


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const uint32 PREVIEW_CACHE_VERSION;	///< Version number to be able to invalidate the whole preview cache at once when there are important changes to how previews are generated
			static const float CAMERA_HORIZONTAL_FOV;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map*						mMap;					///< Always valid when asset preview is enabled
			CameraComponent*			mCameraComponent;		///< Always valid when asset preview is enabled
			bool						mAssetPreview;
			detail::FastLookupLRUCache* mImageCache;
			int							mRenderTargetWidth;
			int							mRenderTargetHeight;
			int							mThumbnailWidth;
			int							mThumbnailHeight;
			Ogre::TexturePtr*			mRenderTargetTexture;	///< This is a pointer to avoid inclusion of OGRE headers
			JobProxy					mOfflinePreviewGenerationJob;
			std::list<GlobalAssetId>	mPreviewGenerationQueue;
			float						mNextPreviewCountdown;
			QImage						mNoPreviewImage;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
