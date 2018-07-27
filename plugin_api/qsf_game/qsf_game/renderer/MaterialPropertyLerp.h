// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/job/JobArguments.h>
#include <qsf/job/JobProxy.h>
#include <qsf/asset/AssetProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash MaterialPropertyId;	///< Material property identifier, internally just a POD "uint32", result of hashing the property name


	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Helper class to change a material property over time
		*/
		class QSF_GAME_API_EXPORT MaterialPropertyLerp
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			MaterialPropertyLerp();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MaterialPropertyLerp();

			/**
			*  @brief
			*    Start to change a material property over time
			*
			*  @param[in] entity
			*    Entity to affect
			*  @param[in] materialPropertyId
			*    ID of the material property we want to change
			*  @param[in] timeToChange
			*    Time the property has to change
			*  @param[in] startValue
			*    Starting value for the property
			*  @param[in] endValue
			*    End value of the property
			*/
			//TODO(sw): use Vector3 instead of single float
			void startChange(Entity& entity, MaterialPropertyId materialPropertyId, const Time& timeToChange, float startValue, float endValue, bool isVector3);

			/**
			*  @brief
			*    Stop to change the material property
			*/
			void stopChange();

			/**
			*  @brief
			*    Directly set the given value for the material property
			*/
			void setValue(float value);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void update(const JobArguments& jobArguments);

			static float lerp(float f1, float f2, float a);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Entity*			   mEntity;	// TODO(fw): Better use an entity ID
			MaterialPropertyId mMaterialPropertyId;
			Time			   mTimePassed;
			Time			   mTimeToChange;
			float			   mStartValue;
			float			   mEndValue;
			bool			   mIsVector3;
			JobProxy		   mUpdateJobProxy;	///< Job proxy


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
