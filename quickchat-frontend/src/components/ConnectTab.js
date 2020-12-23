import React, { useState, useEffect } from 'react';
import { useSelector, useDispatch } from 'react-redux';
import PropTypes from 'prop-types';
import { makeStyles } from '@material-ui/core/styles';

import {
  Paper,
  Typography,
  Box,
  Divider,
  Button,
  List,
  ListItem,
  ListSubheader,
  ListItemAvatar,
  ListItemText,
  Avatar,
  ListItemSecondaryAction,
  IconButton,
} from '@material-ui/core';
import AddIcon from '@material-ui/icons/Add';
import PersonAddIcon from '@material-ui/icons/PersonAdd';
import GroupAddIcon from '@material-ui/icons/GroupAdd';
import CloseIcon from '@material-ui/icons/Close';
import DoneIcon from '@material-ui/icons/Done';
import TabPanel, { a11yProps } from './TabPanel'

import DataSource from '../DataSource'
import {
  selectTab,
  selectUser,
  selectNotify,
  notify,
} from '../app/reducer'

import { UserControl, BadgeAvatars } from './Label'
import UserSearchDialog, { GroupDialog } from './UserSearchDialog'

function FriendItem(props) {
  return (
    <>
      <ListItem button>
        <ListItemAvatar>
          <BadgeAvatars
            alt="Avatar"
            src={props.src || 'user.ico'}
          />
        </ListItemAvatar>
        <ListItemText primary={props.fullname} />
      </ListItem>
      <Divider />
    </>
  )
}
function FriendRequestItem(props) {
  return (
    <>
      <ListItem button>
        <ListItemAvatar>
          <Avatar
            alt="Avatar"
            src={props.src || 'user.ico'}
          />
        </ListItemAvatar>
        <ListItemText primary={props.fullname} />
        <ListItemSecondaryAction>
          <IconButton
            edge="end"
            onClick={() => props.onAccept(props.userId)}
          >
            <DoneIcon />
          </IconButton>
          <IconButton
            edge="end"
            onClick={() => props.onDecline(props.userId)}
          >
            <CloseIcon />
          </IconButton>
        </ListItemSecondaryAction>
      </ListItem>
      <Divider />
    </>
  )
}


export default function ConnectTab() {
  const tab = useSelector(selectTab);
  const classes = useStyles();
  const user = useSelector(selectUser);
  const dispatch = useDispatch()

  const [friendDialog, setFriendDialog] = useState({ open: false })
  const [groupDialog, setGroupDialog] = useState({ open: false })

  const [listFriends, setListFriends] = useState([])
  const [listFriendRequests, setListFriendRequests] = useState([])

  async function getFriends() {
    try {
      const data = await DataSource.getFriendList(user.sessionId);
      setListFriends(data.data.friends)
      setListFriendRequests(data.data.receivedRequest)
    } catch (err) {
      console.log(err)
    }
  }

  useEffect(() => {
    getFriends()
  }, [user])

  const handleAddFriend = () => {
    setFriendDialog({ open: true })
  }

  const handleCreateGroup = () => {
    setGroupDialog({ open: true })
  }

  const addFriend = async (id) => {
    try {
      const res = await DataSource.addFriend(user.sessionId, id);
      if (res.error == 0) {
        dispatch(notify({ open: true, severity: 'success', message: res.message }))
        getFriends()
      } else {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
  }

  const removeFriend = async (id) => {
    try {
      const res = await DataSource.removeFriend(user.sessionId, id);
      if (res.error == 0) {
        dispatch(notify({ open: true, severity: 'success', message: res.message }))
        getFriends()
      } else {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
  }

  const handleClickFriend = async (u) => {
    addFriend(u.id)
    handleCloseFriend()
  }

  const handleCloseFriend = () => {
    setFriendDialog({ open: false })
  }

  const handleCloseGroup = () => {
    setGroupDialog({ open: false })
  }

  const handleClickGroup = async (name, users) => {
    try {
      const res = await DataSource.createGroup(user.sessionId, user.id, name, users);
      if (res.error == 0) {
        dispatch(notify({ open: true, severity: 'success', message: res.message }))
      } else {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
    handleCloseGroup()
  }


  return (
    <TabPanel value={tab.index} index={1} className={classes.root}>
      <Box p={1}>
        <Paper>
          <UserControl alt='username' src='user.ico' />
          <Divider />
          <Box p={1} >
            <List className={classes.clist}>
              <ListItem>
                <Button
                  variant="contained"
                  color="primary"
                  startIcon={<PersonAddIcon />}
                  onClick={handleAddFriend}
                >Add friend
                </Button>
              </ListItem>

              <ListItem>
                <Button
                  variant="contained"
                  color="primary"
                  startIcon={<GroupAddIcon />}
                  onClick={handleCreateGroup}
                >Create group
                </Button>
              </ListItem>
            </List>

          </Box>
          <UserSearchDialog
            open={friendDialog.open}
            title="Add new friend"
            onClick={handleClickFriend}
            onClose={handleCloseFriend}
          />
          <GroupDialog
            open={groupDialog.open}
            title="Create group"
            onClick={handleClickGroup}
            onClose={handleCloseGroup}
          />
        </Paper>
      </Box>

      <div className={classes.three}>
        <List
          className={classes.list}
        >
          <ListSubheader >
            Friends
        </ListSubheader>

          {listFriends.map(f => <FriendItem {...f} />)}
        </List>

        <Divider />

        <List
          className={classes.list}
        >
          <ListSubheader >
            Friends requests
        </ListSubheader>
          {listFriendRequests.map(f => (
            <FriendRequestItem {...f}
              onAccept={addFriend}
              onDecline={removeFriend}
            />
          ))}
        </List>
      </div>
    </TabPanel>

  )
}
const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    flexDirection: 'column',
    overflow: 'hidden',
    //maxHeight: '300px',
    //height: '100%',
  },
  three: {
    display: 'flex',
    flexDirection: 'column',
    height: '100%',
    overlow: 'hidden',
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
  clistspace: {
    //padding: theme.spacing(3),
  },

}))


