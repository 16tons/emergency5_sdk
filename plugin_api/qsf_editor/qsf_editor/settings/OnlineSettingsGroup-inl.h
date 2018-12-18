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
		inline OnlineSettingsGroup::OnlineSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			mTimeoutRetransmitCount(32),
			mTimeoutMinimum(30000),
			mTimeoutMaximum(300000),
			mMinimumNumberOfUnusedGlobalAssetIds(1000),
			mMinimumNumberOfUnusedEntityIds(5000)
		{
			// Nothing to do in here
		}

		inline OnlineSettingsGroup::~OnlineSettingsGroup()
		{
			// Nothing to do in here
		}

		inline uint32 OnlineSettingsGroup::getTimeoutRetransmitCount() const
		{
			return mTimeoutRetransmitCount;
		}

		inline void OnlineSettingsGroup::setTimeoutRetransmitCount(uint32 timeoutRetransmitCount)
		{
			assignAndPromoteChange(mTimeoutRetransmitCount, timeoutRetransmitCount, TIMEOUT_RETRANSMIT_COUNT);
		}

		inline uint32 OnlineSettingsGroup::getTimeoutMinimum() const
		{
			return mTimeoutMinimum;
		}

		inline void OnlineSettingsGroup::setTimeoutMinimum(uint32 timeoutMinimum)
		{
			assignAndPromoteChange(mTimeoutMinimum, timeoutMinimum, TIMEOUT_MINIMUM);
		}

		inline uint32 OnlineSettingsGroup::getTimeoutMaximum() const
		{
			return mTimeoutMaximum;
		}

		inline void OnlineSettingsGroup::setTimeoutMaximum(uint32 timeoutMaximum)
		{
			assignAndPromoteChange(mTimeoutMaximum, timeoutMaximum, TIMEOUT_MAXIMUM);
		}

		inline uint32 OnlineSettingsGroup::getMinimumNumberOfUnusedGlobalAssetIds() const
		{
			return mMinimumNumberOfUnusedGlobalAssetIds;
		}

		inline void OnlineSettingsGroup::setMinimumNumberOfUnusedGlobalAssetIds(uint32 minimumNumberOfUnusedGlobalAssetIds)
		{
			assignAndPromoteChange(mMinimumNumberOfUnusedGlobalAssetIds, minimumNumberOfUnusedGlobalAssetIds, MINIMUM_NUMBER_OF_UNUSED_GLOBAL_ASSET_IDS);
		}

		inline uint32 OnlineSettingsGroup::getMinimumNumberOfUnusedEntityIds() const
		{
			return mMinimumNumberOfUnusedEntityIds;
		}

		inline void OnlineSettingsGroup::setMinimumNumberOfUnusedEntityIds(uint32 minimumNumberOfUnusedEntityIds)
		{
			assignAndPromoteChange(mMinimumNumberOfUnusedEntityIds, minimumNumberOfUnusedEntityIds, MINIMUM_NUMBER_OF_UNUSED_ENTITY_IDS);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
