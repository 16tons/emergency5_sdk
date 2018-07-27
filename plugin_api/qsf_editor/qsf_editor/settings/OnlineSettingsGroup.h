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
		*    QSF online editor settings group implementation
		*/
		class QSF_EDITOR_API_EXPORT OnlineSettingsGroup : public SettingsGroup, public qsf::UniqueInstance<OnlineSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;								///< "qsf::editor::OnlineSettingsGroup" unique pluginable settings group ID
			static const uint32 TIMEOUT_RETRANSMIT_COUNT;					///< "TimeoutRetransmitCount" unique class property ID inside the class
			static const uint32 TIMEOUT_MINIMUM;							///< "TimeoutMinimum" unique class property ID inside the class
			static const uint32 TIMEOUT_MAXIMUM;							///< "TimeoutMaximum" unique class property ID inside the class
			static const uint32 MINIMUM_NUMBER_OF_UNUSED_GLOBAL_ASSET_IDS;	///< "MinimumNumberOfUnusedGlobalAssetIDs" unique class property ID inside the class
			static const uint32 MINIMUM_NUMBER_OF_UNUSED_ENTITY_IDS;		///< "MinimumNumberOfUnusedEntityIDs" unique class property ID inside the class


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
			inline explicit OnlineSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~OnlineSettingsGroup();

			/**
			*  @brief
			*    Get the maximum retransmission count
			*
			*  @return
			*    The maximum retransmission count
			*/
			inline uint32 getTimeoutRetransmitCount() const;

			/**
			*  @brief
			*    Set the maximum retransmission count
			*
			*  @param[in] timeoutLimit
			*    New maximum retransmission count
			*/
			inline void setTimeoutRetransmitCount(uint32 timeoutRetransmitCount);

			/**
			*  @brief
			*    Get the timeout minimum
			*
			*  @return
			*    The timeout minimum in milliseconds
			*/
			inline uint32 getTimeoutMinimum() const;

			/**
			*  @brief
			*    Set timeout minimum
			*
			*  @param[in] timeoutMinimum
			*    New timeout minimum in milliseconds
			*/
			inline void setTimeoutMinimum(uint32 timeoutMinimum);

			/**
			*  @brief
			*    Get the timeout maximum
			*
			*  @return
			*    The timeout maximum in milliseconds
			*/
			inline uint32 getTimeoutMaximum() const;

			/**
			*  @brief
			*    Set timeout maximum
			*
			*  @param[in] timeoutMaximum
			*    New timeout maximum in milliseconds
			*/
			inline void setTimeoutMaximum(uint32 timeoutMaximum);

			/**
			*  @brief
			*    Get the minimum number of unused global asset ids
			*
			*  @return
			*    Minimum number of unused global asset ids, if the local buffer has fewer elements we need to ask the server for more
			*/
			inline uint32 getMinimumNumberOfUnusedGlobalAssetIds() const;

			/**
			*  @brief
			*    Set minimum number of unused global asset ids
			*
			*  @param[in] minimumNumberOfUnusedGlobalAssetIds
			*    New minimum number of unused global asset ids, if the local buffer has fewer elements we need to ask the server for more
			*/
			inline void setMinimumNumberOfUnusedGlobalAssetIds(uint32 minimumNumberOfUnusedGlobalAssetIds);

			/**
			*  @brief
			*    Get the minimum number of unused entity ids
			*
			*  @return
			*    Minimum number of unused entity ids, if the local buffer has fewer elements we need to ask the server for more
			*/
			inline uint32 getMinimumNumberOfUnusedEntityIds() const;

			/**
			*  @brief
			*    Set minimum number of unused entity ids
			*
			*  @param[in] minimumNumberOfUnusedEntityIds
			*    New minimum number of unused entity ids, if the local buffer has fewer elements we need to ask the server for more
			*/
			inline void setMinimumNumberOfUnusedEntityIds(uint32 minimumNumberOfUnusedEntityIds);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			uint32 mTimeoutRetransmitCount;					///< The maximum retransmission count
			uint32 mTimeoutMinimum;							///< The timeout minimum in milliseconds
			uint32 mTimeoutMaximum;							///< The timeout maximum in milliseconds
			uint32 mMinimumNumberOfUnusedGlobalAssetIds;	///< Minimum number of unused global asset ids, if the local buffer has fewer elements we need to ask the server for more
			uint32 mMinimumNumberOfUnusedEntityIds;			///< Minimum number of unused entity ids, if the local buffer has fewer elements we need to ask the server for more


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
#include "qsf_editor/settings/OnlineSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::OnlineSettingsGroup)
