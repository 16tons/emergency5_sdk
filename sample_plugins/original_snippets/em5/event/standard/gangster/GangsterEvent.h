// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GangsterBaseLogic;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 gangster freeplay event base class
	*/
	class EM5_API_EXPORT GangsterEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the ID of the gangster
		*/
		uint64 getGangsterId() const;

		/**
		*  @brief
		*    Set the gangster
		*/
		void setGangster(qsf::Entity& entity, bool resolveSpawnPoint = true);

		/**
		*  @brief
		*    Get the ID of the victim
		*/
		uint64 getVictimId() const;

		/**
		*  @brief
		*    Set the victim
		*/
		void setVictim(qsf::Entity& entity);

		GangsterBaseLogic* getGangsterLogic() const;

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EscapeTargetTag, const std::string&, mEscapeTargetTag);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		GangsterEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GangsterEvent();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<qsf::Entity> mGangster;
		qsf::WeakPtr<qsf::Entity> mVictim;
		std::string				  mEscapeTargetTag;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterEvent)
