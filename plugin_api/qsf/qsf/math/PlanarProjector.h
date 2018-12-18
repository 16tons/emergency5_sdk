// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Axis.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	/**
	* A planar converter helps to convert coordinates between 2D and 3D continuous space.
	* It allows to insert or remove vector components based on a coordinate system configuration.
	*/
	template <typename ValueType = float>
	class PlanarProjector
	{
	public:
		friend class CoordinateSystem; // Access planar projectors through the coordinate system.

		// Comfort typedef for the glm vec3 with the defined component type
		typedef glm::tvec3<ValueType, glm::defaultp> Vector3;
		// Comfort typedef for the glm vec2 with the defined component type
		typedef glm::tvec2<ValueType, glm::defaultp> Vector2;

		// Create a vector3 from a vector2 by inserting the value for the conversion axis component.
		Vector3 insertValue(const Vector2& vector, ValueType valueToInsert) const;

		// Create a vector2 from a vector3 by dropping the value for the conversion axis component.
		Vector2 dropValue(const Vector3& vector) const;

		// Overwrite the conversion axis component in a vector3
		void setValue(Vector3& vector, ValueType value) const;

		// return a copy of the vector with the value replacing the conversion axis component
		Vector3 copyReplaceWith(const Vector3& vector, ValueType value) const;

		// Retrieve the value for the conversion axis component from a vector3.
		ValueType getValue(const Vector3& vector) const;

		// Return a reference to the conversion axis value for manipulating it
		ValueType& accessValue(Vector3& vector) const;

		// Transfer the value for the conversion axis from valueProvider to ValueReceiver.
		// Only a shortcut for setValue(valueReceiver, getValue(valueProvider)
		void transferValue(const Vector3& valueProvider, Vector3& valueReceiver) const;

		// create a vector with the value at the conversion axis position and the other parts are zero
		Vector3 makeVector(ValueType value) const;

		// Retrieve a unit vector for the conversion axis.
		Vector3 getConversionAxisUnitVector() const;

		// Create a perpendicular in 2D that is turned 90 degrees to the left
		Vector2 getLeftPerpendicular(const glm::tvec2<ValueType, glm::defaultp>& v) const;
		// Create a perpendicular in 2D that is turned 90 degrees to the right
		Vector2 getRightPerpendicular(const glm::tvec2<ValueType, glm::defaultp>& v) const;

		// Returns false for 180 degree turns - these are considered neither clockwise nor counter clockwise
		bool isClockwiseTurn(const Vector2& dir1, const Vector2& dir2) const;
		// Returns false for 180 degree turns - these are considered neither clockwise nor counter clockwise
		bool isCounterClockwiseTurn(const Vector2& dir1, const Vector2& dir2) const;

		// Returns whether the 2D coordinate system is right handed or left handed in nature
		bool isRightHanded() const;

		// Returns 1.f or -1.f as a sign factor to be multiplied to a turn in direction as signaled by the clockwise flag
		float getAngleDirectionFactor(bool clockwise) const;

		// Return the axis that is used to remove or modify values
		axis::TwoDimensionalUnsigned getConversionAxis() const;

	private:
		// Pass the axis to remove or insert when using this converter
		// Private because only the coordinate system should create these.
		PlanarProjector(axis::TwoDimensionalUnsigned conversionAxis, bool rightHanded);

		axis::TwoDimensionalUnsigned mConversionAxis;
		bool mIsRightHanded; // Store whether the resulting 2D coordinate plane is right handed or left handed for purposes of determining turning directions
	};


	// inline implementation
	template <typename ValueType>
	PlanarProjector<ValueType>::PlanarProjector(axis::TwoDimensionalUnsigned conversionAxis, bool rightHanded) :
		mConversionAxis(conversionAxis),
		mIsRightHanded(rightHanded)
	{}

	template <typename ValueType>
	glm::tvec3<ValueType, glm::defaultp> PlanarProjector<ValueType>::insertValue(const glm::tvec2<ValueType, glm::defaultp>& vector, ValueType valueToInsert) const
	{
		glm::tvec3<ValueType, glm::defaultp> result;

		const ValueType* vectorIterator = &vector[0];
		for (glm::length_t component = 0; component < result.length(); ++component)
		{
			if (component == mConversionAxis)
				result[component] = valueToInsert;
			else
				result[component] = *(vectorIterator++);
		}

		return result;
	}

	template <typename ValueType>
	glm::tvec2<ValueType, glm::defaultp> PlanarProjector<ValueType>::dropValue(const glm::tvec3<ValueType, glm::defaultp>& vector) const
	{
		glm::tvec2<ValueType, glm::defaultp> result;

		ValueType* resultIterator = &result[0];
		for (glm::length_t component = 0; component < vector.length(); ++component)
		{
			if (component == mConversionAxis)
				continue;

			*(resultIterator++) = vector[component];
		}

		return result;
	}

	template <typename ValueType>
	ValueType PlanarProjector<ValueType>::getValue(const glm::tvec3<ValueType, glm::defaultp>& vector) const
	{
		return vector[mConversionAxis];
	}

	template <typename ValueType>
	ValueType& PlanarProjector<ValueType>::accessValue(glm::tvec3<ValueType, glm::defaultp>& vector) const
	{
		return vector[mConversionAxis];
	}

	template <typename ValueType>
	glm::tvec3<ValueType, glm::defaultp> PlanarProjector<ValueType>::makeVector(ValueType value) const
	{
		glm::tvec3<ValueType, glm::defaultp> result;
		setValue(result, value);

		return result;
	}

	template <typename ValueType>
	glm::tvec3<ValueType, glm::defaultp> PlanarProjector<ValueType>::getConversionAxisUnitVector() const
	{
		glm::tvec3<ValueType, glm::defaultp> unitVector;
		unitVector[mConversionAxis] = static_cast<ValueType>(1);

		return unitVector;
	}

	template <typename ValueType>
	void PlanarProjector<ValueType>::setValue(glm::tvec3<ValueType, glm::defaultp>& vector, ValueType value) const
	{
		vector[mConversionAxis] = value;
	}

	template <typename ValueType>
	glm::tvec3<ValueType, glm::defaultp> PlanarProjector<ValueType>::copyReplaceWith(const glm::tvec3<ValueType, glm::defaultp>& vector, ValueType value) const
	{
		glm::tvec3<ValueType, glm::defaultp> copy(vector);
		setValue(copy, value);
		return copy;
	}

	template <typename ValueType>
	glm::tvec2<ValueType, glm::defaultp> PlanarProjector<ValueType>::getLeftPerpendicular(const glm::tvec2<ValueType, glm::defaultp>& v) const
	{
		if (mIsRightHanded)
			return glm::tvec2<ValueType, glm::defaultp>(-v.y, v.x);
		else
			return glm::tvec2<ValueType, glm::defaultp>(v.y, -v.x);
	}

	template <typename ValueType>
	glm::tvec2<ValueType, glm::defaultp> PlanarProjector<ValueType>::getRightPerpendicular(const glm::tvec2<ValueType, glm::defaultp>& v) const
	{
		if (mIsRightHanded)
			return glm::tvec2<ValueType, glm::defaultp>(v.y, -v.x);
		else
			return glm::tvec2<ValueType, glm::defaultp>(-v.y, v.x);
	}

	template <typename ValueType>
	bool PlanarProjector<ValueType>::isClockwiseTurn(const glm::tvec2<ValueType, glm::defaultp>& dir1, const glm::tvec2<ValueType, glm::defaultp>& dir2) const
	{
		const glm::tvec2<ValueType, glm::defaultp> dir1Perp = getRightPerpendicular(dir1);
		return (dot(dir1Perp, dir2) > 0.0f);
	}

	template <typename ValueType>
	bool PlanarProjector<ValueType>::isCounterClockwiseTurn(const glm::tvec2<ValueType, glm::defaultp>& dir1, const glm::tvec2<ValueType, glm::defaultp>& dir2) const
	{
		const glm::tvec2<ValueType, glm::defaultp> dir1Perp = getLeftPerpendicular(dir1);
		return (dot(dir1Perp, dir2) > 0.0f);
	}

	template <typename ValueType>
	bool PlanarProjector<ValueType>::isRightHanded() const
	{
		return mIsRightHanded;
	}

	template <typename ValueType>
	float PlanarProjector<ValueType>::getAngleDirectionFactor(bool clockwise) const
	{
		return mIsRightHanded != clockwise ? 1.f : -1.f;
	}

	template <typename ValueType>
	void PlanarProjector<ValueType>::transferValue(const Vector3& valueProvider, Vector3& valueReceiver) const
	{
		setValue(valueReceiver, getValue(valueProvider));
	}

	template <typename ValueType>
	axis::TwoDimensionalUnsigned PlanarProjector<ValueType>::getConversionAxis() const
	{
		return mConversionAxis;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
