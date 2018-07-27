// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Macro definitions                                     ]
//[-------------------------------------------------------]
#define QSF_DEFINE_SIMPLE_GETTER(name, type, variable) \
	inline type get##name() const		{ return variable; }

#define QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(name, type, variable) \
	inline type get##name() const		{ return variable; } \
	inline void set##name(type value)	{ variable = value; }

#define QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(name, type, variable, propertyId) \
	inline type get##name() const		{ return variable; } \
	inline void set##name(type value)	{ assignAndPromoteChange(variable, value, propertyId); }

#define QSF_DEFINE_FLAG_GETTER_AND_SETTER(name, variable, flag) \
	inline bool get##name() const		{ return variable.isSet(flag); } \
	inline void set##name(bool value)	{ variable.set(flag, value); }
