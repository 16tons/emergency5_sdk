// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GameLogic;
	class GameLogicManager;
	class StringHash;
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
	*    Game logic component class
	*/
	class QSF_API_EXPORT GameLogicComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GameLogicManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::GameLogicComponent" unique component ID

		struct GameLogicConfig
		{
			virtual inline ~GameLogicConfig() {};

			std::string mLogicTypeId;
			// TODO(fw): Extend this, e.g. adding parameters

			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
		};
		typedef std::vector<GameLogicConfig> GameLogicConfigArray;	///< Shortcut typedef for an array of game logic IDs


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		GameLogicComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameLogicComponent();

		/**
		*  @brief
		*    Create a game logic instance of the given type
		*
		*  @return
		*    The game logic instance, or a null pointer if none could be created; do not destroy the instance
		*/
		template <typename T>
		T* createGameLogic();

		/**
		*  @brief
		*    Create a game logic instance of the given type
		*
		*  @param[in] logicTypeId
		*    The game logic type ID
		*
		*  @return
		*    The game logic instance, or a null pointer if none could be created; do not destroy the instance
		*/
		GameLogic* createGameLogicByTypeId(uint32 logicTypeId);

		/**
		*  @brief
		*    Get a game logic instance of the given type, if there is one
		*
		*  @return
		*    The game logic instance, or a null pointer if none was found; do not destroy the instance
		*/
		template <typename T>
		T* getGameLogic() const;

		/**
		*  @brief
		*    Get a game logic instance of the given type ID, if there is one
		*
		*  @param[in] logicTypeId
		*    The game logic type ID
		*
		*  @return
		*    The game logic instance, or a null pointer if none was found; do not destroy the instance
		*/
		GameLogic* getGameLogicByTypeId(uint32 logicTypeId) const;

		/**
		*  @brief
		*    Get a game logic instance of the given type, if there is none create one and return it
		*
		*  @return
		*    The game logic instance, or a null pointer if none was found; do not destroy the instance
		*/
		template <typename T>
		T* getOrCreateGameLogic();

		/**
		*  @brief
		*    Get a game logic instance of the given type safe, if there is none create one and return it
		*
		*  @return
		*    The game logic instance.
		*
		*  @note
		*    - This is only an ease of use method, avoid using it if you can and use "qsf::Component::creatLogic()" instead
		*    - In case the instance did not exist and could not be created, this method will throw an exception
		*/
		template <typename T>
		T& getOrCreateGameLogicSafe();

		/**
		*  @brief
		*    Get all game logic instance of the given type
		*
		*  @return
		*    The game logic instance.
		*
		*  @note
		*    - Internally a reinterpret_cast is used
		*/
		template <typename T>
		const std::vector<T*>& getAllGameLogics();

		/**
		*  @brief
		*    Get or create game logic by type ID
		*
		*  @return
		*    Returns null pointer if the game logic could not be created
		*/
		GameLogic* getOrCreateGameLogicByTypeId(uint32 logicTypeId);

		/**
		*  @brief
		*    Get all logics by type ID
		*/
		const std::vector<GameLogic*>& getAllGameLogics(uint32 logicTypeId);

		/**
		*  @brief
		*    Get all game logic instance. Don`t matter what type
		*/
		void getAllGameLogicsAllType(std::vector<GameLogic*>& result) const;

		void deleteGameLogic(GameLogic& gameLogic);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetDebug(bool debug) override;
		virtual void onSetActive(bool active) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		static void collectBaseClassTypeIds(uint32 logicTypeId, std::vector<uint32>& outBaseTypeIds);

		void registerGameLogic(uint32 logicUniqueId, GameLogic& logic);
		void unregisterGameLogic(uint32 logicUniqueId);

		GameLogicManager& getGameLogicManager() const;
		void createGameLogicInstances();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<GameLogic*> GameLogicList;
		typedef boost::container::flat_map<uint32, GameLogic*> GameLogicMap;		///< Maps unique IDs to game logic instances
		typedef boost::container::flat_map<uint32, GameLogicList> GameLogicTypeMap;	///< Maps game logic type IDs to sets of instances


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GameLogicMap mGameLogicsById;			///< Map of game logic unique IDs to game logic instances; do not destroy the instances, they are owned by the game logic manager
		GameLogicTypeMap mGameLogicsByTypeId;	///< Map of game logic type IDs to sets of all game logic instances registered under this ID; do not destroy the instances, they are owned by the game logic manager

		// Connected to CAMP
		GameLogicConfigArray mGameLogicConfigArray;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	QSF_CAMP_ARRAY_PROPERTY(GameLogicArray, GameLogicComponent, GameLogicConfig)	// Adds public member variable "GameLogicArray" which is the middle part of the game logics <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/gamelogic/GameLogicComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::GameLogicComponent);
QSF_CAMP_TYPE_NONCOPYABLE(qsf::GameLogicComponent::GameLogicConfig);
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::GameLogicComponent::GameLogicArrayInterface)
