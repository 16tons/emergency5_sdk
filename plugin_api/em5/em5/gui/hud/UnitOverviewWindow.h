// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/game/units/Department.h"

#include <qsf/math/Color4.h>
#include <qsf/job/JobProxy.h>
#include <qsf/gui/GuiDocument.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class JobArguments;
}
namespace em5
{
	class Player;
}
namespace Rocket
{
	namespace Core
	{
		class Element;
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
	/**
	*  @brief
	*    Unit overview window
	*/
	class EM5_API_EXPORT UnitOverviewWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit UnitOverviewWindow(qsf::GuiContext& guiContext);
		virtual ~UnitOverviewWindow();

		/**
		*  @brief
		*    Get the ID/index of the player to show the units from; uninitialized means current local player
		*/
		uint32 getPlayerId() const;

		/**
		*  @brief
		*    Set the ID/index of the player to show the units from; uninitialized means current local player
		*/
		void setPlayerId(uint32 playerId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct UnitEntry
		{
			uint64				   entityId;
			Rocket::Core::Element* unitListEntryElement;
			qsf::Color4			   borderColor;
			qsf::Color4			   actionPriorityColor;
			std::string			   localizedEquipmentId;
			UnitEntry(uint64 _entityId, Rocket::Core::Element& _unitListEntryElement, const qsf::Color4& _actionPriorityColor, const std::string& _localizedEquipmentId) :
				entityId(_entityId),
				unitListEntryElement(&_unitListEntryElement),
				borderColor(_actionPriorityColor),
				actionPriorityColor(_actionPriorityColor),
				localizedEquipmentId(_localizedEquipmentId)
			{ }
		};
		typedef std::vector<uint64>    EntityIds;
		typedef std::vector<UnitEntry> UnitEntries;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();
		Player* getPlayer() const;
		void selectUnitsDepartment(units::Department unitsDepartment);
		void updateRelevantEntityIds();
		void fillUnitsList();
		void addUnitEntry(uint64 entityId);
		void update(const qsf::JobArguments& jobArguments);
		void setMouseoverHighlighting(uint64 entityId);
		void setMouseoverHighlightingInternal(uint64 entityId, bool enabled);
		void setUnitEntryTooltip(const UnitEntry& unitEntry);
		bool isEntityPerson(uint64 entityId) const;
		const std::string& getLocalizedEquipmentIdByEntity(qsf::Entity& entity) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32					mPlayerId;			///< The ID/index of the player to show the units from; uninitialized means current local player
		units::Department		mUnitsDepartment;
		EntityIds				mRelevantEntityIds;	///< Entity IDs relevant for visualization, might contain entities not registered inside the GUI, yet
		UnitEntries				mUnitEntries;		///< Units (vehicles and persons, unsorted) currently shown inside the GUI
		qsf::JobProxy			mUpdateJobProxy;
		uint64					mMouseoverHighlightedEntityId;
		Rocket::Core::Element*	mVehicleListElement;
		Rocket::Core::Element*	mPersonListElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
