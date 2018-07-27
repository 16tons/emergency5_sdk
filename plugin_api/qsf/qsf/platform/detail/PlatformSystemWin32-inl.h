// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlatformSystemWin32::~PlatformSystemWin32()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlatformSystemImpl methods        ]
	//[-------------------------------------------------------]
	inline std::string PlatformSystemWin32::getPlatform() const
	{
		return "Windows";
	}

	inline std::string PlatformSystemWin32::getOsArchitecture() const
	{
		return (64 == getOsBitArchitecture()) ? "x64" : "x86";
	}

	inline char PlatformSystemWin32::getPathSeparator() const
	{
		return '\\';
	}

	inline std::string PlatformSystemWin32::getSharedLibraryPrefix() const
	{
		return "";
	}

	inline std::string PlatformSystemWin32::getSharedLibraryExtension() const
	{
		return "dll";
	}

	inline std::string PlatformSystemWin32::getDataDirectoryName(const std::string& name) const
	{
		return name;
	}

	inline bool PlatformSystemWin32::isMouseVisible() const
	{
		return mMouseVisible;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
