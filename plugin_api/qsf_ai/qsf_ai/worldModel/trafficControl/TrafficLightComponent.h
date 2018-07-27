// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficControl/NavigationElementConnector.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/base/NamedIdentifier.h>

namespace qsf
{
	namespace ai
	{
		class WorldElementState;

		/**
		* An AI traffic light component manages the connection to the AI navigation map parts that are controlled by a traffic light.
		* The actual schedule is governed by an AI traffic light director component.
		* Traffic lights are synchronized in groups.
		* Each traffic light has a group id that synchronizes it with all other traffic lights with the same id by a traffic light timer.
		* This id is implemented as a StringHash so it is set and read as a string but used as an integer at runtime.
		*/
		class QSF_AI_API_EXPORT TrafficLightComponent : public NavigationElementConnector
		{
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const ExportedProperty<std::string> PROPERTY_GROUP;
			//@}

			explicit TrafficLightComponent(Prototype* prototype);

			// overwritten public component interface
			virtual void serialize(BinarySerializer& serializer) override;

			// Access to the traffic light group id
			//@{
			const std::string& getTrafficLightGroupName() const;
			void setTrafficLightGroupName(const std::string& name);
			unsigned int getTrafficLightGroupId() const; // A string hash of the group name
			//@}

			// Set classical traffic light modes indicated by their color
			//@{
			void setGreen();
			void setYellow();
			void setRed();
			void setRedAndYellow();
			//@}

		private:
			NamedIdentifier mTrafficLightGroup; // associating the plain text name with its hash used as id

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline TrafficLightComponent::TrafficLightComponent(Prototype* prototype) :
		NavigationElementConnector(prototype),
		mTrafficLightGroup(PROPERTY_GROUP.mDefault)
		{}

		inline const std::string& TrafficLightComponent::getTrafficLightGroupName() const
		{
			return mTrafficLightGroup.getName();
		}

		inline unsigned int TrafficLightComponent::getTrafficLightGroupId() const
		{
			return mTrafficLightGroup.getHash();
		}

		inline void TrafficLightComponent::setTrafficLightGroupName(const std::string& name)
		{
			if (mTrafficLightGroup.getName() == name)
				return;

			mTrafficLightGroup = NamedIdentifier(name);
			promotePropertyChange(PROPERTY_GROUP.mId);
		}

		inline void TrafficLightComponent::setRed()
		{
			setManagedState(worldElement::State::TEMPORARILY_BLOCKED_STATE);
		}

		inline void TrafficLightComponent::setYellow()
		{
			setManagedState(worldElement::State::SOON_BLOCKED_STATE);
		}

		inline void TrafficLightComponent::setGreen()
		{
			setManagedState(worldElement::State::FREE_STATE);
		}

		inline void TrafficLightComponent::setRedAndYellow()
		{
			setManagedState(worldElement::State::SOON_UNBLOCKED_STATE);
		}

		inline void TrafficLightComponent::serialize(BinarySerializer& serializer)
		{
			NavigationElementConnector::serialize(serializer);

			serializer & mTrafficLightGroup;
		}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::TrafficLightComponent);
