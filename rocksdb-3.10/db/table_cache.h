//  Copyright (c) 2013, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Thread-safe (provides internal synchronization)

#pragma once
#include <string>
#include <vector>
#include <stdint.h>

#include "thirdparty/rocksdb-3.10/db/dbformat.h"
#include "thirdparty/rocksdb-3.10/port/port.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/cache.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/env.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/table.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/options.h"
#include "thirdparty/rocksdb-3.10/table/table_reader.h"

namespace rocksdb {

class Env;
class Arena;
struct FileDescriptor;
class GetContext;

class TableCache {
 public:
  TableCache(const ImmutableCFOptions& ioptions,
             const EnvOptions& storage_options, Cache* cache);
  ~TableCache();

  // Return an iterator for the specified file number (the corresponding
  // file length must be exactly "file_size" bytes).  If "tableptr" is
  // non-nullptr, also sets "*tableptr" to point to the Table object
  // underlying the returned iterator, or nullptr if no Table object underlies
  // the returned iterator.  The returned "*tableptr" object is owned by
  // the cache and should not be deleted, and is valid for as long as the
  // returned iterator is live.
  Iterator* NewIterator(const ReadOptions& options, const EnvOptions& toptions,
                        const InternalKeyComparator& internal_comparator,
                        const FileDescriptor& file_fd,
                        TableReader** table_reader_ptr = nullptr,
                        bool for_compaction = false, Arena* arena = nullptr);

  // If a seek to internal key "k" in specified file finds an entry,
  // call (*handle_result)(arg, found_key, found_value) repeatedly until
  // it returns false.
  Status Get(const ReadOptions& options,
             const InternalKeyComparator& internal_comparator,
             const FileDescriptor& file_fd, const Slice& k,
             GetContext* get_context);

  // Evict any entry for the specified file number
  static void Evict(Cache* cache, uint64_t file_number);

  // Find table reader
  Status FindTable(const EnvOptions& toptions,
                   const InternalKeyComparator& internal_comparator,
                   const FileDescriptor& file_fd, Cache::Handle**,
                   const bool no_io = false);

  // Get TableReader from a cache handle.
  TableReader* GetTableReaderFromHandle(Cache::Handle* handle);

  // Get the table properties of a given table.
  // @no_io: indicates if we should load table to the cache if it is not present
  //         in table cache yet.
  // @returns: `properties` will be reset on success. Please note that we will
  //            return Status::Incomplete() if table is not present in cache and
  //            we set `no_io` to be true.
  Status GetTableProperties(const EnvOptions& toptions,
                            const InternalKeyComparator& internal_comparator,
                            const FileDescriptor& file_meta,
                            std::shared_ptr<const TableProperties>* properties,
                            bool no_io = false);

  // Return total memory usage of the table reader of the file.
  // 0 of table reader of the file is not loaded.
  size_t GetMemoryUsageByTableReader(
      const EnvOptions& toptions,
      const InternalKeyComparator& internal_comparator,
      const FileDescriptor& fd);

  // Release the handle from a cache
  void ReleaseHandle(Cache::Handle* handle);

 private:
  const ImmutableCFOptions& ioptions_;
  const EnvOptions& env_options_;
  Cache* const cache_;
};

}  // namespace rocksdb
