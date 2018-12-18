// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ProjectManager::~ProjectManager()
	{
		// Clear the project manager
		clear();
	}

	inline const ProjectManager::ProjectList& ProjectManager::getProjectList() const
	{
		return mProjectList;
	}

	inline Project* ProjectManager::getProject(const StringHash& stringHash) const
	{
		const ProjectMap::const_iterator iterator = mProjectMap.find(stringHash);
		return (iterator != mProjectMap.cend()) ? iterator->second : nullptr;
	}

	uint32 ProjectManager::getCurrentProjectId() const
	{
		return mCurrentProjectId;
	}

	inline Project* ProjectManager::getCurrentProject() const
	{
		return getProject(mCurrentProjectId);
	}

	void ProjectManager::setCurrentProjectId(uint32 id)
	{
		mCurrentProjectId = id;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
