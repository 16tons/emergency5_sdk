// Copyright (C) 2012-2017 Promotion Software GmbH


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
		*    Particles prototype fabricator
		*
		*  @todo
		*    - TODO(co) "qsf::editor::MeshPrototypeFabricator::fabricatePrototypeByFilename()" and "qsf::editor::ParticlesPrototypeFabricator::fabricatePrototypeByFilename()" are quite similar, can we unify something wihout extreme overengineering?
		*/
		class ParticlesPrototypeFabricator : public OgrePrototypeFabricator
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ParticlesPrototypeFabricator" unique pluginable prototype fabricator ID


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
			explicit ParticlesPrototypeFabricator(PrototypeFabricatorManager* prototypeFabricatorManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ParticlesPrototypeFabricator();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PrototypeFabricator methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual bool canFabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename) const override;
			virtual Prototype* fabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename, AssetPackage& assetPackage, AssetImportListener* assetImportListener) const override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ParticlesPrototypeFabricator)
