import React, { Component, useState, useEffect } from 'react';
import {
  Button as CButton,
  MessageBox,
  ChatItem,
  ChatList,
  SystemMessage,
  MessageList,
  Input,
  Avatar,
  Navbar,
  SideBar,
  Dropdown,
  Popup,
} from 'react-chat-elements';

import moment from 'moment'
import FormControl from '@material-ui/core/FormControl';
import OutlinedInput from '@material-ui/core/OutlinedInput';
import InputLabel from '@material-ui/core/InputLabel';
import InputAdornment from '@material-ui/core/InputAdornment';

import FaSearch from 'react-icons/lib/fa/search';
import FaComments from 'react-icons/lib/fa/comments';
import FaClose from 'react-icons/lib/fa/close';
import FaMenu from 'react-icons/lib/md/more-vert';
import FaSquare from 'react-icons/lib/md/crop-square';

import { makeStyles } from '@material-ui/core/styles';
import Paper from '@material-ui/core/Paper';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import PhoneIcon from '@material-ui/icons/Phone';
import FavoriteIcon from '@material-ui/icons/Favorite';
import PersonPinIcon from '@material-ui/icons/PersonPin';
import Typography from '@material-ui/core/Typography';
import Container from '@material-ui/core/Container';
import Box from '@material-ui/core/Box';
import AppBar from '@material-ui/core/AppBar';
import PropTypes from 'prop-types';
import Drawer from '@material-ui/core/Drawer';
import CssBaseline from '@material-ui/core/CssBaseline';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import Divider from '@material-ui/core/Divider';
import Toolbar from '@material-ui/core/Toolbar';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Fab from '@material-ui/core/Fab';
import AddIcon from '@material-ui/icons/Add';
import EditIcon from '@material-ui/icons/Edit';
import ChatIcon from '@material-ui/icons/Chat';
import PeopleIcon from '@material-ui/icons/People';
import Button from '@material-ui/core/Button';
import MonetizationOnIcon from '@material-ui/icons/MonetizationOn';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import debounce from 'lodash/debounce';
import { Timestamp } from 'google-protobuf/google/protobuf/timestamp_pb';
import PersonAddIcon from '@material-ui/icons/PersonAdd';
import { UserControl, ChannelControl, MoneyControl } from "./Label";
import GroupAddIcon from '@material-ui/icons/GroupAdd';
import { FriendRequestList, FriendList } from './Friends'
import AddCircleIcon from '@material-ui/icons/AddCircle';
import TransferWithinAStationIcon from '@material-ui/icons/TransferWithinAStation';
import Search from './Search'
import RemoveIcon from '@material-ui/icons/Remove';
import ListSubheader from '@material-ui/core/ListSubheader';
import Snackbar from '@material-ui/core/Snackbar';
import TextField from '@material-ui/core/TextField';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';
import MuiAlert from '@material-ui/lab/Alert';
import CreditCardIcon from '@material-ui/icons/CreditCard';
import Grid from '@material-ui/core/Grid';
import DoneIcon from '@material-ui/icons/Done';
import ErrorIcon from '@material-ui/icons/Error';
import { green } from '@material-ui/core/colors';
import Carousel from 'react-material-ui-carousel'
import UploadButtons from './UploadButtons'
import IconButton from '@material-ui/core/IconButton';
import SendIcon from '@material-ui/icons/Send';
//import DataSource from './DataSource'
import DataSource from './FakeDataSource'
import WebSocketUtil from './WebSocket'

import 'react-chat-elements/dist/main.css';
import './App.css';
import MyChatList from './ChatList'
import UserSearchDialog from './UserSearchDialog'
import SlideItem from './SlideItem'
// import classes from '*.module.css';

const msg = require('./payload_pb');

function a11yProps(index) {
  return {
    id: `simple-tab-${index}`,
    'aria-controls': `simple-tabpanel-${index}`,
  };
}
function TabPanel(props) {
  const { children, value, index, ...other } = props;
  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`simple-tabpanel-${index}`}
      aria-labelledby={`simple-tab-${index}`}
      {...other}
    >
      {value === index && (
        <Box p={0}>
          {children}
        </Box>
      )}
    </div>
  );
}

TabPanel.propTypes = {
  children: PropTypes.node,
  index: PropTypes.any.isRequired,
  value: PropTypes.any.isRequired,
};


const leftPannelWidth = window.innerWidth / 4;
const rightPannelWidth = window.innerWidth / 5;

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    position: 'absolute',
    top: 0,
    bottom: 0,
    left: 0,
    right: 0,
  },
  chatListPanel: {
    display: 'flex',
    flexDirection: 'column',
    // width: '100%',
    // padding: 0,
    overflow: 'hidden',
  },
  chatList: {
    // minHeight: '100%',
    height: '700px',
    overflow: 'auto',
    // minWidth: '100%',
  },
  controlAppBar: {
    // overflow: 'hidden',
    // zIndex: theme.zIndex.chatListPanel + 1,
  },
  appBar: {
    width: `calc(100% - ${leftPannelWidth}px) `,
    marginLeft: leftPannelWidth,
  },
  centerPannel: {
    flex: 1,
    display: 'flex',
    flexDirection: 'column',
    width: `calc(100% - ${leftPannelWidth}px)`,
    marginLeft: leftPannelWidth,
  },
  slide: {
    position: 'center',
  },
  leftPannel: {
    width: leftPannelWidth,
    height: '100vh',
    // overflow: 'hidden',
    flexShrink: 0,
  },
  rightPannel: {
    width: rightPannelWidth,
    flexShrink: 0,
  },
  drawerPaper: {
    width: leftPannelWidth,
  },
  drawerPaperRight: {
    width: rightPannelWidth,
  },
  content: {
    display: 'flex',
    flexGrow: 1,
    padding: theme.spacing(3),
  },
  button: {
    margin: theme.spacing(1),
  },
  connectPanel: {

  },
  input: {
    display: 'flex',
    alignItems: 'center',
    width: '100%',
  },
  inputText: {
    marginLeft: theme.spacing(1),
    flex: 1,
  },
  messageList: {
    width: '100%',
    flex: '1 1 auto',
    overflow: 'auto',
    paddingTop: '1px',
    padding: '0px',
    margin: '0px',
  },
  spacing: {
    margin: theme.spacing(1),
  },
  fab: {
    position: 'absolute',
    bottom: theme.spacing(7),
    right: theme.spacing(2),
  },
  pad: {
    height: 56,
    [`${theme.breakpoints.up("xs")} and (orientation: landscape)`]: {
      height: 48
    },
    [theme.breakpoints.up("sm")]: {
      height: 64
    }
  },
  centerAppbar: {
    flexGrow: 1,
  },
}));

function Alert(props) {
  return <MuiAlert elevation={6} variant="filled" {...props} />;
}

const Conveter = {
  toChatListView: (chatList, userId) => {
    if (!chatList) return [];
    return chatList.map(chatItem => ({
      avatar: chatItem.channel.type === 'group' ? 'group.svg' : 'user.ico',
      channel: chatItem.channel,
      alt: chatItem.channel.name,
      title: chatItem.channel.name,
      subtitle: 'sub title',
      statusColor: '#44b700',
      statusColorType: 'encircle',
      subtitle: chatItem.lastMessage.content ? chatItem.lastMessage.content.content : '',
      date: chatItem.lastMessage.createAt ?
        Date.parse(chatItem.lastMessage.createAt) :
        null,
      unread: 1,
    }));
  },
  toMessageView: (user, message) => {
    let messageText = message.content.content;
    let messageType = 'text';
    if (message.content.type == 'MONEY'
      || message.content.type == msg.ChatContent.ChatContentType.MONEY
    ) {
      const commaPos = messageText.indexOf(',');
      messageType = 'system';
      if (message.userId == user.id) {
        messageText = `Transferred ${messageText.substr(0, commaPos)} VND with message: '${messageText.substr(commaPos + 1)}'`
      } else {
        messageText = `Received ${messageText.substr(0, commaPos)} VND with message: '${messageText.substr(commaPos + 1)}'`
      }
    }
    return ({
      channelId: message.channelId,
      messageId: message.messageId,
      userId: message.userId,
      // title : message.userId,
      position: (user.id === message.userId ? 'right' : 'left'),
      type: messageType,
      status: 'read',
      text: messageText,
      date: Date.parse(message.createAt),
      // avatar: 'user.ico'
    })
  }
  ,
  toMessageListView: (user, messageList) => {
    if (!messageList) return [];
    return messageList.map(message => Conveter.toMessageView(user, message));
  },
  toChatMessage: (pbChat) => {
    return ({
      channelId: pbChat.getChannelId(),
      messageId: pbChat.getMessageId(),
      userId: pbChat.getUserId(),
      content: {
        content: pbChat.getContent().getContent(),
        type: pbChat.getContent().getType()
      },
      createAt: pbChat.getTimestamp().toDate()
    })
  },
}

let ws;

export default function App(props) {

  // return (
  // )

  const [tabs, setTabs] = useState(0);
  const classes = useStyles();
  const [chatList, setChatList] = useState([]);
  const [messageList, setMessageList] = useState([]);
  const [currentChannel, setCurrentChannel] = useState(null);
  const [friendList, setFriendList] = useState({
    open: false,
    friends: [],
    sentRequests: [],
    receivedRequest: []
  });
  const [openNewChat, setOpenNewChat] = useState(false);
  const [openCreateMoneyAccount, setOpenCreateMoneyAccount] = useState(false);
  const [transaction, setTransaction] = useState({ open: false });
  const [userInfo, setUserInfo] = useState({ id: 0 });
  const [sessionId, setSessionId] = useState(DataSource.getSessionIdFromUrl());
  const [snackBar, setSnackBar] = useState({ open: false });
  const [moneyInfo, setMoneyInfo] = useState({});
  const [group, setGroup] = useState({});
  const [transactionHistory, setTransactionHistory] = useState([]);
  const [slide, setSlide] = useState([{ name: 'slide 1' }, { name: 'slide 2' }])
  //const [chatInput, setChatInput] = useState();
  // const [ws, setWs] = useState(null);
  const data = {}; //useState({});

  async function getUserInfo() {
    const data = await DataSource.getUserAuthenInfo(sessionId);
    setUserInfo(data.data);
  }

  useEffect(() => {
    getUserInfo();
  }, []);

  async function getMoneyInfo() {
    const data = await DataSource.getMoneyInfo(sessionId);
    setMoneyInfo(data.data);
  }
  useEffect(() => {
    getMoneyInfo();
  }, []);

  async function getTransactionHistory() {
    const data = await DataSource.history(sessionId);
    setTransactionHistory(data.data || []);
  };

  useEffect(() => {
    getTransactionHistory();
  }, []);

  async function getChatList() {
    const data = await DataSource.getChatList(sessionId);
    setChatList(Conveter.toChatListView(data.data));
  }
  useEffect(() => {
    getChatList();
  }, []);

  async function getFriendList() {
    const data = await DataSource.getFriendList(sessionId);
    setFriendList(data.data)
  }

  useEffect(() => {
    getFriendList();
  }, []);

  const updateChatList = (msg) => {
    setChatList(c => {
      const channelIdx = c.findIndex((chatItem) => chatItem.channel.id === msg.channelId);
      if (channelIdx === -1) {
        console.log('todo: //channelIdx == -1')
        return;
      }
      c[channelIdx].subtitle = msg.content.content;
      c[channelIdx].date = msg.createAt;
      if (userInfo && msg.userId != userInfo.id) {
        c[channelIdx].unread = 1;
      } else {
        c[channelIdx].unread = 0;
      }
      let update = []
      c.forEach(obj => update.push(obj))
      console.log('update chat list');
      return update;
    })
  }

  const updateMessageList = (msg) => {
    setCurrentChannel(chan => {
      if (!chan) return chan;
      if (chan.id === msg.channelId) {
        setMessageList(msgList => {
          const updated = msgList.find(
            (m) => (m.channelId == msg.channelId
              && m.messageId == msg.messageId)
          );
          if (updated) {
            return msgList;
          }
          console.log('update message list');
          let newMsgList = [];
          msgList.forEach(m => newMsgList.push(m));
          setUserInfo(u => {
            newMsgList.push(Conveter.toMessageView(u, msg));
            return u;
          })
          return newMsgList;
        })
      }
      return chan;
    })
  }

  const onChatMessage = (msg) => {
    updateChatList(msg);
    updateMessageList(msg);
  }

  useEffect(() => {
    ws = new WebSocket(URL.getWSURL(sessionId));
    ws.onopen = () => {
      console.log('Websocket is opened');
      setSnackBar({ open: true, severity: 'success', message: 'websocket is opened' });
    }

    ws.onclose = () => {
      console.log('Websocket is closed');
      setSnackBar({ open: true, severity: 'error', message: 'websocket is closed' });
    }
    return () => {
      ws.close()
    }
  }, [])

  useEffect(() => {
    ws.onmessage = async (evt) => {
      const data = await evt.data.arrayBuffer();
      const incoming = msg.InMessage.deserializeBinary(data);
      switch (incoming.getTypeCase()) {
        case msg.InMessage.TypeCase.CHAT:
          console.log('incoming');
          // console.log(incoming.getChat().toObject());
          onChatMessage(Conveter.toChatMessage(incoming.getChat()));
          const type = incoming.getChat().getContent().getType();
          if (type == msg.ChatContent.ChatContentType.MONEY) {
            getMoneyInfo();
            getTransactionHistory();
          }
          // props.onChatMessage();
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

  }, [])

  function handleClickOpenCreateMoneyAccount() {
    setOpenCreateMoneyAccount(true);
  }
  function handleCloseCreateMoneyAccount() {
    setOpenCreateMoneyAccount(false);
  }

  const handleCreateMoneyAccount = async () => {
    const res = await DataSource.createMoneyAccount(sessionId);
    if (res.error) {
      setSnackBar({ open: true, severity: 'error', message: res.message })

    } else {
      getMoneyInfo();
      setSnackBar({ open: true, severity: 'success', message: 'Done' })
    }
    setOpenNewChat(false);
  }

  function handleClickOpenTransaction(type) {
    //setTransaction(tran => {tran.open = true; tran.type = type; return tran;});
    setTransaction({ open: true, type: type, toUser: transaction.toUser });
  }

  function handleCloseTransaction() {
    //setTransaction(tran => { tran.open = false; return tran; })
    setTransaction({ open: false, type: transaction.type, toUser: transaction.toUser })
  }

  const handleTransaction = async () => {
    let res = null;
    console.log('transaction');
    console.log(transaction)
    switch (transaction.type) {
      case 'deposit':
        res = await DataSource.deposit(sessionId, transaction.amount);
        break;
      case 'withdraw':
        res = await DataSource.withdraw(sessionId, transaction.amount);
        break;
      case 'transfer':
        res = await DataSource.transfer(sessionId, transaction.toUser, transaction.amount, transaction.message);
        break;
      default:
        console.log("can't go here")
        return;
    }
    if (res == null || res.error) {
      const message = res ? res.message : 'unknow';
      setSnackBar({ open: true, severity: 'error', message: message })
    } else {
      getMoneyInfo();
      getTransactionHistory();
      setSnackBar({ open: true, severity: 'success', message: 'Done' })
    }
    handleCloseTransaction();
  }

  function handleOpenCreateGroup() {
    setGroup({ open: true })
  }

  function handleOpenAddFriend() {
    setFriendList({ ...friendList, open: true })
  }
  function handleCloseAddFriend() {
    setFriendList({ ...friendList, open: false })
  }

  const addFriend = async (userId) => {
    const res = await DataSource.addFriend(sessionId, userId);
    return res;
  }

  const handleAddFriend = async () => {
    if (!friendList.userId) {
      setSnackBar({ open: true, severity: 'error', message: 'invalid user ID' })
      handleCloseAddFriend();
      return;
    }
    const res = await addFriend(friendList.userId);
    if (res.error) {
      setSnackBar({ open: true, severity: 'error', message: res.message })
    } else {
      setSnackBar({ open: true, severity: 'success', message: res.message || 'Done' })
    }
    handleCloseAddFriend();
  }

  function handleCloseGroup() {
    setGroup({ open: false });
  }

  const handleCreateGroup = async () => {
    if (!group.members) {
      setSnackBar({ open: true, severity: 'error', message: 'empty list' })
      handleCloseGroup();
      return;
    }
    const members = group.members.split(',');
    const res = await DataSource.createGroup(sessionId, userInfo.id, group.name, members);
    if (res.error) {
      setSnackBar({ open: true, severity: 'error', message: res.message })
    } else {
      setSnackBar({ open: true, severity: 'success', message: 'Done' })
    }
    getChatList();
    handleCloseGroup();
  }

  function handleClickOpenNewChat() {
    setOpenNewChat(true);
  }

  function handleCloseNewChat() {
    setOpenNewChat(false);
  }


  const handleCreateNewChat = async (evt) => {
    const res = await DataSource.createNewChat(sessionId, userInfo.id, data.newChatUserId);
    if (res.error) {
      setSnackBar({ open: true, severity: 'error', message: res.message })
    } else {
      setSnackBar({ open: true, severity: 'success', message: 'Done' })
    }
    setOpenNewChat(false);
  }

  const onChangeNewChatUserId = (evt) => {
    data.newChatUserId = evt.target.value
  }

  const onChatListClick = (chatItem) => {
    setCurrentChannel(chatItem.channel)
    const toUser = chatItem.channel.members.find(u => u.userId != userInfo.id).userId;
    setTransaction(tran => { tran.toUser = toUser; return tran })

    DataSource.getMessageList(sessionId, chatItem.channel.id).then((res) => {
      if (res.error) {
        setSnackBar({ open: true, severity: 'error', message: res.message })
      } else {
        setMessageList(Conveter.toMessageListView(userInfo, res.data))
      }
    })

    const idx = chatList.findIndex(c => c.channel.id === chatItem.channel.id);
    if (idx != -1) {
      chatList[idx].unread = 0;
    }
    setChatList(chatList);
  }

  const handleCloseSnackBar = () => {
    setSnackBar({ open: false });
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
    ws.send(out.serializeBinary());
  }

  return (
    <div className={classes.root}>
      <CssBaseline />
      <Snackbar
        anchorOrigin={{ vertical: 'top', horizontal: 'center' }}
        open={snackBar.open}
        onClose={handleCloseSnackBar}
        autoHideDuration={10000}
      >
        <Alert onClose={handleCloseSnackBar} severity={snackBar.severity}>
          {snackBar.message}
        </Alert>
      </Snackbar>

      <Drawer
        className={classes.leftPannel}
        variant="permanent"
        anchor="left"
      // classes={{
      //   paper: classes.drawerPaper,
      // }}
      >
        <AppBar position="static" color="default" className={classes.controlAppBar}>
          <Tabs
            value={tabs}
            onChange={(evt, newValue) => { setTabs(newValue) }}
            // variant="fullWidth"
            indicatorColor="primary"
            textColor="primary"
            aria-label="tabs"
            maxWidth="sm"
          >
            <Tab icon={<ChatIcon />} aria-label="chat" label="chat" {...a11yProps(0)} />
            <Tab icon={<PeopleIcon />} aria-label="connect" label="connect" {...a11yProps(1)} />
            <Tab icon={<MonetizationOnIcon />} aria-label="money" label="money" {...a11yProps(2)} />
          </Tabs>
        </AppBar>
        <TabPanel value={tabs} index={0} className={classes.chatListPanel}>
          <Paper>
            <UserControl alt='username' src='user.ico' {...userInfo} />
          </Paper>

          <Button
            variant="contained"
            color="primary"
            className={classes.button}
            startIcon={<AddIcon />}
            onClick={handleClickOpenNewChat}
          >New Chat</Button>

          <UserSearchDialog
            open={openNewChat}
            title="New chat"
            onClose={handleCloseNewChat}
            onClick={handleCreateNewChat}
            onClose={handleCloseNewChat}
          />
          <Divider />
          <ChatList
            className={classes.chatList}
            dataSource={chatList}
            onClick={onChatListClick}
          />

        </TabPanel>
        <TabPanel value={tabs} index={1} className={classes.connectPanel}>
          <List>
            <ListItem>
              <Search />
            </ListItem>
            <Divider />
            <ListItem >
              <Button
                variant="contained"
                color="primary"
                className={classes.button}
                startIcon={<PersonAddIcon />}
                onClick={handleOpenAddFriend}
              >Add Friend</Button>

              <Button
                variant="contained"
                color="secondary"
                className={classes.button}
                startIcon={<GroupAddIcon />}
                onClick={handleOpenCreateGroup}
              >Create Group</Button>
            </ListItem>

            <Divider />

            <ListItem>
              <FriendRequestList
                dataSource={friendList.receivedRequest}
                onAccept={async (userId) => {
                  const res = await addFriend(userId);
                  if (res.error) {
                    setSnackBar({ open: true, severity: 'error', message: res.message })
                  } else {
                    setSnackBar({ open: true, severity: 'success', message: 'Done' })
                  }
                  getFriendList()
                }}
                onDecline={(userId) => {
                  alert('unimplement');
                }}
              />
            </ListItem>

            <Divider />

            <ListItem>
              <FriendList dataSource={friendList.friends} />
            </ListItem>
            <Dialog open={friendList.open} onClose={handleCloseAddFriend} aria-labelledby="form-dialog-title">
              <DialogTitle id="form-dialog-title">Add new friend</DialogTitle>
              <DialogContent>
                <TextField
                  onChange={(evt) => { setFriendList({ ...friendList, userId: evt.target.value }) }}
                  autoFocus
                  margin="dense"
                  id="userId"
                  label="User ID:"
                  fullWidth
                />
              </DialogContent>
              <DialogActions>
                <Button onClick={handleAddFriend} color="primary">
                  Add
                                    </Button>
                <Button onClick={handleCloseAddFriend} color="secondary">
                  Cancel
                                    </Button>
              </DialogActions>
            </Dialog>

            <Dialog open={group.open} onClose={handleCloseGroup} aria-labelledby="form-dialog-title">
              <DialogTitle id="form-dialog-title">Create new group</DialogTitle>
              <DialogContent>
                <TextField
                  onChange={(evt) => { setGroup(g => { g.name = evt.target.value; return g; }) }}
                  autoFocus
                  margin="dense"
                  id="name"
                  label="Group name:"
                  fullWidth
                />
                <TextField
                  onChange={(evt) => { setGroup(g => { g.members = evt.target.value; return g; }) }}
                  autoFocus
                  margin="dense"
                  id="userId"
                  label="Members ID (eg: 1,2,3):"
                  fullWidth
                />
              </DialogContent>
              <DialogActions>
                <Button onClick={handleCreateGroup} color="primary">
                  Create
                                    </Button>
                <Button onClick={handleCloseGroup} color="secondary">
                  Cancel
                                    </Button>
              </DialogActions>
            </Dialog>

          </List>
        </TabPanel>
        <TabPanel value={tabs} index={2} className={classes.moneyPanel}>
          {moneyInfo && moneyInfo.userId &&
            <React.Fragment>
              <List>
                <MoneyControl alt='username' src='user.ico' {...userInfo} {...moneyInfo} />
                <Divider />
                <Paper>
                  <Button
                    variant="contained"
                    color="primary"
                    className={classes.button}
                    startIcon={<AddCircleIcon />}
                    onClick={() => handleClickOpenTransaction('deposit')}
                  >Deposit</Button>
                  {/*
                                        <Button
                                        variant="contained"
                                        color="default"
                                        className={classes.button}
                                        startIcon={<TransferWithinAStationIcon />}
                                        onClick={() => handleClickOpenTransaction('transfer')}
                                    >Transfer</Button>
                                     */}
                  <Button
                    variant="contained"
                    color="secondary"
                    className={classes.button}
                    startIcon={<RemoveIcon />}
                    onClick={() => handleClickOpenTransaction('withdraw')}
                  >Withdraw</Button>
                </Paper>
              </List>

              <TransactionHistory dataSource={transactionHistory} />


            </React.Fragment>
          }
          {!moneyInfo &&
            <React.Fragment>
              <Paper>
                <UserControl alt='username' src='user.ico' {...userInfo} />
              </Paper>
              <Divider />
              <Paper>
                <Button
                  variant="contained"
                  color="primary"
                  className={classes.button}
                  startIcon={<CreditCardIcon />}
                  onClick={handleClickOpenCreateMoneyAccount}
                >Create Account</Button>
              </Paper>
              <Dialog open={openCreateMoneyAccount} onClose={handleCloseCreateMoneyAccount} aria-labelledby="form-dialog-title">
                <DialogTitle id="form-dialog-title">Create Account</DialogTitle>
                <DialogContent>
                  <DialogContentText>
                    Do You want to open an account?
                                    </DialogContentText>
                </DialogContent>
                <DialogActions>
                  <Button autoFocus onClick={handleCreateMoneyAccount} color="primary">
                    Yes
                                    </Button>
                  <Button onClick={handleCloseCreateMoneyAccount} color="secondary">
                    No
                                    </Button>
                </DialogActions>
              </Dialog>
            </React.Fragment>
          }

        </TabPanel>
      </Drawer>

      {currentChannel &&
        <React.Fragment>
          <AppBar position="fixed" color="default" className={classes.appBar}>
            <Toolbar>
              <ChannelControl {...currentChannel} />
              {currentChannel.type == 'private' &&
                <Fab color="secondary" variant="extended" onClick={() => {
                  handleClickOpenTransaction('transfer')
                }
                }>
                  <TransferWithinAStationIcon />
                </Fab>
              }
            </Toolbar>
          </AppBar>
          <div className="right-panel">
            <MessageList
              className={classes.messageList}
              lockable={true}
              toBottomHeight={'100%'}
              downButtonBadge={10}
              dataSource={messageList}
            />
            <Paper component="form" className={classes.input}>
              <OutlinedInput fullWidth classeName={classes.inputText}
                // id="outlined-adornment-amount"
                multiline={true}
                rowsMax={3}
                onKeyPress={(e) => {
                  if (e.key === 'Enter' && !e.shiftKey) {
                    sendChatMessage(e.target.value);
                    e.target.value = '';
                    e.preventDefault()
                  }
                }}
              />
              <UploadButtons />
              <IconButton color="primary">
                <SendIcon fontSize="large" />
              </IconButton>
            </Paper>
          </div>

          <Drawer
            className={classes.rightPannel}
            variant="permanent"
            anchor="right"
            classes={{
              paper: classes.drawerPaperRight,
            }}
          >
          </Drawer>
        </React.Fragment>
      }
      {!currentChannel &&
        <React.Fragment>
          <Carousel className={classes.slide}>
            {slide.map((item, i) => <SlideItem {...item} />)}
          </Carousel>
        </React.Fragment>
      }
      <Dialog open={transaction.open} onClose={handleCloseTransaction} aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">{`Transaction ${transaction.type}`}</DialogTitle>
        <DialogContent>
          {transaction.type == 'transfer' &&
            <React.Fragment>
              {/*
                                            <TextField
                                                onChange={(evt) => setTransaction(tran => { tran.toUser = evt.target.value; return tran })}
                                                autoFocus
                                                margin="dense"
                                                id="userId"
                                                label="User ID:"
                                                fullWidth
                                            />
                                        */}

              <TextField
                onChange={(evt) => setTransaction(tran => { tran.message = evt.target.value; return tran })}
                autoFocus
                margin="dense"
                id="message"
                label="Messagge:"
                fullWidth
              />
            </React.Fragment>
          }

          <TextField
            onChange={(evt) => setTransaction(tran => { tran.amount = evt.target.value; return tran })}
            autoFocus
            margin="dense"
            id="amount"
            label="Amount:"
            fullWidth
          />
        </DialogContent>
        <DialogActions>
          <Button onClick={handleTransaction} color="primary">
            OK
                                    </Button>
          <Button onClick={handleCloseTransaction} color="secondary">
            Cancel
                                    </Button>
        </DialogActions>
      </Dialog>

    </div>
  );
}

const TransactionHistory = (props) => {
  return (
    <React.Fragment>
      <Typography align="center">
        Transactions History
            </Typography>
      <List>
        {props.dataSource.map(history => (
          <Paper>
            <ListItem button key={history.requestId}>
              <Grid container spacing={2}>
                <Grid item xs={12}>
                  <Grid container justify="center" spacing={2}>

                    <Grid item>
                      {history.type === "deposit" && <AddCircleIcon />}
                      {history.type === "transfer" && <TransferWithinAStationIcon />}
                      {history.type === "withdraw" && <RemoveIcon />}
                    </Grid>

                    <Grid item>
                      <Typography>
                        {history.type}
                      </Typography>
                    </Grid>

                    <Grid item>
                      <Typography>
                        {history.amount > 0 ? `+${history.amount}` : history.amount}
                      </Typography>
                    </Grid>

                    <Grid item>
                      {history.status === "success"
                        && <DoneIcon style={{ color: green[500] }} />}
                      {history.status === "error"
                        && <ErrorIcon color="action" />}
                    </Grid>

                    <Grid item>
                      <Typography>
                        {formatTime(history.createAt)}
                      </Typography>
                    </Grid>

                  </Grid>
                </Grid>
              </Grid>
            </ListItem>
            <Divider />
          </Paper>
        ))}
      </List>
    </React.Fragment>
  )
}


const formatTime = (str) => {
  return str;
  // const t = Date.parse(str)
  // return `${dt.getHours()}:${dt.getMinutes()} ${dt.toDateString()}`
}
