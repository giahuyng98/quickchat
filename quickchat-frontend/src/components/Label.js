import React, { useState } from 'react';
import Badge from '@material-ui/core/Badge';
import Avatar from '@material-ui/core/Avatar';
import { makeStyles, withStyles } from '@material-ui/core/styles';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemAvatar from '@material-ui/core/ListItemAvatar';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import Button from '@material-ui/core/Button'
import Paper from '@material-ui/core/Paper';
import { Divider, IconButton } from '@material-ui/core';
import InputLabel from '@material-ui/core/InputLabel';
import MenuItem from '@material-ui/core/MenuItem';
import FormControl from '@material-ui/core/FormControl';
import Select from '@material-ui/core/Select';
import Hidden from '@material-ui/core/Hidden';
import FiberManualRecordIcon from '@material-ui/icons/FiberManualRecord';

import { useSelector, useDispatch } from 'react-redux';
import { selectUser, changeUser, changeStatus } from '../app/reducer'

import { green, red } from '@material-ui/core/colors';
const statusGreen = green[500];
const statusRed = red[500];

const StyledBadge = withStyles((theme) => ({
  badge: {
    backgroundColor: statusGreen,
    color: statusGreen,
    boxShadow: `0 0 0 2px ${theme.palette.background.paper}`,
    '&::after': {
      position: 'absolute',
      top: 0,
      left: 0,
      width: '100%',
      height: '100%',
      borderRadius: '50%',
      animation: '$ripple 1.2s infinite ease-in-out',
      border: '1px solid currentColor',
      content: '""',
    },
  },
  '@keyframes ripple': {
    '0%': {
      transform: 'scale(.8)',
      opacity: 1,
    },
    '100%': {
      transform: 'scale(2.4)',
      opacity: 0,
    },
  },
}))(Badge);

const StyleOnline = withStyles((theme) => ({
  badge: {
    backgroundColor: statusGreen,
    color: statusGreen,
    boxShadow: `0 0 0 2px ${theme.palette.background.paper}`,
    '&::after': {
      position: 'absolute',
      top: 0,
      left: 0,
      width: '100%',
      height: '100%',
      borderRadius: '50%',
      animation: '$ripple 1.2s infinite ease-in-out',
      border: '1px solid currentColor',
      content: '""',
    },
  },
  '@keyframes ripple': {
    '0%': {
      transform: 'scale(.8)',
      opacity: 1,
    },
    '100%': {
      transform: 'scale(2.4)',
      opacity: 0,
    },
  },
}))(Badge);

const StyleOffline = withStyles((theme) => ({
  badge: {
    backgroundColor: statusRed,
    color: statusRed,
    boxShadow: `0 0 0 2px ${theme.palette.background.paper}`,
    '&::after': {
      position: 'absolute',
      top: 0,
      left: 0,
      width: '100%',
      height: '100%',
      borderRadius: '50%',
      animation: '$ripple 1.2s infinite ease-in-out',
      border: '1px solid currentColor',
      content: '""',
    },
  },
  '@keyframes ripple': {
    '0%': {
      transform: 'scale(.8)',
      opacity: 1,
    },
    '100%': {
      transform: 'scale(2.4)',
      opacity: 0,
    },
  },
}))(Badge);

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    '& > *': {
    },
  },
  noBadge: {
    display: 'flex',
    '& > *': {
      margin: theme.spacing(1),
    },
  },
  formControl: {
    display: 'none',
    margin: theme.spacing(1),
    // minWidth: 120,
  },
}));

export function BadgeAvatars(props) {
  const classes = useStyles();

  return (
    <div className={classes.root}>
      <StyledBadge
        overlap="circle"
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
        variant="dot"
      >
        <Avatar alt={props.alt} src={props.src} />
      </StyledBadge>
    </div>
  );
}

export function ChannelAvatar(props) {
  const classes = useStyles();

  //TODO: WTF noBadge
  return (
    <div className={classes.noBadge}>
      <StyledBadge
        overlap="circle"
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
        variant="dot"
      >
        <Avatar alt={props.alt} src={props.src} />
      </StyledBadge>
    </div>
  );
}
const statusStyles = makeStyles(theme => ({
  root: {
    display: 'flex',
    '& > *': {
      margin: theme.spacing(1),
    },
  },
  badge: {
    backgroundColor: statusGreen,
    color: statusGreen,
    boxShadow: `0 0 0 2px ${theme.palette.background.paper}`,
    '&::after': {
      position: 'absolute',
      top: 0,
      left: 0,
      width: '100%',
      height: '100%',
      borderRadius: '50%',
      animation: '$ripple 1.2s infinite ease-in-out',
      border: '1px solid currentColor',
      content: '""',
    },
  },
  '@keyframes ripple': {
    '0%': {
      transform: 'scale(.8)',
      opacity: 1,
    },
    '100%': {
      transform: 'scale(2.4)',
      opacity: 0,
    },
  },
}))

const StatusAvatar = (props) => {
  const classes = statusStyles();
  return (
    <div className={classes.root}>
      <Badge
        //className={classes.badge}
        //background="#ff0000"
        overlap="circle"
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
        variant="dot"
      >
        <Avatar alt={props.alt} src={props.src} />
      </Badge>
    </div>
  )
}

function NoBadgeAvatars(props) {
  const classes = useStyles();

  return (
    <div className={classes.noBadge}>
      <StyledBadge
        background="#ff0000"
        overlap="circle"
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
      >
        <Avatar alt={props.alt} src={props.src} />
      </StyledBadge>
    </div>
  );
}

//export function UserControl(props) {
//  const classes = useStyles();
//  return (
//    <ListItem>
//      <ListItemAvatar>
//        <div className={classes.root}>
//          <IconButton>
//            <StatusAvatar {...props}/>
//          </IconButton>
//        </div>
//      </ListItemAvatar>
//      <ListItemText primary={`${props.fullname} (${props.username})`} />
//      <Divider />
//    </ListItem>
//  )
//}

export function UserControl(props) {
  const classes = useStyles();
  const [open, setOpen] = useState(false);
  const dispatch = useDispatch()

  const user = useSelector(selectUser);

  const handleClose = () => {
    setOpen(false);
  };

  const handleOpen = () => {
    setOpen(true);
  };

  const handleChange = (evt) => {
    dispatch(changeStatus(evt.target.value));
  };

  return (
    <ListItem>
      <ListItemAvatar>
        <div className={classes.root}>
          <IconButton
            onClick={handleOpen}
          >
            {user.status == "offline" &&
              <StyleOffline
                overlap="circle"
                anchorOrigin={{
                  vertical: 'bottom',
                  horizontal: 'right',
                }}
                variant="dot"
              >
                <Avatar alt={props.alt} src={props.src} />
              </StyleOffline>
            }
            {user.status == "online" &&
              <StyleOnline
                overlap="circle"
                anchorOrigin={{
                  vertical: 'bottom',
                  horizontal: 'right',
                }}
                variant="dot"
              >
                <Avatar alt={props.alt} src={props.src} />
              </StyleOnline>
            }

          </IconButton>
          {open &&
            <FormControl component='span' className={classes.formControl}>
              <InputLabel id="lb-status">Status</InputLabel>
              <Select
                labelId="lb-status"
                id="status"
                open={open}
                onClose={handleClose}
                onOpen={handleOpen}
                value={user.status}
                onChange={handleChange}
              >
                <MenuItem value={'online'}>
                  <ListItemIcon>
                    <FiberManualRecordIcon style={{ color: statusGreen }} />
                  </ListItemIcon>
                  <ListItemText>
                    Online
                  </ListItemText>
                </MenuItem>
                <MenuItem value={'offline'}>
                  <ListItemIcon>
                    <FiberManualRecordIcon style={{ color: statusRed }} />
                  </ListItemIcon>
                  <ListItemText>
                    Offline
                  </ListItemText>
                </MenuItem>
                <MenuItem value={'busy'}>Busy</MenuItem>
                <MenuItem value={'away'}>Away</MenuItem>
              </Select>
            </FormControl>
          }
        </div>
      </ListItemAvatar>
      <ListItemText primary={`${user.fullname} (${user.username})`} />
      <Divider />
    </ListItem>
  )
}

export function MoneyControl(props) {
  const classes = useStyles();
  return (
    <List>
      <ListItem>
        <ListItemAvatar>
          <div className={classes.root}>
            <IconButton>
              <StyledBadge
                overlap="circle"
                anchorOrigin={{
                  vertical: 'bottom',
                  horizontal: 'right',
                }}
                variant="dot"
              >
                <Avatar alt={props.alt} src={props.src} />
              </StyledBadge>
            </IconButton>
          </div>
        </ListItemAvatar>
        <ListItemText primary={`${props.fullname} (${props.username})`} />
      </ListItem>
      <Divider />
      <ListItem>
        <ListItemText primary={`Balance: ${props.balance} VND`} />
      </ListItem>
    </List>
  )
}


export function FriendControl(props) {
  const classes = useStyles();
  return (
    <ListItem>
      <ListItemAvatar>
        <div className={classes.root}>
          <StyledBadge
            overlap="circle"
            anchorOrigin={{
              vertical: 'bottom',
              horizontal: 'right',
            }}
            variant="dot"
          >
            <Avatar alt={props.alt} src={props.src} />
          </StyledBadge>
        </div>
      </ListItemAvatar>
      <ListItemText primary={props.text} />
    </ListItem>
  )
}


export function ChannelControl(props) {
  return (
    <ListItem>
      <ListItemAvatar>
        {props.type === 'group' ?
          <ChannelAvatar alt={props.alt} src={'group.svg'} />
          :
          <ChannelAvatar alt={props.alt} src={'user.ico'} />}
      </ListItemAvatar>
      <ListItemText primary={props.name} />
    </ListItem>
  )
}

