import { ADD_MESSAGE, DELETE_MESSAGE } from "./actions";

const initialState = [
    // {id: '1', text: 'alma1'},
    // {id: '2', text: 'alma2'}
];

export const messageReducer = (state = initialState, action) => {
    const { type, payload } = action;

    if(type === ADD_MESSAGE) {
        return [...state, payload];
    }

    if(type === DELETE_MESSAGE) {
        return state.filter(message => message.id !== payload.id);
    }

    return state;
};
