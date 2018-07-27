// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/ShaderPieces.h"

#include <set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Archive;
}


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
	*    Manager for static shader pieces
	*
	*  @note
	*    - One shader pieces instance may contain 0-n shader pieces, hence plural in the class name
	*    - Similar to the shader source code blueprint class, but other context
	*    - Unlike dynamic C++ generated shader pieces, the static shader pieces are usually defined in text files
	*/
	class ShaderPiecesManager : public ElementManager<ShaderPiecesId, ShaderPieces, detail::DirectAllocator<ShaderPieces>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class HlmsBlueprint;				///< The blueprint HLMS fills this class
		friend class ShaderBlueprintManager;	///< The shader blueprint manager is the master of this class


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct OgreShaderPiece
		{
			Ogre::Archive* ogreArchive;
			std::string    pieceFile;

			OgreShaderPiece(Ogre::Archive* _ogreArchive, const std::string& _pieceFile) :
				ogreArchive(_ogreArchive),
				pieceFile(_pieceFile)
			{
				// Nothing here
			}

			bool operator<(const OgreShaderPiece& other) const
			{
				return (pieceFile < other.pieceFile);
			}
			bool operator==(const OgreShaderPiece& other) const
			{
				return (ogreArchive == other.ogreArchive && pieceFile == other.pieceFile);
			}
		};
		typedef std::set<OgreShaderPiece> OgreShaderPieces;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ShaderPiecesManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ShaderPiecesManager();

		/**
		*  @brief
		*    Fill the shader pieces manager by using OGRE shader pieces
		*
		*  @param[in] ogreShaderPieces
		*    OGRE shader pieces to fill this shader pieces manager with
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		void fillWithOgreShaderPieces(const OgreShaderPieces& ogreShaderPieces);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderPiecesManager-inl.h"
