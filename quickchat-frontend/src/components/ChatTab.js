import React, { useState, useEffect } from 'react';
import { useSelector, useDispatch } from 'react-redux';
import PropTypes from 'prop-types';

import {
  Paper,
  Typography,
  Box,
  List,
  Divider,
  ListItem,
  ListItemText,
  ListItemAvatar,
  ListSubheader,
  ListItemSecondaryAction,
  Avatar,
  Button,
  Badge,
} from '@material-ui/core';
import AddIcon from '@material-ui/icons/Add';

import { makeStyles, withStyles } from '@material-ui/core/styles';

import { UserControl, BadgeAvatars } from './Label'
import UserSearchDialog from './UserSearchDialog'
import TabPanel, { a11yProps } from './TabPanel'

import {
  selectTab,
  selectChatList,
  changeChatList,
  selectCurrentChannel,
  changeCurrentChannel,
  selectUser,
  changeUser,
  selectTyping,
  notify,
  selectNotify,
} from '../app/reducer'

//import DataSource from '../FakeDataSource'
import DataSource from '../DataSource'
import URL from '../URL'
import { blue } from '@material-ui/core/colors';

const msg = require('../payload_pb');
const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    flexDirection: 'column',
    overflow: 'hidden',
    //height: '100%',
  },
  content: {
    display: 'flex',
    flexDirection: 'column',
    overlow: 'hidden',
  },
  list: {
    overflowY: 'auto',
  },
  clist: {
    display: 'flex',
    flexDirection: 'row',
  },

}))

const BlueTypography = withStyles({
  root: {
    color: blue[500],
  }
})(Typography)


export default function ChatTab(props) {
  const classes = useStyles();
  const tab = useSelector(selectTab);
  const currentChannel = useSelector(selectCurrentChannel);
  const user = useSelector(selectUser);
  const chatList = useSelector(selectChatList);
  const dispatch = useDispatch();
  const typing = useSelector(selectTyping)

  const [openNewChat, setOpenNewChat] = useState(false);

  const handleClickOpenNewChat = () => {
    setOpenNewChat(true);
  }

  const handleCloseNewChat = () => {
    setOpenNewChat(false);
  }

  const handleCreateNewChat = async (data) => {
    console.log(data);
    try {
      const res = await DataSource.createNewChat(user.sessionId, user.id, data.id);
      if (!res.error) {
        // TODO: reload chatlist
        dispatch(notify({ open: true, severity: 'success', message: res.message }))
      } else {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
    handleCloseNewChat();
  }
  return (
    <TabPanel value={tab.index} index={0} className={classes.root}>
      <Box p={1}>
        <Paper>
          <UserControl alt='username' src='user.ico' />
          <Divider />
          <Box p={1}>
            <List className={classes.clist}>
              <ListItem>
                <Button
                  variant="contained"
                  color="primary"
                  startIcon={<AddIcon />}
                  onClick={handleClickOpenNewChat}
                >New Chat</Button>
              </ListItem>
            </List>

          </Box>
          <UserSearchDialog
            open={openNewChat}
            title="New chat"
            onClick={handleCreateNewChat}
            onClose={handleCloseNewChat}
          />
        </Paper>
      </Box>

      <List
        className={classes.list}
      >
        <ListSubheader disableSticky={true}>
          Chat list
        </ListSubheader>
        {chatList.map((chatItem, index) => {
          const showTyping = (typing && typing.showOnChatItem && chatItem.channel.id == typing.data.channelId)

          return (
            <ChatItem
              selected={currentChannel && currentChannel.id == chatItem.channel.id}
              onClick={() => {
                dispatch(changeCurrentChannel(chatItem.channel));
              }}
              typing={showTyping ? typing.message : null}
              {...chatItem}
            />
          )
        })}
      </List>
    </TabPanel >
  )
}
function getMessageType(type) {
  switch (type) {
    case "TEXT":
    case "text":
    case msg.ChatContent.ChatContentType.TEXT:
      return "text"
    case "MONEY":
    case "money":
    case msg.ChatContent.ChatContentType.MONEY:
      return "money"
  }
  return ""
}

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

function ChatItemText(props) {
  const [name, setName] = useState(null)
  useEffect(() => {
    async function getUserFullname() {
      const res = await DataSource.getUserFullname(props.user.sessionId, props.lastMessage.userId)
      setName(res)
    }
    getUserFullname()
  }, [props.user])

  if (!props.lastMessage || !props.lastMessage.content) {
    return (<></>)
  }

  if (props.lastMessage && getMessageType(props.lastMessage.content.type) == "money") {
    const [type, amount, message] = getMoneyContent(props.lastMessage.content.content)
    return (
      <Typography
        component="span"
        variant="body2"
        color="textPrimary"
      >
        <b>VND {amount} </b>: message
      </Typography>
    )
  }

  const msg = props.lastMessage.userId == props.user.id ?
    (
      <Typography
        component="span"
        variant="body2"
        color="textPrimary"
      >
        <b><i>You</i></b>: {props.lastMessage.content.content}
      </Typography>
    ) :
    (
      <Typography
        component="span"
        variant="body2"
        color="textPrimary"
      >
        {props.channel.type == "group" ? (<b><i>{name}: </i></b>) : ''}
        {props.lastMessage && props.lastMessage.content && props.lastMessage.content.content}
      </Typography>
    )
  return msg
}

function ChatItem(props) {

  const handleClick = (evt) => {
    props.onClick(props)
  }
  const user = useSelector(selectUser);
  return (
    <>
      <ListItem
        selected={props.selected}
        button onClick={handleClick}>

        <ListItemAvatar>
          {props.channel.type == "private" ?
            <BadgeAvatars
              src={props.src ?? 'user.ico'}
            />
            : <Avatar src={props.src ?? 'group.svg'} />
          }

        </ListItemAvatar>

        <Divider />

        <ListItemText
          primary={props.channel.name}
          secondary={
            <>
              {props.typing ?
                <BlueTypography
                  component="span"
                  variant="body2"
                  color="textSecondary"
                >
                  <Box fontStyle="italic">{props.typing}</Box>
                </BlueTypography> :
                <ChatItemText {...props} user={user} />
              }
            </>
          }
        />
        {
          (props.lastMessage.unread > 0) &&
          <ListItemSecondaryAction>
            <Badge badgeContent={props.lastMessage.unread} color="error" />
          </ListItemSecondaryAction>
        }
      </ListItem>
      <Divider />
    </>
  )
}


