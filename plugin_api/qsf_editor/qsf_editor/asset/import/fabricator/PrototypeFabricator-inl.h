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
		inline PrototypeFabricator::~PrototypeFabricator()
		{
			// Nothing to do in here
		}

		inline PrototypeFabricatorManager& PrototypeFabricator::getPrototypeFabricatorManager() const
		{
			return getManager();
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline PrototypeFabricator::PrototypeFabricator(PrototypeFabricatorManager* prototypeFabricatorManager) :
			Pluginable(prototypeFabricatorManager)
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
