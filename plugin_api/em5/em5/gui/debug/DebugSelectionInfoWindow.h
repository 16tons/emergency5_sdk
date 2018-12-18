// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class ElementText;
	}
}
namespace qsf
{
	class Entity;
	class ContainerLink;
	struct LinkAnchorId;
	namespace game
	{
		class CommandManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class DebugSelectionInfoWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_set<uint64> EntityIdSet;
		typedef boost::container::flat_map<qsf::LinkAnchorId, qsf::ContainerLink*> ContainerLinkMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit DebugSelectionInfoWindow(qsf::GuiContext& guiContext);
		~DebugSelectionInfoWindow();

		void setEntityIds(const EntityIdSet& entityIds);

		void updateDebugText(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::string buildDebugText(const EntityIdSet& entitySet) const;

		void addEntityNameToStringStream(const qsf::Entity& entity, std::stringstream& stringStream) const;
		void addEntityTransformToStringStream(const qsf::Entity& entity, std::stringstream& stringStream) const;

		void addEntityTypeAndPropertiesToStringStream(qsf::Entity& entity, std::stringstream& stringStream) const;
		void addGangsterDetailsToStringStream(qsf::Entity& entity, std::stringstream& stringStream) const;

		void addEventTagToStringStream(qsf::Entity& entity, std::stringstream& stringStream) const;

		void addContainedEntitiesToStringStream(qsf::Entity& entity, std::stringstream& stringStream) const;

		void addActionPlanToStringStream(const qsf::Entity& entity, std::stringstream& stringStream) const;

		void addLogicToStringStream(const qsf::Entity& entity, std::stringstream& stringStream) const;

		void addEntityCommandsToStringStream(const qsf::Entity& entity, std::stringstream& stringStream) const;
		void addCommandNameToStringStream(uint32 commandId, std::stringstream& stringStream, const qsf::game::CommandManager& commandManager) const;

		void addEntityComponentsToStringStream(qsf::Entity& entity, std::stringstream& stringStream) const;

		void showText(const std::string& debugText);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EntityIdSet				mEntityIds;		///< Current set of entity Ids
		qsf::JobProxy			mDebugJobProxy;	///< Regular simulation job proxy; for updates once a second

		Rocket::Core::Element*	mMessageText;	///< Rocket element where to show the text


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
