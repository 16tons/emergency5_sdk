// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Core/DecoratorInstancer.h>


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
	*    EMERGENCY 5 SEK scan decorator instancer
	*
	*  @note
	*    - The decorator instancer basically exposes the variables a custom decorator has to the rcss properties, making adjustments through the rcss files possible.
	*/
	class DecoratorInstancerSEKScan : public Rocket::Core::DecoratorInstancer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline DecoratorInstancerSEKScan();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DecoratorInstancerSEKScan();

		/**
		*  @brief
		*    Instances a decorator given the property tag and attributes from the RCSS file
		*
		*  @param[in] name
		*    The type of decorator desired. For example, "background-decorator: simple;" is declared as type "simple".
		*  @param[in] properties
		*    All RCSS properties associated with the decorator
		*
		*  @return
		*    The decorator if it was instanced successful, null pointer if an error occurred
		*/
		virtual Rocket::Core::Decorator* InstanceDecorator(const Rocket::Core::String& name, const Rocket::Core::PropertyDictionary& properties) override;

		/**
		*  @brief
		*    Releases the given decorator
		*
		*  @param[in] decorator
		*    Decorator to release. This is guaranteed to have been constructed by this instancer.
		*/
		virtual void ReleaseDecorator(Rocket::Core::Decorator* decorator) override;

		/**
		*  @brief
		*    Releases the instancer
		*/
		virtual void Release() override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/decorator/DecoratorInstancerSEKScan-inl.h"
