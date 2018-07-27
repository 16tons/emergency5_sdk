// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlatformSystemMacOsX::~PlatformSystemMacOsX()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlatformSystemImpl methods        ]
	//[-------------------------------------------------------]
	inline std::string PlatformSystemMacOsX::getPlatform() const
	{
		return "Mac OS X";
	}

	inline std::string PlatformSystemMacOsX::getOsArchitecture() const
	{
		return (64 == getOsBitArchitecture()) ? "x64" : "x86";
	}

	inline char PlatformSystemMacOsX::getPathSeparator() const
	{
		return '/';
	}

	inline std::string PlatformSystemMacOsX::getSharedLibraryPrefix() const
	{
		return "lib";
	}

	inline std::string PlatformSystemMacOsX::getSharedLibraryExtension() const
	{
		return "dylib";
	}

	inline std::string PlatformSystemMacOsX::getDataDirectoryName(const std::string& name) const
	{
		return name;
	}

	inline bool PlatformSystemMacOsX::isMouseVisible() const
	{
		return mMouseVisible;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
