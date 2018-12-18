// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace game
	{
		class Command;
	}
}
namespace em5
{
	class CommandContext;
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
	*    Static entity logic helper class
	*/
	class EntityLogicHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Check if the given command is able to execute for the caller
		*
		*  @note
		*    Use this inside the automatism to execute checked commandos
		*    @code
		*    EntityLogicHelper::checkAndExecuteCommand<PickupPersonByParamedicCommand>(getEntity(), target);
		*    @endcode
		*/
		template<typename T>
		static bool checkAndExecuteAutomatism(qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition = boost::optional<glm::vec3>());


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		EntityLogicHelper() {};
		~EntityLogicHelper() {};

		static bool internalCheckAndExecuteAutomatism(uint32 commandId, qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition);
		static void buildCommandContext(CommandContext& outCommandContext, qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition);


	};


	// TODO(fw): Move implementation to an -inl.h file
	template<typename T>
	bool EntityLogicHelper::checkAndExecuteAutomatism(qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition)
	{
		return internalCheckAndExecuteAutomatism(T::PLUGINABLE_ID, caller, target, hitPosition);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
