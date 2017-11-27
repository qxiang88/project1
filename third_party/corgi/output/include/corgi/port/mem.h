
#ifndef CORGI_MEM_H_
#define CORGI_MEM_H_

#include "corgi/base/types.h"

namespace corgi {
namespace port {

void* AlignedMalloc(size_t size, int minimum_alignment);
void AlignedFree(void* aligned_memory);

void* Malloc(size_t size);
void* Realloc(void* ptr, size_t size);
void Free(void* ptr);

void MallocExtension_ReleaseToSystem(std::size_t num_bytes);

std::size_t MallocExtension_GetAllocatedSize(const void* p);

}  // namespace port
}  // namespace corgi

#endif  // CORGI_MEM_H_
