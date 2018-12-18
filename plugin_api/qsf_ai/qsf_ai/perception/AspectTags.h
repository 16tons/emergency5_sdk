// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UsuallySmallArray.h"

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class BinarySerializer;
}


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
		*    Manages a list of aspect-tags.
		*    An aspect-tag is a string describing one aspect of an entity in the context of the
		*    sensor-/aspect-based perception system.
		*    You may specify any number of aspect-tags by supplying a comma-separated aspect-tag-list.
		*    Internally those are converted into a hash-list.
		*    AspectTags are used to configure aspect- and the sensor-components.
		*    A sensor reacts on entities supplied with a matching aspects.
		*/
		class AspectTags
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Default value. Usually an empty string, mainly for testing purposes and to be used with the property system.
			static const std::string DEFAULT_TAGS;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			AspectTags();
			AspectTags(const std::string& defaultTags);

			// Serialize this object. Only the string representation is serialized, the hashs are recreated on deserialization.
			void serialize(BinarySerializer& serializer);

			// Checks if this AspectTags contains at least one tag
			// that matches the class-name of one of the entity's components.
			bool matches(const Entity& entity) const;

			// Checks if this AspectTags object matches the other AspectTags object.
			// The result is true if either this object contains no tags at all (= matches everything)
			// or if at least one tag exists in both objects.
			bool matches(const AspectTags& otherAspect) const;

			// Access to the managed list of tags.
			//@{
			size_t getNumTags() const;
			const std::string& getTags() const;
			void setTags(const std::string& tags);
			//@}


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			UsuallySmallArray<uint32,size_t,4> mTagHashs; // a sorted array of string-hashes corresponding to the comma-separate mTags-strings.

			std::string mTags; // the original tags-string
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/AspectTags-inl.h"
