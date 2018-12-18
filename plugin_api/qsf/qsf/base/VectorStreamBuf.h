// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <streambuf>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	template<typename CharT, typename TraitsT = std::char_traits<CharT> >
	class BaseVectorStreamBuf : public std::basic_streambuf<CharT, TraitsT>
	{
	public:
		typedef typename std::basic_streambuf<CharT, TraitsT>::traits_type traits_type;

		explicit BaseVectorStreamBuf(std::vector<CharT>& vector) :
			mVector(vector),
			mIsReadOnly(false)
		{
			this->setg(mVector.data(), mVector.data(), mVector.data() + mVector.size());
			this->setp(mVector.data(), mVector.data() + mVector.size());
		}

		explicit BaseVectorStreamBuf(const std::vector<CharT>& vector) :
			mVector(const_cast<std::vector<CharT>&>(vector)),
			mIsReadOnly(true)
		{
			this->setg(mVector.data(), mVector.data(), mVector.data() + mVector.size());
		}

		virtual std::streampos seekoff(std::streamoff offset, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override
		{
			std::streampos result = std::streampos(std::streamoff(-1));

			switch (way)
			{
				case std::ios_base::beg:
					result = seekpos(offset, which);
					break;

				case std::ios_base::end:
					result = seekpos(mVector.size() + offset, which);
					break;

				case std::ios_base::cur:
				default:
					const bool useWritePosition = ((which & std::ios_base::out) != 0);
					if (!useWritePosition || !mIsReadOnly)
					{
						const std::streampos basePosition = (useWritePosition ? (this->pptr() - this->pbase()) : (this->gptr() - this->eback()));
						result = seekpos(basePosition + offset, which);
					}
			}

			return result;
		}

		virtual std::streampos seekpos(std::streampos position, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override
		{
			std::streampos result = std::streampos(std::streamoff(-1));

			if (position >= 0)
			{
				const bool setWritePosition = !mIsReadOnly && ((which & std::ios_base::out) != 0);
				const bool setReadPosition = ((which & std::ios_base::in) != 0);

				const size_t newPosition = static_cast<size_t>(position);

				if (setWritePosition)
				{
					if (newPosition > mVector.size())
					{
						mVector.resize(newPosition);
					}

					const std::streampos readPosition = setReadPosition ? position : static_cast<std::streampos>(this->gptr() - this->eback());

					this->setp(mVector.data(), mVector.data() + mVector.size());
					this->pbump(static_cast<int>(position));

					this->setg(mVector.data(), mVector.data() + static_cast<int>(readPosition), mVector.data() + mVector.size());

					result = position;
				}
				else if (setReadPosition && newPosition <= mVector.size())
				{
					this->setg(mVector.data(), mVector.data() + static_cast<int>(position), mVector.data() + mVector.size());

					result = position;
				}
			}

			return result;
		}

		virtual std::streamsize showmanyc() override
		{
			return mVector.size() - (this->gptr() - this->eback());
		}

		virtual std::streamsize xsputn(const char* s, std::streamsize n) override
		{
			std::streamsize result = 0;

			if (!mIsReadOnly)
			{
				std::streamoff remainingSpace = this->epptr() - this->pptr();

				if (remainingSpace < n)
				{
					const std::streampos readPosition = this->gptr() - this->eback();
					const std::streampos writePosition = this->pptr() - this->pbase();

					mVector.resize(static_cast<std::size_t>(mVector.size() - remainingSpace + n));

					this->setp(mVector.data(), mVector.data() + mVector.size());
					this->pbump(static_cast<int>(writePosition));

					this->setg(mVector.data(), mVector.data() + static_cast<int>(readPosition), mVector.data() + mVector.size());

					remainingSpace = this->epptr() - this->pptr();
				}

				if (n != 0 && n <= remainingSpace)
				{
					memcpy(this->pptr(), s, static_cast<rsize_t>(n));
					this->pbump(static_cast<int>(n));
					result = n;
				}
				else
				{
					n = 0;
				}
			}

			return result;
		}

		virtual int overflow(int c = traits_type::eof()) override
		{
			if (!mIsReadOnly)
			{
				if (c != traits_type::eof())
				{
					const std::streamoff remainingSpace = this->epptr() - this->pptr();
					if (remainingSpace == 0)
					{
						const std::streampos readPosition = this->gptr() - this->eback();
						const std::streampos writePosition = this->pptr() - this->pbase();

						mVector.push_back(c);

						this->setp(mVector.data(), mVector.data() + mVector.size());
						this->pbump(static_cast<int>(writePosition));

						this->setg(mVector.data(), mVector.data() + static_cast<int>(readPosition), mVector.data() + mVector.size());
					}
					else if (remainingSpace > 0)
					{
						mVector[this->pptr() - this->pbase()] = c;
					}
					else
					{
						c = traits_type::eof();
					}
				}
			}
			else
			{
				c = traits_type::eof();
			}

			return traits_type::to_int_type(c);
		}


	private:
		std::vector<CharT>& mVector;
		bool mIsReadOnly;
	};

	template<typename CharT, typename TraitsT = std::char_traits<CharT> >
	class BaseVectorIOStream : public std::basic_iostream<CharT, TraitsT>
	{
	public:
		explicit BaseVectorIOStream(std::vector<CharT>& vector) :
			std::basic_iostream<CharT, TraitsT>(nullptr),
			mStreamBuf(vector)
		{
			this->rdbuf(&mStreamBuf);
		}


	private:
		BaseVectorStreamBuf<CharT, TraitsT> mStreamBuf;


	};

	template<typename CharT, typename TraitsT = std::char_traits<CharT> >
	class BaseVectorOStream : public std::basic_ostream<CharT, TraitsT>
	{
	public:
		explicit BaseVectorOStream(std::vector<CharT>& vector) :
			std::basic_ostream<CharT, TraitsT>(nullptr),
			mStreamBuf(vector)
		{
			this->rdbuf(&mStreamBuf);
		}


	private:
		BaseVectorStreamBuf<CharT, TraitsT> mStreamBuf;


	};

	template<typename CharT, typename TraitsT = std::char_traits<CharT> >
	class BaseVectorIStream : public std::basic_istream<CharT, TraitsT>
	{
	public:
		explicit BaseVectorIStream(const std::vector<CharT>& vector) :
			std::basic_istream<CharT, TraitsT>(nullptr),
			mStreamBuf(vector)
		{
			this->rdbuf(&mStreamBuf);
		}


	private:
		BaseVectorStreamBuf<CharT, TraitsT> mStreamBuf;


	};


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef BaseVectorStreamBuf<char> VectorStreamBuf;
	typedef BaseVectorIOStream<char> VectorIOStream;
	typedef BaseVectorOStream<char> VectorOStream;
	typedef BaseVectorIStream<char> VectorIStream;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
