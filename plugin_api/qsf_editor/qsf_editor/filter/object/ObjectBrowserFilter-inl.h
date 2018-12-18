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
		inline ObjectBrowserFilter::~ObjectBrowserFilter()
		{
			// Nothing to do in here
		}

		inline ObjectBrowserFilterManager& ObjectBrowserFilter::getObjectBrowserFilterManager() const
		{
			return getManager();
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline ObjectBrowserFilter::ObjectBrowserFilter(ObjectBrowserFilterManager* ObjectBrowserFilterManager) :
			Pluginable(ObjectBrowserFilterManager)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
