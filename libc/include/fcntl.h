/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once
#define _FCNTL_H

/**
 * @file fcntl.h
 * @brief File control operations.
 */

#include <sys/cdefs.h>
#include <sys/types.h>
#include <linux/fadvise.h>
#include <linux/falloc.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/uio.h>

#include <bits/fcntl.h>
#include <bits/seek_constants.h>

#if defined(__USE_GNU) || defined(__USE_BSD)
#include <bits/lockf.h>
#endif

__BEGIN_DECLS

#ifdef __LP64__
/* LP64 kernels don't have F_*64 defines because their flock is 64-bit. */
/** Flag for flock(). */
#define F_GETLK64  F_GETLK
/** Flag for flock(). */
#define F_SETLK64  F_SETLK
/** Flag for flock(). */
#define F_SETLKW64 F_SETLKW
#endif

/** Flag for open(). */
#define O_ASYNC FASYNC
/** Flag for open(). */
#define O_RSYNC O_SYNC

#if __ANDROID_API__ >= __ANDROID_API_L__
/** Flag for splice(). */
#define SPLICE_F_MOVE 1
/** Flag for splice(). */
#define SPLICE_F_NONBLOCK 2
/** Flag for splice(). */
#define SPLICE_F_MORE 4
/** Flag for splice(). */
#define SPLICE_F_GIFT 8
#endif

#if __ANDROID_API__ >= __ANDROID_API_O__
/** Flag for sync_file_range(). */
#define SYNC_FILE_RANGE_WAIT_BEFORE 1
/** Flag for sync_file_range(). */
#define SYNC_FILE_RANGE_WRITE 2
/** Flag for sync_file_range(). */
#define SYNC_FILE_RANGE_WAIT_AFTER 4
#endif

/**
 * [creat(2)](http://man7.org/linux/man-pages/man2/creat.2.html)
 * creates a file.
 *
 * Returns a new file descriptor on success and returns -1 and sets `errno` on
 * failure.
 */
int creat(const char* __path, mode_t __mode);
/** See creat(). */
int creat64(const char* __path, mode_t __mode) __INTRODUCED_IN(21);

/**
 * [openat(2)](http://man7.org/linux/man-pages/man2/openat.2.html)
 * opens (and possibly creates) a file.
 *
 * Returns a new file descriptor on success and returns -1 and sets `errno` on
 * failure.
 */
int openat(int __dir_fd, const char* __path, int __flags, ...);
/** See openat(). */
int openat64(int __dir_fd, const char* __path, int __flags, ...) __INTRODUCED_IN(21);

/**
 * [open(2)](http://man7.org/linux/man-pages/man2/open.2.html)
 * opens (and possibly creates) a file.
 *
 * Returns a new file descriptor on success and returns -1 and sets `errno` on
 * failure.
 */
int open(const char* __path, int __flags, ...);
/** See open(). */
int open64(const char* __path, int __flags, ...) __INTRODUCED_IN(21);

/**
 * [splice(2)](http://man7.org/linux/man-pages/man2/splice.2.html)
 * splices data to/from a pipe.
 *
 * Valid flags are `SPLICE_F_MOVE`, `SPLICE_F_NONBLOCK`, `SPLICE_F_MORE`, and
 * `SPLICE_F_GIFT`.
 *
 * Returns the number of bytes spliced on success and returns -1 and sets
 * `errno` on failure.
 *
 * Available since API level 21.
 */
ssize_t splice(int __in_fd, off64_t* __in_offset, int __out_fd, off64_t* __out_offset, size_t __length, unsigned int __flags) __INTRODUCED_IN(21);

/**
 * [tee(2)](http://man7.org/linux/man-pages/man2/tee.2.html)
 * duplicates data from one pipe to another.
 *
 * Valid flags are `SPLICE_F_MOVE`, `SPLICE_F_NONBLOCK`, `SPLICE_F_MORE`, and
 * `SPLICE_F_GIFT`.
 *
 * Returns the number of bytes duplicated on success and returns -1 and sets
 * `errno` on failure.
 *
 * Available since API level 21.
 */
ssize_t tee(int __in_fd, int __out_fd, size_t __length, unsigned int __flags) __INTRODUCED_IN(21);

/**
 * [vmsplice(2)](http://man7.org/linux/man-pages/man2/vmsplice.2.html)
 * splices data to/from a pipe.
 *
 * Valid flags are `SPLICE_F_MOVE`, `SPLICE_F_NONBLOCK`, `SPLICE_F_MORE`, and
 * `SPLICE_F_GIFT`.
 *
 * Returns the number of bytes spliced on success and returns -1 and sets
 * `errno` on failure.
 *
 * Available since API level 21.
 */
ssize_t vmsplice(int __fd, const struct iovec* __iov, size_t __count, unsigned int __flags) __INTRODUCED_IN(21);

/**
 * [fallocate(2)](http://man7.org/linux/man-pages/man2/fallocate.2.html)
 * is a Linux-specific extension of posix_fallocate().
 *
 * Valid flags are `FALLOC_FL_KEEP_SIZE`, `FALLOC_FL_PUNCH_HOLE`,
 * `FALLOC_FL_NO_HIDE_STALE`, `FALLOC_FL_COLLAPSE_RANGE`,
 * `FALLOC_FL_ZERO_RANGE`, `FALLOC_FL_INSERT_RANGE`, and
 * `FALLOC_FL_UNSHARE_RANGE`.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 *
 * Available since API level 21.
 */
int fallocate(int __fd, int __mode, off_t __offset, off_t __length) __RENAME_IF_FILE_OFFSET64(fallocate64) __INTRODUCED_IN(21);
/** See fallocate(). */
int fallocate64(int __fd, int __mode, off64_t __offset, off64_t __length) __INTRODUCED_IN(21);

/**
 * [posix_fadvise(2)](http://man7.org/linux/man-pages/man2/posix_fadvise.2.html)
 * declares an expected access pattern for file data.
 *
 * Valid flags are `POSIX_FADV_NORMAL`, `POSIX_FADV_RANDOM`,
 * `POSIX_FADV_SEQUENTIAL`, `POSIX_FADV_WILLNEED`, `POSIX_FADV_DONTNEED`,
 * and `POSIX_FADV_NOREUSE`.
 *
 * Returns 0 on success and returns an error number on failure.
 *
 * Available since API level 21.
 */
int posix_fadvise(int __fd, off_t __offset, off_t __length, int __advice) __RENAME_IF_FILE_OFFSET64(posix_fadvise64) __INTRODUCED_IN(21);
/** See posix_fadvise(). */
int posix_fadvise64(int __fd, off64_t __offset, off64_t __length, int __advice) __INTRODUCED_IN(21);

/**
 * [posix_fallocate(2)](http://man7.org/linux/man-pages/man2/posix_fallocate.2.html)
 * allocates file space.
 *
 * Returns 0 on success and returns an error number on failure.
 *
 * Available since API level 21.
 */
int posix_fallocate(int __fd, off_t __offset, off_t __length) __RENAME_IF_FILE_OFFSET64(posix_fallocate64) __INTRODUCED_IN(21);
/** See posix_fallocate(). */
int posix_fallocate64(int __fd, off64_t __offset, off64_t __length) __INTRODUCED_IN(21);

#if defined(__USE_GNU)

/**
 * [readahead(2)](http://man7.org/linux/man-pages/man2/readahead.2.html)
 * initiates readahead for the given file.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
ssize_t readahead(int __fd, off64_t __offset, size_t __length);

/**
 * [sync_file_range(2)](http://man7.org/linux/man-pages/man2/sync_file_range.2.html)
 * syncs part of a file with disk.
 *
 * Valid flags are `SYNC_FILE_RANGE_WAIT_BEFORE`, `SYNC_FILE_RANGE_WRITE`, and
 * `SYNC_FILE_RANGE_WAIT_AFTER`.
 *
 * Returns 0 on success and returns -1 and sets `errno` on failure.
 */
int sync_file_range(int __fd, off64_t __offset, off64_t __length, unsigned int __flags) __INTRODUCED_IN(26);

#endif

#if defined(__BIONIC_INCLUDE_FORTIFY_HEADERS)
#include <bits/fortify/fcntl.h>
#endif

__END_DECLS
