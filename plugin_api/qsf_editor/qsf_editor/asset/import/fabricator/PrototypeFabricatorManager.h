// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class AssetPackage;
	class PrototypeManager;
	namespace editor
	{
		class AssetImportListener;
		class PrototypeFabricator;
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
		*    Prototype fabricator manager class
		*
		*  @remarks
		*    The prototype fabricator manager is automatically able to use all CAMP classes which are derived from "qsf::PrototypeFabricator".
		*/
		class QSF_EDITOR_API_EXPORT PrototypeFabricatorManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototypeManager
			*    Prototype manager instance to use
			*/
			inline explicit PrototypeFabricatorManager(PrototypeManager& prototypeManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PrototypeFabricatorManager();

			/**
			*  @brief
			*    Return the prototype manager to use
			*
			*  @return
			*    The prototype manager to use, do not destroy the instance
			*/
			inline PrototypeManager& getPrototypeManager() const;

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
			bool canFabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename) const;

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
			Prototype* fabricatePrototypeByAbsoluteFilename(const std::string& absoluteFilename, AssetPackage& assetPackage, AssetImportListener* assetImportListener) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PrototypeManager* mPrototypeManager;	///< Prototype manager to use, always valid, we're responsible for destroying the instance if we no longer need it


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
#include "qsf_editor/asset/import/fabricator/PrototypeFabricatorManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PrototypeFabricatorManager)
