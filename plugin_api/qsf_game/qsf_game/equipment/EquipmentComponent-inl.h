// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline EquipmentComponent::EquipmentComponent(Prototype* prototype) :
			Component(prototype),
			mEquipFromVehicle(false),
			mEquipmentType(EQUIPMENTTYPE_RIGHTHANDED),
			mUseSharedAnimation(true),
			ParticlesArray(*this, mParticlesIds)
		{
			// Nothing to do in here
		}

		inline EquipmentComponent::~EquipmentComponent()
		{
			// Nothing to do in here
		}

		inline const std::string& EquipmentComponent::getEquipmentName() const
		{
			return mEquipmentName;
		}

		inline void EquipmentComponent::setEquipmentName(const std::string& name)
		{
			mEquipmentName = name;
		}

		inline const glm::vec3& EquipmentComponent::getPositionOffset() const
		{
			return mPositionOffset;
		}

		inline void EquipmentComponent::setPositionOffset(const glm::vec3& offset)
		{
			mPositionOffset = offset;
		}

		inline bool EquipmentComponent::getEquipFromVehicle() const
		{
			return mEquipFromVehicle;
		}

		inline void EquipmentComponent::setEquipFromVehicle(bool equipFromVehicle)
		{
			mEquipFromVehicle = equipFromVehicle;
		}

		inline const AssetProxy& EquipmentComponent::getIdleAnimation() const
		{
			return mIdleAnimation;
		}

		inline void EquipmentComponent::setIdleAnimation(const AssetProxy& idleAnimation)
		{
			mIdleAnimation = idleAnimation;
		}

		inline const AssetProxy& EquipmentComponent::getRunAnimation() const
		{
			return mRunAnimation;
		}

		inline void EquipmentComponent::setRunAnimation(const AssetProxy& runAnimation)
		{
			mRunAnimation = runAnimation;
		}

		inline EquipmentComponent::EquipmentType EquipmentComponent::getEquipmentType() const
		{
			return mEquipmentType;
		}

		inline void EquipmentComponent::setEquipmentType(EquipmentComponent::EquipmentType equipmentType)
		{
			mEquipmentType = equipmentType;
		}

		inline bool EquipmentComponent::getUseSharedAnimation() const
		{
			return mUseSharedAnimation;
		}

		inline void EquipmentComponent::setUseSharedAnimation(bool useSharedAnimation)
		{
			mUseSharedAnimation = useSharedAnimation;
		}

		inline const std::string& EquipmentComponent::getLocalizedEquipmentId() const
		{
			return mLocalizedEqiupmentId;
		}

		inline void EquipmentComponent::setLocalizedEquipmentId(const std::string& localizedEquipmentId)
		{
			mLocalizedEqiupmentId = localizedEquipmentId;
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t EquipmentComponent::EntityIdArrayInterface::size() const
		{
			return getVariable().size();
		}

		inline const uint64& EquipmentComponent::EntityIdArrayInterface::get(std::size_t index) const
		{
			return getVariable()[index];
		}

		inline void EquipmentComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
		{
			getVariable()[index] = value;
		}

		inline void EquipmentComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
		{
			getVariable().insert(getVariable().begin() + before, value);
		}

		inline void EquipmentComponent::EntityIdArrayInterface::remove(std::size_t index)
		{
			getVariable().erase(getVariable().begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
