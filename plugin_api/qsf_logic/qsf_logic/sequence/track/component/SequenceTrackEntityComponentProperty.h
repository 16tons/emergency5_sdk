// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrack.h"

#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the sequencer entity component property track class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    TODO(co) Document me
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackEntityComponentProperty : public SequenceTrack
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Helper function to split a component property path into reflection data
			*/
			static void splitComponentPropertyPath(const std::string& path, const camp::Class*& outComponentClass, const camp::Property*& outComponentProperty);

			/**
			*  @brief
			*    Helper function to combine a component property path from reflection data
			*/
			static void combineComponentPropertyPath(std::string& outPath, const camp::Class& componentClass, const camp::Property& componentProperty);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline SequenceTrackEntityComponentProperty(Sequence* sequence, uint32 id);

			inline uint64 getEntityId() const;
			inline void setEntityId(uint64 entityId);
			inline Entity* getEntity() const;

			inline const camp::Class* getComponentType() const;
			inline const camp::Class& getComponentTypeSafe() const;	// Can throw exception
			inline const camp::Property* getComponentProperty() const;
			inline const camp::Property& getComponentPropertySafe() const;	// Can throw exception
			Component* getComponent() const;

			/**
			*  @brief
			*    No return by reference as the name is figured out dynamically. Copy elision might work well here!
			*/
			std::string getComponentPropertyName() const;

			void setComponentPropertyName(const std::string& propertyName);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceTrack methods      ]
		//[-------------------------------------------------------]
		public:
			virtual void serializeOriginalControlledValue(BinarySerializer& serializer) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint64					mEntityId;
			const camp::Class*		mComponentType;
			const camp::Property*	mComponentProperty;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackEntityComponentProperty)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/component/SequenceTrackEntityComponentProperty-inl.h"
