import { configureStore } from '@reduxjs/toolkit';
import appReducer from './reducer'

export default configureStore({
  reducer: {
    //counter: counterReducer,
    app: appReducer,
  },
});
