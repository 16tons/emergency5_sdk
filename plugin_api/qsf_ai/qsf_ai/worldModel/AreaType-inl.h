// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline AreaType::AreaType(unsigned int id) :
			mId(id)
		{
			mCompatibleTypes.insert(id);
		}

		inline unsigned int AreaType::getId() const
		{
			return mId;
		}

		inline void AreaType::addCostFactor(unsigned int moverType, bool facingEndNode, UnsignedFloat factor)
		{
			const auto& insertionResult = mCostFactors.emplace(DataKey(moverType, facingEndNode), factor);
			if (!insertionResult.second)
			{
				QSF_WARN("Overwriting cost factor for mover type " << moverType << " facing " << (facingEndNode ? " forward " : " backward "),
					QSF_REACT_NONE);

				insertionResult.first->second = factor;
			}
		}

		inline const UnsignedFloat* AreaType::tryGetCostFactorFor(unsigned int moverType, bool facingEndNode) const
		{
			const auto& storedFactor = mCostFactors.find(DataKey(moverType, facingEndNode));
			if (storedFactor == mCostFactors.end())
				return nullptr;

			return &storedFactor->second;
		}

		inline const AreaType::SpeedImpact* AreaType::tryGetSpeedImpactFor(unsigned int moverType) const
		{
			const auto& storedFactor = mSpeedFactors.find(moverType);
			if (storedFactor == mSpeedFactors.end())
				return nullptr;

			return &storedFactor->second;
		}

		inline const std::string& AreaType::getName() const
		{
			return mName;
		}

		inline void AreaType::setName(const std::string& name)
		{
			mName = name;
		}

		inline bool AreaType::isValid() const
		{
			return isInitialized(mId);
		}

		inline void AreaType::addCompatibleType(unsigned int areaType)
		{
			mCompatibleTypes.insert(areaType);
		}

		inline bool AreaType::isAreaTypeCompatible(unsigned int areaType) const
		{
			return mCompatibleTypes.count(areaType) != 0;
		}

		inline bool AreaType::canBeUsedAtAll(unsigned int moverType) const
		{
			return tryGetCostFactorFor(moverType, true) || tryGetCostFactorFor(moverType, false);
		}
	}
}
