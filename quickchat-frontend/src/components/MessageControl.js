import React, { useState, useEffect } from 'react';
import { useSelector, useDispatch } from 'react-redux';

import throttle from 'lodash/throttle';
import TransferWithinAStationIcon from '@material-ui/icons/TransferWithinAStation';
import EmojiEmotionsIcon from '@material-ui/icons/EmojiEmotions';
import Fab from '@material-ui/core/Fab';
import {
  Paper,
  Typography,
  AppBar,
  Toolbar,
  List,
  ListItem,
  OutlinedInput,
  IconButton,
  ListItemText,
  ListItemAvatar,
  Chip,
  Box,
  Badge,
  Card,
  CardActions,
  CardContent,
  Avatar,
  Dialog,
  DialogActions,
  DialogTitle,
  DialogContent,
  TextField,
  Button,
} from '@material-ui/core';

//import Picker from 'emoji-picker-react';
import Emoji from "react-emoji-render";
import 'emoji-mart/css/emoji-mart.css'
import { Picker } from 'emoji-mart'
import MuiAlert from '@material-ui/lab/Alert';
import SendIcon from '@material-ui/icons/Send';
import UploadButtons from './UploadButtons'

import PeopleIcon from '@material-ui/icons/People';
import PersonIcon from '@material-ui/icons/Person';
import { makeStyles } from '@material-ui/core/styles';
//import DataSource from '../FakeDataSource'
import DataSource from '../DataSource'

import { Timestamp } from 'google-protobuf/google/protobuf/timestamp_pb';
import {
  selectMessageList,
  selectCurrentChannel,
  changeMessageList,
  addMessageList,
  selectUser,
  selectWs,
  changeWs,
  selectTyping,
  changeTyping,
  selectChatList,
  changeChatList,
  selectNotify,
  notify,
  addChatListMessage,
} from '../app/reducer'

import {
  ChannelControl,
} from './Label'

import WS from '../WebSocket'

const msg = require('../payload_pb');

const inputHeight = 80;

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    height: '100%',
    flexDirection: 'column',
  },
  list: {
    display: 'flex',
    flexGrow: 1,
    flexDirection: 'column',
    overflowY: 'auto',
  },
  appBar: {
    maxHeight: '100px',
  },
  content: {
    display: 'flex',
    flexDirection: 'column',
    height: '300px',
    overlow: 'hidden',
    flexGrow: 1,
  },
  input: {
    marginTop: 'auto',
    marginBottom: 0,
    display: 'flex',
    alignItems: 'center',
    width: '100%',
  },
  message: {
    display: 'flex',
    //flexGrow: 1,
    flexDirection: 'row',
  },
  myMessage: {
    justifyContent: 'flex-end',
  },
  theirMessage: {
    justifyContent: 'flex-start',
  },
  seens: {
    display: 'flex',
    flexDirection: 'row',
  },
  timestamp: {
    justifyContent: 'center',
    color: '#999',
    fontWeight: 600,
    fontSize: 12,
    textTransform: 'uppercase',
  },
  textMessage: {
    fontSize: 15,
  }
}));

function Seen(props) {
  const classes = useStyles()
  //TODO: fix
  return (<></>)
  return (
    <Box >
      <Badge
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
      >
        {props.seens.map(s => (<PeopleIcon fontSize="small" />))}
      </Badge>
    </Box>
  )
}

function formatDate(date) {
  return `${date.getDate()}-${date.getMonth() + 1}-${date.getFullYear()} ${date.getHours()}:${date.getMinutes()}`
}

function toDate(input) {
  if (typeof input == "string") {
    return new Date(Date.parse(input));
  } else if (typeof input == "object") {
    return input;
  } else {
    return new Date()
  }
}

function formatFullDate(input) {
  if (typeof input == "string") {
    const date = new Date(Date.parse(input));
    return formatDate(date)
    //return date.toLocaleString();
  } else if (typeof input == "object") {
    return formatDate(input)
    //return input.toLocaleString()
  } else {
    return "NULL"
  }
}

function TextMessage(props) {
  const user = useSelector(selectUser)
  const currentChannel = useSelector(selectCurrentChannel)
  const handleClick = (data) => {
    console.log(data)
  }

  const [groupData, setGroupData] = useState()
  const classes = useStyles()

  useEffect(() => {
    async function get() {
      const name = await DataSource.getUserFullname(user.sessionId, props.userId)
      setGroupData({ userFullName: name })
    }
    if (currentChannel.type == "group" && user.id != props.userId) {
      get()
    }
  }, [props.messageId])

  let timeComponent = formatFullDate(props.createAt)

  if (props.previous) {
    const previousTime = toDate(props.previous.createAt)
    const thisTime = toDate(props.createAt)

    if (previousTime.toDateString() === thisTime.toDateString()) {
      // 60s
      if ((thisTime - previousTime) / 1000 > 60) {
        timeComponent = `${thisTime.getHours()}:${thisTime.getMinutes()}`
      } else {
        timeComponent = null
      }
    }
  }

  return (
    <>
      {timeComponent &&
        <ListItem className={classes.timestamp} key={`D${props.messageId}`}>
          {timeComponent}
        </ListItem>
      }
      <ListItem
        className={props.userId == user.id ?
          classes.myMessage :
          classes.theirMessage
        }
        //autoFocus={true}
        key={props.messageId}
      >
        <span className={classes.message}>
          {
            groupData &&
            <Chip
              variant="outlined"
              label={
                <Box fontSize={18}>
                  {groupData.userFullName}:
              </Box>
              }
            />

          }
          <Chip
            label={
              <Box fontSize={18}>
                <Emoji text={props.content.content} />
              </Box>
            }
            onClick={handleClick}
            color={props.userId == user.id ? "primary" : "default"}
          />
          <Seen {...props} />
        </span>
      </ListItem>
    </>
  )
}

// TODO: use function
function getMoneyContent(str) {
  const arr = str.split(',')
  if (arr[0] == 'baokim') {
    let res = [arr[0], arr[1]]
    let str = ''
    for (let i = 2; i < arr.length; ++i) {
      str += arr[i]
    }
    res.push(str)
    return res
  } else {
    arr.unshift("quickchat")
    let res = [arr[0], arr[1]]
    let str = ''
    for (let i = 2; i < arr.length; ++i) {
      str += arr[i]
    }
    res.push(str)
    return res
  }
}
function getMoneyType(props) {

}

function Money(props) {
  const [type, amount, message] = getMoneyContent(props.content.content)
  return (
    <Card >
      <CardContent >
        <Typography color="textSecondary" gutterBottom>
          {`Transfer Money ${type}`}
        </Typography>
        <Typography variant="h5" component="h2">
          {`${props.what}${amount} VND`}
        </Typography>
        <Typography color="textSecondary">
          {message}
        </Typography>

      </CardContent>
    </Card>
  )
}

function MoneyMessage(props) {
  const user = useSelector(selectUser)
  const what = props.userId == user.id ? "-" : "+";
  const handleClick = (data) => {
    console.log(data)
  }
  const classes = useStyles()
  return (
    <>
      <ListItem className={classes.timestamp} key={`D${props.messageId}`}>
        {formatFullDate(props.createAt)}
      </ListItem>
      <ListItem
        className={props.userId == user.id ?
          classes.myMessage :
          classes.theirMessage
        }
        autoFocus={true}
        key={props.messageId}
      >
        <Money {...props} what={what} />
      </ListItem>
    </>
  )
}


//TODO: use function in chat tab

function Message(props) {

  switch (props.content.type) {
    case "TEXT":
    case "text":
    case msg.ChatContent.ChatContentType.TEXT:
      return <TextMessage {...props} />

    case "MONEY":
    case "money":
    case msg.ChatContent.ChatContentType.MONEY:
      return <MoneyMessage {...props} />
  }
  return (<></>)
}

export default function MessageControl(props) {

  const classes = useStyles();
  const messageList = useSelector(selectMessageList);
  const dispatch = useDispatch();
  const currentChannel = useSelector(selectCurrentChannel);
  const user = useSelector(selectUser);
  const ws = useSelector(selectWs);
  const typing = useSelector(selectTyping)

  const [emoji, setEmoji] = useState({ open: false });
  const [input, setInput] = useState('')

  const onEmojiClick = (emoji) => {
    //setEmoji({ open: false, obj: emojiObject });
    setInput(i => i + emoji.native)
  };

  const handleEmoij = () => {
    setEmoji({ open: true })
  }

  const scrollToBottom = () => {
    if (props.messageListRef.current) {
      props.messageListRef.current.scrollTop = props.messageListRef.current.scrollHeight
    }
  }

  const onChatMessage = async (obj) => {
    //console.log('onchat')
    //console.log(obj)
    if (!obj.createAt) {
      obj.createAt = new Date().toUTCString()
    }

    dispatch(addMessageList(obj))
    dispatch(addChatListMessage(obj))
  }

  const onTypingMessage = async (obj) => {
    if (!obj.createAt) {
      obj.createAt = new Date()
    }
    //console.log('typing')
    //console.log(obj)
    const timeout = 1400

    if (obj.isTyping) {
      if (obj.userId == user.id) return;

      const name = await DataSource.getUserFullname(user.sessionId, obj.userId)

      const cancel = setTimeout(() => {
        dispatch(changeTyping({ showOnChatItem: false, show: false }))
      }, timeout)

      dispatch(changeTyping({
        message: `${name} is typing...`,
        showOnChatItem: true,
        show: true,
        last: obj.createAt,
        cancel: cancel,
        data: obj,
      }))
    } else {
      //TODO: hanle
    }
  }

  useEffect(() => {
    async function getMessages() {
      //console.log(user)
      //console.log(currentChannel)
      if (!user.sessionId || !currentChannel) return
      const data = await DataSource.getMessageList(user.sessionId, currentChannel.id).then(res => res.data)
      //console.log(data)
      dispatch(changeMessageList(data))
    }
    getMessages();
  }, [currentChannel]);

  useEffect(() => {
    scrollToBottom()
  }, [messageList]);

  useEffect(() => {
    if (!WS.instance) return

    WS.instance.onopen = async (evt) => {
      console.log('websocket is opened')
      dispatch(notify({ open: true, severity: 'success', message: 'websocket is opened' }))
    }
    WS.instance.onclose = async (evt) => {
      console.log('websocket is closed')
      dispatch(notify({ open: true, severity: 'error', message: 'websocket is closed' }))
      // TODO: reconnect
      //setTimeout(() => {}, 5000)
    }

    WS.addOnMessageCallback((incoming) => {
      switch (incoming.getTypeCase()) {
        case msg.InMessage.TypeCase.CHAT:
          onChatMessage(incoming.getChat().toObject())
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
          const typing = incoming.getTyping().toObject();
          onTypingMessage(typing)
          break;
        case msg.InMessage.TypeCase.NOTIFICATION:
          const notification = incoming.getNotification();
          console.log('notification: ' + JSON.stringify(notification.toObject()));
          break;
        default:
          console.log(incoming.toObject());
      }
    })

  }, [ws, user])

  const [transaction, setTransaction] = useState({ open: false });
  const [baokim, setBaokim] = useState({ open: false })

  const handleCloseBaokimDialog = () => {
    setBaokim({ open: false })
  }
  const handleTransferBaokimMoney = async () => {
    try {

      const toUser = currentChannel.members.find(u => u.userId != user.id);

      const res = await DataSource.transferBaokim(user, toUser, baokim.amount, baokim.message)
      const message = JSON.stringify(res.message || {})
      if (res.code) {
        dispatch(notify({ open: true, severity: 'error', message: message }))
      } else {
        dispatch(notify({ open: true, severity: 'info', message: 'Waiting for payment' }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
    handleCloseBaokimDialog()
  }

  const handleOpenTransferDialog = () => {
    setTransaction({ open: true })
  }

  const handleOpenTransferBaokimDialog = () => {
    setBaokim({ open: true })
  }

  const handleCloseTransferDialog = () => {
    setTransaction({ open: false })
  }

  const handleTransferMoney = async () => {
    try {
      const toUser = currentChannel.members.find(u => u.userId != user.id).userId;
      const res = await DataSource.transfer(user.sessionId, toUser, transaction.amount, transaction.message)
      if (res.error) {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      } else {
        dispatch(notify({ open: true, severity: 'success', message: res.message || 'Done' }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
    handleCloseTransferDialog()
  }

  if (!currentChannel) {
    return (
      <>
      </>
    )
  }

  const sendChatMessage = (text) => {
    const out = new msg.OutMessage();
    const chat = new msg.Chat();
    const chatContent = new msg.ChatContent();
    chatContent.setContent(text);
    chatContent.setReplyTo(null);
    chatContent.setType(msg.ChatContent.ChatContentType.TEXT);

    chat.setChannelId(currentChannel.id);
    chat.setContent(chatContent);
    chat.setTimestamp(Timestamp.fromDate(new Date()));
    out.setChat(chat);
    WS.instance.send(out.serializeBinary());
  }

  const sendTypingMessage = () => {
    const out = new msg.OutMessage();
    const typing = new msg.Typing()

    typing.setChannelId(currentChannel.id);
    typing.setUserId(user.id)
    typing.setIsTyping(true)
    typing.setTimestamp(Timestamp.fromDate(new Date()));
    out.setTyping(typing);
    const res = WS.instance.send(out.serializeBinary());
  }

  const delay = 500;
  const handleTyping = throttle(() => {
    sendTypingMessage()
  }, delay)


  return (
    <div className={classes.root}>
      <AppBar position="relative" color="default" className={classes.appBar}>
        <Toolbar>
          <ChannelControl
            type={currentChannel.type}
            name={currentChannel.name}
          />
          {currentChannel.type == 'private' &&
            <>
              <Fab
                color="primary"
                variant="extended"
                onClick={handleOpenTransferDialog}
              >
                <TransferWithinAStationIcon />
              </Fab>
              <Fab
                color="default"
                variant="extended"
                onClick={handleOpenTransferBaokimDialog}
              >
                <BaoKimIcon />
              </Fab>
            </>
          }
        </Toolbar>

      </AppBar>
      <div className={classes.content}>
        <List className={classes.list} ref={props.messageListRef}>
          {messageList.map((message, index) => (
            <Message {...message} previous={messageList[index - 1]} />
          ))}
        </List>
      </div>

      {typing && typing.show &&
        <MuiAlert elevation={6} variant="filled" severity="info">
          {typing.message}
        </MuiAlert>
      }

      <Paper component="form" className={classes.input}>
        <OutlinedInput fullWidth
          //classeName={classes.inputText}
          // id="outlined-adornment-amount"
          multiline={true}
          rowsMax={3}
          value={input}
          onChange={(e) => {
            setInput(e.target.value)
          }}
          onKeyPress={(e) => {
            if (e.key === 'Enter' && !e.shiftKey) {
              sendChatMessage(input);
              dispatch(changeTyping({ showOnChatItem: false, show: false }))
              setInput('')
              e.preventDefault()
            } else {
              handleTyping()
            }
          }}
        />
        <UploadButtons />

        <IconButton color="primary" onClick={handleEmoij}>
          <EmojiEmotionsIcon fontSize="medium" />
        </IconButton>

        <IconButton color="primary" onClick={() => {
          sendChatMessage(input)
          setInput('')
        }}>
          <SendIcon fontSize="large" />
        </IconButton>

      </Paper>
      <Dialog open={emoji.open} onClose={() => setEmoji({ open: false })}>
        <DialogTitle id="form-dialog-title">Pick emoji</DialogTitle>
        <DialogContent>
          <Picker set='apple' onSelect={onEmojiClick} />
        </DialogContent>
      </Dialog>

      <Dialog open={transaction.open} onClose={handleCloseTransferDialog} aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">Transfer money</DialogTitle>
        <DialogContent>
          <TextField
            onChange={(evt) => setTransaction(t => ({ ...t, amount: evt.target.value }))}
            autoFocus
            margin="dense"
            id="amount"
            label="Amount:"
            fullWidth
          />
          <TextField
            onChange={(evt) => setTransaction(t => ({ ...t, message: evt.target.value }))}
            margin="dense"
            id="message"
            label="Messagge:"
            fullWidth
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={handleTransferMoney} color="primary">
            OK
          </Button>
          <Button onClick={handleCloseTransferDialog} color="secondary">
            Cancel
          </Button>
        </DialogActions>
      </Dialog>


      <Dialog open={baokim.open} onClose={handleCloseBaokimDialog} aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">Transfer Baokim money</DialogTitle>
        <DialogContent>
          <TextField
            onChange={(evt) => setBaokim(b => ({ ...b, amount: +evt.target.value }))}
            autoFocus
            margin="dense"
            id="amount"
            label="Amount:"
            fullWidth
          />
          <TextField
            onChange={(evt) => setBaokim(b => ({ ...b, message: evt.target.value }))}
            margin="dense"
            id="message"
            label="Messagge:"
            fullWidth
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={handleTransferBaokimMoney} color="primary">
            OK
          </Button>
          <Button onClick={handleCloseBaokimDialog} color="secondary">
            Cancel
          </Button>
        </DialogActions>
      </Dialog>

    </div>
  )
}

function BaoKimIcon() {
  return (
    <Avatar src="baokim.ico" />
  )
}

