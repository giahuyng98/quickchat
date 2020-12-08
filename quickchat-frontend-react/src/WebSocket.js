
import URL from './URL'
import { Timestamp } from 'google-protobuf/google/protobuf/timestamp_pb'

const msg = require('./payload_pb')

export default function WebSocketUtil(props) {

  const toChatMessage = (chat) => {
    return ({
      channelId: chat.getChannelId(),
      messageId: chat.getMessageId(),
      userId: chat.getUserId(),
      content: {
        content: chat.getContent().getContent()
      },
      createAt: chat.getTimestamp().toDate()
    })
  }
  const createWebSocket = () => new WebSocket(URL.getWSURL(props.sessionId));

  const setUp = (ws) => {
    ws.onopen = () => {
      console.log('Websocket is opened');
      props.onOpen();
    }

    ws.onclose = () => {
      console.log('Websocket is closed');
      props.onClose();
    }

    ws.onmessage = async (evt) => {
      const data = await evt.data.arrayBuffer();
      const incoming = msg.InMessage.deserializeBinary(data);
      switch (incoming.getTypeCase()) {
        case msg.InMessage.TypeCase.CHAT:
          props.onChatMessage(toChatMessage(incoming.getChat()));
          break;
        case msg.InMessage.TypeCase.SEEN:
          const seen = incoming.getSeen();
          console.log('seen' + JSON.stringify(seen.toObject()));
          break;
        case msg.InMessage.TypeCase.STATUS:
          const status = incoming.getStatus();
          console.log('status: ' + JSON.stringify(status.toObject()));
          break;
        case msg.InMessage.TypeCase.TYPING:
          const typing = incoming.getTyping();
          console.log('typing: ' + JSON.stringify(typing.toObject()));
          break;
        case msg.InMessage.TypeCase.NOTIFICATION:
          const notification = incoming.getNotification();
          console.log('notification: ' + JSON.stringify(notification.toObject()));
          break;
        default:
          console.log(incoming.toObject());
      }
    }
  }

  const sendChatMessage = (ws, channelId, text) => {
    const out = new msg.OutMessage();
    const chat = new msg.Chat();
    const chatContent = new msg.ChatContent();
    chatContent.setContent(text);
    chatContent.setReplyTo(null);
    chatContent.setType(msg.ChatContent.ChatContentType.TEXT);

    chat.setChannelId(channelId);
    chat.setContent(chatContent);
    chat.setTimestamp(Timestamp.fromDate(new Date()));
    out.setChat(chat);
    ws.send(out.serializeBinary());
  }

  return {
    socket: null,
    createWebSocket: createWebSocket,
    setUp: setUp,
    sendChatMessage: sendChatMessage,
  }
}
