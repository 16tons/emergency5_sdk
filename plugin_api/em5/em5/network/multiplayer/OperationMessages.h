// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/hint/HintMessage.h"
#include "em5/game/units/OrderInfo.h"
#include "em5/network/matchmaking/MatchmakingTypes.h"
#include "em5/network/multiplayer/BitStreamAndEnumClassHelper.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/StringHash.h>
#include <qsf/math/Color4.h>
#include <qsf/time/Time.h>

#include <type_traits> // For std::underlying_type
#include <memory> // For std::make_unique


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		typedef boost::container::flat_map<qsf::NamedIdentifier, uint64> ContentAuthenticationChecksums;

		/**
		*  @brief
		*    EMERGENCY 5 multiplayer operation message types
		*/
		// Note: We use here enum class to avoid that the names of the enum pollutes the scope in which the enum was defined
		// The only downside is: that an enum class type isn't implicit convertible into an integer type
		// Please make sure that when adding new types to the list to put them at the end. Otherwise we might get into problems
		// We support up to 254 operation messages
		enum class OperationMessage : uint8
		{
			UNKNOWN,
			// Pre game messages
			CLIENT_JOIN_REQUEST,			///< A client wants to join to a session
			SERVER_JOIN_RESPONSE,			///< The response which the host sends to a client after a join request

			CLIENT_CHANGE_READY,			///< An client changes it's ready state (ready for starting the game, is normally triggered by the player)

			SERVER_LOAD_MAP,				///< The host wants that every client should now load the map (the game will shortly begin)
			CLIENT_MAP_LOADED,				///< Response from the clients after they received the SERVER_LOAD_MAP operation message and have successfully loaded the map

			SERVER_GAME_START,				///< Send by the host to all clients, when everyone has successfully loaded the map and the game can start

			DATA_UPDATE,					///< This operation message gets periodically send from the host to all clients. With this message the hosts informs any client about data changes of any entity (e.g. changed by simulation steps)
			CLIENT_COMMAND_EXECUTION,		///< This operation message gets send by each client to the host, when the player on the client initiates any command (e.g. Let unit move to a sepcific position)

			SERVER_PLAYER_JOINED,			///< Send by the host, when a player joins the session (contains a list of player informations, at least one)
			SERVER_PLAYER_LEFT,				///< Send by the host, when a player left the session
			SERVER_PLAYER_CHANGED_READY,	///< Send by the host, when a player changed it's ready state

			SERVER_PLAYER_CHANGED_USERDATA,	///< Send by the host, when a player changed it's userData
			CLIENT_PLAYER_CHANGED_USERDATA,	///< Send by the client, when this player changed it's userData

			CHAT_MESSAGE,					///< Send by host or client, when a chat message was created

			// Other operation message combined with DATA_UPDATE message in one stream
			CREATE_ENTITY_FROM_PREFAB,		///< An entity was created on host side from an prefab (Contains a list of the data, at least one)
			DESTROY_ENTITY,					///< An entity was destroyed on host side (Contains a list of the entity ids, at least one)
			UNIT_SPAWNED,					///< An unit has spawned (Contains a list of the data, at least one)
			UNIT_SPAWNED_PLAYER_UPDATE_SELECTION,	///< An unit has spawned and qualifies for a selection update (Contains a list of the data, at least one)
			FREEPLAY_EVENT_UPDATE,			///< An freeplay event has changed on the host side
			PLAYER_SCORE_UPDATE,			///< A player score has changed
			GAME_MODE_UPDATE,				///< The game mode data has changed
			NEW_HINTS,						///< A list of hints which were started between two update calls
			MINIMAP_MESSAGES,				///< A list of minimap related messages
			MINIMAP_ICON_MESSAGES,				///< A list of minimap icon messages
			AUDIO_UPDATE,					///< A list of changed audio information (play/stop)
			FIREHOSE_UPDATE,				///< A list of data for newly created firehoses
			SPAWN_PARTICLE,					///< A list of data for newly spawned particles

			CLIENT_ORDERED_UNIT,			///< Send by the client, when a unit should be ordered (one message per unit)
			CLIENT_CAMERA_POSITION,			///< Send by the client to inform the host about the camera position of the player (needed for cleanup)

			SERVER_COMMAND_EXECUTION_RESPONSE, ///< This operation message gets send by the host as the response to a CLIENT_COMMAND_EXECUTION message

			HIGHSCORE_SUBMIT_INITIATED,			///< Gets send by host or an client if someone wants to submit the highscore
			HIGHSCORE_SUBMIT_DECLINED,			///< Gets send by host or an client if someone declined to submit the highscore
			HIGHSCORE_SUBMIT_ACCEPT,			///< Gets send by host or an client if someone accept to submit the highscore
			HIGHSCORE_SUBMIT_SUCCEEDED,			///< Gets send by the host, when the highscore publish was done

			// Other operation message combined with DATA_UPDATE message in one stream
			LAYER_CHANGED,						///< Contains a list of layer data which have been changed (got activated/deactivated)

			SERVER_PING,						///< Send by the server
			CLIENT_PING_ACKNOWLEDGE,			///< Send by the client as a response to SERVER_PING

			// Other operation message combined with DATA_UPDATE message in one stream
			FADE_ENTITY,						///< Send by the server to let a entity fade in/out via fade effect component
			MINIMAP_PING,						///< Send by server and client

			_NUM_MESSAGES,
		};

		// Note: We use here enum class to avoid that the names of the enum pollutes the scope in which the enum was defined
		// The only downside is: that an enum class type isn't implicit convertible into an integer type
		enum class JoinResponse : uint8
		{
			UNKNOWN,
			JOIN_SUCCESS,
			JOIN_BAD_PASSWORD,
			JOIN_INCOMPATIBLE_GAME_MODIFICATIONS,
			JOIN_CONTENT_AUTHENTICATION_FAILED,
			JOIN_SESSION_FULL
		};

		struct JoinResponseData
		{
			JoinResponse		response;
			qsf::GlobalAssetId	globalMapId;
			uint32				playerIndex;
			std::string			playerName;

			// The following is for direct-connect only, but doesn't hurt to transport this information in a generic way
			std::string			  sessionName;
			multiplayer::GameMode gameMode;
			uint32				  maxPlayerCount;
			uint32				  gameDuration;
			bool				  playerSeparation;
		};

		struct JoinRequestData
		{
			std::string						playerName;
			uint8							edition;		// Enumerated by "VersionSwitch::Edition" enum definition
			std::string						password;
			std::string						gameModifications;
			ContentAuthenticationChecksums	checksums;
			uint8							mapQualiy;   // Enumerated by "qsf::RendererSettingsGroup::MapQuality" enum definition
		};

		struct EntityCreatedFromPrefabData
		{
			qsf::StringHash			mLocalAssetId;

			EntityCreatedFromPrefabData() :
				mLocalAssetId(0)
			{}

			EntityCreatedFromPrefabData(const qsf::StringHash& localAssetId) :
				mLocalAssetId(localAssetId)
			{}
		};

		struct UnitSpawnedData
		{
			uint64 mSpawnedUnitEntityId;	///< The ID of the spawned unit
			uint32 mPlayerIndex;			///< The player index, which caused the spawing
			uint32 mOrderInfoId;			///< The ID of the order info, which describes the unit
			uint64 mContainerEntityId;		///< The entity ID to which the entity is linked to on spawn (is valid for unit persons)
			uint64 mSpawnpointEntityId;		///< The entity ID of the spawn point
			uint64 mUserData;				///< Optional user data to send in "em5::Messages::EM5_SPAWN_UNIT", "qsf::getUninitialized<uint64>()" if there's no user data

			UnitSpawnedData() :
				mSpawnedUnitEntityId(qsf::getUninitialized<uint64>()),
				mPlayerIndex(qsf::getUninitialized<uint32>()),
				mOrderInfoId(qsf::getUninitialized<uint32>()),
				mContainerEntityId(qsf::getUninitialized<uint64>()),
				mSpawnpointEntityId(qsf::getUninitialized<uint64>()),
				mUserData(qsf::getUninitialized<uint64>())
			{}

			UnitSpawnedData(uint64 spawnedUnitEntityId, uint32 playerIndex, uint32 orderInfoId, uint64 containerEntityId, uint64 spawnpointEntityId, uint64 userData) :
				mSpawnedUnitEntityId(spawnedUnitEntityId),
				mPlayerIndex(playerIndex),
				mOrderInfoId(orderInfoId),
				mContainerEntityId(containerEntityId),
				mSpawnpointEntityId(spawnpointEntityId),
				mUserData(userData)
			{}
		};

		struct UnitSpawnedPlayerUpdateSelectionData
		{
			uint32 mPlayerId;	///< The id of the owning player
			uint64 mEntityId;	///< The id of the spawned unit

			UnitSpawnedPlayerUpdateSelectionData() :
				mPlayerId(qsf::getUninitialized<uint32>()),
				mEntityId(qsf::getUninitialized<uint64>())
			{
				// Nothing to do in here
			}

			UnitSpawnedPlayerUpdateSelectionData(uint32 playerId, uint64 entityId) :
				mPlayerId(playerId),
				mEntityId(entityId)
			{
				// Nothing to do in here
			}
		};

		struct OrderUnitData
		{
			uint32						mPlayerIndex;		///< The player index which initiated the unit ordering on a client
			uint32						mOrderInfoId;		///< The order info id of the ordered unit
			glm::vec3					mTargetPosition;	///< The target position to which the ordered unit should go first
			OrderInfo::OrderPairList	mOrderPairList;		///< A list how many personnel the ordered unit should have

			OrderUnitData() :
				mPlayerIndex(qsf::getUninitialized<uint32>()),
				mOrderInfoId(qsf::getUninitialized<uint32>())
			{}

			OrderUnitData(uint32 playerIndex, uint32 orderInfoId, const glm::vec3& targetPosition, const OrderInfo::OrderPairList& orderPairList) :
				mPlayerIndex(playerIndex),
				mOrderInfoId(orderInfoId),
				mTargetPosition(targetPosition),
				mOrderPairList(orderPairList)
			{}
		};

		struct CommandExecutionData
		{
			uint32						mCommandId;				///< The player index which initiated the unit ordering on a client
			std::vector<uint64>			mCallerIds;				///< The entity id of the command caller
			uint64						mTargetId;				///< The entity id of the command target
			bool						mAllowSelfExecution;	///< Indicates if the command is executable on the caller itself
			boost::optional<glm::vec3>	mTargetPosition;		///< The target position where the command should be take place

			CommandExecutionData() :
				mCommandId(qsf::getUninitialized<uint32>()),
				mTargetId(qsf::getUninitialized<uint64>()),
				mAllowSelfExecution(false)
			{}

			CommandExecutionData(const uint32 commandId, const uint64 targetId, const bool allowSelfExecution, const boost::optional<glm::vec3>& targetPosition) :
				mCommandId(commandId),
				mTargetId(targetId),
				mAllowSelfExecution(allowSelfExecution),
				mTargetPosition(targetPosition)
			{}
		};

		struct CommandExecutionResult
		{
			bool						mSuccess;				///< Was the command successfully executed=
			uint32						mCommandId;				///< The player index which initiated the unit ordering on a client
			std::vector<uint64>			mCallerIds;				///< The entity id of the command caller
			uint64						mTargetId;				///< The entity id of the command target
			boost::optional<glm::vec3>	mTargetPosition;		///< The target position where the command should be take place

			CommandExecutionResult() :
				mSuccess(false),
				mCommandId(qsf::getUninitialized<uint32>()),
				mTargetId(qsf::getUninitialized<uint64>())
			{}

			CommandExecutionResult(bool success, uint32 commandId, const uint64 targetId, const boost::optional<glm::vec3>& targetPosition) :
				mSuccess(success),
				mCommandId(commandId),
				mTargetId(targetId),
				mTargetPosition(targetPosition)
			{}
		};

		struct FreeplayObjectiveData
		{
			bool		mRemoved;
			std::string	mText;
			uint8		mType;
			uint32		mTypeId;
			uint32		mCurrentNumber;
			uint32		mNeededNumber;
			uint32		mOrderPriority;

			FreeplayObjectiveData() :
				mRemoved(false),
				mType(0),
				mTypeId(0),
				mCurrentNumber(0),
				mNeededNumber(0),
				mOrderPriority(0)
			{}

			FreeplayObjectiveData(const std::string& text, uint32 typeId, uint8 type, uint8 dataType) :
				mRemoved(false),
				mText(text),
				mType(type),
				mTypeId(typeId),
				mCurrentNumber(0),
				mNeededNumber(0),
				mOrderPriority(0)
			{}
		};

		struct FreeplayEventData
		{
			bool			mIsNew;
			bool			mIsFinished;
			std::string		mName;
			uint32			mId;
			uint8			mState;
			bool			mDestroyed;
			uint64			mFocusEntityId;
			uint8			mPlayerIndex;
			bool			mHasEventLocationEntity;
			glm::vec3		mEventLocationPosition;
			// We need the items to be ordered by key
			boost::container::flat_map<uint32, FreeplayObjectiveData>	mObjectives;

			FreeplayEventData() :
				mIsNew(false),
				mIsFinished(false),
				mId(0),
				mState(0),
				mDestroyed(false),
				mFocusEntityId(qsf::getUninitialized<uint64>()),
				mPlayerIndex(qsf::getUninitialized<uint8>()),
				mHasEventLocationEntity(false)
			{}

			FreeplayEventData(const uint32 id, const std::string& name) :
				mIsNew(true),
				mIsFinished(false),
				mName(name),
				mId(id),
				mState(0),
				mDestroyed(false),
				mFocusEntityId(qsf::getUninitialized<uint64>()),
				mPlayerIndex(qsf::getUninitialized<uint8>()),
				mHasEventLocationEntity(false)
			{}
		};

		struct GameModeUpdateData
		{
			bool	mChanged;			// This is only internal
			bool	mFinished;
			uint16	mTimeLeftInSeconds; // This supports max of 18 hours

			GameModeUpdateData() :
				mChanged(false),
				mFinished(false),
				mTimeLeftInSeconds(0)
			{}
		};

		struct NewHintData
		{
			HintMessage::MessageType	mType;
			std::string					mMessage;
			uint32						mPlayerIndex;

			NewHintData() :
				mType(HintMessage::MESSAGETYPE_UNKNOWN),
				mPlayerIndex(qsf::getUninitialized<uint32>())
			{}

			NewHintData(HintMessage::MessageType type, const std::string& message, uint32 playerIndex) :
				mType(type),
				mMessage(message),
				mPlayerIndex(playerIndex)
			{}
		};

		struct MinimapMessageData
		{
			uint32	mMessageId;
			uint64	mTargetEntityId;
			uint64	mCallerId;

			MinimapMessageData() :
				mMessageId(qsf::getUninitialized<uint32>()),
				mTargetEntityId(qsf::getUninitialized<uint64>()),
				mCallerId(qsf::getUninitialized<uint64>())
			{}

			MinimapMessageData(const uint32 messageId, const uint64 targetEntityId) :
				mMessageId(messageId),
				mTargetEntityId(targetEntityId),
				mCallerId(qsf::getUninitialized<uint64>())
			{}

			MinimapMessageData(const uint32 messageId, const uint64 targetEntityId, const uint64 callerEntityId) :
				mMessageId(messageId),
				mTargetEntityId(targetEntityId),
				mCallerId(callerEntityId)
			{}
		};

		struct MinimapIconMessageData
		{
			enum MessageType
			{
				RemoveIcon,
				AddIcon,
				AddCircle,
				RemovePolygon
			};

			struct Parameters {};

			struct IconParameters : public Parameters
			{
				int8	mIconType;
				float	mIconScale;

				IconParameters() :
					mIconType(qsf::getUninitialized<int8>()),
					mIconScale(qsf::getUninitialized<float>())
				{}

				IconParameters(int8 iconType, float iconScale) :
					mIconType(iconType),
					mIconScale(iconScale)
				{}
			};

			struct CircleParameters : public Parameters
			{
				float		mRadius;
				uint8		mPoints;
				qsf::Color4	mColor;
				bool		mUpdatePosition;

				CircleParameters() :
					mRadius(qsf::getUninitialized<float>()),
					mPoints(qsf::getUninitialized<uint8>()),
					mUpdatePosition(false)
				{}

				CircleParameters(float radius, uint8 points, const qsf::Color4& color, bool updatePosition) :
					mRadius(radius),
					mPoints(points),
					mColor(color),
					mUpdatePosition(updatePosition)
				{}
			};

			MessageType	mMessageType;
			uint64	mEntityId;
			std::unique_ptr<Parameters> mParameters;

			MinimapIconMessageData() :
				mMessageType(RemoveIcon),
				mEntityId(qsf::getUninitialized<uint64>())
			{}

			MinimapIconMessageData(MessageType messageType, uint64 entityId, std::unique_ptr<Parameters>&& parameters) :
				mMessageType(messageType),
				mEntityId(entityId),
				mParameters(std::move(parameters))
			{}

			MinimapIconMessageData(const MinimapIconMessageData& other) :
				mMessageType(other.mMessageType),
				mEntityId(other.mEntityId)
			{
				switch (mMessageType)
				{
					case RemoveIcon:
					case RemovePolygon:
						break;

					case AddIcon:
						mParameters = std::make_unique<IconParameters>(*static_cast<const IconParameters*>(other.mParameters.get()));
						break;

					case AddCircle:
						mParameters = std::make_unique<CircleParameters>(*static_cast<const CircleParameters*>(other.mParameters.get()));
						break;
				}
			}

			static inline MinimapIconMessageData CreateRemoveIconMessageData(uint64 entityId)
			{
				return MinimapIconMessageData(RemoveIcon, entityId, nullptr);
			}

			static inline MinimapIconMessageData CreateAddIconMessageData(uint64 entityId, int8 iconType, float iconScale)
			{
				return MinimapIconMessageData(AddIcon, entityId, std::make_unique<IconParameters>(iconType, iconScale));
			}

			static inline MinimapIconMessageData CreateAddCirclePolygonMessageData(uint64 entityId, float radius, uint8 points, const qsf::Color4& color, bool updatePosition)
			{
				return MinimapIconMessageData(AddCircle, entityId, std::make_unique<CircleParameters>(radius, points, color, updatePosition));
			}

			static inline MinimapIconMessageData CreateRemovePolygonMessageData(uint64 entityId)
			{
				return MinimapIconMessageData(RemovePolygon, entityId, nullptr);
			}
		};

		struct AudioUpdateData
		{
			uint32		mId;
			bool		mIsStart;
			bool		mIsStop;
			uint16		mDelayTimeInMilliseconds; // This supports max of 65,5 seconds
			uint32		mLocalAssetId;
			uint64		mContextEntityId;
			std::string	mFileName;
			bool		mIsSpartial;
			bool		mIsLooping;
			bool		mIsDetached;
			uint8		mVolume; // 0-100%
			uint32		mChannel;

			AudioUpdateData() :
				mId(0),
				mIsStart(false),
				mIsStop(false),
				mDelayTimeInMilliseconds(0),
				mLocalAssetId(qsf::getUninitialized<uint32>()),
				mContextEntityId(qsf::getUninitialized<uint64>()),
				mIsSpartial(false),
				mIsLooping(false),
				mIsDetached(false),
				mVolume(100),
				mChannel(0)
			{}
		};

		struct LayerChangedData
		{
			uint32	mLayerId;
			bool	mActive;

			LayerChangedData() :
				mLayerId(qsf::getUninitialized<uint32>()),
				mActive(false)
			{}

			LayerChangedData(const uint32 layerId, bool active) :
				mLayerId(layerId),
				mActive(active)
			{}
		};

		struct ServerPingData
		{
			std::vector<uint16> mPing;	///< List of ping values per player (player id = index in the list)
			qsf::Time mServerTime;

			ServerPingData() :
				mServerTime(qsf::Time::ZERO)
			{}

			ServerPingData(const qsf::Time& serverTime) :
				mServerTime(serverTime)
			{}
		};

		struct ClientPingAcknowledgeData
		{
			qsf::Time mServerTime;

			ClientPingAcknowledgeData() :
				mServerTime(qsf::Time::ZERO)
			{}

			ClientPingAcknowledgeData(const qsf::Time& serverTime) :
				mServerTime(serverTime)
			{}
		};

		struct CreateFireHoseData
		{
			uint64		mConnectorEntityId;
			uint64		mNozzleEntityId;
			std::string	mConnectorBoneName;
			std::string	mNozzleBoneName;

			CreateFireHoseData() :
				mConnectorEntityId(qsf::getUninitialized<uint64>()),
				mNozzleEntityId(qsf::getUninitialized<uint64>())
			{}

			CreateFireHoseData(const uint64 connectorEntityId, const uint64 nozzleEntityId, const std::string& connectorBoneName, const std::string& nozzleBoneName) :
				mConnectorEntityId(connectorEntityId),
				mNozzleEntityId(nozzleEntityId),
				mConnectorBoneName(connectorBoneName),
				mNozzleBoneName(nozzleBoneName)
			{}
		};

		struct SpawnParticleData
		{
			uint32		mPrefabAssetId;
			float		mTimeout;
			glm::vec3	mPosition;
			glm::vec3	mScale;

			SpawnParticleData() :
				mPrefabAssetId(qsf::getUninitialized<uint32>()),
				mTimeout(0.0f),
				mScale(1.0f, 1.0f, 1.0f)
			{}

			SpawnParticleData(const uint32 prefabAssetId, const float timeout, const glm::vec3& position, const glm::vec3& scale) :
				mPrefabAssetId(prefabAssetId),
				mTimeout(timeout),
				mPosition(position),
				mScale(scale)
			{}
		};

		struct FadeEntityData
		{
			uint64		mEntityId;
			qsf::Time	mFadeTime;
			bool		mFadeIn;

			FadeEntityData() :
				mEntityId(qsf::getUninitialized<uint64>()),
				mFadeTime(qsf::Time::ZERO),
				mFadeIn(false)
			{}

			FadeEntityData(const uint64 entityId, const qsf::Time& fadeTime, const bool fadeIn) :
				mEntityId(entityId),
				mFadeTime(fadeTime),
				mFadeIn(fadeIn)
			{}
		};

		struct MinimapPingData
		{
			glm::vec3	mPingPosition;

			MinimapPingData()
			{}

			MinimapPingData(const glm::vec3& pingPosition) :
				mPingPosition(pingPosition)
			{}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
