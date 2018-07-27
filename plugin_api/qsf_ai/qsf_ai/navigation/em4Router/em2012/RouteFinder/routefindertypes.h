// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		enum ERouterFlags
		{
			ERF_NONE					= 0,		// no special flags
			ERF_STATIC_OBJECTS			= 0x01,		// just look for static objects
			ERF_IGNORE_MOVING_OBJECTS	= 0x02,		// ignore moving objects
			ERF_IGNORE_WATER			= 0x08,
			ERF_IGNORE_CIVILIANS		= 0x10,		// civilians and animals
		//	ERF_ONLY_HELIS				= 0x20,		// nur Hubschrauber beachten	TODO(co) We don't need the helicopter handling in here
		//	ERF_HELI_LANDING			= 0x40,		// Hubschrauber landen			TODO(co) We don't need the helicopter handling in here
		//	ERF_HELI_SECONDTRY			= 0x80,		//								TODO(co) We don't need the helicopter handling in here
			ERF_IS_LF					= 0x100,	// Löschflugzeug
			ERF_ONLY_DIRECT_WAY			= 0x200,	// überprüft nur ob direkter Pfad möglich ist
			ERF_NO_BACKWARDS			= 0x1000,	// no backward moves allowed
		//	ERF_HELI_ON_GROUND			= 0x4000,	// find free position on ground and correctly handle helis		TODO(co) We don't need the helicopter handling in here
			ERF_NOCURVES				= 0x8000,	// only use straight moves
			ERF_IGNORE_CARRIER			= 0x10000,	// ignoriert, die Person, die einen gerade trägt
		//	ERF_INCLUDESTARTANDENDINSTREETPATH = 0x20000, // fügt die Start- und Zielposition mit in den Streetpath ein bei RouterStreetSearch		TODO(co) We don't need the street handling in here
		//	ERF_CALCSTREETPATHTIME		= 0x40000,	// RouterStreetSearch berechnet auch die geschätzte Zeitdauer für das Abschreiten des Pfades	TODO(co) We don't need the street handling in here
			ERF_DONT_IGNORE_OWNER		= 0x80000,	// Routefinder nimmt das übergebene Objekt selber auch als Hinderniss wahr
			ERF_IGNORE_FOLLOWERS		= 0x100000,	// Routefinder ignoriert die Personen, die vom Polizisten gerade weggeführt werden
			ERF_IGNORE_UNMOVED			= 0x200000,	// für Routefinder nicht unterstützt, FindCollision ignoriert Objekte, die sich seit TimeStamp nicht bewegt haben
			ERF_UNMOVED_NOSTATICFLAG	= 0x400000	// für Routefinder nicht unterstützt, FindCollision ignoriert Objekte, die sich seit TimeStamp nicht bewegt haben
		};
		typedef int ERouterFlagsSet;

		enum ECollisionType
		{
			ERFCT_NONE					= 0,		// keine Kollision
			ERFCT_MOVINGOBJECT			= 0x01,		// mit bewegtem Objekt
			ERFCT_STATICOBJECT			= 0x02,		// mit unbewegtem Objekt
			ERFCT_POTENTIALMOVINGOBJECT = 0x04,		// mit unbewegtem Objekt, das aber FindPath-Aktion hat
		};
		typedef int ECollisionTypeSet;

		enum ERouterTraceLineFlags
		{
			RTLF_NONE					= 0,		// no special flags
			RTLF_IGNORE_NONENEMY_UNITS	= 0x01,		// ignoriert Units, die keine Feinde des ignoreObject_ sind
		};
		typedef int ERouterTraceLineFlagsSet;


		// TODO(co) We don't need the street handling in here
		/*
		enum EStreetRouterSearchMode		// nur für streets, keine Auswirkung bei lokaler Suche
		{
			SRSM_SHORTEST_PATH,
			SRSM_FASTEST_PATH,
			SRSM_KICUSTOM_PATH,

			_SRSM_COUNT
		};
		*/


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
