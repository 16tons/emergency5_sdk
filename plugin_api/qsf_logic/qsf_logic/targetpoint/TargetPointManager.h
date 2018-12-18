// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/targetpoint/TargetPointProvider.h"

#include <qsf/plugin/pluginable/PluginableManager.h>

#include <boost/container/flat_map.hpp>


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
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Target point manager class
		*/
		class QSF_LOGIC_API_EXPORT TargetPointManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		friend class TargetPointProvider;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_map<uint32, TargetPointProvider*> TargetPointProviderMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline TargetPointManager();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~TargetPointManager();

			/**
			*  @brief
			*    Deliver target points based on the two entities' current state.
			*    Tries to create a fallback target point at the targets location if the requested target point provider is not able to create results.
			*    If that is not possible because the target has no transform an exception is thrown.
			*
			*  @param[in] targetPointId
			*    The target point type identifier
			*  @param[in] caller
			*    The caller entity, i.e. the entity requesting a target point; is possibly ignored when calculating the target point
			*  @param[in] target
			*    The target entity, i.e. the one to deliver a target point for
			*  @param[out] targetPoints
			*    The target point options that should be used for an interaction between these two entities.
			*/
			void getTargetPoints(uint32 targetPointId, Entity& caller, Entity& target, std::vector<TargetPoint>& targetPoints) const;

			/**
			*  @brief
			*    Deliver target points based on the two entities' current state but assuming a specific transform for both caller as well as target entity.
			*    Tries to create a fallback target point at the targets location if the requested target point provider is not able to create results.
			*    If that is not possible because the target has no transform an exception is thrown.
			*
			*  @param[in] targetPointId
			*    The target point type identifier
			*  @param[in] caller
			*    The caller entity, i.e. the entity requesting a target point; is possibly ignored when calculating the target point
			*  @param[in] target
			*    The target entity, i.e. the one to deliver a target point for
			*  @param[out] targetPoints
			*    The target point options that should be used for an interaction between these two entities.
			*/
			void getTargetPoints(uint32 targetPointId, Entity& caller, const Transform& callerTransform, Entity& target, const Transform& targetTransform, std::vector<TargetPoint>& targetPoints) const;

			/**
			*  @brief
			*    Check if a target point is reached
			*
			*  @param[in] targetPointId
			*    The target point type identifier
			*  @param[in] caller
			*    The caller entity, i.e. the entity requesting a target point; is possibly ignored when calculating the target point
			*  @param[in] target
			*    The target entity, i.e. the one to deliver a target point for
			*
			*  @return
			*    "true" is target point is reached, "false" otherwise
			*/
			bool isTargetPointReached(uint32 targetPointId, Entity& caller, Entity& target);

			/**
			*  @brief
			*    Register a provider with its unique id.
			*
			*  @param[in] targetPointId
			*    The unique id of the provider to register
			*  @param[in] provider
			*    The new provider to register
			*/
			void registerProvider(uint32 targetPointId, TargetPointProvider* provider);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void unregisterProvider(uint32 targetPointId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			* Simple fallback to create a dummy target point if the requested target point doesn't exist.
			* Uses a current transform typically read from an attached transform component.
			*/
			TargetPoint createDefaultTargetPoint(const Entity& entity, const Transform& transform) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TargetPointProviderMap mProviders;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/targetpoint/TargetPointManager-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::TargetPointManager)
