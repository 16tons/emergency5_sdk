// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/fabricator/PrototypeFabricatorManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class AssetPackage;
	namespace editor
	{
		class AssetImportListener;
		class PrototypeFabricatorManager;
	}
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
		*    Abstract prototype fabricator class
		*/
		class PrototypeFabricator : public Pluginable<PrototypeFabricatorManager>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class PrototypeFabricatorManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PrototypeFabricator();

			/**
			*  @brief
			*    Return the prototype fabricator manager
			*
			*  @return
			*    The prototype fabricator manager, do not destroy the instance
			*/
			inline PrototypeFabricatorManager& getPrototypeFabricatorManager() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PrototypeFabricator methods    ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Check whether or not it's possible to fabricate prototype by using a given filename
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation of the of the file to use as raw material for the fabrication of a prototype
			*
			*  @return
			*    "true" if it's possible to fabricate prototype by using a given filename, else "false"
			*/
			virtual bool canFabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename) const = 0;

			/**
			*  @brief
			*    Fabricate prototype by using a given filename
			*
			*  @param[in] absoluteFilename
			*    Absolute UTF-8 filename in platform-independent notation of the of the file to use as raw material for the fabrication of a prototype
			*  @param[in] assetPackage
			*    Asset package instance new imported assets should be registered in, instance must stay valid as long as this asset import instance exists
			*  @param[in] assetImportListener
			*    Optional asset import listener, can be a null pointer
			*
			*  @return
			*    The fabricated prototype, null pointer on error, do not destroy the instance because it's managed by the prototype system
			*/
			virtual Prototype* fabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename, AssetPackage& assetPackage, AssetImportListener* assetImportListener) const = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototypeFabricatorManager
			*    The prototype fabricator manager this prototype fabricator is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			inline explicit PrototypeFabricator(PrototypeFabricatorManager* prototypeFabricatorManager);


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/fabricator/PrototypeFabricator-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PrototypeFabricator)
