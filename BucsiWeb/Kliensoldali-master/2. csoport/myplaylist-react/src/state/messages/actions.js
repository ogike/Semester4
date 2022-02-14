import { socketApi } from '../../api/socket';
import { fetchTracks } from '../tracks/actions';

export const ADD_MESSAGE = 'ADD_MESSAGE';
export const DELETE_MESSAGE = 'DELETE_MESSAGE';

export const addMessage = (message) => ({
    type: ADD_MESSAGE,
    payload: message,
});

export const deleteMessage = (message) => ({
    type: DELETE_MESSAGE,
    payload: message,
});

export const wsConnect = () => (dispatch) => {
    socketApi.connect();

    socketApi.onReceiveMessage((message) => {
        dispatch(addMessage(message));
        setTimeout(() => {
            dispatch(deleteMessage(message));
        }, 5000);
        dispatch(fetchTracks());
    });
};

export const sendMessage = (text) => (dispatch) => {
    socketApi.sendMessage({
        id: Date.now().toString(),
        text,
    });
};
