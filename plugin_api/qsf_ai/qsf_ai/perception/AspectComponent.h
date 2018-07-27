// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/perception/AspectTags.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    An aspect component adds the ability to this entity to be perceived by
		*    sensor components attached to other entities.
		*    An aspect component contains any number of tags (for example "child,person"),
		*    in fact it is a wrapper around an AspectTags-object for direct use with
		*    the component system.
		*    An entity with such an aspect component can be perceived by any sensor component
		*    in range that's configured to react on at least one of those tags.
		*/
		class QSF_AI_API_EXPORT AspectComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// unique CAMP ids for the component and for properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int TAGS_PROPERTY_ID;
			//@}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			AspectComponent(Prototype* prototype);

			// Overridden Component interface
			//@{
			virtual bool onStartup() override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Access to the concrete list of aspect-tags.
			//@{
			const AspectTags& getAspectTags() const;
			const std::string& getTags() const;
			void setTags(const std::string& tags);
			//@}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AspectTags mAspectTags; // the aspect tags


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI();

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/AspectComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::AspectComponent);
