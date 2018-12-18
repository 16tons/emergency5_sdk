// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/forward/BoostPTree.h>

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Project;
	class NamedIdentifier;
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
	*    EMERGENCY 5 mod class
	*/
	class EM5_API_EXPORT Mod : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ModSystem;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum EnableCheckResult
		{
			MOD_OKAY,					// Everything okay, mod is supported
			MOD_PLUGINS_UNSUPPORTED		// Mod can't be enabled because it has plugins that are unsupported on current platform
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline explicit Mod(uint32);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Mod();

		// Getters
		inline const std::string& getName() const;
		inline const std::string& getDisplayName() const;
		inline const std::string& getIconFilename() const;

		inline qsf::Project& getProject() const;

		inline uint32 getOrderingIndex() const;
		inline void setOrderingIndex(uint32 index);

		EnableCheckResult canBeEnabled();

		inline bool isEnabled() const;
		inline void setEnabled(bool enabled);

		inline bool isMounted() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void initialize(qsf::Project& project, uint32 index = 0);

		void mount();
		void unmount();

		void loadPlugins(const std::vector<qsf::NamedIdentifier>& applicationTags);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Project* mProject;			///< Associated mod project
		uint32		  mOrderingIndex;	///< Index for load order of mods; smaller indices get loaded first

		// Mod metadata
		std::string mName;				///< UTF-8 mod name
		std::string mIconFilename;		///< UTF-8 absolute file path of an image representing the mod; may be empty

		// State
		bool mIsEnabled;	///< If "true", the mod is enabled by the user, but not necessarily mounted yet
		bool mIsMounted;	///< If "true", the mod is mounted, i.e. its plugins are loaded and its assets are mounted


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/modding/Mod-inl.h"
