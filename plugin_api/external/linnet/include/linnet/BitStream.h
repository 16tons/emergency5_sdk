/*=============================================================================
Description: the bit stream interface

Revision history:
* Created by Lin Luo, November 2, 2005
=============================================================================*/

#pragma once

#include "Common.h"

//!< desc: the bit stream interface
class IBitStream {
public:
	//!< the static constructor to create an empty read-write bit stream object
	static IBitStream* CreateInstance();

	//!< the static constructor to create a bit stream through a list of buckets (probably from the network)
	static IBitStream* CreateInstance(const std::vector<BUCKET*>& buckets_, size_t BitLength_);

	//!< the static constructor to create an bit stream that is an exact copy of another bit stream
	static IBitStream* CreateInstance(const IBitStream* other_);

	static void CreateCompressionEncoder();
	static void DestroyComptressionEncode();

	//!< the final method to release the current bit stream
	void Release();

	//!< resets the read write position of the stream (without purging the existing memory)
	enum EResetHint {RESET_ALL, RESET_READ, RESET_WRITE};
	virtual void Reset(EResetHint hint_ = RESET_ALL) = 0;

	//!< retieves the current size of the bit stream in bits
	virtual size_t GetBitLength() const = 0;

	//!< sets the read position in bits
	virtual void SetReadPosition(size_t position_) const = 0;
	
	//!< retieves the current read position the bit stream in bits
	virtual size_t GetReadPosition() const = 0;

	//!< writes a bool value_ in 1 bit
	virtual void Write(bool value_) = 0;

	//!< reads a 1-bit bool value_
	virtual bool Read(bool& value_) const = 0;

	//!< writes a byte value_ in number_ bits
	virtual void Write(uint8_t value_, size_t number_ = 8) = 0;

	//!< reads an unsigned byte value_ in number_ bits
	virtual bool Read(uint8_t& value_, size_t number_ = 8) const = 0;

	//!< writes a signed char value_ in number_ bits (including a sign indicating bit)
	virtual void Write(int8_t value_, size_t number_ = 8) = 0;

	//!< reads a signed char value_ in number_ bits (including a sign indicating bit)
	virtual bool Read(int8_t& value_, size_t number_ = 8) const = 0;

	//!< writes an unsigned 16bit value_ in number_ bits
	virtual void Write(uint16_t value_, size_t number_ = 16) = 0;

	//!< reads an unsigned 16bit value_ in number_ bits
	virtual bool Read(uint16_t& value_, size_t number_ = 16) const = 0;

	//!< writes a signed 16bit value_ in number_ bits (including a sign indicating bit)
	virtual void Write(int16_t value_, size_t number_ = 16) = 0;

	//!< reads a signed 16bit value_ in number_ bits (including a sign indicating bit)
	virtual bool Read(int16_t& value_, size_t number_ = 16) const = 0;

	//!< writes an unsigned 32bit value_ in number_ bits
	virtual void Write(uint32_t value_, size_t number_ = 32) = 0;

	//!< reads an unsigned 32bit value_ in number_ bits
	virtual bool Read(uint32_t& value_, size_t number_ = 32) const = 0;

	//!< writes a signed 32bit value_ in number_ bits (including a sign indicating bit)
	virtual void Write(int32_t value_, size_t number_ = 32) = 0;

	//!< reads a signed 32bit value_ in number_ bits (including a sign indicating bit)
	virtual bool Read(int32_t& value_, size_t number_ = 32) const = 0;

	//!< writes an unsigned 64bit value_ in number_ bits
	virtual void Write(uint64_t value_, size_t number_ = 64) = 0;

	//!< reads an unsigned 64bit value_ in number_ bits
	virtual bool Read(uint64_t& value_, size_t number_ = 64) const = 0;

	//!< writes a signed 64bit value_ in number_ bits (including a sign indicating bit)
	virtual void Write(int64_t value_, size_t number_ = 64) = 0;

	//!< reads a signed 64bit value_ in number_ bits (including a sign indicating bit)
	virtual bool Read(int64_t& value_, size_t number_ = 64) const = 0;

	//!< writes a complete float value_
	virtual void Write(float value_) = 0;

	//!< reads a complete float value_
	virtual bool Read(float& value_) const = 0;

	//!< writes a complete double value_
	virtual void Write(double value_) = 0;

	//!< reads a complete double value_
	virtual bool Read(double& value_) const = 0;

	//!< writes a byte array in length_ bytes
	virtual void Write(byte buffer_[], size_t length_) = 0;

	//!< reads a byte array in length_ bytes
	virtual bool Read(byte buffer_[], size_t length_) const = 0;

	//!< writes number_ bits of stream_ into "this" stream
	virtual void Write(const IBitStream* stream_, size_t number_) = 0;

	//!< reads number_ bits of "this" stream into stream_
	virtual bool Read(IBitStream* stream_, size_t number_) const = 0;

	//!< retrieves the internal buffer for special purposes (for network transmission etc.)
	virtual const std::vector<BUCKET*>& RetrieveInternalBuffer() const = 0;

	//!< creates a compressed copy of this bit stream
	virtual IBitStream *HuffmanCompress() const = 0;

	//!< creates an uncompressed copy of this bit stream
	virtual IBitStream *HuffmanUncompress() const = 0;

#ifdef _DEBUG
	//!< dumps the content of the bit stream
	virtual void Dump() const = 0;
#endif // _DEBUG

protected:
	virtual ~IBitStream() {}

	static class HuffmanEncodingTree *GetHuffmanEncoder();

private:
	static class HuffmanEncodingTree *mHuffmanEncoder;
};


class BucketByteArray {
	friend class CBitStream; // CBitStream::RetrieveInternalBuffer

public:
	BucketByteArray()
	{
		mWriteOffset = 0;
	}

	BucketByteArray(const std::vector<BUCKET*>& buckets_)
	{
		for ( size_t i = 0; i < buckets_.size(); ++i )
		{
			BUCKET* bucket = MEMORYPOOL_ALLOCATE(GBucketPool); //GBucketPool.Allocate();
			memcpy(bucket, buckets_[i], BUCKETSIZE);
			mBucketList.push_back(bucket);
		}

		mWriteOffset = -1;	// writes are illegal, because we don't know where the data ends
	}

	BucketByteArray(const BucketByteArray& other_)
	{
		for ( size_t i = 0; i < other_.mBucketList.size(); ++i )
		{
			BUCKET* bucket = MEMORYPOOL_ALLOCATE(GBucketPool); //GBucketPool.Allocate();
			memcpy(bucket, other_.mBucketList[i], BUCKETSIZE);
			mBucketList.push_back(bucket);
		}

		mWriteOffset = -1;	// writes are illegal, because we don't know where the data ends
	}

	~BucketByteArray()
	{
		for ( size_t i = 0; i < mBucketList.size(); ++i )
		{
			GBucketPool.Free(mBucketList[i]);
			mBucketList[i] = 0;
		}
	}

	//!< clears all buckets
	void clear()
	{
		for ( size_t i = 0; i < mBucketList.size(); ++i )
			GBucketPool.Free(mBucketList[i]);
		mBucketList.clear();
	}

	//!< expands the buffer by the number of buckets given
	void grow(size_t number_)
	{
		for (size_t i = 0; i < number_; ++i)
		{
			BUCKET* bucket = MEMORYPOOL_ALLOCATE(GBucketPool); //GBucketPool.Allocate();
			mBucketList.push_back(bucket);
		}
	}

	//!< retrieves the size in bytes
	size_t size() const
	{
		return mBucketList.size() * BUCKETSIZE;
	}

	//!< retrieves the byte at a given byte index
	byte& operator[](size_t index_)
	{
		if ( index_ >= mBucketList.size() * BUCKETSIZE )
			return mDummy;
		size_t BucketIndex = index_ / BUCKETSIZE;
		size_t ByteOffset = index_ % BUCKETSIZE;
		return mBucketList[BucketIndex]->buffer[ByteOffset];
	}

	//!< retrieves the byte at a given byte index
	const byte& operator[](size_t index_) const
	{
		if ( index_ >= mBucketList.size() * BUCKETSIZE )
			return mDummy;
		size_t BucketIndex = index_ / BUCKETSIZE;
		size_t ByteOffset = index_ % BUCKETSIZE;
		return mBucketList[BucketIndex]->buffer[ByteOffset];
	}

	//!< retrieves the number of buckets
	size_t number() const
	{
		return mBucketList.size();
	}

	//!< retrieves the bucket at a given index
	BUCKET* bucket(size_t index_)
	{
		return mBucketList[index_];
	}

	//!< retrieves the bucket at a given index
	const BUCKET* bucket(size_t index_) const
	{
		return mBucketList[index_];
	}

	//!< writes a byte to the end of the bucket list
	void push_back(byte value_)
	{
		// writing is only allowed if the bucket list is created empty
		assert(mWriteOffset >= 0);
		if (static_cast<int>(mBucketList.size()) * BUCKETSIZE <= mWriteOffset)
		{
			assert(mBucketList.size() * BUCKETSIZE == mWriteOffset);
			grow(1);
			assert(static_cast<int>(mBucketList.size()) * BUCKETSIZE > mWriteOffset);
		}

		int lastBucketIndex = static_cast<int>(mBucketList.size()) - 1;
		assert(lastBucketIndex >= 0);
		BUCKET *lastBucket = mBucketList[lastBucketIndex];
		int bucketOffset = mWriteOffset % BUCKETSIZE;
		lastBucket->buffer[bucketOffset] = value_;
		mWriteOffset++;
	}

	//!< retrieves the number of bytes written to the bucket list
	int number_of_bytes_written() const
	{
		// this info is only available if the bucket list is created empty
		assert(mWriteOffset >= 0);
		return mWriteOffset;
	}

#ifdef _DEBUG
	void dump() const
	{
		for ( size_t i = 0; i < mBucketList.size(); ++i )
		{
			byte* buffer = mBucketList[i]->buffer;
			for (size_t j = 0; j < BUCKETSIZE; ++j)
				printf("%02x ", buffer[j]);
			printf("\n");
		}
	}
#endif // _DEBUG

private:
	std::vector<BUCKET*> mBucketList;
	byte mDummy;
	int mWriteOffset;
};
