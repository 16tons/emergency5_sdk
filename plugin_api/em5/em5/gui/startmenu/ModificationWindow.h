// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/input/InputListener.h>


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
	*    EMERGENCY 5 modification window GUI
	*/
	class ModificationWindow : public qsf::GuiDocument, public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit ModificationWindow(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ModificationWindow();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onRowSelection(Rocket::Core::Element* element);
		void updateSelectionInfo(const std::string& title, const std::string& description, const std::string& imageName, bool showImage = true);
		void setPreviewImage(const std::string& absolutePath);

		Rocket::Core::Element* getSelectedModElement() const;
		std::string getSelectedModName() const;
		void onDeleteModConfirm(bool buttonResult);
		void onReplaceExistingProjectConfirm(bool buttonResult, const std::string& filename);

		void restoreOriginallyEnabledMods();

		void importMod(const std::string& filename, bool replaceExistingProject = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<uint32> mOriginallyEnabledModIds;
		std::string			mSelectedElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
