import React, { useState, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import { useSelector, useDispatch } from 'react-redux';
import PropTypes from 'prop-types';

import {
  Paper,
  Typography,
  Box,
  Divider,
  List,
  ListItem,
  Button,
  Dialog,
  DialogTitle,
  DialogContent,
  TextField,
  DialogActions,
} from '@material-ui/core';

import TabPanel, { a11yProps } from './TabPanel'

import AddCircleIcon from '@material-ui/icons/AddCircle';
import RemoveIcon from '@material-ui/icons/Remove';

import CreditCardIcon from '@material-ui/icons/CreditCard';
import {
  selectTab,
  selectUser,
  notify,
} from '../app/reducer'

import TransactionHistory from './TransactionHistory'
import DataSource from '../DataSource'
import { UserControl, BadgeAvatars, MoneyControl } from './Label'

export default function ConnectTab() {
  const dispatch = useDispatch()
  const tab = useSelector(selectTab);
  const user = useSelector(selectUser)
  const classes = useStyles();
  const [transactionHistory, setTransactionHistory] = useState([]);
  const [moneyInfo, setMoneyInfo] = useState({});
  const [transaction, setTransaction] = useState({ open: false })

  async function getMoneyInfo() {
    const data = await DataSource.getMoneyInfo(user.sessionId);
    setMoneyInfo(data.data);
  }
  async function getTransactionHistory() {
    const data = await DataSource.history(user.sessionId);
    setTransactionHistory(data.data || []);
  };
  useEffect(() => {
    getMoneyInfo();
  }, [user]);


  useEffect(() => {
    getTransactionHistory();
  }, [user]);

  const handleCreateAccount = async () => {
    try {
      const res = await DataSource.createMoneyAccount(user.sessionId)
      if (!res.error) {
        dispatch(notify({ open: true, severity: 'success', message: 'Done' }))
        getMoneyInfo()
        getTransactionHistory()
      } else {
        dispatch(notify({ open: true, severity: 'error', message: res.message }))
      }
    } catch (err) {
      dispatch(notify({ open: true, severity: 'error', message: err.message }))
    }
  }

  const handleTransaction = async () => {
    let res
    switch (transaction.type) {
      case 'deposit':
        res = await DataSource.deposit(user.sessionId, transaction.amount);
        break;
      case 'withdraw':
        res = await DataSource.withdraw(user.sessionId, transaction.amount);
        break;
      case 'transfer':
        res = await DataSource.transfer(user.sessionId, transaction.toUser, transaction.amount, transaction.message);
        break;
      default:
        console.log("can't go here")
        return;
    }
    if (res == null || res.error) {
      const message = res ? res.message : 'unknow';
      dispatch(notify({ open: true, severity: 'error', message: message }))
    } else {
      getMoneyInfo();
      getTransactionHistory();
      dispatch(notify({ open: true, severity: 'success', message: 'Done' }))
    }
    handleCloseTransaction();
  }

  const handleCloseTransaction = () => {
    setTransaction({ open: false })
  }

  return (
    <TabPanel value={tab.index} index={2} className={classes.root}>
      <Box p={1}>
        <Paper>
          {moneyInfo &&
            <MoneyControl alt='username' src='user.ico' {...user} {...moneyInfo} />
          }
          <Divider />
          <Box p={1} >
            <List className={classes.clist}>
              {moneyInfo &&
                <>
                  <ListItem>

                    <Button
                      variant="contained"
                      color="primary"
                      className={classes.button}
                      startIcon={<AddCircleIcon />}
                      onClick={() => setTransaction({ open: true, type: 'deposit' })}
                    >Deposit</Button>
                  </ListItem>

                  <ListItem>
                    <Button
                      variant="contained"
                      color="secondary"
                      className={classes.button}
                      startIcon={<RemoveIcon />}
                      onClick={() => setTransaction({ open: true, type: 'withdraw' })}
                    >Withdraw</Button>
                  </ListItem>
                </>
              }

              {!moneyInfo &&
                <Button
                  variant="contained"
                  color="primary"
                  startIcon={<CreditCardIcon />}
                  onClick={handleCreateAccount}
                >Create Account</Button>
              }

            </List>

          </Box>

        </Paper>
      </Box>
      {moneyInfo &&
        <TransactionHistory dataSource={transactionHistory} />
      }
      <Dialog
        open={transaction.open}
        onClose={handleCloseTransaction}
        aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">
          {`Transaction ${transaction.type}`}
        </DialogTitle>
        <DialogContent>
          {transaction.type == 'transfer' &&
            <React.Fragment>
              <TextField
                onChange={(evt) => setTransaction(
                  tran => ({ ...tran, message: evt.target.value }))}
                autoFocus
                margin="dense"
                id="message"
                label="Messagge:"
                fullWidth
              />
            </React.Fragment>
          }

          <TextField
            onChange={(evt) => setTransaction(
              tran => ({ ...tran, amount: evt.target.value }))}
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


