// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/type/AssetTypes.h"
#include "qsf/base/NamedIdentifier.h"


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
	*    Static QSF plugin asset types collection class
	*/
	class QSF_API_EXPORT QsfAssetTypes : public AssetTypes
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class QsfPlugin;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Unknown
		static const NamedIdentifier UNKNOWN;					///< Asset type name "unknown"
		// Prefab
		static const NamedIdentifier PREFAB;					///< Asset type name "prefab"
		// Localization
		static const NamedIdentifier LOCALIZATION;				///< Asset type name "localization"
		// Map
		static const NamedIdentifier MAP;						///< Asset type name "map"
		// Terrain
		static const NamedIdentifier TERRAIN;					///< Asset type name "terrain"
		static const NamedIdentifier TERRAIN_DATA_MAP;			///< Asset type name "terrain_data_map"
		// GUI
		static const NamedIdentifier GUI;						///< Asset type name "gui"
		// Script
		static const NamedIdentifier SCRIPT;					///< Asset type name "script"
		static const NamedIdentifier SCRIPT_CATEGORY_ACTION;	///< Script asset category name "action"
		// Audio
		static const NamedIdentifier AUDIO;						///< Asset type name "audio"
		// Video
		static const NamedIdentifier VIDEO;						///< Asset type name "video"
		// Physics
		static const NamedIdentifier COLLISION_TYPE;			///< Asset type name "collision_type"
		// Renderer
			// Font
			static const NamedIdentifier FONT;					///< Asset type name "font"
			// Mesh
			static const NamedIdentifier MESH;					///< Asset type name "mesh"
			// Skeleton
			static const NamedIdentifier SKELETON;				///< Asset type name "skeleton"
			// Particles
			static const NamedIdentifier PARTICLES;				///< Asset type name "particles"
			// Shader
			static const NamedIdentifier SHADER;				///< Asset type name "shader"
			// Texture
			static const NamedIdentifier TEXTURE;				///< Asset type name "texture"
			static const NamedIdentifier TEXTURE_LUT;			///< Asset type name "texture_lut"
			static const NamedIdentifier TEXTURE_CUBE;			///< Asset type name "texture_cube"
			// Material
			static const NamedIdentifier MATERIAL;				///< Asset type name "material"
		// Renderer - OGRE
			// OGRE terrain
			static const NamedIdentifier OGRE_TERRAIN;			///< Asset type name "ogre_terrain"


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		QsfAssetTypes();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		~QsfAssetTypes();

		/**
		*  @brief
		*    Register the asset types
		*/
		static void registerAssetTypes();

		/**
		*  @brief
		*    Unregister the asset types
		*/
		static void unregisterAssetTypes();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
