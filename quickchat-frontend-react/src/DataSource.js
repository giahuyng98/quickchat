
import URL from './URL'
import { v4 as uuidv4 } from 'uuid';

const DataSource = {

  login: async (username, password) => {
    const data = {
      username: username,
      password: password
    }
    const res = await fetch(URL.getLoginURL(),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res;
  },

  getChatList: async (sessionId) => {
    const json = await fetch(URL.getChatListURL(sessionId)).then(data => data.json());
    return json;
  },

  getFriendList: async (sessionId) => {
    const json = await fetch(URL.getFriendListURL(sessionId)).then(data => data.json());
    return json;
  },

  getMessageList: async (sessionId, channelId) => {
    const json = await fetch(URL.getMessageListURL(sessionId, channelId)).then(data => data.json());
    return json;
  },

  getUserAuthenInfo: async (sessionId) => {
    const json = await fetch(URL.getAuthenURL(sessionId)).then(data => data.json());
    return json;
  },

  getUserInfo: async (userId) => {
    const userInfo = await fetch(URL.getUserURL(userId)).then(data => data.json());
    return userInfo.error != 0 ? null : userInfo.data;
  },

  getSessionIdFromUrl: () => {
    return document.location.search.substr(11);
  },

  getMoneyInfo: async (sessionId) => {
    const account = await fetch(URL.getMoneyInfoURL(sessionId)).then(data => data.json());
    return account;
  },

  createGroup: async (sessionId, me, name, users) => {
    const data = {
      channel: {
        name: name,
        type: 'group',
        members: [
          { userId: me }
        ].concat(users.map(u => ({ userId: u })))
      }
    }
    const res = await fetch(URL.createNewChatURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },

  addFriend: async (sessionId, toUser) => {
    const data = {
      "to": toUser
    };
    const res = await fetch(URL.addFriendURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },


  createNewChat: async (sessionId, me, them) => {
    const data = {
      channel: {
        type: 'private',
        members: [
          { userId: me },
          { userId: them }
        ]
      }
    }
    const res = await fetch(URL.createNewChatURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },
  createMoneyAccount: async (sessionId) => {
    const res = await fetch(URL.createMoneyAccountURL(sessionId),
      {
        method: 'POST',
      }).then(data => data.json());
    return res
  },

  deposit: async (sessionId, amount) => {
    const data = {
      id: uuidv4(),
      amount: amount
    }
    const res = await fetch(URL.getDepositMoneyURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },
  withdraw: async (sessionId, amount) => {
    const data = {
      id: uuidv4(),
      amount: amount
    }
    const res = await fetch(URL.getWithdrawMoneyURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },

  transfer: async (sessionId, toUser, amount, message) => {
    const data = {
      id: uuidv4(),
      toUser: toUser,
      amount: amount,
    }
    if (message) data.message = message;
    const res = await fetch(URL.getTransferMoneyURL(sessionId),
      {
        method: 'POST',
        body: JSON.stringify(data)
      }).then(data => data.json());
    return res
  },

  history: async (sessionId, limit = 100, offset = 0) => {
    const res = await
      fetch(URL.getTransactionHistory(sessionId, limit, offset))
        .then(data => data.json());
    return res;
  },

  finduser: async (pattern) => {

  },

}

export default DataSource;
