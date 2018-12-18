// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/classbuilder.hpp>


//[-------------------------------------------------------]
//[ Defines                                               ]
//[-------------------------------------------------------]

// Macros to streamline CAMP export

/*
 * @brief Macro to declare a unique class ID for a component exported to CAMP:
 */
#define QSF_DECLARE_CAMP_COMPONENT_ID static const uint32 COMPONENT_ID;

/*
 * @brief Macro to declare a unique class ID for a pluginable exported to CAMP:
 */
#define QSF_DECLARE_CAMP_PLUGINABLE_ID static const uint32 PLUGINABLE_ID;

/*
 *  @brief
 *    Start CAMP class export
 *
 *  @remarks
 *    You always need to issue this start macro, then additional macros (e.g. properties) and finally end the definition with QSF_END_CAMP_CLASS_EXPORT
 */
#define QSF_START_CAMP_CLASS_EXPORT(type, name, description) \
	addCampClass(camp::Class::declare<type>() \
		.tag("Name", name) \
		.tag("Description", description)

/*
 *  @brief
 *    Mark the class as an abstract component base class
 */
#define QSF_CAMP_IS_ABSTRACT_COMPONENT \
		.base<qsf::Component>()

/*
 *  @brief
 *    Mark the class as a component derived from "qsf::Component"
 */
#define QSF_CAMP_IS_COMPONENT \
		.base<qsf::Component>() \
		.constructor1<qsf::Prototype*>() \
		.constructor1<qsf::Entity*>()

/*
 *  @brief
 *    Mark the class as a component derived from the given base class
 */
#define QSF_CAMP_IS_COMPONENT_DERIVED(baseclass) \
		.base<baseclass>() \
		.constructor1<qsf::Prototype*>() \
		.constructor1<qsf::Entity*>()

/*
 *  @brief
 *    Mark the class as a core component
 */
#define QSF_CAMP_IS_CORE_COMPONENT \
		.base<qsf::Component>() \
		.constructor1<qsf::Prototype*>() \
		.constructor1<qsf::Entity*>() \
		.tag("CoreComponent", true)

/*
 *  @brief
 *    Mark the class as a core component derived from the given base class
 */
#define QSF_CAMP_IS_CORE_COMPONENT_DERIVED(baseclass) \
		.base<baseclass>() \
		.constructor1<qsf::Prototype*>() \
		.constructor1<qsf::Entity*>() \
		.tag("CoreComponent", true)

/*
 *  @brief
 *    Mark the class as a settings group
 */
#define QSF_CAMP_IS_SETTINGS_GROUP \
		.base<qsf::SettingsGroup>() \
		.constructor1<qsf::SettingsGroupManager*>()

/*
 *  @brief
 *    Mark the class as a debug group
 */
#define QSF_CAMP_IS_DEBUG_GROUP \
		.base<qsf::DebugGroup>() \
		.constructor1<qsf::DebugGroupManager*>()

/*
 *  @brief
 *    Mark the class as an action
 */
#define QSF_CAMP_IS_ACTION \
		.base<qsf::Action>() \
		.constructor0()

/*
 *  @brief
 *    Mark the class as a game logic
 */
#define QSF_CAMP_IS_GAME_LOGIC \
		.base<qsf::GameLogic>() \
		.constructor0()

/*
 *  @brief
 *    Mark the class as a game logic derived from the given base class
 */
#define QSF_CAMP_IS_GAME_LOGIC_DERIVED(baseclass) \
		.base<baseclass>() \
		.constructor0()

/*
 *  @brief
 *    Add a property with given getter and setter
 */
#define QSF_ADD_CAMP_PROPERTY(name, getter, setter, description, defaultValue) \
	.property(#name, &getter, &setter) \
		.tag("Description", description) \
		.tag("Default",	defaultValue)

/*
 *  @brief
 *    Add a property that represents a publicly accessible variable
 */
#define QSF_ADD_CAMP_PROPERTY_DIRECT_ACCESS(name, variable, description, defaultValue) \
	.property(#name, &variable) \
		.tag("Description", description) \
		.tag("Default",	defaultValue)

/*
 *  @brief
 *    Add a numeric property with given getter and setter, and a lower bound
 */
#define QSF_ADD_CAMP_PROPERTY_WITH_LOWER_BOUND(name, getter, setter, description, defaultValue, minimum) \
	.property(#name, &getter, &setter) \
		.tag("Description", description) \
		.tag("Default",	defaultValue) \
		.tag("RangeMinimum", minimum)

/*
 *  @brief
 *    Add a numeric property with given getter and setter, and an upper bound
 */
#define QSF_ADD_CAMP_PROPERTY_WITH_UPPER_BOUND(name, getter, setter, description, defaultValue, maximum) \
	.property(#name, &getter, &setter) \
		.tag("Description", description) \
		.tag("Default",	defaultValue) \
		.tag("RangeMaximum", maximum)

/*
 *  @brief
 *    Add a numeric property with given getter and setter, and given value bounds
 */
#define QSF_ADD_CAMP_PROPERTY_WITH_BOUNDS(name, getter, setter, description, defaultValue, minimum, maximum) \
	.property(#name, &getter, &setter) \
		.tag("Description", description) \
		.tag("Default",	defaultValue) \
		.tag("RangeMinimum", minimum) \
		.tag("RangeMaximum", maximum)

/*
 *  @brief
 *    Add an array property that represents a publicly accessible CAMP array interface variable
 */
#define QSF_ADD_CAMP_ARRAY_PROPERTY(name, variable, description) \
	.property(#name, &variable) \
		.tag("Description",	description)

/*
 *  @brief
 *    Tag the class or property as hidden
 *
 *  @remarks
 *    Hidden classes and properties won't show up in editor views for the end-user
 */
#define QSF_CAMP_HIDE \
	.tag("Hidden", true)

/*
 *  @brief
 *    Tag the class or property as not serializable
 *
 *  @remarks
 *    Classes and properties that are not serializable won't be saved and loaded, or sent to the server on change
 */
#define QSF_CAMP_NOT_SERIALIZABLE \
	.tag("Serializable", false)

/*
 *  @brief
 *    Tag the property as an asset, with the given asset type
 */
#define QSF_CAMP_ASSET(type) \
	.tag("AssetType", type)

/*
 *  @brief
 *    Tag the property as a slot, with the given slot link type
 */
#define QSF_CAMP_SLOT(type) \
	.tag("Slot", qsf::SlotHelper::type)

/*
 *  @brief
 *    End CAMP class export
 *
 *  @remarks
 *    This ends a definition started with QSF_START_CAMP_CLASS_EXPORT
 */
#define QSF_END_CAMP_CLASS_EXPORT \
		.getClass() \
	);

/*
 *  @brief
 *    Start CAMP enum export
 *
 *  @remarks
 *    You always need to issue this start macro for exporting an enum, then add enum value definitions with QSF_CAMP_ENUM_VALUE, and end with QSF_CAMP_END_ENUM_EXPORT
 */
#define QSF_CAMP_START_ENUM_EXPORT(type) \
	{ \
		typedef type CAMP_EXPORT_CURRENT_CLASS; \
		camp::Enum::declare<type>()

/*
 *  @brief
 *    Add a CAMP enum value for export
 */
#define QSF_CAMP_ENUM_VALUE(name) \
		.value(#name, static_cast<long>(CAMP_EXPORT_CURRENT_CLASS::name))

/*
 *  @brief
 *    End CAMP enum export
 *
 *  @remarks
 *    This ends a definition started with QSF_START_CAMP_ENUM_EXPORT
 */
#define QSF_CAMP_END_ENUM_EXPORT \
		; \
	}
