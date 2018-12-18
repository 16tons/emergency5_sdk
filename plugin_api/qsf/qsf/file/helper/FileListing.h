// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetProxy;
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
	*    File listing class
	*/
	class QSF_API_EXPORT FileListing
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FileListing();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileListing();

		const std::vector<std::string>& getFilenames() const;

		void clear();

		void addFile(const std::string& filename);

		bool addAsset(const qsf::AssetProxy& assetProxy);

		uint32 addFilesWithExtensionFromDirectory(const std::string& extension, const std::string& directory, bool recursive = false);

		uint32 addAssetFilesOfType(const std::string& assetType);

		uint32 addAssetFilesFromPackage(const std::string& projectName, const std::string& assetPackageName, const std::string& assetType = "", const std::string& assetCategory = "");


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<std::string> mFilenames;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
