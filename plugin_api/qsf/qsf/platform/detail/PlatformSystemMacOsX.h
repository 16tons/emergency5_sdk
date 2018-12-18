// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


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
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Mac OS X platform system implementation
	*/
	class PlatformSystemMacOsX : public PlatformSystemImpl
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlatformSystemMacOsX();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlatformSystemMacOsX();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PlatformSystemImpl methods        ]
	//[-------------------------------------------------------]
	public:
		inline virtual std::string getPlatform() const override;
		virtual std::string getOs() const override;
		inline virtual std::string getOsArchitecture() const override;
		virtual uint32 getOsBitArchitecture() const override;

		inline virtual char getPathSeparator() const override;
		inline virtual std::string getSharedLibraryPrefix() const override;
		inline virtual std::string getSharedLibraryExtension() const override;

		virtual std::string getComputerName() const override;
		virtual std::string getUserName() const override;
		virtual std::string getUserHomeDirectory() const override;
		virtual std::string getUserDataDirectory() const override;
		inline virtual std::string getDataDirectoryName(const std::string& name) const override;

		virtual std::string getBundleDataDirectory() const;

		virtual std::string getExecutableFilename() const override;
		virtual std::string getModuleFilenameByMemoryAddress(const void* memoryAddress) const override;

		virtual std::string getEnvironmentVariable(const std::string& name) const override;
		virtual bool setEnvironmentVariable(const std::string& name, const std::string& value) const override;
		virtual void deleteEnvironmentVariable(const std::string& name) const override;

		virtual bool execute(const std::string& command, const std::string& parameters, const std::string& workingDirectory = "") const override;
		virtual bool openUrlExternal(const std::string& url) const override;

		virtual std::string getLocaleLanguage() const override;
		virtual std::string getCurrentDirectory() const override;
		virtual bool setCurrentDirectory(const std::string& path) override;

		inline virtual bool isMouseVisible() const override;
		virtual void setMouseVisible(bool visible) override;
		virtual void enableMouseCursorTrap(int left, int top, int right, int bottom) override;
		virtual void disableMouseCursorTrap() override;

		virtual void urgentMessage(const std::string& message, const std::string& title) const override;
		virtual void getDesktopWidthAndHeight(uint32& width, uint32& height) const override;

		virtual Time getSystemTime() const override;
		virtual void sleep(const Time& sleepTime) const override;
		virtual void yield() const override;

		virtual float getPercentageOfUsedPhysicalMemory() const override;
		virtual uint64 getTotalPhysicalMemory() const override;
		virtual uint64 getFreePhysicalMemory() const override;
		virtual uint64 getTotalVirtualMemory() const override;
		virtual uint64 getFreeVirtualMemory() const override;
		virtual uint32 getCpuMhz() const override;

		virtual bool openFileDialog(std::string& outFilename, const std::string& initialDirectory, const std::string& filter) const override;

		virtual bool enableDragDrop(const boost::function<void(const std::string&)>& callback, handle nativeWindowHandle) override;
		virtual void disableDragDrop() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mMouseVisible;	///< Is the mouse cursor visible?


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/platform/detail/PlatformSystemMacOsX-inl.h"
