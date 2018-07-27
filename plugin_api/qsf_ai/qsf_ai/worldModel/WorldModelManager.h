// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/AreaType.h"
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/worldModel/dynamicUpdate/DynamicCollisionConverterComponent.h"
#include "qsf_ai/worldModel/ManagedNavigationMap.h"
#include "qsf_ai/worldModel/ManagedNavigationMapWriteAccess.h"
#include "qsf_ai/worldModel/ManagedNavigationMapReadAccess.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/WeakPtr.h>

#include <boost/container/flat_map.hpp>

#include <string>


namespace qsf
{
	class JobArguments;
	class ParameterGroup;

	namespace ai
	{
		class PathSearch;
		class WorldModel;
		class NavigationDebugGroup;
		class NavigationTaskThread;

		/** A simple manager for AI world structures.
		* These are identified by a unique ID.
		* It is not expected that this mapping changes dynamically at runtime normally.
		* It is intended to be used as a singleton component attached only to the dedicated core entity of a map.
		* It also manages the global map of area type definitions.
		*/
		class QSF_AI_API_EXPORT WorldModelManager : public StandardSystem<DynamicCollisionConverterComponent, WorldModelManager>
		{
		public:
			/**
			* Inner helper class for a world model manager that allows listening to changes on registered worlds.
			*/
			class QSF_AI_API_EXPORT AssetChangeListener
			{
			public:
				virtual ~AssetChangeListener();

				virtual void onAssetUpdated(GlobalAssetId assetId) = 0;
				virtual void onAssetDeleted(GlobalAssetId assetId) = 0;
			};


			// Unique CAMP Ids and default values for exported properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			//@}

			// Plain text identifier
			static const char* NAME;

			// TODO(vs) It would be cleaner to use a flat_set here but I have problems selecting a world by Id in this case without having a pointer available
			typedef boost::container::flat_map<unsigned int, ManagedNavigationMap*> Maps;
			typedef std::vector<AreaType> Areas;

			WorldModelManager(Prototype* prototype);
			~WorldModelManager();

			// overridden Component interface
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Required template interface of the StandardSystem
			//@{
			effort::Indicator updateComponent(DynamicCollisionConverterComponent& client, const JobArguments& arguments);
			void createDebugOutput(const DynamicCollisionConverterComponent& client) const;
			void updateGlobals(const JobArguments&);
			//@}

			// Takes ownership of the world model passed and the parameter is reset when it is copied at the call site
			void addWorld(std::auto_ptr<WorldModel> model);

			// check whether a nav map is there without requiring shared access
			bool isLegalWorldId(unsigned int id) const;

			// Individual world model access.
			// These accessors throw when trying to access an unknown model or overwrite an existing model.
			// They grant unique access and let other threads sleep until the unique ptr is reset.
			// The try variants still block in case the map exists and just can't be blocked now - they only silently return a nullptr in case the map isn't available
			//@{
			std::unique_ptr<ManagedNavigationMapReadAccess> acquireReadAccess(unsigned int mapId);
			std::unique_ptr<ManagedNavigationMapReadAccess> tryAcquireReadAccess(unsigned int mapId);
			std::unique_ptr<ManagedNavigationMapWriteAccess> acquireWriteAccess(unsigned int mapId);
			std::unique_ptr<ManagedNavigationMapWriteAccess> tryAcquireWriteAccess(unsigned int mapId);
			//@

			// Variant that lock two maps as an atomic operation.
			// Provide uninitialized unique ptrs as reference and you receive two initialized write locks back
			void acquireParallelWriteAccess(unsigned int mapAId, unsigned int mapBId, std::unique_ptr<ManagedNavigationMapWriteAccess>& lockMapA, std::unique_ptr<ManagedNavigationMapWriteAccess>& lockMapB);
			void acquireParallelReadAccess(unsigned int mapAId, unsigned int mapBId, std::unique_ptr<ManagedNavigationMapReadAccess>& lockMapA, std::unique_ptr<ManagedNavigationMapReadAccess>& lockMapB);

			// Returns the first world id that is not yet used
			unsigned int getNextFreeWorldId() const;

			// Returns all worlds
			const Maps& getWorlds() const;
			// Returns all known area types
			const Areas& getAreaTypes() const;

			// Return if the world with WorldId contains walkableLevel
			bool isWorldOnWalkablelevel(unsigned int worldId, unsigned int walkableLevel);

			// Removes a world
			void deleteWorld(unsigned int id);
			// Replace the representation for one world with a newer one
			void exchangeWorld(std::auto_ptr<WorldModel> model);
			// Swap two existing maps.
			void swapWorlds(unsigned int mapAId, unsigned int mapBId);

			// Connects the two navigation maps denoted by their id
			// The flag serialize to file is actually a HACK because we use it inside EM5 during the startup phase which should not change the delivered data
			void connectWorlds(unsigned int worldAId, unsigned int worldBId, bool serializeToFile = true);
			// Removes connection information between the two worlds if available. Silently does nothing if the worlds are not known to be connected
			void disconnectWorlds(unsigned int worldAId, unsigned int worldBId);

			// Allow setting a custom listener to changes to assets that are managed inside this manager.
			// Does not take control. The lifecycle of any instance passed here needs to be managed on the outside.
			void setAssetChangeListener(AssetChangeListener* listener);

			// Adding and removing dynamic collision to the map data.
			// This is normally done automatically via the update and doesn't need to be called manually, yet the functions are part of the public API because they need to be called from the component.
			//@{
			void registerCollisionOnAllMaps(const DynamicCollisionConverterComponent& collision);
			void unregisterCollisionFromAllMaps(const DynamicCollisionConverterComponent& collision);
			//@}

			// Tests whether there is free space for an entity. The collision component is optional and may be a nullptr. Otherwise, its values are used to test for instant collisions also
			// The mover type is also an optional constraint and tests whether the entity could continue to move from there if provided.
			bool isPositionFreeFor(unsigned int aiMapId, const Transform& transform, BulletCollisionComponent* collision,
				const boost::optional<unsigned int>& moverType = boost::optional<unsigned int>()) const;

		private:
			// startup sub methods
			void loadAllNavigationMaps();
			void loadAllAreaTypes();
			void clearNavigationElementConnectors();
			void registerNavigationElementConnectors();

			// tear down sub methods
			void clearWorldData();
			void clearPersistentDebugDrawings();

			// The dynamic debug visualizations are redrawn every tick while the static parts are created once and explicitly canceled afterwards
			void updateDynamicDebugVisualizations();
			void updateStaticDebugVisualizations();

			// Listener for debug group changes. This will be used as argument to a boost bind
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			// This updates the inter map connections for this world as well as all the connections to other worlds.
			// Changed results are immediately written back to their file.
			// As a result many other worlds may change their state upon this call.
			// This is currently not needed anymore with the current workflow recalculating the connections at startup.
			// But I wanted to keep the method in case we change our minds again so I kept the method private
			void updateConnections(WorldModel& world);

			Maps mModels; // All loaded navigation maps

			NavigationDebugGroup* mDebugSettings; // pointer to the debug settings, may be null meaning no debugging output should be generated
			// The ids of persistent debug drawings that are currently alive and were created from this manager
			std::vector<unsigned int> mPersistentDebugDrawIds;

			Areas mAreaTypes; // All loaded area types

			AssetChangeListener* mAssetChangeListener; // optional listener to changes to the managed assets. May be a nullptr if not used

			WeakPtr<NavigationTaskThread> mTaskThread; // pointer to the manager for background navigation tasks

			std::vector<unsigned int> mCachedWalkableLevels; // Just to speed up checking for walkable levels

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::WorldModelManager);

#include "qsf_ai/worldModel/WorldModelManager-inl.h"
