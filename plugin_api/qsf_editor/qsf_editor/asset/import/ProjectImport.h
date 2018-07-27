// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qobject.h>

#include <boost/noncopyable.hpp>

#include <set>
#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Project;
	class AssetSystem;
	class AssetPackage;
	namespace editor
	{
		class AssetEditHelper;
		class AssetStorageManager;
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
		*    Project import class
		*
		*  @note
		*    - It's highly recommended to only create temporary instances of this class
		*    - For runtime project import, for initial server filling use "qsf::editor::ImportProjectDialog" instead
		*/
		class ProjectImport : public QObject, public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::vector<Project*> ProjectVector;


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void projectImportFinished(ProjectImport& projectImport);	///< Emitted when the project import has been finished
			void projectImportFailed(ProjectImport& projectImport);		///< Emitted when the project import has failed


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			ProjectImport();

			/**
			*  @brief
			*    Destructor
			*/
			~ProjectImport();

			/**
			*  @brief
			*    Non blocking import project from zip file
			*
			*  @note
			*    - Listen to the "qsf::editor::ProjectImport::projectImportFinished" signal to get informed when the project import has been finished (online editor server transfers etc. are also done by then)
			*    - Listen to the "qsf::editor::ProjectImport::projectImportFailed" signal to get informed when the project import has failed
			*/
			void importProjectFromZipFile(const std::string& filename, const std::string& targetBaseDirectory, const std::string& targetLocalPath);

			const ProjectVector& getImportedProjects() const;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onProjectCreated(const Project& project);
			void onAssetPackageCreated(const AssetPackage& assetPackage);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			bool checkForProjectImportCompletion();
			void destroyTemporaryProjects();
			void pushAssetUpload(const Asset& asset, std::shared_ptr<AssetEditHelper> newAssetEditHelper);

			/**
			*  @brief
			*    Called as soon as submitting the new asset is done
			*/
			void onNewAssetSubmitDone(std::shared_ptr<AssetEditHelper> newAssetEditHelper, GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::unordered_map<GlobalAssetId, std::vector<std::pair<const Asset*, std::shared_ptr<AssetEditHelper>>>> PendingUploads;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetSystem&			mAssetSystem;
			AssetStorageManager&	mAssetStorageManager;
			bool					mImportInProgress;
			ProjectVector			mTemporaryProjects;	///< We're responsible for destroying the instances
			ProjectVector			mProjectsAdded;
			std::set<GlobalAssetId>	mImportedGlobalAssetIds;
			uint32					mTotalNumberOfAssetPackages;
			uint32					mTotalNumberOfAssets;
			uint32					mCurrentNumberOfAssetPackages;
			uint32					mCurrentNumberOfAssets;
			PendingUploads			mPendingUploads;	///< Key = global asset ID of the asset other assets depend on, value = list of currently blocked asset uploads due to missing resolved dependency


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
