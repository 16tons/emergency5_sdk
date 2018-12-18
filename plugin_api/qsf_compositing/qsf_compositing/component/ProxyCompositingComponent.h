// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/reflection/type/CampUint64.h>
#include <qsf/renderer/compositor/CompositingComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Proxy compositing component
		*/
		class QSF_COMPOSITING_API_EXPORT ProxyCompositingComponent : public CompositingComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;		///< "qsf::ProxyCompositingComponent" unique component ID
			static const uint32 SOURCE_ENTITY_ID;	///< "SourceEntityId" unique class property ID inside the class


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			inline explicit ProxyCompositingComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ProxyCompositingComponent();

			/**
			*  @brief
			*    Return the ID of the entity which compositing component should be used as compositing source
			*
			*  @return
			*    The ID of the entity which compositing component should be used as compositing source
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline uint64 getSourceEntityId() const;

			/**
			*  @brief
			*    Set the ID of the entity which compositing component should be used as compositing source
			*
			*  @param[in] sourceEntityId
			*    The ID of the entity which compositing component should be used as compositing source
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline void setSourceEntityId(uint64 sourceEntityId);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::CompositingComponent methods      ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getCompositorWorkspaceDefinitionName() const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return the compositing component instance this proxy compositing component redirects to
			*
			*  @return
			*    The compositing component instance this proxy compositing component redirects to
			*/
			CompositingComponent* getSourceCompositingComponent() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint64 mSourceEntityId;	///< ID of the entity which compositing component should be used as compositing source, 0 by intent (= core entity ID)


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_compositing/component/ProxyCompositingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::ProxyCompositingComponent)
