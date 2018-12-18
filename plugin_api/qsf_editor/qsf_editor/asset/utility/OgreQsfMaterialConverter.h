// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <OGRE/OgreBlendMode.h>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Material;
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
		*    OGRE QSF material converter
		*/
		class QSF_EDITOR_API_EXPORT OgreQsfMaterialConverter : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Serialize a given OGRE material as QSF material
			*
			*  @param[in] ogreMaterial
			*    OGRE material to serialize as QSF material
			*  @param[in] qsfBaseMaterial
			*    Which is the QSF base material?
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename of the QSF material
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			static bool serializeOgreMaterialAsQsfMaterial(Ogre::Material& ogreMaterial, const std::string& qsfBaseMaterial, const std::string& absoluteFilename);


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			static std::string getSceneBlendFactorAsString(Ogre::SceneBlendFactor ogreSceneBlendFactorSource, Ogre::SceneBlendFactor ogreSceneBlendFactorDestination);
			static std::string getSceneBlendFactorAsString(Ogre::SceneBlendFactor ogreSceneBlendFactor);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
