// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#pragma once
#ifndef ROCKSDB_LITE
#include <vector>
#include <string>

#include "thirdparty/rocksdb-3.10/include/rocksdb/utilities/stackable_db.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/utilities/db_ttl.h"
#include "thirdparty/rocksdb-3.10/include/rocksdb/db.h"

namespace rocksdb {

// Please don't use this class. It's deprecated
class UtilityDB {
 public:
  // This function is here only for backwards compatibility. Please use the
  // functions defined in DBWithTTl (rocksdb/utilities/db_ttl.h)
  // (deprecated)
  __attribute__((deprecated)) static Status OpenTtlDB(const Options& options,
                                                      const std::string& name,
                                                      StackableDB** dbptr,
                                                      int32_t ttl = 0,
                                                      bool read_only = false);
};

} //  namespace rocksdb
#endif  // ROCKSDB_LITE
