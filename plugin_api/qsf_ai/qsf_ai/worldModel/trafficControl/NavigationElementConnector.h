// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/reflection/ExportedProperty.h"
#include "qsf_ai/worldModel/WorldElementState.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/time/Time.h>

namespace qsf
{
	namespace ai
	{
		/**
		* Abstract super class for components that are connected to elements of a navigation map and allow to change their state.
		* Holds the data to create the connection but doesn't yet say how it is used.
		* This is done by the specializations.
		*
		* There are actually two ids that identify the connected element.
		* One is the edited tag by the user that is used during the initialization to find the connected elements in the raw street data.
		* The other is the final id that is unique for the created AI world and is set during the conversion to the AI data.
		* This might be either a node or an area of the world.
		* The latter is not available in the editor view because you don't know the values normally and it doesn't make sense to edit them by hand.
		*/
		class QSF_AI_API_EXPORT NavigationElementConnector : public Component
		{
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const ExportedProperty<unsigned int> PROPERTY_MAP;
			static const ExportedProperty<unsigned int> PROPERTY_ELEMENT_TAG;
			static const ExportedProperty<unsigned int> PROPERTY_ELEMENT_AI_ID;
			static const ExportedProperty<bool> PROPERTY_MANAGES_NODE;
			static const ExportedProperty<Time> PROPERTY_APPROACH_TIME;
			static const ExportedProperty<bool> PROPERTY_TRACK_APPROACHES;
			//@}

			NavigationElementConnector(Prototype* prototyp);

			// overridden component interface
			//@{
			virtual void onShutdown() override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Access to the world id property
			//@{
			unsigned int getWorldId() const;
			void setWorldId(unsigned int world);
			//@}

			// Access to both IDs for the managed element.
			//@{
			unsigned int getManagedElementTag() const;
			void setManagedElementTag(unsigned int tag);
			unsigned int getManagedElementAiId() const;
			void setManagedElementAiId(unsigned int element);
			//@}

			void setManagesNode(bool enable);
			bool managesNode() const;
			bool managesArea() const;

			bool tracksApproaches() const;
			void setTrackApproaches(bool enable);

			Time getTimeOfLastRegisteredApproach() const;

			// Register an approaching entity with its estimated time of arrival
			void registerApproach(Time estimatedArrival);

			// Direct setter for the managed world element state.
			virtual void setManagedState(const worldElement::State& state);

		private:
			unsigned int mWorldId;
			unsigned int mManagedElementTag; // The temporary id used inside the editor to refer to a node or lane in the same street crossing.
			unsigned int mManagedElementAiId; // The final id of the node or lane inside the created AI data.
			bool mManagesNode; // A traffic light might either manage a node or a lane. True means managing a node, false means managing a lane.
			bool mTracksApproaches; // A flag whether this connector should track approaches. This is needed when a DynamicNavigationBlocker should be attached

			// Entities approaching this connector signal their expected time of arrival at the element.
			// This value just tracks the last one that approached.
			// More sophisticated logic that tries to find the nearest one or such calculations have to be done in client components listening to the changes
			Time mLastRegisteredApproachTime;

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline NavigationElementConnector::NavigationElementConnector(Prototype* prototype) :
		Component(prototype),
		mWorldId(PROPERTY_MAP.mDefault),
		mManagedElementTag(PROPERTY_ELEMENT_TAG.mDefault),
		mManagedElementAiId(PROPERTY_ELEMENT_AI_ID.mDefault),
		mManagesNode(PROPERTY_MANAGES_NODE.mDefault),
		mTracksApproaches(PROPERTY_TRACK_APPROACHES.mDefault),
		mLastRegisteredApproachTime(PROPERTY_APPROACH_TIME.mDefault)
		{}

		inline unsigned int NavigationElementConnector::getWorldId() const
		{
			return mWorldId;
		}

		inline void NavigationElementConnector::setWorldId(unsigned int world)
		{
			assignAndPromoteChange(mWorldId, world, PROPERTY_MAP.mId);
		}

		inline unsigned int NavigationElementConnector::getManagedElementTag() const
		{
			return mManagedElementTag;
		}

		inline void NavigationElementConnector::setManagedElementTag(unsigned int managedNodeTag)
		{
			assignAndPromoteChange(mManagedElementTag, managedNodeTag, PROPERTY_ELEMENT_TAG.mId);
		}

		inline void NavigationElementConnector::setManagesNode(bool enable)
		{
			assignAndPromoteChange(mManagesNode, enable, PROPERTY_MANAGES_NODE.mId);
		}

		inline bool NavigationElementConnector::managesNode() const
		{
			return mManagesNode;
		}

		inline bool NavigationElementConnector::managesArea() const
		{
			return !mManagesNode;
		}

		inline void NavigationElementConnector::setManagedElementAiId(unsigned int element)
		{
			assignAndPromoteChange(mManagedElementAiId, element, PROPERTY_ELEMENT_AI_ID.mId);
		}

		inline unsigned int NavigationElementConnector::getManagedElementAiId() const
		{
			return mManagedElementAiId;
		}

		inline void NavigationElementConnector::registerApproach(Time estimatedArrival)
		{
			assignAndPromoteChange(mLastRegisteredApproachTime, estimatedArrival, PROPERTY_APPROACH_TIME.mId);
		}

		inline Time NavigationElementConnector::getTimeOfLastRegisteredApproach() const
		{
			return mLastRegisteredApproachTime;
		}

		inline bool NavigationElementConnector::tracksApproaches() const
		{
			return mTracksApproaches;
		}

		inline void NavigationElementConnector::setTrackApproaches(bool enable)
		{
			assignAndPromoteChange(mTracksApproaches, enable, PROPERTY_TRACK_APPROACHES.mId);
		}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::NavigationElementConnector);
