// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline MaterialPropertyValue MaterialProperty::materialPropertyValueFromReference(ValueType valueType, uint32 reference)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	 = valueType;
		materialPropertyValue.mValue.Integer = static_cast<int>(reference);
		return materialPropertyValue;
	}

	inline bool MaterialProperty::isReferenceUsage(Usage usage)
	{
		return (usage >= Usage::TEXTURE_REFERENCE && usage <= Usage::INSTANCE_REFERENCE);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MaterialProperty::MaterialProperty() :
		MaterialPropertyValue(fromUnknown()),
		mMaterialPropertyId(0),
		mUsage(Usage::UNKNOWN),
		mOverwritten(false)
	{
		// Nothing here
	}

	inline MaterialProperty::MaterialProperty(MaterialPropertyId materialPropertyId, Usage usage, const MaterialPropertyValue& materialPropertyValue) :
		MaterialPropertyValue(materialPropertyValue),
		mMaterialPropertyId(materialPropertyId),
		mUsage(usage),
		mOverwritten(false)
	{
		// Nothing here
	}

	inline MaterialProperty::~MaterialProperty()
	{
		// Nothing here
	}

	inline MaterialPropertyId MaterialProperty::getMaterialPropertyId() const
	{
		return mMaterialPropertyId;
	}

	inline MaterialProperty::Usage MaterialProperty::getUsage() const
	{
		return mUsage;
	}

	inline bool MaterialProperty::isOverwritten() const
	{
		return mOverwritten;
	}

	inline bool MaterialProperty::isReferenceUsage() const
	{
		return isReferenceUsage(mUsage);
	}

	inline uint32 MaterialProperty::getReferenceValue() const
	{
		QSF_ASSERT(isReferenceUsage(), "qsf::MaterialProperty::getReferenceValue(): Invalid internal type", QSF_REACT_NONE);
		return static_cast<uint32>(mValue.Integer);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
