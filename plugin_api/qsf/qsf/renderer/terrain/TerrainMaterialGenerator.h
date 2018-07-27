// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"

#include <OGRE/OgreGpuProgramParams.h>
#include <OGRE/Terrain/OgreTerrainPrerequisites.h>
#include <OGRE/Terrain/OgreTerrainMaterialGenerator.h>
#undef TRANSPARENT	// Header hell: The OGRE headers include some nasty macros


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OGRE terrain material generator
	*
	*  @remarks
	*    Some notes about certain OGRE terrain features which we don't use
	*    - Light map - which we don't use because those are far to expensive to calculate during runtime (we had quite some issues)
	*    - Composite map - which we don't use because those are far to expensive to calculate during runtime (we had quite some issues)
	*/
	class TerrainMaterialGenerator : public Ogre::TerrainMaterialGenerator
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		class Profile : public Ogre::TerrainMaterialGenerator::Profile
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			Profile(Ogre::TerrainMaterialGenerator* parent, const Ogre::String& name, const Ogre::String& desc);
			virtual ~Profile();


		//[-------------------------------------------------------]
		//[ Public virtual Ogre::TerrainMaterialGenerator::Profile methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool isVertexCompressionSupported() const override;
			virtual Ogre::MaterialPtr generate(const Ogre::Terrain* ogreTerrain) override;
			virtual Ogre::MaterialPtr generateForCompositeMap(const Ogre::Terrain* ogreTerrain) override;
			virtual void setLightmapEnabled(bool enabled) override;
			virtual Ogre::uint8 getMaxLayers(const Ogre::Terrain* ogreTerrain) const override;
			virtual void updateParams(const Ogre::MaterialPtr& ogreMaterial, const Ogre::Terrain* ogreTerrain) override;
			virtual void updateParamsForCompositeMap(const Ogre::MaterialPtr& ogreMaterial, const Ogre::Terrain* ogreTerrain) override;
			virtual void requestOptions(Ogre::Terrain* ogreTerrain) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void createMaterial(const Ogre::String& matName, const Ogre::Terrain* ogreTerrain);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ogre::String mMatName;
			Ogre::String mMatNameComp;


		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TerrainMaterialGenerator();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TerrainMaterialGenerator();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onAssetsMounted(const Assets& assets);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
