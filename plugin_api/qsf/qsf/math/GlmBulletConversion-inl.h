// Copyright (C) 2012-2017 Promotion Software GmbH

namespace qsf
{
	inline glm::vec3 convertVector3(const btVector3& in)
	{
		return glm::vec3(in.x(), in.y(), in.z());
	}

	inline btVector3 convertVector3(const glm::vec3& in)
	{
		return btVector3(in.x, in.y, in.z);
	}

	inline btQuaternion convertQuaternion(const glm::quat& in)
	{
		return btQuaternion(in.x, in.y, in.z, in.w);
	}

	inline glm::quat convertQuaternion(const btQuaternion& in)
	{
		return glm::quat(in.w(), in.x(), in.y(), in.z());
	}
}
