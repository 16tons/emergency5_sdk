// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Core/Context.h>
#include <Rocket/Core/Plugin.h>


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
	*    Librocket plugin
	*
	*  @note
	*    A plugin used to scale the height and width of Librocket documents and elements.
	*/
	class ScalePlugin : public Rocket::Core::Plugin
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		virtual ~ScalePlugin();


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Core::Plugin methods           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when a new element is created.
		*/
		virtual void OnElementCreate(Rocket::Core::Element* element) override;

		/**
		*  @brief
		*     Called when a document is successfully loaded from file or instanced, initialised and added to its context.
		*     This is called before the document's 'load' event.
		*/
		virtual void OnDocumentLoad(Rocket::Core::ElementDocument* document) override;

		/**
		*  @brief
		*    Called when Rocket is initialised.
		*/
		virtual void OnInitialise() override;

		/**
		*  @brief
		*    Called when Rocket shuts down.
		*/
		virtual void OnShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Used to scalle all child elements of another element.
		*/
		void scaleElementsRecursive(Rocket::Core::Element& element);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
