// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/PlatformSystemImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline std::string PlatformSystem::getPlatform() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getPlatform();
	}

	inline std::string PlatformSystem::getPlatformArchitecture() const
	{
		#if QSF_X64
			return "x64";
		#else
			return "x86";
		#endif
	}

	inline uint32 PlatformSystem::getPlatformBitArchitecture() const
	{
		#if QSF_X64
			return 64;
		#else
			return 32;
		#endif
	}

	inline std::string PlatformSystem::getOs() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getOs();
	}

	inline std::string PlatformSystem::getOsArchitecture() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getOsArchitecture();
	}

	inline uint32 PlatformSystem::getOsBitArchitecture() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getOsBitArchitecture();
	}

	inline char PlatformSystem::getPathSeparator() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getPathSeparator();
	}

	inline std::string PlatformSystem::getSharedLibraryPrefix() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getSharedLibraryPrefix();
	}

	inline std::string PlatformSystem::getSharedLibraryExtension() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getSharedLibraryExtension();
	}

	inline std::string PlatformSystem::getComputerName() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getComputerName();
	}

	inline std::string PlatformSystem::getUserName() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getUserName();
	}

	inline std::string PlatformSystem::getUserHomeDirectory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getUserHomeDirectory();
	}

	inline std::string PlatformSystem::getUserDataDirectory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getUserDataDirectory();
	}

	inline std::string PlatformSystem::getDataDirectoryName(const std::string& name) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getDataDirectoryName(name);
	}

	inline std::string PlatformSystem::getBundleDataDirectory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getBundleDataDirectory();
	}

	inline std::string PlatformSystem::getExecutableFilename() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getExecutableFilename();
	}

	inline std::string PlatformSystem::getModuleFilenameByMemoryAddress(const void* memoryAddress) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getModuleFilenameByMemoryAddress(memoryAddress);
	}

	inline std::string PlatformSystem::getEnvironmentVariable(const std::string& name) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getEnvironmentVariable(name);
	}

	inline bool PlatformSystem::setEnvironmentVariable(const std::string& name, const std::string& value) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->setEnvironmentVariable(name, value);
	}

	inline void PlatformSystem::deleteEnvironmentVariable(const std::string& name) const
	{
		// Redirect to the concrete implementation
		mPlatformSystem->deleteEnvironmentVariable(name);
	}

	inline bool PlatformSystem::execute(const std::string& command, const std::string& parameters, const std::string& workingDirectory) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->execute(command, parameters, workingDirectory);
	}

	inline bool PlatformSystem::openUrlExternal(const std::string& url) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->openUrlExternal(url);
	}

	inline std::string PlatformSystem::getLocaleLanguage() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getLocaleLanguage();
	}

	inline std::string PlatformSystem::getCurrentDirectory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getCurrentDirectory();
	}

	inline bool PlatformSystem::setCurrentDirectory(const std::string& path)
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->setCurrentDirectory(path);
	}

	inline bool PlatformSystem::isMouseVisible() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->isMouseVisible();
	}

	inline void PlatformSystem::setMouseVisible(bool visible) const
	{
		// Redirect to the concrete implementation
		mPlatformSystem->setMouseVisible(visible);
	}

	inline void PlatformSystem::enableMouseCursorTrap(int left, int top, int right, int bottom)
	{
		// Redirect to the concrete implementation
		mPlatformSystem->enableMouseCursorTrap(left, top, right, bottom);
	}

	inline void PlatformSystem::disableMouseCursorTrap()
	{
		// Redirect to the concrete implementation
		mPlatformSystem->disableMouseCursorTrap();
	}

	inline void PlatformSystem::urgentMessage(const std::string& message, const std::string& title) const
	{
		// Redirect to the concrete implementation
		mPlatformSystem->urgentMessage(message, title);
	}

	inline void PlatformSystem::getDesktopWidthAndHeight(uint32& width, uint32& height) const
	{
		// Redirect to the concrete implementation
		mPlatformSystem->getDesktopWidthAndHeight(width, height);
	}

	inline Time PlatformSystem::getSystemTime() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getSystemTime();
	}

	inline void PlatformSystem::sleep(const Time& sleepTime) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->sleep(sleepTime);
	}

	inline void PlatformSystem::yield() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->yield();
	}

	inline float PlatformSystem::getPercentageOfUsedPhysicalMemory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getPercentageOfUsedPhysicalMemory();
	}

	inline uint64 PlatformSystem::getTotalPhysicalMemory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getTotalPhysicalMemory();
	}

	inline uint64 PlatformSystem::getFreePhysicalMemory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getFreePhysicalMemory();
	}

	inline uint64 PlatformSystem::getTotalVirtualMemory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getTotalVirtualMemory();
	}

	inline uint64 PlatformSystem::getFreeVirtualMemory() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getFreeVirtualMemory();
	}

	inline uint32 PlatformSystem::getCpuMhz() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->getCpuMhz();
	}

	inline bool PlatformSystem::openFileDialog(std::string& outFilename, const std::string& initialDirectory, const std::string& filter) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->openFileDialog(outFilename, initialDirectory, filter);
	}

	inline bool PlatformSystem::enableDragDrop(const boost::function<void(const std::string&)>& callback, handle nativeWindowHandle) const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->enableDragDrop(callback, nativeWindowHandle);
	}

	inline void PlatformSystem::disableDragDrop() const
	{
		// Redirect to the concrete implementation
		return mPlatformSystem->disableDragDrop();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* PlatformSystem::getName() const
	{
		return "Platform system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
