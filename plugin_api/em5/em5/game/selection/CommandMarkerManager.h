// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/effect/CommandTargetComponent.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/NamedIdentifier.h>
#include <qsf/base/manager/ElementManager.h>
#include <qsf/base/manager/PagedAllocator.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Command;
}
namespace qsf
{
	class Map;
	class Entity;
	class CameraComponent;
	class Clock;
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
	*    EMERGENCY 5 command target marker manager class that cares for highlighting of possible targets
	*/
	class EM5_API_EXPORT CommandMarkerManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CommandMarkerManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CommandMarkerManager();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_set<uint64> VisibleEntityIdSet;

		struct TargetEntityData
		{
			Command* mCommand;
			qsf::WeakPtr<qsf::Entity> mEntity;
			qsf::WeakPtr<CommandTargetComponent> mCommandTargetComponent;
			qsf::Time mLastCheckTime;
			bool mValid;	// Only used temporarily

			TargetEntityData(uint64 key = 0) : mCommand(nullptr), mValid(false) {}
		};

		// Use an element manager with paged allocator here:
		//  - We need a fast find by ID functionality -> some kind of std::map-like data structure
		//  - Copying elements is not too lightweight, so the elements should stay on stable location -> that's why no simple Boost flat_map is used
		//  - Elements get inserted and removed very often -> std::map or std::unordered_map would have to allocate/deallocate internal elements all the time
		typedef qsf::ElementManager<uint64, TargetEntityData, qsf::PagedAllocator<TargetEntityData>> TargetEntityMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onEntitySelected(uint64 entityId);
		void onEntityDeselected(uint64 entityId);

		void searchNewCommandTargetEntities(qsf::Map& map);
		void checkPossibleTargets(qsf::Map& map, const qsf::Clock& clock);

		// Show and delete highlights and icons for target entities
		void createHighlight(TargetEntityData& targetEntityData);
		void deleteHighlight(TargetEntityData& targetEntityData);

		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*/
		void onPreCompositorWorkspaceUpdateHighlight(const qsf::CameraComponent& cameraComponent);

		// Jobs
		void updateSearchTargetJob(const qsf::JobArguments& jobArguments);

		bool getCameraPosition(glm::vec3& positionOut);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::signals2::connection		mSelectSignalConnection;
		boost::signals2::connection		mDeselectSignalConnection;

		qsf::JobProxy					mUpdateSearchTargetsJobProxy;
		boost::signals2::connection		mPreCompositorWorkspaceUpdateHighlightConnection;

		// Management of target entities (for commands)
		TargetEntityMap					mTargetEntityMap;

		// TODO(tl): At the moment just used to disable highlight, remove it later maybe
		bool							mUseHighlights;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
