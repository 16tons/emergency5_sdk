// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/reflection/CampClass.h>
#include <qsf/link/LinkProxy.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class BinarySerializer;
	class ContainerLink;
	class Entity;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Prototype container base class
		*
		*  @remarks
		*    A container consists of a set of contained prototypes.
		*/
		class QSF_GAME_API_EXPORT PrototypeContainer
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit PrototypeContainer(Prototype& prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PrototypeContainer();

			/**
			*  @brief
			*    Get the link proxy instance holding references to all connected container links
			*/
			inline const LinkProxy& getLinkProxy() const;

			/**
			*  @brief
			*    Get the map of all connected container links
			*/
			inline const boost::container::flat_map<LinkAnchorId, ContainerLink*>& getLinks() const;

			/**
			*  @brief
			*    Check whether the given prototype is inside this container
			*/
			bool isInContainer(Prototype& prototype) const;

			/**
			*  @brief
			*    Get the container type of a contained prototype, or getUninitialized<uint32>() if it is not contained
			*/
			uint32 getContainerType(Prototype& prototype) const;

			/**
			*  @brief
			*    Add the given prototype to this container
			*/
			void addToContainer(Prototype& prototype, uint32 category);

			/**
			*  @brief
			*    Remove the given prototype from this container
			*
			*  @return
			*    "true" in case the prototype could be removed, "false" if it was not found
			*/
			bool removeFromContainer(const Prototype& prototype);

			std::vector<Entity*> getAllLinksByContainerType(uint32 containerType) const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Serialize the list of contained entities
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Enable simulating mode
			*/
			void enableSimulatingMode(bool simulating);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void addPrototypeToContainerById(uint64 prototypeId, uint32 category);
			void removePrototypeFromContainerById(uint64 prototypeId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Prototype&			mPrototype;
			LinkProxy			mContentLinkProxy;	///< Link proxy pointing to the contained prototypes
			std::vector<uint64> mContainedEntityIds;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

		public:
			QSF_CAMP_ARRAY_INTERFACE_CLASS(PrototypeIdArrayInterface, PrototypeContainer, uint64);
			PrototypeIdArrayInterface ContainedPrototypeIdArray;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/PrototypeContainer-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::PrototypeContainer)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::game::PrototypeContainer::PrototypeIdArrayInterface)
