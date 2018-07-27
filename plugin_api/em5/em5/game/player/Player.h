// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/application/VersionSwitch.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/math/Color4.h>
#include <qsf/math/EnumBitmask.h>
#include <qsf/renderer/component/CameraComponent.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class Map;
	class Entity;
	class Clock;
	class FreeMoveInputHandler;
}
namespace em5
{
	class UnitPool;
	class IngameCameraInputHandler;
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
	*    EMERGENCY 5 player class
	*/
	class EM5_API_EXPORT Player : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 MAIN_CAMERA_INPUT_HANDLER_ID;

		enum class CameraType
		{
			UNKNOWN_CAMERA = 0,
			INGAME_CAMERA,
			FREE_CAMERA,
			CUTSCENE_CAMERA
		};

		struct BuyOption
		{
			std::string  mUnitName;
			std::string  mAdditionalEventPools;
			std::string  mInfoText;
			std::string  mUpgrade;
			std::string  mIconName;
			uint32		 mCost;
			mutable	bool mIsBought;
			bool		 mIsPromotion;

			BuyOption() : mCost(999), mIsBought(false), mIsPromotion(false) {}
		};
		typedef std::vector<const BuyOption*> BuyOptionPointerArray;

		enum DebugFlag
		{
			DEBUG_PLAYER,
			DEBUG_UNITPOOL
		};
		typedef qsf::EnumBitmask<uint8, DebugFlag> DebugFlagSet;

		typedef std::vector<uint64> EntityIdArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Player(uint32 index, bool isLocal, VersionSwitch::Edition gameEdition);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Player();

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Start up the player
		*/
		void startup();

		/**
		*  @brief
		*    Shutdown the player
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Basic identification                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the player's name
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Set the player's name
		*/
		inline void setName(const std::string& name);

		/**
		*  @brief
		*    Return the player's index
		*/
		inline uint32 getPlayerIndex() const;

		/**
		*  @brief
		*    Set the player's index
		*/
		inline void setPlayerIndex(uint32 playerIndex);

		/**
		*  @brief
		*    Return the player's team ID
		*/
		inline uint16 getTeamId() const;

		/**
		*  @brief
		*    Set the player's team ID
		*/
		inline void setTeamId(uint16 teamId);

		/**
		*  @brief
		*    Returns the state of the player if he is local or not
		*/
		inline bool isPlayerLocal() const;

		/**
		*  @brief
		*    Returns the player's game edition (e.g. Standard or Deluxe edition)
		*/
		inline VersionSwitch::Edition getGameEdition() const;

		// Player color
		inline qsf::Color4 getPlayerColor() const;
		inline void setPlayerColor(const qsf::Color4& color);

		// UserData
		inline uint32 getUserData() const;
		inline void setUserData(const uint32 userData);

		//[-------------------------------------------------------]
		//[ Camera                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the main camera entity
		*/
		qsf::Entity* getMainCameraEntity() const;

		/**
		*  @brief
		*    Get the main camera component
		*/
		inline qsf::CameraComponent* getMainCameraComponent() const;

		/**
		*  @brief
		*    Get the main camera input handler
		*/
		qsf::FreeMoveInputHandler* getMainCameraInputHandler() const;
		inline CameraType getCameraType() const;
		inline qsf::CameraComponent* getCameraComponent() const;
		void setCameraType(CameraType cameraType, qsf::CameraComponent* cameraComponent = nullptr, bool inputChange = true);

		void resetCameraToStartTransform();
		void setCameraToTransform(const qsf::Transform& transform);
		void setCameraToEntityTransform(const qsf::Entity& cameraEntity);
		void setCameraToEntityTransform(const std::string& cameraEntityEventTag);

		inline const glm::vec3& getViewPosition() const;
		inline void setViewPosition(const glm::vec3& viewPosition);
		void updateViewTransform();
		qsf::Ray getCameraViewRay() const;
		qsf::CameraComponent* getStartCameraInMap(qsf::Map& map) const;

		//[-------------------------------------------------------]
		//[ Unit pool                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the player's unit pool
		*/
		inline UnitPool& getUnitPool() const;

		void unblockUnitPool();

		/**
		*  @brief
		*    Return the saved group; can be empty or misleading uints
		*
		*  @param[in] index
		*    The index of the saved group. Only valid for '0' - '8'
		*/
		const std::vector<uint64>& getUnitGroupByIndex(uint32 index);

		void setUnitGroupByIndex(uint32 index, const std::vector<uint64>& savedIds);

		//[-------------------------------------------------------]
		//[ Score, credits                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the score of the player
		*
		*  @return
		*    The player's current score
		*/
		inline uint32 getScore() const;

		/**
		*  @brief
		*    Set the score to the given value
		*
		*  @param[in] score
		*    Score value to set
		*
		*  @note
		*    - Emits a message to inform e.g. the in-game HUD about the change
		*/
		void setScore(uint32 score);

		/**
		*  @brief
		*    Add the given value to the score
		*
		*  @param[in] score
		*    Value to add to the player's score, can be negative as well
		*/
		inline void addScore(int32 score);

		/**
		*  @brief
		*    Return the player's credits
		*
		*  @return
		*    Amount of credits the player currently owns
		*/
		inline uint32 getCredits() const;

		/**
		*  @brief
		*    Set the credits to the given value
		*
		*  @param[in] credits
		*    Amount of credits to set
		*
		*  @note
		*    - Emits a message to inform e.g. the in-game HUD about the change
		*/
		void setCredits(uint32 credits);

		/**
		*  @brief
		*    Subtract the given value from the credits
		*
		*  @param[in] credits
		*    Amount of credits to be added, can be negative as well
		*/
		inline void addCredits(int32 credits);

		//[-------------------------------------------------------]
		//[ Buy options                                           ]
		//[-------------------------------------------------------]
		const BuyOptionPointerArray& getBuyOptions() const;
		void clearBuyOptions();
		void addBuyOption(const BuyOption& buyOption);
		bool purchaseBuyOptionByIndex(uint32 index);
		bool purchaseBuyOption(const BuyOption& buyOption);
		bool hasUnboughtBuyOptions() const;

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Toggle debug flag
		*/
		inline void toggleDebugFlag(DebugFlag flag);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialization of the player
		*/
		void serialize(qsf::BinarySerializer& serializer);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		void updateViewPosition(const qsf::JobArguments&);
		void updateDynamicPssmLambda(const qsf::JobArguments&);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Camera
		void createMainCamera();
		void setMainCamera(qsf::CameraComponent* cameraComponent, bool isFreeCamera, bool inputChange);
		void switchToIngameCamera(bool inputChange = true);
		void switchToFreeCamera(bool inputChange = true);
		void switchToCutsceneCamera(qsf::CameraComponent* cameraComponent, bool inputChange = true);

		// Shadow
		void setFixedPssmLambda();
		void setDynamicPssmLambda();
		void setPssmLambda(float pssmLambda);

		// Visualization
		void setMapBorderVisualizationEnabled(bool enabled) const;

		// Debug
		void updateDebugOutput(const qsf::JobArguments&);
		void refreshDebug(const qsf::Clock* clock = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General player information
		std::string			mName;				///< Displayed player name
		uint32				mIndex;				///< Player index beginning with 0 for multi-player sessions
		uint16				mTeamId;			///< Player team ID beginning with 0 for multi-player sessions
		bool				mIsLocal;			///< "true" for local player, "false" for distant players in a multi-player session
		VersionSwitch::Edition mGameEdition;	///< Game edition the player uses
		qsf::Color4			mPlayerColor;		///< The color of the player (mainly used for multiplay)
		uint32				mUserData;			///< A user/game defined value, currently unused in EM5

		// Unit pool
		UnitPool*				   mUnitPool;	///< The player's unit pool
		std::vector<EntityIdArray> mUnitGroups;	///< The player can save groups of units via strg + [1-9]

		// Score, money
		uint32 mScore;		///< Player's current score
		uint32 mCredits;	///< Money of the player

		// Buy options
		BuyOptionPointerArray mBuyOptions;

		// Camera
		CameraType							mCameraType;
		qsf::WeakPtr<qsf::CameraComponent>	mCameraComponent;		///< The currently used camera component
		qsf::WeakPtr<qsf::CameraComponent>	mMainCameraComponent;	///< The player's current main camera
		glm::vec3							mViewPosition;			///< World position of the player camera's target
		IngameCameraInputHandler*			mIngameCameraInputHandler;

		// Job update
		qsf::JobProxy	mUpdateJobProxy;		///< Job proxy for regular updates
		qsf::JobProxy	mUpdateDynamicPssmLambdaJobProxy;

		// Debug
		//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
		qsf::JobProxy		mDebugUpdateJobProxy;	///< Job proxy for debug output
		DebugFlagSet		mDebugFlags;			///< Flags for debug output
		qsf::DebugDrawProxy	mDebugDrawProxy;		///< Proxy for debug output

		bool mIsSerialized;	///< Indicates if the player is serialized


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/player/Player-inl.h"
