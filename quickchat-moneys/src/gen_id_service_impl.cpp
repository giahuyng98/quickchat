//
// Created by giahuy on 9/28/20.
//

#include "gen_id_service_impl.h"

snowflake<GenIdServiceImpl::EPOCH> GenIdServiceImpl::sf;

int64_t GenIdServiceImpl::nextIdImpl() {
    return GenIdServiceImpl::sf.nextid();
}
