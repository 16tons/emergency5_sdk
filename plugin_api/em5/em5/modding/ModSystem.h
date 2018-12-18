// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/modding/Mod.h"

#include <qsf/base/System.h>
#include <qsf/base/manager/ElementManager.h>
#include <qsf/reflection/CampClass.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class NamedIdentifier;
}
namespace boost
{
	namespace filesystem
	{
		class path;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 mod system
	*
	*  @todo
	*    - TODO(fw): Move this to QSF (best qsf::game) as it's not really EM5-specific
	*/
	class EM5_API_EXPORT ModSystem : public qsf::System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ModSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ModSystem();

		inline const std::string& getModsUserDirectory() const;

		inline const boost::container::flat_map<uint32, Mod*>& getMods() const;
		void getModsSorted(std::vector<Mod*>& outModList) const;

		bool isAnyModEnabled() const;
		void getEnabledMods(std::vector<Mod*>& outMods) const;
		void disableAllMods() const;

		void mountEnabledMods();
		void unmountMods();

		void filterCoreProjectsFromMods();

		void loadModPluginsForApplicationTags(const std::vector<qsf::NamedIdentifier>& applicationTags);

		bool loadModUserSettings();
		bool saveModUserSettings();

		void gatherModStateFromProjects();

		void eraseModFromDisk(uint32 modId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef qsf::ElementManager<uint32, Mod> ModManager;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addProjectAsMod(qsf::Project& project);
		void scanProjectsForMods();

		bool loadModUserSettings(const boost::filesystem::path& filePath);
		bool saveModUserSettings(const boost::filesystem::path& filePath);

		void onProjectAdded(qsf::Project& project);
		void onProjectRemoval(qsf::Project& project);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ModManager  mMods;					///< Management of mod instances
		std::string mModsUserDirectory;		///< UTF-8 encoded path of "mods" directory in user data

		boost::signals2::connection mConnectionProjectAdded;
		boost::signals2::connection mConnectionProjectRemoval;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/modding/ModSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ModSystem)
