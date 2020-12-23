import { createSlice } from '@reduxjs/toolkit';

import DataSource from '../FakeDataSource'
import WS from '../WebSocket'

export const appSlice = createSlice({
  name: 'app',
  initialState: {
    tab: {
      index: 0,
      //name : ['Chat', 'Connect', 'Money'],
    },
    snackbar: {
      open: false,
      severity: 'success',
      message: 'hi there',
    },
    user: {
      id: 0,
      username: 'giahuy',
      fullname: 'Nguyen Gia Huy',
      sessionId: null,
      status: "offline",
    },
    ws: {},
    typing: {
      show: false,
      showOnChatItem: false,
      last: Date.now(),
    },
    chatList: [],
    messageList: [],
    currentChannel: null,
    refs: {
      scrollableListRef: null,
    },
  },
  reducers: {
    notify: (state, action) => {
      if (action.payload.open == true) {
        state.snackbar = action.payload;
      } else {
        state.snackbar.open = false;
      }
    },
    changeTab: (state, action) => {
      state.tab.index = action.payload.index;
    },
    changeChatList: (state, action) => {
      state.chatList = action.payload;
    },
    addChatListMessage: (state, action) => {
      const idx = state.chatList.findIndex(
        (v) => v.channel.id == action.payload.channelId)
      if (idx != -1) {
        // TODO: move
        let msg = { ...action.payload }

        if (action.payload.userId == state.user.id) {
          msg.unread = 0;
        } else {
          msg.unread = (state.chatList[idx].lastMessage.unread || 0) + 1
        }
        state.chatList[idx].lastMessage = msg
      }
    },
    changeMessageList: (state, action) => {
      state.messageList = action.payload;
    },
    addMessageList: (state, action) => {
      if (state.currentChannel &&
        action.payload.channelId == state.currentChannel.id) {
        state.messageList.push(action.payload);

        if (state.typing && state.typing.cancel) {
          clearTimeout(state.typing.cancel)
        }
        state.typing = null;
        //changeTyping(selectTyping(null))
      }
    },
    changeCurrentChannel: (state, action) => {
      const idx = state.chatList.findIndex(
        (v) => v.channel.id == action.payload.id)
      if (idx != -1) {
        state.chatList[idx].lastMessage = { ...state.chatList[idx].lastMessage, unread: 0 }
        //console.log(state.chatList[idx].lastMessage )
      }
      state.currentChannel = action.payload;
    },
    changeUser: (state, action) => {
      state.user = action.payload;
      state.user.status = "online"
    },
    changeWs: (state, action) => {
      state.ws = action.payload;
    },
    changeTyping: (state, action) => {
      // TODO check created time

      if (state.typing && state.typing.cancel) {
        clearTimeout(state.typing.cancel)
      }
      if (!action.payload.show) {
        state.typing = null;
      } else {
        state.typing = action.payload;
        state.typing.show = state.currentChannel &&
          action.payload.data &&
          state.currentChannel.id == action.payload.data.channelId
      }
    },
    changeStatus: (state, action) => {
      state.user.status = action.payload
      WS.changeStatus(action.payload);
    },
  },
});

export const {
  notify,
  changeTab,
  changeChatList,
  changeMessageList,
  addMessageList,
  addChatListMessage,
  changeCurrentChannel,
  changeUser,
  changeWs,
  changeTyping,
  changeStatus,
} = appSlice.actions;

export const selectNotify = state => state.app.snackbar;
export const selectTab = state => state.app.tab;
export const selectChatList = state => state.app.chatList;
export const selectMessageList = state => state.app.messageList;
export const selectCurrentChannel = state => state.app.currentChannel;
export const selectUser = state => state.app.user;
export const selectWs = state => state.app.ws;
export const selectTyping = state => state.app.typing;

export default appSlice.reducer;
