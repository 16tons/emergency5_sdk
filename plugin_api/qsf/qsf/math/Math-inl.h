// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	template <typename Real>
	Real Math::clamp(Real value, Real minimum, Real maximum)
	{
		return (value <= minimum) ? minimum : (value >= maximum) ? maximum : value;
	}

	inline float Math::saturate(float value)
	{
		return (value <= 0.0f) ? 0.0f : (value >= 1.0f) ? 1.0f : value;
	}

	inline bool Math::isPowerOfTwo(int value)
	{
		// 0 is not a power of 2, so we need to perform an additional test to catch this special case
		return (0 == value) ? false : ((value & (value - 1)) == 0);
	}

	/**
	* Tests each component of a vector for whether it is not nan or infinite.
	*/
	template <typename Vector>
	bool isValid(const Vector& v)
	{
		for (glm::length_t component = 0; component < v.length(); ++component)
		{
			if (v[component] != v[component]) // nan check
				return false;

			if (std::abs(v[component]) == std::numeric_limits<typename Vector::value_type>::infinity()) // inf check
				return false;
		}

		return true;
	}

	/**
	* Tests if an quaternion has valid members
	*/
	inline bool isQuaternionValid(const glm::quat& q)
	{
		for (glm::length_t component = 0; component < q.length(); ++component)
		{
			if (q[component] != q[component]) // nan check
				return false;

			if (std::abs(q[component]) == std::numeric_limits<float>::infinity()) // inf check
				return false;
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
