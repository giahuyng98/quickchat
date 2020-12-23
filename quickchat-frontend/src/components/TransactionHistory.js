import React, { useState, useEffect } from 'react';
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
  Grid,
} from '@material-ui/core';

import { green } from '@material-ui/core/colors';

import TransferWithinAStationIcon from '@material-ui/icons/TransferWithinAStation';
import RemoveIcon from '@material-ui/icons/Remove';
import DoneIcon from '@material-ui/icons/Done';
import ErrorIcon from '@material-ui/icons/Error';
import AddCircleIcon from '@material-ui/icons/AddCircle';

import { makeStyles } from '@material-ui/core/styles';
const useStyles = makeStyles((theme) => ({
  list: {
    overflowY: 'auto',
  },

}))



export default function TransactionHistory(props) {
  const classes = useStyles();
  return (
    <React.Fragment>
      <List className={classes.list}>
        <ListSubheader >
          Transactions History
          </ListSubheader >
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
                        {history.createAt}
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

