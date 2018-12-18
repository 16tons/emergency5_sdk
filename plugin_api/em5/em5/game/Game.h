// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/game/Game.h>

#include <qsf/base/UniqueInstance.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class CameraComponent;
	class BinarySerializer;
	class SoundTrackManager;
	namespace logic
	{
		class TargetPointManager;
	}
	namespace game
	{
		class GameCounterManager;
		class GameTimerManager;
	}
}
namespace em5
{
	class RightsManager;
	class GameAnimation;
	class GameSimulation;
	class GameSessionConfiguration;
	class CampaignDefinition;
	class MiniMapIconRegistrationLogic;
	class PlayerManager;
	class SelectionManager;
	class SelectionMarkerManager;
	class CommandMarkerManager;
	class OrderInfoManager;
	class GangsterTypeManager;
	class SpecsGroupManager;
	class HintSystem;
	class CutscenePlayer;
	class OverlayManager;
	class AiSpeedLimitFunctor;
	class AiCollisionDetectedFunctor;
	class AiSpecialSteeringStateCallback;
	class MapIdentifier;
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
	*    EMERGENCY 5 game class
	*/
	class EM5_API_EXPORT Game : public qsf::game::Game, public qsf::UniqueInstance<Game>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Specific input handlers for EM5
		static const uint32 FIRE_SIMULATION_INPUT_HANDLER_ID;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Game();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Game();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		void prepareMap(qsf::Map& map, bool wasLoadedBefore);
		void onLoadComplete();

		//[-------------------------------------------------------]
		//[ Managers and sub-systems                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the game simulation instance
		*/
		inline GameSimulation& getSimulation() const;

		/**
		*  @brief
		*    Shortcut method to return the game simulation clock
		*/
		const qsf::Clock& getSimulationClock() const;

		/**
		*  @brief
		*    Return the game animation instance
		*/
		inline GameAnimation& getAnimation() const;

		/**
		*  @brief
		*    Return the game mini map registration logic instance
		*/
		MiniMapIconRegistrationLogic& getMiniMapIconRegistrationLogic() const;

		/**
		*  @brief
		*    Return sound track manager instance
		*/
		inline qsf::SoundTrackManager& getSoundTrackManager() const;

		/**
		*  @brief
		*    Return the game session configuration
		*/
		inline GameSessionConfiguration& getGameSessionConfiguration() const;

		/**
		*  @brief
		*    Return a campaign definition
		*/
		const CampaignDefinition& getCampaignDefinitionByIndex(uint32 campaignIndex);

		/**
		*  @brief
		*    Return campaign definitions
		*/
		const std::vector<CampaignDefinition*>& getCampaignDefinitions() const;

		/**
		*  @brief
		*    Return a campaign index by using a given main event number
		*
		*  @remarks
		*    Campaign index 0:  ME01 ... ME15
		*    Campaign index 1: ME100 ... ME104
		*    Campaign index 2: ME200 ... ME204
		*    Campaign index 3: ME301 ... ME310
		*/
		uint32 getCampaignIndexByMainEventNumber(uint32 mainEventNumber) const;

		/**
		*  @brief
		*    Return all data needed to directly launch a main event
		*
		*  @see
		*    "getCampaignIndexByMainEventNumber"
		*/
		bool getMapAndCampaignInfoByMainEventNumber(uint32 mainEventNumber, MapIdentifier& outMapIdentifier, uint32& outCampaignIndex, uint32& outCampaignPhaseIndex) const;

		/**
		*  @brief
		*    Return the player manager
		*/
		inline PlayerManager& getPlayerManager() const;

		/**
		*  @brief
		*    Return the rights manager
		*
		*  @return
		*    The rights manager
		*/
		inline const RightsManager& getRightsManager() const;

		/**
		*  @brief
		*    Return the game's selection manager
		*/
		inline SelectionManager& getSelectionManager() const;

		/**
		*  @brief
		*    Return the game counter manager
		*/
		inline qsf::game::GameCounterManager& getGameCounterManager() const;

		/**
		*  @brief
		*    Return the game timer manager
		*/
		inline qsf::game::GameTimerManager& getGameTimerManager() const;

		/**
		*  @brief
		*    Return the target point manager
		*/
		inline qsf::logic::TargetPointManager& getTargetPointManager() const;

		/**
		*  @brief
		*    Return the cutscene player
		*/
		inline CutscenePlayer& getCutscenePlayer() const;

		/**
		*  @brief
		*    Return the overlay manager
		*/
		inline OverlayManager& getOverlayManager() const;

		/**
		*  @brief
		*    Returns the order info manager
		*/
		inline OrderInfoManager& getOrderInfoManager() const;

		/**
		*  @brief
		*    Returns the gangster type manager
		*/
		inline GangsterTypeManager& getGangsterTypeManager() const;

		/**
		*  @brief
		*    Returns the specs group manager
		*/
		inline SpecsGroupManager& getSpecsGroupManager() const;

		/**
		*  @brief
		*    Returns the hint system
		*/
		inline HintSystem& getHintSystem() const;

		//[-------------------------------------------------------]
		//[ Camera                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the main camera
		*/
		qsf::CameraComponent* getMainCamera() const;

		/**
		*  @brief
		*    Set the main camera
		*/
		void setMainCamera(qsf::CameraComponent* cameraComponent);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialization of the whole game
		*/
		void serialize(qsf::BinarySerializer& serializer);

		/**
		*  @brief
		*    Load the game from the given file
		*/
		bool loadGame(const std::string& filename);

		/**
		*  @brief
		*    Save the game to the given file
		*/
		bool saveGame(const std::string& filename);

		//[-------------------------------------------------------]
		//[ Interaction                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether the game interaction (camera movement, selection, commands, most of in-game GUI) is blocked
		*
		*  @note
		*    - This is the case when pausing the game in EM20 missions (to stay consistent with old EM titles)
		*/
		bool isGameInteractionBlocked() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::game::Game methods             ]
	//[-------------------------------------------------------]
	protected:
		virtual void onStartup() override;
		virtual void onShutdown() override;
		virtual void onUpdateGame(const qsf::Clock& realtimeClock) override;
		virtual void onGameSpeedChange(float newGameSpeed, float oldGameSpeed) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Extracted part of the setup that is called during the initial setup and after loading
		void startupAiSystems();
		void shutdownAiSystems();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Game animation and simulation
		GameAnimation*	mGameAnimation;				///< Game animation instance; always valid, we have to destroy the instance in case we no longer need it
		GameSimulation*	mGameSimulation;			///< Game simulation instance; always valid, we have to destroy the instance in case we no longer need it

		// Music
		qsf::SoundTrackManager* mSoundTrackManager;

		// Game session configuration
		GameSessionConfiguration& mGameSessionConfiguration;

		// Campaign
		std::vector<CampaignDefinition*> mCampaignDefinitions;	///< Campaign definition instances, invalid until first access through getter

		// Player management
		PlayerManager*		mPlayerManager;
		RightsManager*		mRightsManager;
		OrderInfoManager*	mOrderInfoManager;		///< The order infos the players can order

		// Entity selection and command targets
		SelectionManager*		mSelectionManager;
		SelectionMarkerManager* mSelectionMarkerManager;
		CommandMarkerManager*	mCommandMarkerManager;

		// Other managers
		qsf::game::GameCounterManager*	mGameCounterManager;	///< Game counter manager; always valid, we're responsible for destroying the instance
		qsf::game::GameTimerManager*	mGameTimerManager;		///< Game timer manager; always valid, we're responsible for destroying the instance
		qsf::logic::TargetPointManager*	mTargetPointManager;	///< Target point manager; always valid, we're responsible for destroying the instance
		GangsterTypeManager*			mGangsterTypeManager;	///< Gangster type manager; always valid, we're responsible for destroying the instance
		SpecsGroupManager*				mSpecsGroupManager;		///< Specs group manager; always valid, we're responsible for destroying the instance
		HintSystem*						mHintSystem;			///< Hint system; always valid, we're responsible for destroying the instance
		CutscenePlayer*					mCutscenePlayer;		///< Cutscene player; always valid, we're responsible for destroying the instance
		OverlayManager*					mOverlayManager;		///< Overlay manager; always valid, we're responsible for destroying the instance

		// Functors to influence the AI that are passed during the init process
		AiSpeedLimitFunctor*			mSpeedLimits;
		AiCollisionDetectedFunctor*		mCollisionReaction;
		AiSpecialSteeringStateCallback*	mSpecialStateReaction;

		bool							mIsGroundMapInitialized;

		qsf::MessageProxy				mDestroyObjectMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/Game-inl.h"
