
import { v4 as uuidv4 } from 'uuid';
let users = [
  { username: 'user1' },
  { username: 'user2' },
  { username: 'user3' },
  { username: 'user4' },
]
for (let c = 97; c < 97 + 26; ++c) {
  users.push({
    username: String.fromCharCode(c)
  })
}
const DataSource = {
  getUserFullname: (sessionId, userId) => {
    return 'Somebody'
  },
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
    let data = []

    for (let i = 0; i < 30; ++i) {
      data.push({
        channel: {
          id: `${i}`,
          name: `Name ${i}`,
          type: 'private',
          createAt: '',
          members: [
            {
              userId: `${i}`,
              role: 'user',
              joinAt: new Date(),
            },
          ]
        },
        lastMessage: {
          channelId: `${i}`,
          messageId: `${i}`,
          userId: Math.floor(Math.random() * 2),
          content: {
            content: 'hi there',
            type: 'text',
            replyTo: '1',
          },
          seens: [],
          reactions: [{ like: 1, haha: 1 }],
          createAt: new Date().toString(),
        }
      })
    }
    return ({
      error: 0,
      message: 'ok',
      data: data,
    })
  },

  getFriendList: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
        friends: [
          { username: 'friend 1', fullname: 'User friend 1' },
          { username: 'friend 2', fullname: 'User friend 2' },
          { username: 'friend 3', fullname: 'User friend 3' },
        ],
        sentRequests: [

          { username: 'friend 2', fullname: 'User friend 2' },
        ],
        receivedRequests: [

          { username: 'friend 3', fullname: 'User friend 3' },
        ],
      }
    })
  },

  getMessageList: async (sessionId, channelId) => {
    let data = []
    const userIds = ['0', '1', '2']
    for (let i = 0; i < 50; ++i) {
      const u = Math.ceil(Math.random() * userIds.length) - 1
      data.push({
        channelId: '2',
        messageId: '2',
        userId: userIds[u],
        content: {
          content: `message ${i}`,
          replyTo: '1',
          type: 'text',
        },
        seens: [1, 2, 3],
        reactions: [{ like: 1, haha: 2 }],
        createAt: new Date().toString(),
      })
    }
    return ({
      error: 0,
      message: 'ok',
      data: data,
    })
  },

  getUserAuthenInfo: async (sessionId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
        fullname: 'Nguyen Gia Huy',
        username: 'giahuyng98',
      }
    })
  },

  getUserInfo: async (userId) => {
    return ({
      error: 0,
      message: 'ok',
      data: {
        fullname: 'Nguyen Gia Huy',
        username: 'giahuyng98',
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

  findUser: async (pattern) => {
    return users.filter(u => u.username.startsWith(pattern)).slice(0, 10)
  },

}

export default DataSource;
