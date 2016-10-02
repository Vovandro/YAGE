//
// Created by bentoo on 9/27/16.
//

#ifndef GAME_MEMORYBLOCK_H
#define GAME_MEMORYBLOCK_H

#include <cstdint>
#include <cstring>
#include "DebugSourceInfo.h"

#define CREATE_NEW(MemBlock, T)\
	Memory::CreateNew<decltype(MemBlock), T>(MemBlock, Utils::DebugSourceInfo(__FILE__, __LINE__))

#define CREATE_NEW_ARRAY(MemBlock, T, length)\
	Memory::CreateNewArray<decltype(MemBlock), T>(MemBlock, length, Utils::DebugSourceInfo(__FILE__, __LINE__))

#define CREATE_NEW_ARRAY_DEF(MemBlock, T, length, defaultValue)\
	Memory::CreateNewArray<decltype(MemBlock), T>(MemBlock, length, defaultValue, Utils::DebugSourceInfo(__FILE__, __LINE__))

namespace Memory
{
	class Allocator;

	class MemoryBlockBase
	{
	protected:
		MemoryBlockBase(){}
	private:
		MemoryBlockBase(const MemoryBlockBase&) = delete;
		MemoryBlockBase(MemoryBlockBase&&) = delete;
	public:
		virtual void* allocate(std::size_t size, std::size_t alignment, const Utils::DebugSourceInfo& sourceInfo) = 0;
		virtual void  deallocate(void* ptr) = 0;
	};

	// todo: mem checking, tracing
	template <typename AllocatorType>
	class MemoryBlock final : public MemoryBlockBase
	{
		static_assert(std::is_base_of<Allocator, AllocatorType>::value, "Allocator must derive from Allocator");

		AllocatorType& _allocator;

	public:
		explicit MemoryBlock(AllocatorType& allocator)
			: _allocator(allocator), MemoryBlockBase()
		{

		}

		void* allocate(std::size_t size, std::size_t alignment, const Utils::DebugSourceInfo& sourceInfo) override
		{
			std::size_t offset = 0;
			void*		allocationAddress = _allocator.allocate(size, alignment, 0);
			// + offset
			// because returned addres is not aligned to it
			return 		allocationAddress;
		}

		/*void copy(void*& ptr, void*& newPtr)
		{
			std::size_t 	offset 			= 0;
			std::uintptr_t 	originalAddress = reinterpret_cast<std::uintptr_t >(ptr) - offset;
			void* 			originalPtr 	= reinterpret_cast<void*>(originalAddress);
			std::size_t 	allocationSize 	= _allocator.getAllocationSize(originalPtr);

			memcpy(newPtr, ptr, allocationSize - offset);
		}

		void* reallocate(void* ptr, std::size_t newSize)
		{
			void* 	newPtr = _allocator.resize(ptr, newSize);
			return 	newPtr;
		}*/

		void deallocate(void* ptr) override
		{
			std::size_t 	offset 			= 0;
			std::uintptr_t 	originalAddress = reinterpret_cast<std::uintptr_t >(ptr) - offset;
			void* 			originalPtr 	= reinterpret_cast<void*>(originalAddress);

			std::size_t allocationSize = _allocator.getAllocationSize(originalPtr);

			_allocator.deallocate(ptr);
		}
	};

	template <typename MemoryBlock, typename T>
	T* CreateNew(MemoryBlock& block, const Utils::DebugSourceInfo& source)
	{
		return new(block.allocate(sizeof(T), alignof(T), source)) T();
	}

	template <typename MemoryBlock, typename T, typename ... Args>
	T* CreateNew(MemoryBlock& block, Args ... args, const Utils::DebugSourceInfo& source)
	{
		return new (block.allocate(sizeof(T), alignof(T), source)) T(args ...);
	}

	template <typename MemoryBlock, typename T>
	T* CreateNew(MemoryBlock& block, const T& obj, const Utils::DebugSourceInfo& source)
	{
		return new (block.allocate(sizeof(T), alignof(T), source)) T(obj);
	}

	template <typename MemoryBlock, typename T>
	T* CreateNewArray(MemoryBlock& block, std::size_t length, const T& defaultValue, const Utils::DebugSourceInfo& source)
	{
		std::size_t memory_size = sizeof(T) * length + sizeof(std::size_t);
		union {
			void* as_raw_ptr;
			std::size_t* as_size;
			T* as_T;
		} memory;

		memory.as_raw_ptr = block.allocate(memory_size, alignof(T), source);
		*memory.as_size++ = length;

		for(const T* const endPtr = memory.as_T + length; memory.as_T < endPtr;)
			new (memory.as_T++) T(defaultValue);

		return memory.as_T - length;
	}

	template <typename MemoryBlock, typename T>
	T* CreateNewArray(MemoryBlock& block, std::size_t length, const Utils::DebugSourceInfo& source)
	{
		std::size_t memory_size = sizeof(T) * length + sizeof(std::size_t);
		union {
			void* as_raw_ptr;
			std::size_t* as_size;
			T* as_T;
		} memory;

		 memory.as_raw_ptr = block.allocate(memory_size, alignof(T), source);
		*memory.as_size++ = length;

		for(const T* const endPtr = memory.as_T + length; memory.as_T < endPtr;)
			new (memory.as_T++) T;

		return memory.as_T - length;
	}

	template <typename MemoryBlock, typename T>
	T* ResizeArray(MemoryBlock& block, const T*& ptr, std::size_t newSize, const Utils::DebugSourceInfo& source)
	{
		T* newPtr = CreateNewArray<T>(block, newSize, source);
	//	block.copy(ptr, newPtr);
		return newPtr;
	};

	template <typename MemoryBlock, typename T>
	void Delete(MemoryBlock& block, T*& ptr)
	{
		if(ptr != nullptr)
		{
			ptr->~T();
			block.deallocate(ptr);
			ptr = nullptr;
		}
	}

	template <typename MemoryBlock, typename T>
	void DeleteArray(MemoryBlock& block, T*& ptr)
	{
		if(ptr != nullptr)
		{
			T* currentPtr = ptr;
			std::uintptr_t 	lengthAddress 	=  reinterpret_cast<std::uintptr_t >(ptr) - sizeof(std::size_t);
			std::size_t 	length 			= *reinterpret_cast<std::size_t*>(lengthAddress);

			for(const T* const endPtr = ptr + length; currentPtr < endPtr;)
				currentPtr++->~T();

			block.deallocate(ptr);
			ptr = nullptr;
		}
	}
}

#endif //GAME_MEMORYBLOCK_H
