// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/math/Color4.h>

#include <Rocket/Core/BaseXMLParser.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Time;
	class Color3;
	class Entity;
	class GuiDocument;
	class AssetProxy;
}
namespace Rocket
{
	namespace Core
	{
		class Element;
		class ElementDocument;
		class ElementText;
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
	*    EMERGENCY 5 static GUI helper class
	*/
	class EM5_API_EXPORT GuiHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::Color4 IDLE_ACTION_COLOR;			// Intense green
		static const qsf::Color4 BUSY_ACTION_COLOR;			// Yellow
		static const qsf::Color4 BUSY_BLOCKED_ACTION_COLOR;	// Orange
		static const qsf::Color4 CALCULATING_PATH;			// Red


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		// Get the text between span or from form inputs like text fields or password input fields
		static Rocket::Core::String getValueById(Rocket::Core::ElementDocument& document, const Rocket::Core::String& id);
		static void setValueById(Rocket::Core::ElementDocument& document, const Rocket::Core::String& id, const Rocket::Core::String& value);

		// Shows and hides elements - this is faster than the show/hide with display but has the side effect that the element is still used in the layouting
		static void showElementWithVisibility(Rocket::Core::Element& element, bool show);
		static void showElementByIdWithVisibility(Rocket::Core::ElementDocument& document, const Rocket::Core::String& id, bool show);

		// Shows and hides elements - this is slower than the show/hide with display property since it will change the layout
		static void showElementWithDisplay(Rocket::Core::Element& element, bool show);
		static void showElementByIdWithDisplay(Rocket::Core::ElementDocument& document, const Rocket::Core::String& id, bool show);

		// Shows and hides elements - just another alternative (TODO(fw): WHat is the difference to the others?)
		static void showElementByIdWithHiddenClass(Rocket::Core::ElementDocument& document, const Rocket::Core::String& id, bool show);

		static Rocket::Core::Element* createElement(Rocket::Core::Element* parentElement, const Rocket::Core::String& tagName, const Rocket::Core::XMLAttributes& attributes);
		static Rocket::Core::ElementText* getOrCreateTextElement(Rocket::Core::Element* parentElement, bool clearChildsWhenNoTextFoundAsFirstChild = true);

		// Check box
		static bool isCheckBoxChecked(Rocket::Core::Element& element);
		static bool toggleCheckBox(Rocket::Core::Element& element);

		/**
		*  @brief
		*    Convert a number to string with separator
		*
		*  @todo
		*    - TODO(fw): Better move this into a helper class, as it's useful not only for GUI document classes
		*/
		static std::string numberStringWithSeparator(uint32 number);

		/**
		*  @brief
		*    Replace the signs "/ \ : * ? " < > |" with "_"
		*/
		static std::string convertStringToUsableFilename(const std::string& filename);

		/**
		*  @brief
		*    Convert a uint32 to string with a fix string size
		*/
		static std::string fixedLength(int value, uint32 digits);

		/**
		*  @brief
		*    Insert white spaces in strings
		*/
		static std::string insertWhiteSpaces(uint32 maximumWordLength, const std::string& string);

		/**
		*  @brief
		*    Replace HTML tags with the corresponding entity
		*/
		static std::string useHTMLEntities(const std::string& string);

		/**
		*  @brief
		*    Get a hex color string representation (including '#' at the beginning) from white color with alpha
		*/
		static std::string getHexColorWhiteAlpha(float alpha);

		/**
		*  @brief
		*    Return the EMERGENCY 5 light blue color usually used everywhere inside the GUI
		*/
		static const qsf::Color3& getSystemColor3();

		/**
		*  @brief
		*    Return the EMERGENCY 5 action priority color showing e.g. a general unit action state (green, orange, yellow)
		*/
		static const qsf::Color4& getActionPriorityColorByEntity(const qsf::Entity& entity);

		/**
		*  @brief
		*    Iterate through all child elements and set the pseudo class "selected" to false, except for the current element which will be set to true
		*
		*  @param[in] document
		*    Document to work on
		*  @param[in] listElementName
		*    Element which acts as list
		*  @param[in] selectedElement
		*    List element to select, can be a null pointer
		*/
		static void selectListElement(const qsf::GuiDocument& document, const std::string& listElementName, Rocket::Core::Element* selectedElement);

		/**
		*  @brief
		*    Return the given time as string usable inside a GUI
		*
		*  @return
		*    Time as string usable inside a GUI ("%2d:%02d:%02d" if hours is not zero, else "%02d:%02d")
		*/
		static std::string getTimeAsGuiString(const qsf::Time& time);

		static std::string getDisplayNameForMap(const qsf::AssetProxy& mapAssetProxy);
		static std::string getPreviewImageForMap(const qsf::AssetProxy& mapAssetProxy);

		static std::string equipmentNameToFilename(const std::string& equipmentName);

		static void getLetterboxDimensions(const glm::ivec2& frameSize, float aspectRatio, glm::ivec2& outSize, glm::ivec2& outOffset);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// GUI helper can't be instantiated
		GuiHelper() {}
		~GuiHelper() {}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
