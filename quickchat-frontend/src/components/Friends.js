import React, { useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemSecondaryAction from '@material-ui/core/ListItemSecondaryAction';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import Checkbox from '@material-ui/core/Checkbox';
import Avatar from '@material-ui/core/Avatar';
import IconButton from '@material-ui/core/IconButton';
import CheckIcon from '@material-ui/icons/Check';
import CloseIcon from '@material-ui/icons/Close';
import DoneIcon from '@material-ui/icons/Done';
import BadgeAvatars from './Label'
import Divider from '@material-ui/core/Divider';
import ListSubheader from '@material-ui/core/ListSubheader';


const useStyles = makeStyles((theme) => ({
  root: {
    width: '100%',
    backgroundColor: theme.palette.background.paper,
  },
}));

export function FriendRequestList(props) {
  const classes = useStyles();
  if (!props.dataSource || props.dataSource.length == 0) {
    return <React.Fragment />
  }
  return (
    <List className={classes.root}>
      <ListSubheader>Friends Request</ListSubheader>
      {props.dataSource.map((f) => {
        return (
          <React.Fragment>
            <ListItem key={f.userId} button >
              <ListItemAvatar>
                <Avatar
                  alt={`Avatar n°`}
                  src={f.src || 'user.ico'}
                />
              </ListItemAvatar>
              <ListItemText id={f.userId} primary={f.fullname} />
              <ListItemSecondaryAction>
                <IconButton
                  edge="end"
                  onClick={() => props.onAccept(f.userId)}
                >
                  <DoneIcon />
                </IconButton>
                <IconButton
                  edge="end"
                  onClick={() => props.onDecline(f.userId)}
                >
                  <CloseIcon />
                </IconButton>
              </ListItemSecondaryAction>
            </ListItem>
            <Divider />
          </React.Fragment>
        )
      })}
    </List>
  );
}

export function FriendList(props) {
  const classes = useStyles();
  return (
    <List className={classes.root}>
      <ListSubheader>Friends</ListSubheader>
      {props.dataSource.map((f) => {
        return (
          <React.Fragment>
            < ListItem key={f.userId} button >
              <BadgeAvatars src={f.src || 'user.ico'} />
              <ListItemText primary={f.fullname} />
            </ListItem>
            <Divider />
          </React.Fragment>
        )
      })}
    </List>
  );
}
