// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline QAction* NodesQActions::getInsertNodeQAction() const
		{
			return mInsertNodeQAction;
		}

		inline QAction* NodesQActions::getSelectAllNodesQAction() const
		{
			return mSelectAllNodesQAction;
		}

		inline QAction* NodesQActions::getAlignToCenterQAction() const
		{
			return mAlignToCenterQAction;
		}

		inline QAction* NodesQActions::getSetCenterQAction() const
		{
			return mSetCenterQAction;
		}

		inline QAction* NodesQActions::getConnectNodeToGatewayQAction() const
		{
			return mConnectNodeToGatewayQAction;
		}

		inline QAction* NodesQActions::getRemoveNodeFromGatewayQAction() const
		{
			return mRemoveNodeFromGatewayQAction;
		}

		inline QAction* NodesQActions::getCreateGatewayQAction() const
		{
			return mCreateGatewayQAction;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
