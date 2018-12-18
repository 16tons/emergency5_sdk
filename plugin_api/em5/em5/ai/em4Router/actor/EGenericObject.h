// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EGenericObjectBase.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h>


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
	*     Emergency 5 implementation of EGenericObject wrapper class for a QSF entity
	*/
	class EGenericObject : public qsf::ai::EGenericObject, public EGenericObjectBase
	{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		EGenericObject(qsf::ai::EActorType actorType, qsf::Entity& entity);
		virtual ~EGenericObject();

		virtual const glm::vec3& GetPosition() override { return EGenericObjectBase::GetPosition(); }


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
