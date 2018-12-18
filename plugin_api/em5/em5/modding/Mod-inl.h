// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Mod::Mod(uint32) :
		mProject(nullptr),
		mOrderingIndex(0),
		mIsEnabled(false),
		mIsMounted(false)
	{
		// Nothing to do in here
	}

	inline Mod::~Mod()
	{
		// Nothing to do in here
	}

	inline const std::string& Mod::getName() const
	{
		return mName;
	}

	inline const std::string& Mod::getDisplayName() const
	{
		// TODO(fw): Currently, there is no such thing as a display name property
		return mName; //mDisplayName;
	}

	inline const std::string& Mod::getIconFilename() const
	{
		return mIconFilename;
	}

	inline qsf::Project& Mod::getProject() const
	{
		return *mProject;
	}

	inline uint32 Mod::getOrderingIndex() const
	{
		return mOrderingIndex;
	}

	inline void Mod::setOrderingIndex(uint32 index)
	{
		mOrderingIndex = index;
	}

	inline bool Mod::isEnabled() const
	{
		return mIsEnabled;
	}

	inline void Mod::setEnabled(bool enabled)
	{
		mIsEnabled = enabled;
	}

	inline bool Mod::isMounted() const
	{
		return mIsMounted;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
