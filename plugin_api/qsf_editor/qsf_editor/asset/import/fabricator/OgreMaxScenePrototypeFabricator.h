// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/fabricator/OgrePrototypeFabricator.h"


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
		*    OgreMax scene prototype fabricator
		*
		*  @note
		*    - The result of the OgreMax scene import is a scene prefab asset with the same name as the OgreMax scene file imported
		*/
		class OgreMaxScenePrototypeFabricator : public OgrePrototypeFabricator
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32		 PLUGINABLE_ID;			///< "qsf::editor::OgreMaxScenePrototypeFabricator" unique pluginable prototype fabricator ID
			static const std::string ASSET_CATEGORY_NAME;	///< Asset category newly imported assets will be in


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototypeFabricatorManager
			*    The prototype fabricator manager this prototype fabricator is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit OgreMaxScenePrototypeFabricator(PrototypeFabricatorManager* prototypeFabricatorManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OgreMaxScenePrototypeFabricator();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PrototypeFabricator methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual bool canFabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename) const override;
			virtual Prototype* fabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename, AssetPackage& assetPackag, AssetImportListener* assetImportListenere) const override;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::OgreMaxScenePrototypeFabricator)
