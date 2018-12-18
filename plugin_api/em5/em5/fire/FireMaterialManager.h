// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/fire/FireMaterial.h"

#include <qsf/base/manager/ElementManager.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetSystem;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Manager contains all fire materials
	*
	*  @note
	*    - Use "EM5_FIRE.getFireMaterialByName("FireMaterialName")" to get the specific material
	*/
	class EM5_API_EXPORT FireMaterialManager : public qsf::ElementManager<uint32, FireMaterial>
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void ()> FireMaterialsChanged;		///< This Boost signal is emitted after the asset of the fire materials got changed (updated/deleted)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FireMaterialManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireMaterialManager();

		/**
		*  @brief
		*    Load the assets
		*/
		void loadAssets();

		/**
		*  @brief
		*    Save the assets
		*/
		void saveAssets();

		/**
		*  @brief
		*    Load fire material JSON file
		*
		*  @param[in] filename
		*    Path of the file to load
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save fire material JSON file
		*
		*  @param[in] filename
		*    Path of the file to save
		*/
		bool saveByFilename(const std::string& filename);

		/**
		*  @brief
		*    Find fire material by name
		*
		*  @param[in] name
		*    Name where searching for
		*
		*  @return
		*    The fire material found, can be a null pointer
		*/
		const FireMaterial* getFireMaterialByName(const std::string& name);

		/**
		*  @brief
		*   Find a unique name for the fire material; used if the user entered a name that is already taken
		*
		*  @param[in] name
		*   The name the user entered
		*
		*  @return
		*    The unique generated name
		*
		*  @note
		*    - See qsf::MapHelper::findUniqueNameforEntity for reference
		*/
		std::string findUniqueNameForMaterial(const std::string& name);

		/** Return true if the content is not matching the saved */
		inline bool getContentHasChanged() const;

		/**
		*  @brief
		*    Flag content of fire material manager as dirty
		*
		*  @note
		*    - Automatically reseted after saving the assets
		*/
		inline void setContentHasChanged(bool contentHasChanged);

		/**
		*  @brief
		*    Return the default fire material, in case there is non, create one.
		*
		*  @note
		*    - Automatically called in after loading, default material must always be valid.
		*/
		const FireMaterial* getOrCreateDefaultMaterial();

		/**
		*  @brief
		*    Let the manager know if the fire materials assets are currently be edited
		*
		*  @param[in] isEditing
		*    Indicates if editing is enabled or not
		*  @param[in] assetRevision
		*    The revision of the asset which the asset will have after editing -> Is used to ignore asset mount/umount events for the localy changed asset of fire materials
		*
		*  @todo
		*    - TODO(sw) remove the qsf::AssetRevision parameter as soon as this information can be gathered inside the manager
		*/
		inline void setIsEditing(bool isEditing, qsf::AssetRevision assetRevision = -1);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::ElementManager methods         ]
	//[-------------------------------------------------------]
	protected:
		virtual void onElementInsertion(const uint32& key, FireMaterial& element) override;
		virtual void onElementRemoval(const uint32& key, FireMaterial& element) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::string createUniqueNameForMaterial(const std::string& name) const;

		void onAssetsMounted(const qsf::Assets& assets);
		void onAssetsUnmounted(const qsf::Assets& assets);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mContentHasChanged;
		bool				mIsEditing;
		qsf::AssetRevision	mAssetRevision;
		qsf::AssetSystem&	mAssetSystem;		///< Asset system instance, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/FireMaterialManager-inl.h"
