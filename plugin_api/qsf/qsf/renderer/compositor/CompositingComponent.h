// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CameraComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract compositing component
	*
	*  @note
	*    - There's usually a compositing core component, see "qsf::Map::getCoreEntity()" documentation for details
	*/
	class QSF_API_EXPORT CompositingComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CompositingComponent" unique component ID


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
		inline explicit CompositingComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CompositingComponent();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CompositingComponent methods      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the name of the compositor workspace definition to use
		*
		*  @return
		*    The name of the compositor workspace definition to use
		*/
		virtual std::string getCompositorWorkspaceDefinitionName() const = 0;

		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*
		*  @note
		*    - The default implementation is empty
		*/
		inline virtual void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/compositor/CompositingComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CompositingComponent)
