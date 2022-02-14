import { authApi } from "../../api/rest"
import { getToken, getUserId } from "./selectors";

export const STORE_USER = 'STORE_USER';
export const REMOVE_USER = 'REMOVE_USER';

// Sync
export const storeUser = authData => ({
    type: STORE_USER,
    payload: authData,
});

export const removeUser = () => ({
    type: REMOVE_USER,
});

// Async

export const login = (username, password) => async dispatch => {
    const response = await authApi.login(username, password);
    console.log(response);
    if(response.code) {
        throw new Error('Authentication failed!');
    }
    dispatch(storeUser(response));
}

export const logout = () => dispatch => {
    authApi.logout();
    dispatch(removeUser());
}

export const restoreUser = () => async dispatch => {
    const token = authApi.getToken();
    if(token) {
        const data = JSON.parse(atob(token.split('.')[1]));
        const userId = data.sub;
        const user = await authApi.getUserById(userId, token);
        dispatch(storeUser({
            user,
            accessToken: token,
        }));
    }
}


// Util
export const addToken = fn => (dispatch, getState, ...rest) => {
    const token = getToken(getState());
    fn(dispatch, getState, ...rest, token);
}

export const addUserId = fn => (dispatch, getState, ...rest) => {
    const userId = getUserId(getState());
    fn(dispatch, getState, ...rest, userId);
}
