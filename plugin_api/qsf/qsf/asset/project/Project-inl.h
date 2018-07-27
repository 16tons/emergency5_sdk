// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Project::Project() :
		mRunning(false),
		mMetadataPTree(nullptr)
	{
		// Nothing to do in here
	}

	inline Project::~Project()
	{
		// Clear the project
		clear();
	}

	inline const std::string& Project::getFilename() const
	{
		return mFilename;
	}

	inline const std::string& Project::getBaseDirectory() const
	{
		return mBaseDirectory;
	}

	inline void Project::setBaseDirectory(const std::string& baseDirectory)
	{
		mBaseDirectory = baseDirectory;
	}

	inline const Project::ProjectNameList& Project::getProjectDependencies() const
	{
		return mProjectDependencies;
	}

	inline void Project::setProjectDependencies(const Project::ProjectNameList& projectDependencies)
	{
		mProjectDependencies = projectDependencies;
	}

	inline const Project::AssetPackageList& Project::getAssetPackageList() const
	{
		return mAssetPackageList;
	}

	inline const Project::PluginMap& Project::getPlugins() const
	{
		return mPlugins;
	}

	inline bool Project::isRunning() const
	{
		return mRunning;
	}

	inline const boost::property_tree::ptree* Project::getMetadataPTree() const
	{
		return mMetadataPTree;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
