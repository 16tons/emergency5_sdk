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
		inline QAction* LayerQActions::getMoveToLayerQAction() const
		{
			return mMoveToLayerQAction;
		}

		inline QAction* LayerQActions::getCopyToLayerQAction() const
		{
			return mCopyToLayerQAction;
		}

		inline QAction* LayerQActions::getReparentLayerQAction() const
		{
			return mReparentLayerQAction;
		}

		inline QAction* LayerQActions::getLoadLayerInGameQAction() const
		{
			return mLoadLayerInGameQAction;
		}

		inline QAction* LayerQActions::getHideAllLayersQAction() const
		{
			return mHideAllLayersQAction;
		}

		inline QAction* LayerQActions::getHideAllLayersExceptSelectedQAction() const
		{
			return mHideAllLayersExceptSelectedQAction;
		}

		inline QAction* LayerQActions::getHideNotInGameLoadedLayersQAction() const
		{
			return mHideNotInGameLoadedLayersQAction;
		}

		inline QAction* LayerQActions::getUnhideAllLayersQAction() const
		{
			return mUnhideAllLayersQAction;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
