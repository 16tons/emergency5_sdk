// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/type/LinkTypes.h"
#include "qsf/base/NamedIdentifier.h"
#include "qsf/link/anchor/prototype/PrototypeLinkAnchorAdapter.h"	// Include this here already, even though it's not necessary; but forgetting this can lead to ugly compiler errors


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
	*    Static QSF plugin link types collection class
	*/
	class QSF_API_EXPORT QsfLinkTypes : public LinkTypes
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class QsfPlugin;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier CHILD;			///< Link type name "Child"
		static const NamedIdentifier BONE;			///< Link type name "Bone"
		static const NamedIdentifier BONE_TO_BONE;	///< Link type name "BoneToBone"
		static const NamedIdentifier CONTAINER;		///< Link type name "Container"


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Register the link types
		*/
		static void registerLinkTypes();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
