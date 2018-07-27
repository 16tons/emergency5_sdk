// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/settings/SettingsGroup.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF developer editor settings group implementation
		*/
		class QSF_EDITOR_API_EXPORT DeveloperSettingsGroup : public SettingsGroup, public qsf::UniqueInstance<DeveloperSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;						///< "qsf::editor::DeveloperSettingsGroup" unique pluginable settings group ID
			static const uint32 IGNORE_EDITOR_SERVER_HOST_PORT;		///< "IgnoreEditorServerHostPort" unique class property ID inside the class
			static const uint32 IGNORE_EDITOR_SERVER_VERSION_CHECK;	///< "IgnoreEditorServerVersionCheck" unique class property ID inside the class


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] settingsGroupManager
			*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
			*/
			inline explicit DeveloperSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~DeveloperSettingsGroup();

			inline bool getIgnoreEditorServerHostPort() const;
			inline void setIgnoreEditorServerHostPort(bool ignoreEditorServerHostPort);

			inline bool getIgnoreEditorServerVersionCheck() const;
			inline void setIgnoreEditorServerVersionCheck(bool ignoreEditorServerVersionCheck);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			bool mIgnoreEditorServerHostPort;
			bool mIgnoreEditorServerVersionCheck;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/settings/DeveloperSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::DeveloperSettingsGroup)
