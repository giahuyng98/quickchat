
export default URL = {
  WS: 'ws://localhost:9001',
  GATE_WAY: 'http://localhost:3010/api/v1',
  WEB: 'http://localhost:3000',

  getChatURL: (sessionId) => {
    return `${URL.WEB}?sessionId=${sessionId}`;
  },

  getLoginURL: () => {
    return `${URL.GATE_WAY}/users/login`;
  },

  getWSURL: (sessionId) => {
    return `${URL.WS}?sessionId=${sessionId}`;
  },

  getAuthenURL: (sessionId) => {
    return `${URL.GATE_WAY}/users?sessionId=${sessionId}`;
  },

  getChatListURL: (sessionId) => {
    return `${URL.GATE_WAY}/chat-list?sessionId=${sessionId}`;
  },

  getMessageListURL: (sessionId, channelId) => {
    return `${URL.GATE_WAY}/channels/${channelId}/messages?sessionId=${sessionId}`;
  },

  getUserURL: (userId) => {
    return `${URL.GATE_WAY}/users/${userId}`;
  },

  createNewChatURL: (sessionId) => {
    return `${URL.GATE_WAY}/channels?sessionId=${sessionId}`;
  },

  getMoneyInfoURL: (sessionId) => {
    return `${URL.GATE_WAY}/money?sessionId=${sessionId}`;
  },
  createMoneyAccountURL: (sessionId) => {
    return `${URL.GATE_WAY}/money?sessionId=${sessionId}`;
  },

  getDepositMoneyURL: (sessionId) => {
    return `${URL.GATE_WAY}/money/deposit?sessionId=${sessionId}`;
  },
  getWithdrawMoneyURL: (sessionId) => {
    return `${URL.GATE_WAY}/money/withdraw?sessionId=${sessionId}`;
  },
  getTransferMoneyURL: (sessionId) => {
    return `${URL.GATE_WAY}/money/transfer?sessionId=${sessionId}`;
  },

  getTransactionHistory: (sessionId, limit, offset) => {
    return `${URL.GATE_WAY}/money/history?sessionId=${sessionId}&limit=${limit}&offset=${offset}`;
  },

  addFriendURL: (sessionId) => {
    return `${URL.GATE_WAY}/friends?sessionId=${sessionId}`;
  },
  getFriendListURL: (sessionId) => {
    return `${URL.GATE_WAY}/friends?sessionId=${sessionId}`;
  },

}
