// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/settings/SettingsGroup.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/base/UniqueInstance.h>
#include <qsf/time/Time.h>
#include <qsf/reflection/type/CampGlmIVec2.h>


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
	*    Multiplayer settings group implementation
	*/
	class MultiplayerSettingsGroup : public qsf::SettingsGroup, public qsf::UniqueInstance<MultiplayerSettingsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;						///< "em5::MultiplayerSettingsGroup" unique pluginable settings group ID
		static const uint32 LAST_USED_IP_FOR_DIRECT_CONNECT;	///< "LastUsedIpForDirectConnect" unique class property ID inside the class


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
		explicit MultiplayerSettingsGroup(qsf::SettingsGroupManager* settingsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MultiplayerSettingsGroup();

		inline bool getUseCompression() const		{ return mUseCompression; }
		inline void setUseCompression(bool value)
		{
			mUseCompression = value;
		}

		inline bool getDoPrepareData() const		{ return mDoPrepareData; }
		inline void setDoPrepareData(bool value)
		{
			mDoPrepareData = value;
		}

		inline uint16 getCivilPercentage() const		{ return mCivilPercentage; }
		inline void setCivilPercentage(uint16 value)
		{
			mCivilPercentage = glm::clamp(value, static_cast<uint16>(0), static_cast<uint16>(100));
		}

		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(LastUsedIpForDirectConnect, const std::string&, mLastUsedIpForDirectConnect, LAST_USED_IP_FOR_DIRECT_CONNECT)


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mUseCompression;
		bool		mDoPrepareData;
		uint16		mCivilPercentage;
		std::string	mLastUsedIpForDirectConnect;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::MultiplayerSettingsGroup)
