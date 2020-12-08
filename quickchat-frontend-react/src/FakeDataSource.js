
import { v4 as uuidv4 } from 'uuid';

const DataSource = {

  login: async (username, password) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
        id: '1',
        username: 'giahuy',
        email: 'giahuy@email.com',
        fullname: 'Nguyen Gia Huy',
        sessionId: '123456',
      }
    })
  },

  getChatList: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: [
        {
          channel: {
            id: '1',
            name: 'Vo Minh Duc',
            type: 'private',
            createAt: '',
            members: [
              {
                userId: '2',
                role: 'user',
                joinAt: '',
              },
            ]
          },
          lastMessage: {
            channelId: '1',
            messageId: '1',
            userId: '1',
            content: {
              content: 'hi there',
              replyTo : '1',
              seens : [],
              reactions: [],
              createAt : new Date(),
            },
          }
        },
        {
          channel: {
            id: '2',
            name: 'Nguyen Huu Canh',
            type: 'private',
            createAt: '',
            members: [
              {
                userId: '3',
                role: 'user',
                joinAt: '',
              },
            ]
          },
          lastMessage: {
            channelId: '2',
            messageId: '2',
            userId: '3',
            content: {
              content: 'canh canh',
              replyTo : '1',
              seens : [],
              reactions: [],
              createAt : new Date(),
            },
          }
        },

      ],
    })
  },

  getFriendList: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
        friends: [],
        sentRequests: [],
        receivedRequests: [],
      }
    })
  },

  getMessageList: async (sessionId, channelId) => {
    return ({
      error: 0,
      message: 'ok',
      data: [],
    })
  },

  getUserAuthenInfo: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
      }
    })
  },

  getUserInfo: async (userId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
      }
    })
  },

  getSessionIdFromUrl: () => {
    return document.location.search.substr(11);
  },

  getMoneyInfo: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
      }
    })
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

}

export default DataSource;
