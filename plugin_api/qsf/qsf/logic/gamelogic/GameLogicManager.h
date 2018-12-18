// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/BufferedManager.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"
#include "qsf/job/JobProxy.h"

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class StringHash;
	class GameLogic;
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
	*    Game logic manager implementation
	*
	*  @remarks
	*    Manages the pool of currently active game logic instances.
	*    It contains all active game logics of a whole map and is used inside a game logic system (core component).
	*/
	class QSF_API_EXPORT GameLogicManager : public BufferedManager<uint32, GameLogic*, std::unordered_map<uint32, GameLogic*>>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GameLogicManager(Map* map);

		/**
		*  @brief
		*    Destructor
		*/
		~GameLogicManager();

		/**
		*  @brief
		*    Return the manager's associated map or a null pointer if there is none
		*/
		inline Map* getMap() const;

		/**
		*  @brief
		*    Get a game logic by its unique ID, do not destroy the returned instance
		*/
		GameLogic* getLogicByUniqueId(uint32 logicUniqueId);

		/**
		*  @brief
		*    Insert a new game logic
		*/
		template <typename TYPE>
		TYPE& insertLogic();

		/**
		*  @brief
		*    Insert a new game logic, do not destroy the returned instance
		*/
		GameLogic* insertLogicByTypeId(const StringHash& logicTypeId, uint64 entityId);

		/**
		*  @brief
		*    Remove and destroy the given game logic
		*
		*  @param[in] logic
		*    The game logic instance to remove
		*/
		void removeLogic(GameLogic& logic);

		/**
		*  @brief
		*    Remove and destroy the given game logic
		*
		*  @param[in] logicUniqueId
		*    Unique ID of the game logic instance to remove
		*/
		void removeLogicById(uint32 logicUniqueId);

		/**
		*  @brief
		*    Remove and destroy all registered game logics
		*/
		void removeAllLogics();

		/**
		*  @brief
		*    Serialize or deserialize the managed game logics using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BufferedManager methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onElementInsertion(const ElementKey& key, Element& element) override;
		virtual void onElementRemoval(const ElementKey& key, Element& element) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		GameLogic* createLogicByTypeId(uint32 logicTypeId);
		void insertNewLogic(GameLogic& logic);
		void insertNewLogicWithId(GameLogic& logic, uint32 uniqueId);
		void performGarbageCollection();

		void onSimulationEnd(const JobArguments& jobArgs);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef ConsecutiveIdGenerator<uint32> IdGenerator;
		typedef std::vector<GameLogic*> GameLogicList;

		Map*		  mMap;			///< Associated map; may be a null pointer, do not destroy
		IdGenerator   mIdGenerator;
		GameLogicList mDestroyedGameComponentList;	///< List of all destroyed game logics, will get deleted at the next "safe" point in time

		// Job update
		JobProxy	  mSimulationEndJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/gamelogic/GameLogicManager-inl.h"
