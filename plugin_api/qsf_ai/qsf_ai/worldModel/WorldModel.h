// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/worldModel/InterMapConnections.h"
#include "qsf_ai/base/DebugSetting.h"
#include "qsf_ai/worldModel/dynamicUpdate/NavigationMapUpdateConfiguration.h"
#include "qsf_ai/worldModel/MapContactPoint.h"
#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <glm/fwd.hpp>

#include <boost/noncopyable.hpp>

#include <vector>
#include <deque>


namespace qsf
{
	class Color4;

	namespace ai
	{
		class WorldElementStateCollection;
		class NavigationDebugGroup;
		class PathSearchConfiguration;

		/** Defines an abstract description of the world to be used by AI systems like navigation or perception.
		* World models are big objects and therefore noncopyable.
		*/
		class QSF_AI_API_EXPORT WorldModel : public boost::noncopyable
		{
		public:
			// Define a collection of connections to other maps
			typedef std::vector<InterMapConnections> MapConnections;

			// Pass a unique id for this world model.
			WorldModel(unsigned int id);
			virtual ~WorldModel();

			const std::string& getName() const;
			void setName(const std::string& name);

			GlobalAssetId getGlobalAssetId() const;
			void setGlobalAssetId(GlobalAssetId assetId);

			// Create a suitable path search instance for this world model based on the parameters passed.
			// Takes ownership of the configuration object passed, the pointer is reset during the call
			virtual std::auto_ptr<PathSearch> createPathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> searchConfig) = 0;

			// Create a suitable map update mechanism that can be scheduled as a task.
			virtual std::auto_ptr<NavigationTask> createMapUpdateTask(const NavigationMapUpdateConfiguration& config) const = 0;

			/** Debug Functionality. Each world may decide which of the options of the navigation debug group is supported.
			* The connectedWorld pointer is optional and may be null in case no (legal) id was provided.
			* The difference between the static and the dynamic version is that the output needs to be canceled manually for the static results.
			* That is why the ids to cancel in this case should be returned in the additional reference parameter.
			*/
			//@{
			virtual void createDynamicDebugOutput(const NavigationDebugGroup& debugOptions, const WorldModel* connectedWorld) const = 0;
			virtual void createStaticDebugOutput(const NavigationDebugGroup& debugOptions, const WorldModel* connectedWorld, std::vector<unsigned int>& createdDebugDrawingIds) const = 0;
			//@}

			// Retrieve the unique id for this world model
			unsigned int getId() const;

			/** Returns the stored connections from this navigation map to the one with the requested id.
			* Silently returns a nullptr if there are no such connections registered.
			*/
			const InterMapConnections* tryGetInterMapConnections(unsigned int mapId) const;
			InterMapConnections* tryGetInterMapConnections(unsigned int mapId);

			// Return the whole set for easy iteration purposes
			const MapConnections& getAllInterMapConnections() const;
			// Adds an empty intersection collection to the connected maps library.
			// This will lead to the intersections being recalculated during startup.
			void prepareInterMapConnections(unsigned int mapId);
			// Silently overwrites existing connections as this may be desired after a recalculation
			void setInterMapConnections(const InterMapConnections& connections);
			// Does nothing if there are no connections stored to the indicated world
			void clearInterMapConnections(unsigned int mapId);
			// Writes the ids of all connected navigation maps to the reference parameter
			void writeAllConnectedMapIds(std::vector<unsigned int>& connectedMapIds) const;

			// Calculates the inter map connections to the other world passed as a parameter and adds it to the list of stored connections for both worlds
			virtual void calculateInterMapConnections(WorldModel& otherWorld) = 0;
			// Updates the inter map connections to the other world with the given changes done to map A
			virtual void updateInterMapConnections(WorldModel& otherWorld, const std::vector<unsigned int>& removed, std::deque<CombinedSearchConnectionPoint>& added) = 0;

			/** Allow the world to prepare the data structures to be used by entities with the turning constraint passed in the future.
			* Optionally returns an id to indicate the optimization setup created.
			* This is used as a parameter for the search configurations created when an entity with this constraint plans a path on this map.
			* Does nothing for worlds that don't support turning radius in the first place and these should return an uninitialized return value.
			* The movementOptions contains advanced possibilities for moving beyond simply moving forwards.
			* See NavigationComponent::mMovementOptions for details.
			*/
			virtual boost::optional<unsigned int> tryOptimizeForTurningConstraint(const PathSearchConfiguration& searchConfig) = 0;

			// Many world representations are created from nodes and atomic areas.
			// Such worlds should return a world elements state instance via this function to allow using the information during pathfinding and steering.
			// It is legal though to silently just return a null pointer here if a representation doesn't support these notions of single elements and nodes.
			virtual const WorldElementStateCollection* tryGetWorldElementsState() const = 0;
			virtual WorldElementStateCollection* tryGetWorldElementsState() = 0;
			// All navigation map formats that support single areas should return the type id of the area indicated by parameter here.
			virtual boost::optional<unsigned int> tryGetAreaType(unsigned int areaId) const = 0;

			// typedef for a range of IDs that is returned as iterators into a vector
			typedef std::pair<std::deque<unsigned int>::const_iterator, std::deque<unsigned int>::const_iterator> IDRange;
			// Returns an iterator range over area ids that may be considered to intersect with the area identified by the parameter.
			// Each world implementation may decide on the details of when two of its areas are intersecting exactly.
			// The core idea is to identify abstract area definitions which denote (partially) the same space.
			virtual IDRange getIntersectingAreaIds(unsigned int areaId) const = 0;

			// Writes the closest legal points to the specified target point
			// The optional user mover type allows to restrict the types of area to the ones that may be used by this move type for navigation purposes
			virtual void writeClosestLegalPoints(const logic::TargetPoint& point, const boost::optional<unsigned int>& moverType, std::vector<MapContactPoint>& closestLegalPoints) const = 0;

			// Updates all nodes and block them if blockNodes returns true at their position
			virtual void updateNodeStates(boost::function<bool(const glm::vec3&)>blockNodes) = 0;
			virtual void resetNodeStates() = 0;

			// The static data is written to custom binary AI formats, while the dynamic data is written to the save games
			// The static part is not empty and needs to be called by every specialization
			//@
			virtual void serializeStaticData(BinarySerializer& serializer);
			virtual void serializeDynamicData(BinarySerializer& serializer) = 0;
			//@}

			// Allow requesting threaded post processing tasks after the data has been loaded. Is called after a static first time load as well after being serialized into.
			// May create none or several tasks on the list passed. Transfers ownership, the caller is responsible for deleting the tasks on the outside
			virtual void onLoaded(std::vector<NavigationTask*>& taskList);
		protected:

			// I assume the number of connections to be so low that a linear search shouldn't be a problem
			MapConnections mMapConnections; // Stored connections to other worlds

		private:
			unsigned int mId; // Unique AI Map ID
			std::string mName; // plain text name used for identifying the model

			// Global Asset System Id is needed when we want to change the stored data.
			// This variable is by design not serialized itself as it is only valid information locally while it is currently created
			GlobalAssetId mGlobalAssetId;
		};
	}
}

#include "qsf_ai/worldModel/WorldModel-inl.h"
