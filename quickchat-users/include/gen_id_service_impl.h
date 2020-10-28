//
// Created by giahuy on 9/28/20.
//

#ifndef QUICKCHAT_USER_GEN_ID_SERVICE_IMPL_H
#define QUICKCHAT_USER_GEN_ID_SERVICE_IMPL_H

#include "gen_id_service.h"
#include "snowflake.hpp"
#include <memory>

class GenIdServiceImpl : public GenIdService<GenIdServiceImpl> {
public:
  friend class GenIdService<GenIdServiceImpl>;

  GenIdServiceImpl() = default;

private:
  template <class Config> void startImpl(const Config &config) {
    GenIdServiceImpl::sf.init(
        config["snowflake"]["workerid"].template get<int>(),
        config["snowflake"]["datacenterid"].template get<int>());
  }

  int64_t nextIdImpl();

private:
  static constexpr int64_t EPOCH = 1599350400;
  static snowflake<EPOCH> sf;
};

#endif // QUICKCHAT_USER_GEN_ID_SERVICE_IMPL_H
