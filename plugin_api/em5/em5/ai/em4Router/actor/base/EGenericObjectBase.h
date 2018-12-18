// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/em4Router/actor/base/EActorBase.h"

#include <qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h>

#include <qsf/component/base/TransformComponent.h>


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
	*    EGenericObjectBase wrapper base class
	*/
	class EGenericObjectBase : public EActorBase
	{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		EGenericObjectBase(qsf::Entity& entity);
		virtual ~EGenericObjectBase();

		const glm::vec3& GetPosition();
		const glm::mat3x3& GetRotationMatrix();
		qsf::ai::EObjectTerrainClass GetTerrainClass();
		qsf::ai::EFloorPlacement GetPlacement();
		void UpdatePlacement(glm::vec3& position_, glm::mat3x3& rotation_);
		int GetCarriedPersonID() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::TransformComponent> mTransformComponent;
		glm::mat3x3 mCachedRotationMatrix;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
