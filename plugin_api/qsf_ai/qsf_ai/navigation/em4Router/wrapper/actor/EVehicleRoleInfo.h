// Copyright (C) 2012-2018 Promotion Software GmbH


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


		//[-------------------------------------------------------]
		//[ General definitions                                   ]
		//[-------------------------------------------------------]
		enum EVehicleRole2
		{
			EVR2_NOTASK,
			EVR2_THW_FGRR_BKF,				//bergungskran
			EVR2_THW_FGRB_BLF,				//mobilebridge vehicle
			EVR2_THW_FGRI_EKW,				//engineer vehicle
			EVR2_FIREFIGHTERS_ASF,			//abschleppfahrzeug
			EVR2_FIREFIGHTERS_DEKONP,		//dekonterminationsfahrzeug
			EVR2_FIREFIGHTERS_DLK,			//drehleiterkran
			EVR2_FIREFIGHTERS_RW,			//r�stwagen
			EVR2_FIREFIGHTERS_TLF,			//tankl�schfahrzeug
			EVR2_FIREFIGHTERS_LF,			//l�schflugzeug
			EVR2_FIREFIGHTERS_FLB,			//firefighterboat
			EVR2_FIREFIGHTERS_LPF,			//l�schpanzer
			EVR2_FIREFIGHTERS_FMB,			//firefightermotorboat
			EVR2_FIREFIGHTERS_TFMB,			//motorboat transporter
			EVR2_POLICE_SW,					//troop carrier (r�umfahrzeug)
			EVR2_POLICE_MTW,				//manschaftstransport wagen (6pack)
			EVR2_POLICE_PHC,				//polizei helicopter
			EVR2_POLICE_STW,				//streifenwagen
			EVR2_POLICE_WAW,				//wasserwerfer
			EVR2_POLICE_GETAWAY,			//fluchtfahrzeug
			EVR2_AMBULANCE_ITW,				//ambunalce bus (has many passenger slots)
			EVR2_AMBULANCE_NEF,				//notarzt fahrzeug (mit doktor)
			EVR2_AMBULANCE_RHC,				//retungshelicopter
			EVR2_AMBULANCE_RHF,				//rettungshund fahrzeug
			EVR2_AMBULANCE_RTW,				//retungstransportwagen (mit sanit�terteam, transportiert verletzte ab)
			EVR2_TAXI,
			EVR2_BUS,
			EVR2_DRIVERCAR,
			EVR2_GANGSTER_GETAWAY,
			EVR2_TV_HELI,
			EVR2_FIREFIGHTERS_TLF_HISTORIC,	//tankl�schfahrzeug was 100 Jahre alt ist
			EVR2_THW_FGRR_RL = 100,			//bulldozer
			EVR2_THW_FGRR_TRL = 101,		//bulldozer transporter
			EVR2_THW_FGRT_BH = 102,			//berungshelicopter
			EVR2_FIREFIGHTERS_GTF = 200,	//grossraumtankfahrzeug
			EVR2_FIREFIGHTERS_LFKF,			//l�schflugzeug kommandofahrzeug
			EVR2_FIREFIGHTERS_LFKF_HELI,	//L�schhelikopter Kommandofahrzeug
			EVR2_FIREFIGHTERS_LF_HELI,		//L�schhelikopter
			EVR2_POLICE_GTW = 300,			//polizei bus (has many passenger slots)
			EVR2_POLICE_SEKHC,				//sek helicopter
			EVR2_POLICE_SEKDF,				//sekdronen fahrzeug
			EVR2_POLICE_SEKD,				//sekdrone
			EVR2_AMBULANCE_TRANSEVAC = 400,	//riesiges hilfeflugzeug
			EVR2_FIREFIGHTERS_FWDF = 410,	//feuerwehr drohnenfahrzeug
			EVR2_FIREFIGHTERS_FWD,			//feuerwehr drohne
			EVR2_ALL_SQUAD_VEHICLES = 500, // nur der Komplettheit halber ...

			EVR2_NUMTYPES = EVR2_TV_HELI + 11,
			_EVR2_NUMTYPES_EM2012 = EVR2_TV_HELI + 9	// f�r das Laden alter Spielst�nde
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class EVehicleRoleInfo
		{
		public:
			EVehicleRole2 mRole;	//!< Funktion des Fahrzeugs
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
