// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			namespace packet
			{


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
				inline EditorOperation::EditorOperation() :
					mIsUndo(false)
				{
					// Nothing to do in here
				}

				inline EditorOperation::~EditorOperation()
				{
					// Nothing to do in here
				}

				inline bool EditorOperation::isUndo() const
				{
					return mIsUndo;
				}

				inline void EditorOperation::setUndo(bool undo)
				{
					mIsUndo = undo;
				}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
			} // packet
		} // base
	} // editor
} // qsf
