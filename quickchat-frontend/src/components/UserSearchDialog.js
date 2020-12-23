import React, { useState } from 'react';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import List from '@material-ui/core/List';
import TextField from '@material-ui/core/TextField';
import Divider from '@material-ui/core/Divider';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import ListSubheader from '@material-ui/core/ListSubheader';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import Box from '@material-ui/core/Box';
import PersonIcon from '@material-ui/icons/Person';
import Search from './Search';
import Datasource from '../DataSource';
import ListItemSecondaryAction from '@material-ui/core/ListItemSecondaryAction';
import Checkbox from '@material-ui/core/Checkbox';
import IconButton from '@material-ui/core/IconButton';
import CommentIcon from '@material-ui/icons/Comment';

function Item(props) {
  return (
    <>
      <ListItem button
        onClick={props.onClick}
      >
        <ListItemIcon>
          <PersonIcon />
        </ListItemIcon>
        <ListItemText>
          {props.fullname} ({props.username})
                  </ListItemText>
        {/*
                  <ListItemSecondaryAction>
                    <IconButton edge="end" aria-label="comments">
                      <CommentIcon />
                    </IconButton>
                  </ListItemSecondaryAction>
                */}
      </ListItem>
      <Divider />
    </>
  )
}

export default function UserSearchDialog(props) {

  const [users, setUsers] = useState([])

  const handleChange = async (e) => {
    setUsers(await Datasource.findUser(e.target.value))
  }

  return (
    <Dialog open={props.open}
      onClose={props.onClose}
      aria-labelledby="form-dialog-title"
      fullWidth={true}
    >
      <DialogTitle id="form-dialog-title">
        {props.title}
      </DialogTitle>
      <DialogContent>
        <Search
          placeholder="Type username or email"
          onChange={handleChange}
        />
        <Divider />
        <List>
          {users.map(u => <Item {...u} onClick={() => props.onClick(u)} />)}
        </List>
      </DialogContent>
      <DialogActions>
        <Button
          variant="contained"
          onClick={props.onClose}
          color="secondary"
        >
          Cancel
        </Button>

      </DialogActions>
    </Dialog >
  )
}

export function GroupDialog(props) {
  const [users, setUsers] = useState([])
  const [selected, setSeletected] = useState([])
  const [name, setName] = useState()

  const handleChange = async (e) => {
    let res = await Datasource.findUser(e.target.value)
    res = res.filter(u => {
      for (const s of selected) {
        if (u.id == s.id) return false
      }
      return true
    })
    setUsers(res)
  }

  return (
    <Dialog open={props.open}
      onClose={props.onClose}
      aria-labelledby="form-dialog-title"
      fullWidth={true}
    >
      <DialogTitle id="form-dialog-title">
        {props.title}
      </DialogTitle>
      <DialogContent>
        <TextField
          onChange={(evt) => setName(evt.target.value)}
          value={name}
          autoFocus
          margin="dense"
          id="name"
          label="Group name:"
          fullWidth
        />
        <Divider />
        <Box>
          <List>
            <Search
              placeholder="Type username or email"
              onChange={handleChange}
            />
            <Divider />
            <List>
              <ListSubheader>
                Members
              </ListSubheader>
              {selected.map(u => <Item {...u} onClick={() => {
                setSeletected(s => s.filter(uu => uu.id != u.id))
              }} />)}
            </List>

            <List>
              <ListSubheader>
                Users
              </ListSubheader>
              {users.map(u => <Item {...u} onClick={() => {
                setSeletected(s => ([...s, u]))
                setUsers(users.filter(uu => uu.id != u.id))
              }} />)}
            </List>
          </List>
        </Box>

        <Divider />
      </DialogContent>
      <DialogActions>
        <Button
          variant="contained"
          onClick={() => props.onClick(name, selected.map(u => u.id))}
          color="primary"
        >
          Create
        </Button>

        <Button
          variant="contained"
          onClick={props.onClose}
          color="secondary"
        >
          Cancel
        </Button>
      </DialogActions>
    </Dialog >
  )
}
