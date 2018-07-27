// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/base/NamedIdentifier.h>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Texture semantic manager class
			*
			*  @remarks
			*    Overview of commonly used texture semantics:
			*    -------------------------------------------
			*    Semantic               Postfix    Data         sRGB
			*    ---------------------------------------------------
			*    Color map              _c         rgb          Yes
			*    Alpha map              _a         luminance    Yes
			*    Specular map           _s         luminance    Yes
			*    Normal map             _n         rgb          No
			*    Height map             _h         luminance    Yes
			*    Gloss map              _g         luminance    Yes
			*    Emissive map           _e         rgb          Yes
			*    Tint map               _t         luminance    Yes
			*    Ambient occlusion map  _ao        luminance    Yes
			*    Smut overlay map       _o         luminance    Yes
			*    Reflection 2D map      _r2d       rgb          Yes
			*    Reflection cube map    _rcube     rgb          Yes
			*    Palette                _p         rgba         Yes
			*/
			class QSF_EDITOR_BASE_API_EXPORT TextureSemanticManager : public Manager
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const NamedIdentifier COLOR_MAP_ID;				///< Color map ASCII name ("ColorMap")
				static const NamedIdentifier ALPHA_MAP_ID;				///< Alpha map ASCII name ("AlphaMap")
				static const NamedIdentifier SPECULAR_MAP_ID;			///< Specular map ASCII name ("SpecularMap")
				static const NamedIdentifier NORMAL_MAP_ID;				///< Normal map ASCII name ("NormalMap")
				static const NamedIdentifier HEIGHT_MAP_ID;				///< Height map ASCII name ("HeightMap")
				static const NamedIdentifier GLOSS_MAP_ID;				///< Gloss map ASCII name ("GlossMap")
				static const NamedIdentifier EMISSIVE_MAP_ID;			///< Emissive map ASCII name ("EmissiveMap")
				static const NamedIdentifier TINT_MAP_ID;				///< Tint map ASCII name ("TintMap")
				static const NamedIdentifier AMBIENT_OCCLUSION_MAP_ID;	///< Ambient occlusion map ASCII name ("AmbientOcclusionMap")
				static const NamedIdentifier SMUT_OVERLAY_MAP_ID;		///< Smut overlay map ASCII name ("SmutOverlayMap")
				static const NamedIdentifier REFLECTION_2D_MAP_ID;		///< Reflection 2D map ASCII name ("Reflection2DMap")
				static const NamedIdentifier REFLECTION_CUBE_MAP_ID;	///< Reflection cube map ASCII name ("ReflectionCubeMap")
				static const NamedIdentifier PALETTE_MAP_ID;			///< Palette ASCII name ("Palette")

				/**
				*  @brief
				*    Texture data layout
				*/
				enum Layout
				{
					LUMINANCE_LAYOUT,	///< 1 channel, luminance
					RGB_LAYOUT,			///< 3 channels, red & green & blue
					RGBA_LAYOUT			///< 4 channels, red & green & blue & alpha
				};

				/**
				*  @brief
				*    Texture semantic
				*/
				struct Semantic
				{
					std::string  name;		///< Texture semantic ASCII name
					uint32		 id;		///< qsf::StringHash("<name>")
					std::string  postfix;	///< ASCII postfix of the semantic, e.g. "_c"
					Layout		 layout;	///< Texture data layout
					bool		 sRgb;		///< sRGB texture format? If "true", gamma correction ("pow(fetchedTexel, 2.2)") has to be applied to the fetched texels before the data can be processed correctly in a linear rendering pipeline.
				};


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				TextureSemanticManager();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~TextureSemanticManager();

				/**
				*  @brief
				*    Add texture semantic
				*
				*  @param[in] name
				*    Texture semantic ASCII name
				*  @param[in] postfix
				*    ASCII postfix of the semantic, e.g. "_c"
				*  @param[in] layout
				*    Texture data layout
				*  @param[in] sRgb
				*    sRGB texture format? (see "qsf::TextureSemanticManager::sRgb"-documentation)
				*
				*  @return
				*    "true" if all went fine, else "false"
				*/
				bool addSemantic(const std::string& name, const std::string& postfix, Layout layout, bool sRgb);

				/**
				*  @brief
				*    Return the texture stem of a given filename
				*
				*  @param[in] filename
				*    UTF-8 filename to return the texture stem for
				*
				*  @return
				*    The texture stem for the given filename, empty string on error (maybe there's no postfix?)
				*
				*  @note
				*    - Example: "wind_engine_c.tif" will result in texture stem "wind_engine"
				*/
				std::string getStemByFilename(const std::string& filename) const;

				/**
				*  @brief
				*    Return the texture semantic postfix of a given filename
				*
				*  @param[in] filename
				*    UTF-8 filename to return the texture semantic postfix for
				*
				*  @return
				*    The texture semantic postfix for the given filename, empty string on error
				*/
				std::string getPostfixByFilename(const std::string& filename) const;

				/**
				*  @brief
				*    Return the texture semantic of a given postfix
				*
				*  @param[in] postfix
				*    ASCII postfix to return the texture semantic for
				*
				*  @return
				*    The texture semantic for the given postfix, null pointer on error, do not destroy the instance
				*/
				const Semantic* getSemanticByPostfix(const std::string& postfix) const;

				/**
				*  @brief
				*    Return the texture semantic by using its name
				*
				*  @param[in] name
				*    ASCII name of the texture semantic
				*
				*  @return
				*    The texture semantic for the given name, null pointer on error, do not destroy the instance
				*/
				const Semantic* getSemanticByName(const std::string& name) const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::vector<Semantic> mSemantics;	///< List of known texture semantics


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/asset/TextureSemanticManager-inl.h"
