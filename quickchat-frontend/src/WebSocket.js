import { Timestamp } from 'google-protobuf/google/protobuf/timestamp_pb';
const msg = require('./payload_pb.js')

const toStatus = (str) => {
  switch (str) {
    case "online": return msg.Status.Type.ONLINE;
    case "offline": return msg.Status.Type.OFFLINE;
    default: return msg.Status.Type.UNKNOWN;
  }
}
const WS = {
  instance: null,
  onMessageCb: [],
  onOpenCb: [],
  onCloseCb: [],
  changeStatus: (str) => {
    const out = new msg.OutMessage();
    const status = new msg.Status();
    status.setTimestamp(Timestamp.fromDate(new Date()));
    status.setType(toStatus(str));
    out.setStatus(status);
    console.log(out.toObject())
    WS.instance.send(out.serializeBinary());
  },
  addOnMessageCallback: (cb) => {
    //console.log('get called')
    //WS.onMessageCb.push(cb)
    WS.instance.onmessage = async (evt) => {
      try {
        const data = await evt.data.arrayBuffer();
        const incoming = msg.InMessage.deserializeBinary(data);
        //for (const cb of WS.onMessageCb) {
        cb(incoming)
        //}
      } catch (err) {
        console.log(err)
      }
    }
  },
}

export default WS;
