// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/terrain/TerrainDataMap.h"
#include "qsf_editor/operation/terrain/TerrainData.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qobject.h>

#include <memory>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TerrainComponent;
	class TerrainDefinition;
	namespace editor
	{
		class AssetEditHelper;
		class TerrainEditManager;
		namespace base
		{
			class Operation;
		}
	}
}
namespace Ogre
{
	class TerrainGroup;
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
		*    Terrain tool base class
		*/
		class TerrainEditHelper : public QObject, public boost::noncopyable, public std::enable_shared_from_this<TerrainEditHelper>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class TerrainEditManager;


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static GlobalAssetId cloneTerrainAssets(const TerrainDefinition& sourceTerrainDefinition, AssetEditHelper& assetEditHelper, TerrainDefinition& targetTerrainDefinition, const std::string& targetAssetPackageName);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainEditHelper();

			bool isGood() const;

			/**
			*  @brief
			*    Returns whether the terrain edit helper is ready (= has no more background tasks running, no locks pending)
			*
			*  @return
			*    "true" if the terrain edit helper is ready, "false" otherwise
			*/
			bool isReady() const;

			uint64 getEntityId() const;
			void callWhenReady(boost::function<void()> readyCallback);
			void save(boost::function<void()> readyCallback = 0);
			TerrainDataMap* getTerrainDataMap(TerrainDataType dataType, bool createIfNotExist = true);
			void updateOgreMap(TerrainDataType dataType);
			void addToModifiedOgreTerrains(const std::unordered_set<Ogre::Terrain*>& modifiedOgreTerrains);
			void setOgreTerrainHeight(uint16 x, uint16 y, float value, uint16 segments, Ogre::TerrainGroup& ogreTerrainGroup, std::unordered_set<Ogre::Terrain*>& modifiedOgreTerrains);
			void importMap(TerrainDataType dataType, const std::string& absoluteFilename, bool scaleAndBias, float minimumValue, float maximumValue);
			void exportMap(TerrainDataType dataType, const std::string& absoluteFilename, bool normalize);
			void importChannel(TerrainDataType dataType, int channelIndex, const std::string& absoluteFilename, float heightMapScale);
			void exportChannel(TerrainDataType dataType, int channelIndex, const std::string& absoluteFilename);
			void markAllOgreTerrainsAsModified();
			void clearWorkspace();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*/
			TerrainEditHelper(GlobalAssetId globalMapAssetId, uint64 entityId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			TerrainComponent* getTerrainComponent() const;

			void startup();
			void shutdown();
			void onTerrainBusy();
			void onTerrainReady(bool isGood);
			void onSaveTerrainDoneAfterCreation(GlobalAssetId globalTerrainAssetId, bool isGood);

			/**
			*  @brief
			*    This method is called after the map has been saved
			*/
			void onPostMapSave();

			void saveTerrain(boost::function<void(bool)> resultCallback);
			void onSaveTerrainDone(bool isGood);
			GlobalAssetId getSourceDataGlobalAssetId(TerrainDataType dataType) const;
			void saveSourceData(TerrainDataType dataType);
			void saveColorMap(const Ogre::TerrainGroup& ogreTerrainGroup, const TerrainComponent& terrainComponent);
			void saveNormalMap(const Ogre::TerrainGroup& ogreTerrainGroup, const TerrainComponent& terrainComponent);
			void saveTerrainChunks(const Ogre::TerrainGroup& ogreTerrainGroup, const TerrainComponent& terrainComponent);


		//[-------------------------------------------------------]
		//[ Private Qt Slots                                      ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			GlobalAssetId						 mGlobalMapAssetId;
			uint64								 mEntityId;
			bool								 mIsGood;
			bool								 mIsReady;
			std::vector<boost::function<void()>> mReadyCallbacks;
			TerrainData							 mTerrainData;
			std::shared_ptr<AssetEditHelper>	 mAssetEditHelper;
			std::string							 mTargetAssetPackageName;
			std::unordered_set<TerrainDataType>	 mModifiedTerrainDataTypes;
			std::unordered_set<Ogre::Terrain*>	 mModifiedOgreTerrains;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
