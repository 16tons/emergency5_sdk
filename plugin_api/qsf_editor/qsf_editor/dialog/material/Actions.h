// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		class Action
		{
		public:
			virtual void execute() = 0;
			virtual ~Action() {}
		};

		class ActionDeleteMaterial : public Action
		{
		public:
			ActionDeleteMaterial(const std::string& name) :
				mName(name)
			{}

			virtual void execute() override;
		private:
			std::string mName;
		};

		class ActionSaveAll : public Action
		{
		public:
			virtual void execute() override;
		};

		// Material

		class ActionSetMaterialProperty : public Action
		{
		public:
			ActionSetMaterialProperty(const std::string& name, const std::string& key, const std::string& value) :
				mName(name),
				mKey(key),
				mValue(value)
			{}

			virtual void execute() override;
		private:
			std::string mName;
			std::string mKey;
			std::string mValue;
		};

		class ActionDeleteMaterialProperty : public Action
		{
		public:
			ActionDeleteMaterialProperty(const std::string& name, const std::string& key) :
				mName(name),
				mKey(key)
			{}

			virtual void execute() override;
		private:
			std::string mName;
			std::string mKey;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
