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

#define _GNU_SOURCE
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>

#include "o1shm.h"

O1_SHM_STATIC void* shm_alloc(
    int* fd,
    const char* name,
    off_t size,
    int open_flags,
    int* error
) {
    void* location = NULL;
    int read_write = O_RDWR == (open_flags & (O_RDWR | O_RDONLY));
    int fd_ = shm_open(
        name,
        open_flags & (O_CREAT | O_RDWR | O_RDONLY),
        0600
    );

    if (fd_ < 0) {
        if (error)
            *error = SHM_ALLOC_OPEN_ERROR;
        return NULL;
    }

    if (open_flags & O_CREAT) {
        if (ftruncate(fd_, size) < 0) {
            close(fd_);
            if (error)
                *error = SHM_ALLOC_RESIZE_ERROR;
            return NULL;
        }
    }

    location = mmap(
        NULL,
        size,
        PROT_READ | (read_write ? PROT_WRITE : 0),
        MAP_SHARED,
        fd_,
        0
    );

    if (!location) {
        close(fd_);
        if (error)
            *error = SHM_ALLOC_MAP_ERROR;
        return NULL;
    }

    *fd = fd_;

    if (error)
        *error = SHM_ALLOC_OK;

    return location;
}

char* shm_error(int result, const char* name, off_t size) {

    char* message = NULL;
    int length = 0;

    if (result == SHM_ALLOC_OK)
        return NULL;

    if (result == SHM_ALLOC_OPEN_ERROR)
        length = asprintf(&message, "Could not open %s: %s", name, strerror(errno));

    if (result == SHM_ALLOC_RESIZE_ERROR)
        length = asprintf(&message, "Could not resize %s to %zu: %s", name, size, strerror(errno));

    if (result == SHM_ALLOC_MAP_ERROR)
        length = asprintf(&message, "Could not mmap %s: %s", name, strerror(errno));

    if (length < 0) {
        fputs("Could not allocate error message.", stderr);
        exit(1);
    }

    return message;
}
