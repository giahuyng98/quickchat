//
// Created by giahuy on 9/28/20.
//

#ifndef QUICKCHAT_USER_SESSION_MGR_HPP
#define QUICKCHAT_USER_SESSION_MGR_HPP

#include <string>
#include "user_info.h"

template<class T>
class SessionMgr {
public:
    template<class Config>
    void start(const Config &config) {
        static_cast<T &>(*this).startImpl(config);
    }

    std::string add(const info::UserInfo &userInfo) {
        return static_cast<T &>(*this).addImpl(userInfo);
    }

    bool remove(const std::string &sessionId) {
        return static_cast<T &>(*this).removeImpl(sessionId);
    }

    bool remove(const uint64_t userId) {
        return static_cast<T &>(*this).removeImpl(userId);
    }
};


#endif //QUICKCHAT_USER_SESSION_MGR_HPP
