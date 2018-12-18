// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/material/EditedMaterialInstance.h"

#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		class Query
		{
		public:
			Query() :
				mDone(false)
			{}

			virtual ~Query()
			{}

			virtual void execute();

			bool mDone;

		protected:
			virtual void executeImpl() = 0;
		};

		class EditedMaterialQuery : public Query
		{
		public:
			EditedMaterialQuery(const std::string& name) :
				mMaterialInstance(nullptr),
				mName(name)
			{}

			virtual void execute() override;

			EditedMaterialInstance* mMaterialInstance;

			std::string mName;
		};

		class MaterialQuery : public EditedMaterialQuery, public MaterialInfo
		{
		public:
			MaterialQuery(const std::string& name) :
				EditedMaterialQuery(name)
			{}

		protected:
			virtual void executeImpl() override;
		};

		class MaterialPropertyQuery : public EditedMaterialQuery
		{
		public:
			MaterialPropertyQuery(const std::string& name, const std::string& propertyName) :
				EditedMaterialQuery(name)
			{}

			std::string mPropertyName;
			std::string mValue;

		protected:
			virtual void executeImpl() override;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
