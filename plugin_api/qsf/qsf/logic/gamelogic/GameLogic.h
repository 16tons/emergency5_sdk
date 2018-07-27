// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/BitFlagSet.h"
#include "qsf/reflection/CampClass.h"
#include "qsf/base/WeakPtr.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GameLogicManager;
	class Entity;
	class Map;
	class BinarySerializer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Game logic base class
	*/
	class QSF_API_EXPORT GameLogic : public WeakPtrTarget
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GameLogicManager;		// Manager needs to access the constructor and destructor
	friend class GameLogicComponent;	// Component needs to set the entity ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~GameLogic();

		/**
		*  @brief
		*    Get the game logic's unique ID
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Get the game logic's type ID
		*/
		inline uint32 getTypeId() const;

		/**
		*  @brief
		*    Get the owning entity's ID or zero if there is none
		*/
		inline uint64 getEntityId() const;

		/**
		*  @brief
		*    Get the owning entity or a null pointer if there is none
		*/
		Entity* getEntity() const;

		/**
		*  @brief
		*    Get the owning entity or a null pointer if there is none
		*/
		Entity& getEntitySafe() const;

		/**
		*  @brief
		*    Get the associated map of the game logic
		*/
		Map& getMap() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the game logic
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool startup();

		/**
		*  @brief
		*    Return whether or not the game logic is up-and-running
		*
		*  @return
		*    "true" if the game logic is up-and-running ("onStartup()" was called successfully), else "false"
		*/
		inline bool isRunning() const;

		/**
		*  @brief
		*    Shutdown the game logic
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ Debug flag                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the logic is considered to be in debug mode
		*
		*  @return
		*    "true" if the logic is considered to be in debug mode, else "false"
		*
		*  @note
		*    - The default implementation always returns "false"
		*    - Not connected to the CAMP reflection system by intent, this should be internal only
		*/
		inline bool isDebug() const;

		/**
		*  @brief
		*    Set whether or not the logic is considered to be in debug mode
		*
		*  @param[in] debugMode
		*    "true" if the logic is considered to be in debug mode, else "false"
		*
		*  @remarks
		*    The semantic of "debug mode" depends of the logic. An implementation is free to ignore the request or do nothing
		*    at all because a debug mode state makes no sense for the logic.
		*
		*  @note
		*    - The default implementation is empty
		*    - Not connected to the CAMP reflection system by intent, this should be internal only
		*/
		void setDebug(bool debugMode);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Startup the game logic
		*/
		inline virtual bool onStartup();

		/**
		*  @brief
		*    Shutdown the game logic
		*/
		inline virtual void onShutdown();

		/**
		*  @brief
		*    Serialize or deserialize the game logic using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		inline virtual void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::GameLogic methods              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Set whether or not the logic is considered to be in debug mode
		*
		*  @param[in] debug
		*    "true" if the logic is considered to be in debug mode, else "false"
		*
		*  @note
		*    - The default implementation is empty
		*    - This method is only called in case there's a real state change
		*
		*  @see
		*    - "qsf::Component::setDebug()"
		*/
		inline virtual void onSetDebug(bool debug);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void removeThis();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		inline GameLogic(uint32 typeId);

		/**
		*  @brief
		*    Registering call from manager
		*/
		inline void registrationAtManager(GameLogicManager& manager, uint32 id);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Game logic flags
		*/
		enum Flags
		{
			RUNNING = 1<<0, ///< This flag indicates that the game logic is up-and-running ("onStartup()" was called successfully, already set inside "onStartup()")
			DEBUG	= 1<<1	///< This flag indicates that the game logic is in debug mode
		};
		typedef BitFlagSet<uint8, Flags> FlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GameLogicManager*	mManager;		///< Pointer to game logic manager where this logic is registered; always valid, do not destroy
		uint32				mId;			///< Game logic unique ID of this instance at the game logic manager
		uint32				mTypeId;		///< Game logic type ID of this class
		uint64				mEntityId;		///< Owner entity ID, is zero in case of a global logic
		FlagSet				mFlags;			///< Internal flags, see "qsf::Component::Flags"


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::GameLogic)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/gamelogic/GameLogic-inl.h"
