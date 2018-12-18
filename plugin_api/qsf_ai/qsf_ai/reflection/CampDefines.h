// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

// Some additional helper macros to ease recurring CAMP related definitions that are only relevant for the AI
#define QSF_DEFINE_CAMP_COMPONENT_ID(component) const unsigned int qsf::ai::component::COMPONENT_ID = StringHash("qsf::ai::" #component);
#define QSF_DEFINE_CAMP_PROPERTY_ID(component, prop) const unsigned int qsf::ai::component::prop = StringHash(#prop);

#define QSF_DEFINE_CAMP_EXPORTED_PROPERTY(type, prop, textName, defaultValue) const ExportedProperty<type> prop(textName, defaultValue);

// Define the necessary interface functionality for a std::vector exported as a public variable to CAMP
#define QSF_DEFINE_EXPORTED_VECTOR(Component, VariableTypeName) \
	inline std::size_t Component::VariableTypeName::size() const \
	{ \
		return getVariable().size(); \
	} \
	\
	inline const Component::VariableTypeName::ElementType& Component::VariableTypeName::get(std::size_t index) const \
	{ \
		return getVariable()[index]; \
	} \
	\
	inline void Component::VariableTypeName::set(std::size_t index, const Component::VariableTypeName::ElementType& value) \
	{ \
		getVariable()[index] = value; \
	} \
	\
	inline void Component::VariableTypeName::insert(std::size_t before, const Component::VariableTypeName::ElementType& value) \
	{ \
		getVariable().insert(getVariable().begin() + before, value); \
	} \
	\
	inline void Component::VariableTypeName::remove(std::size_t index) \
	{ \
		getVariable().erase(getVariable().begin() + index); \
	}

// Special version of the export macro optimized to the (currently experimental) approach of defining the variables to be exported
// inside an own little object (see ExportedProperty)
#define QSF_AI_ADD_CAMP_EXPORTED_PROPERTY(prop, getter, setter, description) \
	.property(prop.mName, &getter, &setter) \
		.tag("Description", description) \
		.tag("Default", prop.mDefault)

// Add a lower bound to an exported property
#define QSF_CAMP_ADD_LOWER_BOUND(lowerBound) .tag("RangeMinimum", lowerBound)
// Add an upper bound to an exported property
#define QSF_CAMP_ADD_UPPER_BOUND(upperBound) .tag("RangeMaximum", upperBound)
