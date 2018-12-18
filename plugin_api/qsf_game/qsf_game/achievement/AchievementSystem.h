// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/achievement/Achievement.h"

#include <qsf/base/System.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class SteamApiRuntimeLinking;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Achievement system implementation
		*
		*  @remarks
		*    The achievement system manages the local player's achievement progress.
		*/
		class QSF_GAME_API_EXPORT AchievementSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			AchievementSystem();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AchievementSystem();

			/**
			*  @brief
			*    Return the Steam API runtime linking instance
			*/
			inline SteamApiRuntimeLinking& getSteamApiRuntimeLinking();

			/**
			*  @brief
			*    Enable/disable achievement write lock; with enabled modifications and/or chats one might not want to reward the player with achievements
			*/
			inline void setWriteLock(bool writeLock);

			/**
			*  @brief
			*    Clear achievement definitions
			*/
			void clearAchievementDefinitions();

			/**
			*  @brief
			*    Add an achievement definition
			*
			*  @param[in] achievement
			*    The achievement definition, ownership is overtaken by the achievement system
			*/
			void addAchievementDefinition(Achievement& achievement);

			/**
			*  @brief
			*    Get all registered achievements
			*/
			inline void getAchievementList(std::vector<const Achievement*>& achievementList) const;

			/**
			*  @brief
			*    Get an achievement by its name
			*/
			const Achievement* getAchievement(const StringHash& achievementName) const;

			/**
			*  @brief
			*    Add progress to an achievement
			*
			*  @note
			*    - ATTENTION: Does not modify the state of completed achievements
			*/
			void addProgress(const StringHash& achievementName, float progress);

			/**
			*  @brief
			*    Directly set the progress of an achievement
			*
			*  @note
			*    - ATTENTION: Does not modify the state of completed achievements
			*/
			void setProgress(const StringHash& achievementName, float progress);

			/**
			*  @brief
			*    Set the given achievement to completed
			*/
			void setCompleted(const StringHash& achievementName);

			/**
			*  @brief
			*    Is the given achievement completed?
			*/
			bool isCompleted(const StringHash& achievementName) const;

			/**
			*  @brief
			*    Is achievement debug output shown?
			*/
			bool isDebugShown() const;

			/**
			*  @brief
			*    Show or hide achievement debug output
			*/
			void showDebug(bool show);

			/**
			*  @brief
			*    Serialization
			*
			*  @note
			*    - ATTENTION: Does not modify the state of achievements not present in the stream when reading
			*    - ATTENTION: Does not emit AchievementUpdated or AchievementCompleted messages when reading
			*
			*  @exception
			*    Throws "std::exception" in case on an error
			*/
			void serialize(BinarySerializer& serializer);

			/**
			*  @brief
			*    Reset the progress of a given achievements
			*/
			void clearProgress(Achievement& achievement);

			/**
			*  @brief
			*    Reset the progress of all registered achievements
			*/
			void clearAllProgress();

			/**
			*  @brief
			*    Set a dummy achievement instance
			*
			*  @param[in] dummyAchievement
			*    The dummy achievement instance; it has to be created on the heap before, then ownership is transferred to the achievement system, which will care for its destruction
			*
			*  @remarks
			*    The dummy achievement has to be of the same concrete "qsf::game::Achievement" sub-class as all other achievements, so serialization reads the same amount of data
			*/
			void setDummyAchievement(Achievement& dummyAchievement);


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
			typedef std::vector<Achievement*> AchievementList;
			typedef boost::container::flat_map<uint32, Achievement*> AchievementMap;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Don't overload public members with private members since MSVC occasionally tries to use the wrong one.
			//   => Leads to error C2248: 'qsf::game::AchievementSystem::getAchievement' : cannot access private member declared in class 'qsf::game::AchievementSystem'
			Achievement* getAchievementWritable(const StringHash& achievementName);
			void setAchievementProgress(Achievement& achievement, float progress);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SteamApiRuntimeLinking* mSteamApiRuntimeLinking;	///< Steam API runtime linking instance, always valid, we're responsible for destroying the instance if we no longer need it
			bool					mWriteLock;					///< Achievement write lock state; with enabled modifications and/or chats one might not want to reward the player with achievements
			AchievementList			mAchievementList;			///< List of achievements in order of registration
			AchievementMap			mAchievementMap;			///< Achievements mapped by their unique ID, which is the hashed achievement name
			Achievement*			mDummyAchievement;

			// Debug
			bool			mShowDebug;
			DebugDrawProxy	mDebugDrawProxy;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/achievement/AchievementSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::AchievementSystem)
