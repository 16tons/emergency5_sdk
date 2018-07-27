// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/NodesComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


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
	*    Decal component base class
	*/
	class QSF_API_EXPORT DecalComponent : public NodesComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::DecalComponent" unique component ID
		static const uint32 TEXTURE_ATLAS;				///< "TextureAtlas" unique class property ID inside the class
		static const uint32 TEXTURE_ATLAS_ENTRIES_U;	///< "TextureAtlasEntriesU" unique class property ID inside the class
		static const uint32 TEXTURE_ATLAS_ENTRIES_V;	///< "TextureAtlasEntriesV" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit DecalComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DecalComponent();

		/**
		*  @brief
		*    Return the texture atlas asset to use
		*
		*  @return
		*    The texture atlas asset to use (e.g. "sample/ogre_material/sample/decals"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getTextureAtlas() const;

		/**
		*  @brief
		*    Set the texture atlas asset to use
		*
		*  @param[in] textureAtlas
		*    The texture atlas asset to use (e.g. "sample/ogre_material/sample/decals")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTextureAtlas(const AssetProxy& textureAtlas);

		/**
		*  @brief
		*    Return the texture atlas entries in a row
		*
		*  @return
		*    The texture atlas entries
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const size_t getTextureAtlasEntriesU() const;

		/**
		*  @brief
		*    Set the texture atlas entries in a row
		*
		*  @param[in] textureAtlasEntriesU
		*    The texture atlas entries
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTextureAtlasEntriesU(size_t textureAtlasEntriesU);

		/**
		*  @brief
		*    Return the texture atlas entries in a column
		*
		*  @return
		*    The texture atlas entries
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const size_t getTextureAtlasEntriesV() const;

		/**
		*  @brief
		*    Set the texture atlas entries in a column
		*
		*  @param[in] textureAtlasEntriesV
		*    The texture atlas entries
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setTextureAtlasEntriesV(size_t textureAtlasEntriesV);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::NodesComponent methods         ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool closedContinuousLines() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AssetProxy	mTextureAtlas;			///< The texture asset to use (e.g. "sample/ogre_material/sample/decals")
		size_t		mTextureAtlasEntriesU;	///< The texture atlas entries in a row
		size_t		mTextureAtlasEntriesV;	///< The texture atlas entries in a column


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/decal/DecalComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DecalComponent)
