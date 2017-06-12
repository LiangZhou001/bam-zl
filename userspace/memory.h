#ifndef __MEMORY_H__
#define __MEMORY_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


/* 
 * Memory range handle
 *
 * Describes an arbitrarily large memory buffer. Contains the bus/physical 
 * addresses to the page-locked memory "units" used by the buffer.
 *
 * Note: A "unit" corresponds to NVM Memory Page Size (MPS)
 */
struct mem_buffer
{
    long            kernel_handle;  // Handle retrieved from kernel module
    int             device;         // CUDA device (-1 if no device)
    void*           virt_addr;      // Pointer to virtual memory (may be device memory)
    size_t          range_size;     // Total memory range size
    size_t          page_size;      // Size of a physical page
    size_t          unit_size;      // Size of a logical memory unit
    size_t          n_addrs;        // Number of memory units
    uint64_t        bus_addr[0];    // Bus/physical addresses to memory units
};


/* Convenience type */
typedef struct mem_buffer buffer_t;


/*
 * Page handle
 *
 * Descriptor to a page of memory. Contains the bus/physical address
 * to the page-locked page.
 */
struct mem_page
{
    long            kernel_handle;  // Handle retrieved from kernel module
    int             device;         // CUDA device (-1 if no device)
    void*           virt_addr;      // Pointer to memory (may be device memory)
    size_t          page_size;      // Size of a page unit
    uint64_t        bus_addr;       // Bus/physical address to page
};


/* Convenience type */
typedef struct mem_page page_t;



/* Allocate a memory buffer
 *
 * Allocate a memory range and pin it in memory.
 *
 * Note: The requested size will be rounded up to the nearest
 * page-aligned address.
 *
 * Returns a handle on success or NULL on failure.
 */
buffer_t* get_buffer(int ioctl_fd, int device, size_t buffer_size, size_t mem_unit_size);


/* Release memory buffer
 *
 * Unpin memory and free allocated memory.
 * After calling this function the handle is invalid.
 */
void put_buffer(int ioctl_fd, buffer_t* buffer_handle);


/* Allocate a page of memory and pin it
 *
 * Returns 0 on success and sets the page handle, or an errno
 * on failure.
 */
int get_page(int ioctl_fd, int device, page_t* page_handle);


/* Release allocated page */
void put_page(int ioctl_fd, page_t* page_handle);


#ifdef __cplusplus
}
#endif
#endif