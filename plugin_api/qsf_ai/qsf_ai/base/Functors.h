// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

// Contains some common functors
namespace qsf
{
	template <typename T>
	struct SelectById
	{
	public:
		SelectById(unsigned int id) :
			mId(id)
		{}

		bool operator () (const T& t) const
		{
			return t.getId() == mId;
		}

		bool operator () (const T* t) const
		{
			return t->getId() == mId;
		}
	private:
		unsigned int mId;
	};
}
