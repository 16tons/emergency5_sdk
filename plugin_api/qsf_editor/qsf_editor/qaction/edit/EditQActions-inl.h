// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline QAction* EditQActions::getUndoQAction() const
		{
			return mUndoQAction;
		}

		inline QAction* EditQActions::getRedoQAction() const
		{
			return mRedoQAction;
		}

		inline QAction* EditQActions::getCutQAction() const
		{
			return mCutQAction;
		}

		inline QAction* EditQActions::getCopyQAction() const
		{
			return mCopyQAction;
		}

		inline QAction* EditQActions::getPasteQAction() const
		{
			return mPasteQAction;
		}

		inline QAction* EditQActions::getDeleteQAction() const
		{
			return mDeleteQAction;
		}

		inline QAction* EditQActions::getRenameQAction() const
		{
			return mRenameQAction;
		}

		inline QAction* EditQActions::getMapPropertiesDialogQAction() const
		{
			return mMapPropertiesDialogQAction;
		}

		inline QAction* EditQActions::getMaterialBrowserQAction() const
		{
			return mMaterialBrowserDialogQAction;
		}

		inline QAction* EditQActions::getSequenceQAction() const
		{
			return mSequenceDialogQAction;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
