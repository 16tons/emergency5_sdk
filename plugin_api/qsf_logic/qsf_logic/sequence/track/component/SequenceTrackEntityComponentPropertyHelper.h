// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/reflection/CampHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		namespace detail
		{
			template <typename T>
			void serializeOriginalControlledValue(BinarySerializer& serializer, Component* component, const camp::Property& campProperty, const T& defaultValue)
			{
				if (serializer.isReading())
				{
					T target;
					serializer.read(target);
					if (nullptr != component)
					{
						campProperty.set(camp::UserObject(component), target);
					}
				}
				else
				{
					T target = (nullptr != component) ? campProperty.get(camp::UserObject(component)).to<T>() : defaultValue;
					serializer.serialize(target);
				}
			}

			template<>
			inline void serializeOriginalControlledValue<std::string>(BinarySerializer& serializer, Component* component, const camp::Property& campProperty, const std::string& defaultValue)
			{
				if (serializer.isReading())
				{
					std::string target;
					serializer.serialize(target);
					if (nullptr != component)
					{
						campProperty.set(camp::UserObject(component), target);
					}
				}
				else
				{
					std::string target = (nullptr != component) ? CampHelper::campValueToString(campProperty.get(camp::UserObject(component))) : defaultValue;
					serializer.serialize(target);
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
