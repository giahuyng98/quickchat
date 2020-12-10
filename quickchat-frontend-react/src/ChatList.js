import React, { useState } from 'react'
import { makeStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import Divider from '@material-ui/core/Divider';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import Avatar from '@material-ui/core/Avatar';
import Typography from '@material-ui/core/Typography';

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    '& > *': {
      margin: theme.spacing(1),
    }
  },
  inline: {
    display: 'inline',
  },
}))

function ChatItem(props) {
  const classes = useStyles()
  const handleClick = () => {
    props.onClick()
  }
  return (
    <ListItem selected={props.selected} alignItems="flex-start" button onClick={handleClick}>
      <ListItemAvatar>
        <Avatar alt="Remy Sharp" src="/static/images/avatar/1.jpg" />
      </ListItemAvatar>
      <ListItemText
        primary={props.channel.name}
        secondary={
          <React.Fragment>
            <Typography
              component="span"
              variant="body2"
              className={classes.inline}
              color="textPrimary"
            >
              Ali Connors
              </Typography>
            {" — I'll be in your neighborhood doing errands this…"}
          </React.Fragment>
        }
      />
    </ListItem>
  )
}

export default function ChatList(props) {
  const classes = useStyles();
  return (
    <div className={classes.root}>
      <List>
        {[1, 2, 3].map(() => {
          return (
            <ChatItem onClick={function() { }} />
          )
        })}
      </List>
    </div >
  )
}

