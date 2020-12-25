
import URL from './URL'
import { v4 as uuidv4 } from 'uuid';
import jwt from 'jsonwebtoken'

const timeout = 500

const DataSource = {
  baokimKey: null,
  users: {},
  getBaoKimKey: async (sessionId) => {
    if (DataSource.baokimKey) {
      return DataSource.baokimKey
    }
    const res = await fetch(URL.getBaokimKeyUrl(sessionId)).then(data => data.json())
    DataSource.baokimKey = res.data
    return DataSource.baokimKey
  },
  getBaoKimCreateOrderUrl: (jwt) => {
    return `https://api.baokim.vn/payment/api/v4/order/send?jwt=${jwt}`
  },

  getBaoKimTransferMoneyUrl: (jwt) => {
    return `https://api.baokim.vn/payment/api/v4/transfer/create?jwt=${jwt}`
  },

  getBaoKimCheckOrderUrl: (jwt, orderId) => {
    return `https://api.baokim.vn/payment/api/v4/order/detail?jwt=${jwt}&id=${orderId}`
  },

  createBaokimOrder: async (user, toUser, amount, message) => {
    const form = new FormData()
    const name = await DataSource.getUserFullname('', toUser.userId)
    form.append("total_amount", amount)
    form.append("description", `Transfer money from ${user.fullname} to ${name}`)
    form.append("url_success", document.location.href)
    form.append("mrc_order_id", uuidv4())
    form.append("url_detail", document.location.href)
    form.append("lang", "vi")
    form.append("customer_email", user.email)
    //form.append("customer_phone", user.email)
    form.append("customer_name", user.fullname)
    const key = await DataSource.getBaoKimKey(user.sessionId)
    const res = await fetch(DataSource.getBaoKimCreateOrderUrl(key), {
      method: 'POST',
      body: form,
    }).then(data => data.json())
    return res
  },

  loopWaitForPayment: async (order, user, toUser, amount, message, loopCnt) => {
    //console.log('order')
    //console.log(order)

    if (loopCnt > 100) {
      return
    }

    const key = await DataSource.getBaoKimKey(user.sessionId)
    const res = await fetch(DataSource.getBaoKimCheckOrderUrl(key, order.order_id)).then(d => d.json())
    console.log('status')
    console.log(res)
    if (res.code == 0 && res.data.stat == 'c') {
      const tran = await DataSource.transferBaokimMoneyToUser(user, toUser, amount, message)
      console.log('tran done:')
      console.log(tran)
      if (tran.code == 0) {
        const m = await DataSource.createTransferBaokimMessage(user.sessionId, toUser.userId, amount, message)
      }
    } else {
      setTimeout(() => { DataSource.loopWaitForPayment(order, user, toUser, amount, message, loopCnt + 1) }, timeout)
    }
  },

  transferBaokimMoneyToUser: async (user, toUser, amount, message) => {
    const toUserInfo = await DataSource.getUserInfo(toUser.userId)
    const description = encodeURIComponent(`Transfer money from ${user.fullname} to ${toUserInfo.fullname}`)

    let url = URL.getBaokimKeyUrl(user.sessionId)
    url += "&form_params=true"
    url += `&to_user=${toUserInfo.email}`
    url += `&amount=${amount}`
    url += `&description=${description}`

    const key = await fetch(url).then(data => data.json()).then(json => json.data)

    const form = new FormData()
    form.append("to_user", toUserInfo.email)
    form.append("amount", amount)
    form.append("description", description)

    const res = await fetch(DataSource.getBaoKimTransferMoneyUrl(key), {
      method: 'POST',
      body: form,
    }).then(data => data.json())
    return res
  },

  transferBaokim: async (user, toUser, amount, message) => {
    try {
      const order = await DataSource.createBaokimOrder(user, toUser, amount, message)
      console.log(order)
      if (order.code == 0) {
        window.open(order.data.payment_url)
      }
      setTimeout(() => { DataSource.loopWaitForPayment(order.data, user, toUser, amount, message, 0) }, timeout)
      return order
    } catch (err) {
      console.log(err)
      return {
        code: 1,
        message: err.message,
      }
    }

  },

  createWebSocket: (sessionId) => {
    const ws = new WebSocket(URL.getWSURL(sessionId));

    return ws
  },
  getUserFullname: async (sessionId, userId) => {
    if (DataSource.users[userId]) {
      return DataSource.users[userId].fullname
    }
    await DataSource.getUserInfo(userId).then(u => DataSource.users[u.id] = u)
    return DataSource.users[userId].fullname
    //return 'Somebody'
  },
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
    if (DataSource.users[userId]) {
      return DataSource.users[userId]
    }
    const userInfo = await fetch(URL.getUserURL(userId)).then(data => data.json());
    DataSource.users[userId] = userInfo.data
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

  removeFriend: async (sessionId, toUser) => {
    const data = {
      "to": toUser
    };
    const res = await fetch(URL.addFriendURL(sessionId),
      {
        method: 'DELETE',
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

  createTransferBaokimMessage: async (sessionId, toUser, amount, message) => {
    const data = {
      id: uuidv4(),
      toUser: toUser,
      amount: amount,
    }
    if (message) data.message = message;
    const res = await fetch(URL.getTransferBaokimMoneyURL(sessionId),
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
    let result = [
      { "id": "6715433118729375744", "username": "admin", "email": "admin@email.com", "fullname": "Admin", "sessionId": "" },
      { "id": "6715433545218789376", "username": "giahuy", "email": "giahuyng98@gmail.com", "fullname": "Nguyễn Gia Huy", "sessionId": "" },
      { "id": "6715434152134578176", "username": "alice", "email": "alice@email.com", "fullname": "Alice", "sessionId": "" },
      { "id": "6715434239023779840", "username": "duc48", "email": "royaldead98@gmail.com", "fullname": "Võ Minh Đức", "sessionId": "" },
      { "id": "6715434324872794112", "username": "charlie", "email": "charlie@email.com", "fullname": "Charlie", "sessionId": "" },
      { "id": "6720216228796960768", "username": "frank", "email": "frank@email.com", "fullname": "Frank", "sessionId": "" },
    ]
    return result
    //for (const u in DataSource.users) {
    //  result.push(DataSource.users[u])
    //}
    //return result
    //return [{ username: 'duc', userId: "6715434239023779840" }]
  },

}

export default DataSource;
