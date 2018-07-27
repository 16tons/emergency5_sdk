// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/WorldModel.h"
#include "qsf_ai/worldModel/WorldElementStateCollection.h"
#include "qsf_ai/worldModel/InterMapConnections.h"
#include "qsf_ai/worldModel/trafficLanes/LaneCollection.h"
#include "qsf_ai/worldModel/trafficLanes/LaneEndNode.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorldCreationSettings.h"
#include "qsf_ai/base/DebugSetting.h"
#include "qsf_ai/voronoi/DynamicVoronoiGraph.h"

#include <vector>
#include <memory>
#include <deque>


namespace qsf
{
	class FastDebugDraw;

	namespace ai
	{
		namespace voronoi
		{
			class TrafficLaneConverter;
		}
		/**
		* @brief Describes a world abstraction made up of connected traffic lanes.
		* Traffic lane worlds may be edited manually or created automatically by the Voronoi graph creation process.
		* This is signaled by the automatic creation parameters being present or absent.
		* Non copyable like all world models by inheritance.
		*/
		class QSF_AI_API_EXPORT TrafficLaneWorld : public WorldModel
		{
		public:
			// The default constructor is needed to be able to use the class for binary serialization
			explicit TrafficLaneWorld(unsigned int id = getUninitialized<unsigned int>());

			// World model public interface implementation
			//@{
			virtual std::auto_ptr<PathSearch> createPathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> searchConfig) override;
			virtual std::auto_ptr<NavigationTask> createMapUpdateTask(const NavigationMapUpdateConfiguration& config) const override;
			virtual boost::optional<unsigned int> tryGetAreaType(unsigned int) const override;
			virtual WorldElementStateCollection* tryGetWorldElementsState() override;
			virtual const WorldElementStateCollection* tryGetWorldElementsState() const override;
			virtual void createDynamicDebugOutput(const NavigationDebugGroup& debugOptions, const WorldModel* connectedWorld) const override;
			virtual void createStaticDebugOutput(const NavigationDebugGroup& debugOptions, const WorldModel* connectedWorld, std::vector<unsigned int>& debugDrawingIds) const override;
			virtual void calculateInterMapConnections(WorldModel& otherWorld) override;
			virtual void updateInterMapConnections(WorldModel& otherWorld, const std::vector<unsigned int>& removed, std::deque<CombinedSearchConnectionPoint>& added) override;
			virtual boost::optional<unsigned int> tryOptimizeForTurningConstraint(const PathSearchConfiguration& searchConfig) override;
			virtual WorldModel::IDRange getIntersectingAreaIds(unsigned int areaId) const override;
			virtual void writeClosestLegalPoints(const logic::TargetPoint& point, const boost::optional<unsigned int>& moverType, std::vector<MapContactPoint>& closestLegalPoints) const override;
			virtual void updateNodeStates(boost::function<bool(const glm::vec3&)>blockNodes) override;
			virtual void resetNodeStates() override;
			virtual void serializeStaticData(BinarySerializer& serializer) override;
			virtual void serializeDynamicData(BinarySerializer& serializer) override;
			virtual void onLoaded(std::vector<NavigationTask*>& taskList) override;
			//@}

			// Returns the total number of nodes defined by the lanes.
			std::size_t getNumNodes() const;

			const LaneCollection& getLanes() const;
			const LaneEndNode& getLaneEndNode(unsigned int id) const;

			// typedef for the return value of all node to lane collections
			typedef std::pair<std::multimap<unsigned int, unsigned int>::const_iterator, std::multimap<unsigned int, unsigned int>::const_iterator> IDRange;

			// Get the ids of all lanes that are connected from the node with the given id to the vector reference parameter
			IDRange getAllConnectionsForNode(unsigned int index) const;
			// Returns all lane id that connects nodes A and B regardless of direction.
			void getAllConnectingLaneIds(unsigned int nodeAIndex, unsigned int nodeBIndex, std::vector<unsigned int>& connectingLaneIds) const;
			// Get the two connecting endNodes of a lane
			void getConnectionsForLane(unsigned int laneIndex, std::vector<unsigned int> &connectedLaneEndNode) const;
			// Insert another connection between lanes and nodes
			void insertNodeToLaneConnection(unsigned int nodeIndex, unsigned int laneIndex);

			// Besides providing the optional override for the general interface we also provide a reference access to be used internally for safer access.
			const WorldElementStateCollection& getWorldElementsState() const;
			WorldElementStateCollection& getWorldElementsState();

			/**
			* Construction helper functions.
			* Returns the unique id for the created element if applicable.
			* These can only be called before onConstructionFinished has been called or during a dynamic adaptation.
			* May throw an exception if requested element id is out of range
			*/
			//@{
			void prepareCoreDataStructures(std::size_t numEndNodes, std::size_t numTotalNodes, std::size_t numLanes, std::size_t numLaneConnections);
			void addOrReplaceLane(const std::vector<LaneNode3D>& positions, unsigned int laneId, unsigned int startNodeId, unsigned int endNodeId, unsigned int type); // position vector parameter may be changed because duplicate positions are filtered
			void addOrReplaceEndNode(const glm::vec3& pos, unsigned int uniqueId, const std::vector<unsigned int>& connectedLanes, bool checkLanes);
			void addOrReplaceEndNode(const glm::vec3& pos, unsigned int uniqueId, const std::vector<unsigned int>& connectedLanes);
			// A node can't be completely removed due to the structure. Instead its state is erased by destroying all connections and setting its position to vec3::ZERO
			void eraseNodeState(unsigned int nodeId);
			void eraseLane(unsigned int laneId);
			/**
			* Calculate and set the intersections between the lanes that make up this world.
			* By design this is only necessary currently for converted street networks because these typically contain crossing lanes like crosswalks and the like.
			* Automatically created worlds via the voronoi algorithm can't create such situations at all.
			*/
			void calculateIntersectingLanes();

			void setLaneChangeOptions(const LaneCollection::LaneChangeOptionContainer& options);
			//@}

			/**
			* Finalize the navigation map construction.
			* Needs to be called exactly once for each map before it is used.
			* This called is integrated into the loader so you don't need to bother with it.
			* It currently sets up the element state data structure.
			*/
			void onConstructionFinished();

			// Is called when we dynamically adapt the map at runtime t get the map back into structure being changed mode
			void requestMapAdaptation();
			// Needs to be called when we are done changing the map again
			void onAdaptationFinished();

			// Access to the settings if this world was automatically created.
			//@{
			bool wasAutomaticallyCreated() const;
			const TrafficLaneWorldCreationSettings& getAutomaticCreationSettings() const; // throws an exception if the world was not automatically created
			void setAutomaticCreationSettings(const TrafficLaneWorldCreationSettings& settings);
			//@}

			// Access to an optional dynamic voronoi graph
			//@{
			bool hasDynamicVoronoiGraph() const;
			void setDynamicVoronoiGraph(std::auto_ptr<voronoi::DynamicGraph> voronoi);
			voronoi::DynamicGraph& getDynamicVoronoiGraph() const; // throws an exception if there is no managed dynamic voronoi
			//@}

			// Looks among the optimized configurations whether the turning constraint passed as a parameter and returns the id assigned in case it was found.
			// Returns an uninitialized value in case the constraint was not optimized for.
			boost::optional<unsigned int> tryFindOptimizedTurningConstraintId(const TurningConstraint& constraint) const;

			void setUseFunnelSmoothing(bool enable);
			bool isUsingFunnelSmoothing() const;

			// Calculate the closest points data on the lane defined by freeSpace around the node array to the point.
			// This is an extracted part of the several closest point related algorithms to avoid code repetition.
			// The flag correctToIdealLane controls whether the position needs to be on the ideal middle line between two nodes
			// The allowedDistance means we may have a distance to the target
			bool calculateClosestPoints(const glm::vec3& point, unsigned int laneIndex, bool correctToIdealLane, UnsignedFloat allowedDistance,
				UnsignedFloat& bestDistanceFound, bool& bestSolutionsFoundAreInsideIdealLane, std::vector<MapContactPoint>& closePoints,
				bool calculationOnPlane, float distanceFromCollision = 0.0f) const;

			bool skipCalculateClosestPoints(const glm::vec3& point, unsigned int laneIndex, UnsignedFloat allowedDistance,
				UnsignedFloat bestDistanceFound, bool calculationOnPlane) const;

			// Draw all lanes of this navigation map with fastDebugDraw
			void debugDraw(FastDebugDraw& fastDebugDraw, bool showArea = false) const;

		private:
			// Helper functions only necessary for sanity checks during construction
			//@{
			const LaneEndNode* tryGetLaneEndNode(unsigned int id) const;
			const Lane* tryGetLane(unsigned int id) const;
			bool isConnectedTo(unsigned int nodeId, unsigned int laneId) const; // Throws an exception if the node data has not been entered
			//@}

			/** Graphical debugging helper functions.
			* Those that create permanent drawings that need to be canceled manually return the debug draw id that needs to be canceled.
			*/
			//@{
			unsigned int drawWorldElementsArea() const;
			void drawNodeState() const;
			unsigned int drawMapPaths() const;	// Deprecated debug draw function
			void drawCombinedSearchConnection(const CombinedSearchConnectionPoint& connection, bool useStartData, CompoundDebugDrawRequest& request, const Color4& color) const;
			//@}

			// Writes an estimate of the rough memory consumption to the log file
			void logMemoryConsumption() const;

			// Replace the connected lanes for the node with the given id by the entries in the optional vector parameter.
			// A nullptr is treated like an empty vector parameter and allows to erase the connections
			void replaceConnectedLanes(unsigned int nodeId, const std::vector<unsigned int>* newConnections);

			// Add the connection if it is missing
			void connectIfNecessary(unsigned int nodeId, unsigned int laneId);


			// Flag toggling between the two states under construction and in use.
			// Adding new elements is only legal during the first period while creating a path search only legal during the latter stage.
			bool mConstructionFinished;

			// The end points of the lanes are called nodes
			std::vector<LaneEndNode> mLaneNodes;

			// The lanes defining this world.
			LaneCollection mLanes;

			// Lanes have nodes with a unique id at both their ends.
			// These may be connected via lanes to other nodes.
			std::multimap<unsigned int, unsigned int> mNodeToLaneConnections;

			// The state of the lanes and nodes with regard to the common properties like being blocked.
			// This is again part of the object state definition that is separated from the final entities to save memory
			WorldElementStateCollection mWorldElementsState;

			typedef std::vector<TurningConstraint> EntityConfigurations;
			EntityConfigurations mOptimizedEntityConfigurations; // Contains the configurations that are already calculated and stored with the single lanes

			// Store the creation settings for worlds that were created automatically.
			// This is not initialized for manually edited worlds.
			std::auto_ptr<TrafficLaneWorldCreationSettings> mAutomaticCreationSettings;

			// An optional dynamic voronoi graph for maps that support dynamic lane updates
			std::auto_ptr<voronoi::DynamicGraph> mDynamicVoronoiGraph;

			// Flag which kind of path smoothing is applied when doing pathfinding on this world. True means dynamic funnel smoothing, false ideal lane curve following.
			bool mUseFunnelPathSmoothing;
		};
	}
}

#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorld-inl.h"
