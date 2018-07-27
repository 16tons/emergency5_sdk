#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/prototype/Prototype.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkComponent;
	class Protoype;
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
	*    Link component helper class
	*/
	class QSF_API_EXPORT LinkComponentHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit LinkComponentHelper(Prototype& prototype);
		explicit LinkComponentHelper(LinkComponent& linkComponent);

		/**
		*  @brief
		*    Destructor
		*/
		~LinkComponentHelper();

		/**
		*  @brief
		*    Returns all given component of the prototype; iterate over all linked child prototypes
		*
		*  @param[out] result
		*    vector with Pointer to the searched component or a empty vector
		*/
		template<typename T>
		void getAllComponentsFromPrototypeOrLinkedChild(std::vector<T*>& result) const;

		void getAllComponentsFromPrototypeOrLinkedChildById(const StringHash& id, std::vector<Component*>& result) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Prototype& mPrototype;	///< prototype to use, always valid, do not destroy the instance
		LinkComponent& mLinkComponent;	///< LinkComponent to use, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/link/LinkComponentHelper-inl.h"
