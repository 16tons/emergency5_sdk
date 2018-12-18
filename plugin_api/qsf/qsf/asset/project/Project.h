// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/project/BaseProject.h"
#include "qsf/base/NamedIdentifier.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
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
	*    Project class
	*/
	class QSF_API_EXPORT Project : public BaseProject
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ProjectManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<std::string> ProjectNameList;
		typedef std::vector<AssetPackage*> AssetPackageList;

		struct PluginData
		{
			NamedIdentifier				 mName;
			std::vector<NamedIdentifier> mApplicationTags;
		};
		typedef std::map<uint32, PluginData> PluginMap;	// Using an std::map instead of Boost flat_map by intent


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Project();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Project();

		/**
		*  @brief
		*    Clear the project
		*/
		void clear();

		/**
		*  @brief
		*    Load a project by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the project to load
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*
		*  @note
		*    - The current project will get lost, so, you might want to save a manipulated project before loading a new one
		*    - The local UTF-8 filename in platform-independent notation of the project is set automatically
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save the project by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the project to save
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - The local UTF-8 filename in platform-independent notation of the project is not changed
		*/
		bool saveByFilename(const std::string& filename) const;

		//[-------------------------------------------------------]
		//[ Content                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local filename of the project, relative to the base directory
		*
		*  @return
		*    The local UTF-8 filename of the project in platform-independent notation (e.g. on Windows: "data/my_project/project.json")
		*/
		inline const std::string& getFilename() const;

		/**
		*  @brief
		*    Return whether or not the physical base directory of the project is inside the base directory of the file system
		*
		*  @return
		*    "true" if the physical base directory of the project is inside the base directory of the file system, else "false" (it's probably inside a user directory)
		*/
		bool isInsideFilesystemBaseDirectory() const;

		/**
		*  @brief
		*    Return the physical base directory of the project
		*
		*  @return
		*    The UTF-8 base directory name in platform-independent notation (e.g. on Windows: "C:/MyApplication")
		*/
		inline const std::string& getBaseDirectory() const;

		/**
		*  @brief
		*    Set the physical base directory of the project
		*
		*  @param[in] baseDirectory
		*    The UTF-8 base directory name in platform-independent notation
		*/
		inline void setBaseDirectory(const std::string& baseDirectory);

		/**
		*  @brief
		*    Return the names of the projects this project depends on
		*
		*  @return
		*    The names of the projects this project depends on
		*/
		inline const ProjectNameList& getProjectDependencies() const;

		/**
		*  @brief
		*    Set the names of the projects this project depends on
		*
		*  @param[in] projectDependencies
		*    The names of the projects this project depends on
		*/
		inline void setProjectDependencies(const ProjectNameList& projectDependencies);

		/**
		*  @brief
		*    Return the physical absolute directory of the project
		*
		*  @return
		*    The UTF-8 absolute directory name in platform-independent notation (e.g. on Windows: "C:/MyApplication/my_project")
		*/
		std::string getAbsoluteDirectory() const;

		/**
		*  @brief
		*    Return the asset packages of this project
		*
		*  @return
		*    The internal list of asset packages of this project, do not destroy the instance
		*/
		inline const AssetPackageList& getAssetPackageList() const;

		/**
		*  @brief
		*    Create and add a new empty asset package to the project
		*
		*  @param[in] name
		*    UTF-8 name of the asset package to add to the project
		*
		*  @return
		*    Pointer on the added asset package on success, else null pointer, do not destroy the returned instance
		*/
		AssetPackage* createAssetPackageByName(const std::string& name);

		/**
		*  @brief
		*    Create and add a new empty intermediate asset package to the project
		*
		*  @param[in] name
		*    UTF-8 name of the asset package to add to the project
		*
		*  @return
		*    Pointer on the added asset package on success, else null pointer, do not destroy the returned instance
		*
		*  @note
		*    - An intermediate asset package is technically just like an ordinary asset package, but with the use case of just being used for processing (e.g. asset import inside the editor)
		*/
		AssetPackage* createIntermediateAssetPackageByName(const std::string& name);

		/**
		*  @brief
		*    Add an existing asset package to the project
		*
		*  @param[in] name
		*    UTF-8 name of the asset package to add to the project
		*
		*  @return
		*    Pointer on the added asset package on success, else null pointer, do not destroy the returned instance
		*
		*  @note
		*     - There must be an existing "asset_package.json"-file
		*/
		AssetPackage* addAssetPackageByName(const std::string& name);

		/**
		*  @brief
		*    Return an asset package by using its name
		*
		*  @param[in] name
		*    UTF-8 name of the asset package to return
		*
		*  @return
		*    The asset package, null pointer on error, do not destroy the instance
		*/
		AssetPackage* getAssetPackageByName(const std::string& name) const;

		/**
		*  @brief
		*    Destroy an asset package including its files by using its name
		*
		*  @param[in] name
		*    UTF-8 name of the asset package to destroy
		*  @param[in] destroyContent
		*    Delete the directory the asset package is in?
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Warning: This method could cause harm (although harm is restricted to files inside mounted directories) because it will also delete the files inside the asset packet
		*      (no issue when the assets are managed by the QSF editor server)
		*/
		bool destroyAssetPackageByName(const std::string& name, bool destroyContent);

		//[-------------------------------------------------------]
		//[ Plugins                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the map of information about plugins
		*
		*  @return
		*    The internal plugin data map, do not destroy the instance
		*/
		inline const PluginMap& getPlugins() const;

		/**
		*  @brief
		*    Add information about a plugin to the project
		*
		*  @param[in] name
		*    UTF-8 file name of the plugin to add to the project, without file ending (e.g. "my_game_plugin")
		*  @param[in] applicationList
		*    Space-separated list of application short identifiers; the plugin will be loaded only for these applications (e.g. "sample_game sample_editor")
		*/
		void addPluginByName(const std::string& name, const std::string& applicationList);

		/**
		*  @brief
		*    Retrieve a listing of all plugins that should be loaded for a specific application
		*
		*  @param[in] applicationTags
		*    List of tags associated with the application
		*  @param[out] outPluginNames
		*    List of UTF-8 file name of the plugins
		*
		*  @note
		*    - The plugins are located inside an architecture-dependent sub-directory of the project, e.g. "x64"
		*/
		void getPluginsByApplicationTags(const std::vector<NamedIdentifier>& applicationTags, std::vector<std::string>& outPluginNames) const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the project
		*/
		void startup();

		/**
		*  @brief
		*    Return whether or not the project is up-and-running
		*
		*  @return
		*    "true" if the project is up-and-running ("startup()" was called successfully), else "false"
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Shutdown the project
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the Boost property tree holding additional meta data for the project
		*
		*  @return
		*    Boost property tree instance or a null pointer in case there is no meta data available at all
		*/
		inline const boost::property_tree::ptree* getMetadataPTree() const;

		/**
		*  @brief
		*    Set a Boost property tree holding additional meta data for the project
		*
		*  @param[in] pTree
		*    Boost property tree instance to be copied
		*/
		void setMetadataPTree(const boost::property_tree::ptree& pTree);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::BaseProject methods               ]
	//[-------------------------------------------------------]
	public:
		virtual bool isLocked() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		std::string		 mFilename;				///< Local UTF-8 filename in platform-independent notation of the project (e.g. on Windows: "data/my_project/project.json")
		std::string		 mBaseDirectory;		///< The UTF-8 base directory name in platform-independent notation (e.g. on Windows: "C:/MyApplication")
		ProjectNameList	 mProjectDependencies;	///< The names of the projects this project depends on
		AssetPackageList mAssetPackageList;		///< List of asset packages of this project
		bool			 mRunning;				///< "true" if the project is up-and-running ("startup()" was called successfully), else "false"
		PluginMap		 mPlugins;				///< Map of plugins, key is the hashed plugin name
		boost::property_tree::ptree* mMetadataPTree;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/project/Project-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Project)
