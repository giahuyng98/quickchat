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
import ListItemIcon from '@material-ui/core/ListItemIcon';
import PersonIcon from '@material-ui/icons/Person';
import Search from './Search';
import Datasource from './FakeDataSource';
import ListItemSecondaryAction from '@material-ui/core/ListItemSecondaryAction';
import Checkbox from '@material-ui/core/Checkbox';
import IconButton from '@material-ui/core/IconButton';
import CommentIcon from '@material-ui/icons/Comment';

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
          {users.map(u => {
            return (
              <>
                <ListItem button>
                  <ListItemIcon>
                    <PersonIcon />
                  </ListItemIcon>
                  <ListItemText>
                    {u.username}
                  </ListItemText>
                  <ListItemSecondaryAction>
                    <IconButton edge="end" aria-label="comments">
                      <CommentIcon />
                    </IconButton>
                  </ListItemSecondaryAction>
                </ListItem>
                <Divider />
              </>
            )
          })}
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
