// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/input/InputListener.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GenericDebugGuiDocument;
}
namespace Rocket
{
	namespace Core
	{
		class Event;
	}
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
	*    EMERGENCY 5 generic GUI instance for user interface development
	*/
	class GenericDebugGui : public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GenericDebugGuiDocument;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GenericDebugGui();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GenericDebugGui();

		/**
		*  @brief
		*    Load the given libRocket document
		*
		*  @param[in] guiDocumentName
		*    UTF-8 virtual filename of the document to load
		*/
		void loadDocument(const std::string& guiDocumentName);

		/**
		*  @brief
		*    Reload the previously defined libRocket document
		*/
		void reloadDocument();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool loadGenericUiWindow(const std::string& filename);
		void processRocketEvent(const std::string& eventKey, Rocket::Core::Event& event, GenericDebugGuiDocument& sender);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string				 mDocumentFilename;
		GenericDebugGuiDocument* mDebugGuiDocument;		///< Generic debug GUI document instance; always valid, we're responsible for destroying the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
