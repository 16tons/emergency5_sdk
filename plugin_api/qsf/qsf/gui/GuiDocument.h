// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Event;
		class Element;
		class EventListener;
		class ElementDocument;
	}
}
namespace qsf
{
	class GuiContext;
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
	*    Graphical User Interface (GUI) document base class
	*/
	class QSF_API_EXPORT GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create an event listener
		*
		*  @note
		*    - Ease-of-use method (redirecting internally to "qsf::GuiSystem::createEventListener()")
		*/
		static Rocket::Core::EventListener* createEventListener(const std::string& eventKey);

		/**
		*  @brief
		*    Return a QSF GUI document instance by using a given libRocket element instance
		*
		*  @return
		*    QSF GUI document instance, can be a null pointer, do not destroy the instance
		*/
		static GuiDocument* getGuiDocumentByRocketCoreElement(Rocket::Core::Element* element);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] guiContext
		*    QSF GUI context to create the document in
		*/
		explicit GuiDocument(GuiContext& guiContext);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] guiContext
		*    QSF GUI context to create the document in
		*  @param[in] rocketCoreElementDocument
		*    Rocket element document which have to copy
		*/
		GuiDocument(GuiContext& guiContext, Rocket::Core::ElementDocument& rocketCoreElementDocument);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GuiDocument();

		/**
		*  @brief
		*    Return the document's owning GUI context
		*/
		inline GuiContext& getGuiContext() const;

		/**
		*  @brief
		*    Return the document's libRocket document
		*/
		inline Rocket::Core::ElementDocument* getRocketCoreElementDocument() const;

		/**
		*  @brief
		*    Load the document from an RML file
		*
		*  @param[in] filename
		*    Relative UTF-8 filename in platform-independent notation of the document file to save
		*
		*  @return
		*    "true" on success, "false" if the file could not be found or loaded
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Use this method if you want to change a documents .rml file at runtime
		*/
		void unload();

		/**
		*  @brief
		*    Show the document
		*/
		void show();

		/**
		*  @brief
		*    Show the document focused
		*/
		void showFocused();

		/**
		*  @brief
		*    Show the document modal
		*/
		void showModal();

		/**
		*  @brief
		*    Hide the document
		*/
		void hide();

		/**
		*  @brief
		*    Check if the document is currently visible
		*/
		bool isVisible() const;

		/**
		*  @brief
		*    Show/hide the document
		*
		*  @note
		*    - Has the same effect as "qsf::GuiDocument::show()"/"qsf::GuiDocument::hide()" but is more compact to write in certain use-cases
		*/
		void setVisible(bool visible);

		/**
		*  @brief
		*    Pull the document to the front
		*/
		void pullToFront();

		/**
		*  @brief
		*    Find an element inside the document by its case-sensitive ID
		*/
		Rocket::Core::Element* findElementById(const std::string& elementId) const;

		/**
		*  @brief
		*    Register a new button
		*
		*  @note
		*    - Don't use this method if you can avoid it
		*/
		void registerNewButton(const std::string& buttonId, const std::string& event, const std::string& eventKey);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Toggle visibility
		*/
		virtual void toggleVisibility();

		inline virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event);
		inline virtual void processViewportDimensionsChanged(int width, int height);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void createEmptyDocument();


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called before show
		*
		*  @return
		*    "true" to show the document, "false" to suppress the show request
		*
		*  @note
		*    - This method gives you the opportunity to manipulate the document before it will show
		*    - The default implementation is empty and returns true
		*/
		virtual bool onPreShow();

		/**
		*  @brief
		*    Called after show
		*
		*  @note
		*    - This method gives you the opportunity to manipulate the document after it was shown
		*    - The default implementation is empty
		*/
		virtual void onPostShow();

		/**
		*  @brief
		*    Called before hide
		*
		*  @note
		*    - This method gives you the opportunity to manipulate the document before it will hide
		*    - The default implementation is empty
		*/
		virtual void onPreHide();

		/**
		*  @brief
		*    Called after hide
		*
		*  @note
		*    - This method gives you the opportunity to manipulate the document after it hid
		*    - The default implementation is empty
		*/
		virtual void onPostHide();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Rocket::Core::Element* findElementRecursive(const Rocket::Core::Element& element, const std::string& elementId) const;

		/**
		*  @brief
		*    Set the character set
		*/
		void initCharacterSet();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GuiContext*					   mGuiContext;					///< QSF GUI context where to add the document on load; always valid, do not destroy the instance
		Rocket::Core::ElementDocument* mRocketCoreElementDocument;	///< libRocket document; valid after successful load, else null pointer; do not manually destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/gui/GuiDocument-inl.h"
