/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Gonzalo Arana
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef O1_SHM_H
#define O1_SHM_H

#include <stdlib.h>

#ifndef O1_SHM_STATIC
#define O1_SHM_STATIC
#endif

#define SHM_ALLOC_OK             ( 0)
#define SHM_ALLOC_OPEN_ERROR     (-1)
#define SHM_ALLOC_RESIZE_ERROR   (-2)
#define SHM_ALLOC_MAP_ERROR      (-3)

/**
 * Creates a new shared memory area, or attaches to an already existing
 * one (based on @param name).
 *
 * To release the resources:
 * - shm_unlink(3): will disassociate the name with the mapped area.
 * - munmap(2): will unmap the area from the current process/thread.
 * - close(2): will not affect the mapping, can be called right after
 *             shm_alloc, if we know the area will not need to be
 *             `ftruncate`d later.
 *
 * @param fd out: will be set to the file descriptor. Required to call (close)
 * @param name specifies the shared memory object to be created or opened
 * @param size if set to non-zero, the memory area will be resized.
 * @param open_flags a combination of: O_CREAT | O_RDWR (or O_RDONLY)
 * @param error if !NULL, the error code (SHM_ALLOC_OPEN_*) is returned here.
 * @return the mapped area, or NULL in case of errors.
 */
O1_SHM_STATIC
void* shm_alloc(
    int* fd,
    const char* name,
    off_t size,
    int open_flags,
    int* error
);

/**
 * Given a shm_alloc return value, returns the error message associated
 * with it, or NULL if if does not denote an error.
 * The returned string is dynamically allocated memory, and should be
 * `free(3)`ed.
 */
O1_SHM_STATIC
char* shm_error(int error, const char* name, off_t size);

#endif //O1_SHM_H
