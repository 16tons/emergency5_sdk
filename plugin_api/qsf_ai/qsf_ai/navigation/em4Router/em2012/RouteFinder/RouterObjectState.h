// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/Export.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*!
		 * \brief Simple storage class for object states used in the router.
		 * An object state consists of a position and a rotation.
		 */
		class QSF_AI_API_EXPORT ERouterObjectState
		{
		public:
			ERouterObjectState() {}
			ERouterObjectState(const glm::vec3& position_, const glm::mat3x3& rotation_) :
				mPosition(position_), mRotation(rotation_)
			{
			}
			ERouterObjectState(const glm::vec3& position_, const float direction_);
			ERouterObjectState(const glm::vec3& position_, const glm::quat &rotation_);

			ERouterObjectState(const SpatialConfiguration3D& configuration_);

			// TODO(co) No (save games) serialization required
			/*
			bool SerializeTo(ESerializer *stream_) const
			{
			stream_.serialize(mPosition);
			stream_.serialize(mRotation);
			return true;
			}

			bool SerializeFrom(ESerializer *stream_)
			{
			stream_->Read(mPosition);
			stream_->Read(mRotation);
			return true;
			}

			bool Save(BLFile *file_);
			bool Load(BLFile *file_);
			*/

			void SetPosition(const glm::vec3& pos_) { mPosition = pos_; }
			inline const glm::vec3&  GetPosition() const { return mPosition; }
			void SetRotation(const glm::mat3x3& rot_) { mRotation = rot_; }
			inline const glm::mat3x3&  GetRotation() const { return mRotation; }

		private:
			glm::vec3 mPosition;
			glm::mat3x3 mRotation;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
