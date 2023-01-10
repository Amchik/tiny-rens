/* vim: ft=c
 * Experement allocator
 * Supports runtime checks, MT-Safe, nonblocking to alloc
 *
 * It calls system memory mapping functions and allocs
 * with metadata:
 *NORMAL:
 * +--[size_t]--+--//////-+
 * | alloc size | data... |
 * +------------+---------+
 *MULTITHREAD:
 * +--[size_t]--+--[int]--+-[uint8_t]-+--//////-+
 * | alloc size |  mutex  | ref count | data... |
 * +------------+---------+-----------+---------+
 *
 * Linux mmap(2) requires to specify allocation size,
 * so we need to store it.
 * Please note: all functions return pointer to data,
 * not real allocation start. Overriding alloc size and
 * ref counter (writting to negative data indexes) can
 * cause Undefined Behaviour.
 * Also, you need to have only one real owner of memory,
 * do not realloc and use old addresses, or etc.
 * Do not use this allocator with free(3) or *alloc(3)
 * functions from libc.
 *
 * You can allocate and deallocate memory using alloc() and
 * dealloc() functions. Also, you can pretend to own memory.
 * Example:
 * 
 * void thread_a() {
 *     char *buff = mtalloc(1024);
 *
 *     // buff has `ref_count` 1. If `ref_count` reach zero
 *     // buff will be deallocated.
 *     run_thread(thread_b, buff); // non-blocking
 *     run_thread(thread_b, buff);
 *
 *     // so, buffer don't more used in this thread, dropping ref.
 *     ref_drop(buff);
 * }
 * void thread_b(char *buff) {
 *     // Note: if you want to lock buffer after claiming use
 *     // ref_claim_lock() and ref_drop_unlock() macro.
 *     ref_claim(buff); // Pretend buffer, increase `ref_count`
 *     // do stuff...
 *     ref_lock(buff); // get exclusive access
 *     buff[0] = 10;
 *     ref_unlock(buff); // unlock mutex
 *     // drop buff, so we don't use it anymore:
 *     ref_drop(buff); // in one of thread it will reached
 *                     // zero, so buffer will be dropped
 * }
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define _libc_malloc DO_NOT_USE_THAT
#define malloc  _libc_malloc
#define realloc _libc_malloc
#define calloc  _libc_malloc

/* Claim and keep ref locked. Works like ref_claim(ptr) and ref_lock(ptr)
 */
#define ref_claim_lock(ptr) ref_lock(ptr); /* SAFETY: <- */ ref_claim_unlocked(ptr)
/* Drops and unlock ref. Works like ref_unlock(ptr) and ref_drop(ptr)
 */
#define ref_drop_unlock(ptr) if (ref_drop_unlocked(ptr)) /* SAFETY: <- */ ref_unlock(ptr)

/* Normal buffer metadata size
 */
#define ALLOC_META_SIZE (sizeof(size_t))
/* Multi-thread buffer metadata size
 */
#define REF_META_SIZE (sizeof(pthread_mutex_t) + sizeof(size_t) + sizeof(uint8_t))

/* Allocates a normal buffer.
 */
void *alloc(size_t count);
/* Deallocate a normal buffer.
 */
void dealloc(void *ptr);

/* Get allocation size of a normal buffer (excluding metadata).
 * Full allocation size can be obtained by adding ALLOC_META_SIZE const.
 */
size_t mem_getsize(void *ptr);

/* Allocates a multi-thread buffer.
 */
void *mtalloc(size_t count);
/* Deallocate a multi-thread buffer.
 * SAFETY:
 *  - Only you should have access to buffer
 */
void mtdealloc(void *ptr);

/* Claim multi-thread memory buffer. After done, drop it
 * using ref_drop().
 * SAFETY:
 *  - You should not to lock ref before in running thread
 */
void ref_claim(void *ptr);
/* Claim multi-thread memory buffer. After done, drop it
 * using ref_drop().
 * This function doesn't locks buffer.
 * SAFETY:
 *  - You should have exclusive access to buffer
 */
void ref_claim_unlocked(void *ptr);
/* Get exclusive access to multi-thread buffer.
 * SAFETY:
 *  - You should not to lock ref before in running thread
 */
void ref_lock(void *ptr);
/* Drop exclusive access to multi-thread buffer.
 * SAFETY:
 *  - Ref should be locked
 */
void ref_unlock(void *ptr);
/* Drop ref owning for multi-thread buffer. It may deallocate
 * memory, or may not. Returns refs count even if it zero.
 * SAFETY:
 *  - You should not to lock ref before in running thread
 */
uint8_t ref_drop(void *ptr);
/* Drop ref owning for multi-thread buffer. It may deallocate
 * memory, or may not. Returns refs count even if it zero.
 * This function doesn't locks buffer.
 * SAFETY:
 *  - You should have exclusive access to buffer
 */
uint8_t ref_drop_unlocked(void *ptr);

/* Get refs count of multi-thread buffer.
 */
uint8_t ref_get_count(void *ptr);
/* Get mutex of multi-thread buffer.
 * If you need lock it please use ref_lock() and ref_unlock().
 */
const pthread_mutex_t *ref_get_mutex(void *ptr);
/* Get allocation size (excluding metadata) of multi-thread buffer.
 * Full allocation size can be obtained by adding REF_META_SIZE const.
 */
size_t ref_get_size(void *ptr);

