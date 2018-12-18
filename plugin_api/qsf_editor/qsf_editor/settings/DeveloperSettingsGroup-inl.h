// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline DeveloperSettingsGroup::DeveloperSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			mIgnoreEditorServerHostPort(false),
			mIgnoreEditorServerVersionCheck(false)
		{
			// Nothing to do in here
		}

		inline DeveloperSettingsGroup::~DeveloperSettingsGroup()
		{
			// Nothing to do in here
		}

		inline bool DeveloperSettingsGroup::getIgnoreEditorServerHostPort() const
		{
			return mIgnoreEditorServerHostPort;
		}

		inline void DeveloperSettingsGroup::setIgnoreEditorServerHostPort(bool ignoreEditorServerHostPort)
		{
			assignAndPromoteChange(mIgnoreEditorServerHostPort, ignoreEditorServerHostPort, IGNORE_EDITOR_SERVER_HOST_PORT);
		}

		inline bool DeveloperSettingsGroup::getIgnoreEditorServerVersionCheck() const
		{
			return mIgnoreEditorServerVersionCheck;
		}

		inline void DeveloperSettingsGroup::setIgnoreEditorServerVersionCheck(bool ignoreEditorServerVersionCheck)
		{
			assignAndPromoteChange(mIgnoreEditorServerVersionCheck, ignoreEditorServerVersionCheck, IGNORE_EDITOR_SERVER_VERSION_CHECK);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
