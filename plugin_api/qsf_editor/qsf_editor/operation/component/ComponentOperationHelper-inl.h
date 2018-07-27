// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>

#include <qsf/QsfHelper.h>
#include <qsf/component/Component.h>	// Needed otherwise compiler error: invalid use of incomplete type 'const class qsf::Component'!
#include <qsf/reflection/ClassSystem.h>
#include <qsf/reflection/CampArrayHelper.h>

#include <camp/arrayproperty.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		template <typename T>
		base::Operation* ComponentOperationHelper::buildSetArrayPropertyOperation(const Component& component, uint32 propertyId, const T& newValue)
		{
			// Get CAMP property
			const camp::Property* campProperty = component.getCampPropertyById(propertyId);
			if (nullptr != campProperty && campProperty->type() == camp::arrayType)
			{
				// Get new value as Boost ptree
				boost::property_tree::ptree newValueAsPTree;
				CampArrayHelper<T>::toBoostPTree(newValue, static_cast<const camp::ArrayProperty&>(*campProperty), newValueAsPTree);

				// Create the operation
				return new base::SetComponentPropertyOperation(component, propertyId, newValueAsPTree);
			}

			// Error!
			return nullptr;
		}

		template <typename T>
		base::Operation* ComponentOperationHelper::buildSetArrayPropertyOperation(const Component& component, uint32 propertyId, const T& newValue, const T& previousValue)
		{
			// Get CAMP property
			const camp::Property* campProperty = component.getCampPropertyById(propertyId);
			if (nullptr != campProperty && campProperty->type() == camp::arrayType)
			{
				// Get new value as Boost ptree
				boost::property_tree::ptree newValueAsPTree;
				CampArrayHelper<T>::toBoostPTree(newValue, static_cast<const camp::ArrayProperty&>(*campProperty), newValueAsPTree);

				// Get previous value as Boost ptree
				boost::property_tree::ptree previousValueAsPTree;
				CampArrayHelper<T>::toBoostPTree(previousValue, static_cast<const camp::ArrayProperty&>(*campProperty), previousValueAsPTree);

				// Create the operation
				return new base::SetComponentPropertyOperation(component, propertyId, newValueAsPTree, &previousValueAsPTree);
			}

			// Error!
			return nullptr;
		}

		template <typename T>
		base::Operation* ComponentOperationHelper::buildSetArrayPropertyOperation(uint64 entityId, uint32 componentId, uint32 propertyId, const T& newValue)
		{
			try
			{
				// Get CAMP property
				const camp::Property& campProperty = QSF_CLASS.getCampPropertyById(propertyId, componentId);
				if (campProperty.type() == camp::arrayType)
				{
					// Get new value as Boost ptree
					boost::property_tree::ptree newValueAsPTree;
					CampArrayHelper<T>::toBoostPTree(newValue, static_cast<const camp::ArrayProperty&>(campProperty), newValueAsPTree);

					// Create the operation
					return new base::SetComponentPropertyOperation(entityId, componentId, propertyId, newValueAsPTree);
				}
			}
			catch (const std::exception&)
			{
				// Nothing to do in here
			}

			// Error!
			return nullptr;
		}

		template <typename T>
		base::Operation* ComponentOperationHelper::buildSetArrayPropertyOperation(uint64 entityId, uint32 componentId, uint32 propertyId, const T& newValue, const T& previousValue)
		{
			try
			{
				// Get CAMP property
				const camp::Property& campProperty = QSF_CLASS.getCampPropertyById(propertyId, componentId);
				if (campProperty.type() == camp::arrayType)
				{
					// Get new value as Boost ptree
					boost::property_tree::ptree newValueAsPTree;
					CampArrayHelper<T>::toBoostPTree(newValue, static_cast<const camp::ArrayProperty&>(campProperty), newValueAsPTree);

					// Get previous value as Boost ptree
					boost::property_tree::ptree previousValueAsPTree;
					CampArrayHelper<T>::toBoostPTree(previousValue, static_cast<const camp::ArrayProperty&>(campProperty), previousValueAsPTree);

					// Need named instance otherwise error: taking address of temporary [-fpermissive]
					camp::Value previousCampValue(previousValueAsPTree);
					// Create the operation
					return new base::SetComponentPropertyOperation(entityId, componentId, propertyId, camp::Value(newValueAsPTree), &previousCampValue);
				}
			}
			catch (const std::exception&)
			{
				// Nothing to do in here
			}

			// Error!
			return nullptr;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
