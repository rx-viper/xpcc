// coding: utf-8
/* Copyright (c) 2017, Raphael Lehmann
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_BLOCK_DEVICE_HEAP_HPP
#define XPCC_BLOCK_DEVICE_HEAP_HPP

#include <xpcc/architecture/interface/block_device.hpp>

#include <xpcc/processing/resumable.hpp>

namespace xpcc
{

/**
 * \brief	Block device using heap (RAM)
 *
 * Usually not persistant, but at least useful for testing.
 * This is also useful for battery backed SRAM, some STM32 have up to 4kB of this stuff.
 *
 * \tparam	DeviceSize The size of the block device
 * \tparam	externalMemory Set to true to use an external supplied memory block (see `initialize(uint8_t*)`)
 *
 * \ingroup	driver_storage
 * \author	Raphael Lehmann
 */
template <size_t DeviceSize_, bool externalMemory = false>
class BdHeap : public xpcc::BlockDevice, protected xpcc::NestedResumable<3>
{
public:
	/// Initializes the memory with zeros
	xpcc::ResumableResult<bool>
	initialize();

	/// Data will be stored at externalMemoryAddress
	xpcc::ResumableResult<bool>
	initialize(uint8_t* externalMemoryAddress);

	/// Deinitializes nothing
	xpcc::ResumableResult<bool>
	deinitialize();

	/** Read data from one or more blocks
	 *
	 *  @param buffer	Buffer to read data into
	 *  @param address	Address to begin reading from
	 *  @param size		Size to read in bytes (multiple of read block size)
	 *  @return			True on success
	 */
	xpcc::ResumableResult<bool>
	read(uint8_t* buffer, bd_address_t address, bd_size_t size);

	/** Program blocks with data
	 *
	 *  Any block has to be erased prior to being programmed
	 *
	 *  @param buffer	Buffer of data to write to blocks
	 *  @param address	Address of first block to begin writing to
	 *  @param size		Size to write in bytes (multiple of read block size)
	 *  @return			True on success
	 */
	xpcc::ResumableResult<bool>
	program(const uint8_t* buffer, bd_address_t address, bd_size_t size);

	/** Erase blocks
	 *
	 *  The state of an erased block is undefined until it has been programmed
	 *
	 *  @param address	Address of block to begin erasing
	 *  @param size		Size to erase in bytes (multiple of read block size)
	 *  @return			True on success
	 */
	xpcc::ResumableResult<bool>
	erase(bd_address_t address, bd_size_t size);

	/** Writes data to one or more blocks after erasing them
	*
	*  The blocks are erased prior to being programmed
	*
	*  @param buffer	Buffer of data to write to blocks
	*  @param address	Address of first block to begin writing to
	*  @param size		Size to write in bytes (multiple of read block size)
	*  @return			True on success
	*/
	xpcc::ResumableResult<bool>
	write(const uint8_t* buffer, bd_address_t address, bd_size_t size);

public:
	static constexpr bd_size_t BlockSizeRead = 1;
	static constexpr bd_size_t BlockSizeWrite = 1;
	static constexpr bd_size_t BlockSizeErase = 1;
	static constexpr bd_size_t DeviceSize = DeviceSize_;

public:
	void
	setMemoryAddress(uint8_t* memory);

private:
	typename xpcc::tmp::Select<externalMemory, uint8_t*, uint8_t[DeviceSize]>::Result data;
};

}
#include "block_device_heap_impl.hpp"

#endif // XPCC_BLOCK_DEVICE_HEAP_HPP
