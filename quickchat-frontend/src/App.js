import React, { useState, useEffect } from 'react';
import { useSelector, useDispatch } from 'react-redux';
import logo from './logo.svg';

import {
  Button,
  CssBaseline,
  Snackbar,
  Input,
  Drawer,
  AppBar,
  Paper,
  Tabs,
  Tab,
  Box,
  Typography,
} from '@material-ui/core';

import PeopleIcon from '@material-ui/icons/People';
import ChatIcon from '@material-ui/icons/Chat';
import MonetizationOnIcon from '@material-ui/icons/MonetizationOn';

import PhoneIcon from '@material-ui/icons/Phone';
import FavoriteIcon from '@material-ui/icons/Favorite';
import PersonPinIcon from '@material-ui/icons/PersonPin';

import { createMuiTheme, ThemeProvider, makeStyles } from '@material-ui/core/styles';
import { green, orange } from '@material-ui/core/colors';
import Alert from '@material-ui/lab/Alert';

import { a11yProps } from './components/TabPanel'
import ChatTab from './components/ChatTab'
import ConnectTab from './components/ConnectTab'
import MoneyTab from './components/MoneyTab'
import MessageControl from './components/MessageControl'
import SignIn from './components/SignIn'
import URL from './URL'

import {
  notify,
  changeTab,
  selectNotify,
  selectTab,
  selectChatList,
  changeChatList,
  selectUser,
  changeUser,
  changeWs,
  selectWs,
} from './app/reducer'

import DataSource from './DataSource'
import FakeDataSource from './FakeDataSource'
import WS from './WebSocket'

import './App.css';

const theme = createMuiTheme({
  palette: {
    secondary: {
      main: orange[500],
    },
  },
});


const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    height: '100vh',
  },
  tabs: {
    height: '100%',
  },
  left: {
    display: 'flex',
    flexDirection: 'column',
    maxWidth: '480px',
    height: '100%',
  },
  leftContent: {

  },
  center: {
    display: 'flex 1 1',
    flexDirection: 'column',
    height: '100%',
    overflow: 'hidden',
    flexGrow: 5,
  },
  right: {

  },

}));


export default function App() {
  const user = useSelector(selectUser);
  const sessionId = DataSource.getSessionIdFromUrl()
  if (sessionId && sessionId.length > 0) {
    return <Main />
  }
  return <SignIn />
}

function Main() {
  const dispatch = useDispatch();
  const snackbar = useSelector(selectNotify);
  const tab = useSelector(selectTab);
  const classes = useStyles();
  const messageListRef = React.createRef();
  const user = useSelector(selectUser);
  const ws = useSelector(selectWs);
  const handleCloseSnackBar = () => {
    dispatch(notify({ open: false }))
  }

  useEffect(() => {
    async function getUser() {
      const data = await DataSource.getUserAuthenInfo(DataSource.getSessionIdFromUrl()).then(res => res.data)
      dispatch(changeUser(data))
    }
    getUser();
  }, [])

  useEffect(() => {
    async function getChatList() {
      if (!user.sessionId) return
      const data = await DataSource.getChatList(user.sessionId).then(res => res.data)
      dispatch(changeChatList(data))
    }
    getChatList();
  }, [user]);

  useEffect(() => {
    if (!user.sessionId) return
    WS.instance = DataSource.createWebSocket(user.sessionId)
    //dispatch(notify({ open: true, severity: 'success', message: 'websocket is opened' }))

    dispatch(changeWs({ open: true }))
  }, [user]);

  return (
    <ThemeProvider
    //theme={theme}
    >
      <div className={classes.root}>
        <CssBaseline />
        <Snackbar
          anchorOrigin={{ vertical: 'top', horizontal: 'center' }}
          open={snackbar.open}
          onClose={handleCloseSnackBar}
          autoHideDuration={10000}
        >
          <Alert onClose={handleCloseSnackBar} severity={snackbar.severity}>
            {snackbar.message}
          </Alert>
        </Snackbar>

        <div className={classes.left}>
          <AppBar position="static" color="default">
            <Tabs
              value={tab.index}
              onChange={(evt, value) => { dispatch(changeTab({ index: value })) }}
              variant="fullWidth"
              indicatorColor="primary"
              textColor="primary"
              className={classes.tabs}
            >
              <Tab icon={<ChatIcon />} label="Chat" {...a11yProps(0)} />
              <Tab icon={<PeopleIcon />} label="Connect" {...a11yProps(1)} />
              <Tab icon={<MonetizationOnIcon />} label="Money" {...a11yProps(2)} />
            </Tabs>
          </AppBar>
          <ChatTab messageListRef={messageListRef} />
          <ConnectTab />
          <MoneyTab />
        </div>

        <div className={classes.center}>
          <MessageControl messageListRef={messageListRef} />
        </div>

      </div>

    </ThemeProvider>
  );
}

