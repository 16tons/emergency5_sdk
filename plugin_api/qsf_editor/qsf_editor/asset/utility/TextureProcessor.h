// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <string>


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
		*    Texture processor
		*/
		class QSF_EDITOR_API_EXPORT TextureProcessor : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Compress normal maps
			*
			*  @param[in] sourceDirectories
			*    List of absolute UTF-8 directories where to recursively look for the source assets we're going to compress (don't dare to touch already compressed dds files)
			*
			*  @todo
			*    TODO(co) This method exists for data migration, we might want to remove it later on
			*/
			static void compressNormalMaps(const std::vector<std::string>& sourceDirectories);

			/**
			*  @brief
			*    Recompile all compiled materials
			*
			*  @todo
			*    TODO(co) This method exists for data migration, we might want to remove it later on
			*/
			static void recompileCompiledMaterials();

			/**
			*  @brief
			*    Search for uncompressed texture maps
			*
			*  @todo
			*    TODO(co) This method exists for data migration, we might want to remove it later on
			*/
			static void findUncompressedTextures();

			/**
			*  @brief
			*    Fix datasource paths
			*
			*  @todo
			*    TODO(co) This method exists for data migration, we might want to remove it later on
			*/
			static void fixDatasourcePath();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
