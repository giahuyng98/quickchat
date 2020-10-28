//
// Created by giahuy on 9/28/20.
//

#ifndef QUICKCHAT_USER_SESSION_MGR_IMPL_HPP
#define QUICKCHAT_USER_SESSION_MGR_IMPL_HPP

#include "session_mgr.hpp"
#include <sw/redis++/redis.h>


class SessionMgrImpl : public SessionMgr<SessionMgrImpl> {
public:
    template<class Config>
    void startImpl(const Config &config) {
        expiredAfterSecond = config["session"]["expired"].template get<int64_t>();
        redisClient = std::make_unique<sw::redis::Redis>(config["redis"]["uri"].template get<std::string>());
    }

    std::string addImpl(const info::UserInfo &userInfo);

    bool removeImpl(const std::string &sessionId);

    bool removeImpl(int64_t userId);

private:
    int64_t expiredAfterSecond;
    std::unique_ptr<sw::redis::Redis> redisClient;
};


#endif //QUICKCHAT_USER_SESSION_MGR_IMPL_HPP
