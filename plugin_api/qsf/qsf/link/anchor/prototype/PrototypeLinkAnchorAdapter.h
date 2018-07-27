// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/link/anchor/LinkAnchorAdapter.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class Entity;
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
	*    Prototype link anchor adapter
	*/
	class QSF_API_EXPORT PrototypeLinkAnchorAdapter : public LinkAnchorAdapter
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PrototypeLinkAnchorAdapter(const Prototype& prototype);

		/**
		*  @brief
		*    Constructor
		*/
		PrototypeLinkAnchorAdapter(const Entity& entity);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::LinkAnchorAdapter methods         ]
	//[-------------------------------------------------------]
	public:
		virtual LinkAnchor& constructLinkAnchor() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Prototype& mPrototype;


	};


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	namespace link
	{
		template<>
		struct GetLinkAnchorAdapter<Prototype>
		{
			// For prototypes, use the specialized prototype link anchor adapter
			typedef PrototypeLinkAnchorAdapter Adapter;
		};

		template<>
		struct GetLinkAnchorAdapter<Entity>
		{
			// For entities, use the specialized prototype link anchor adapter, too
			typedef PrototypeLinkAnchorAdapter Adapter;
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
