import { ADD_PLAYLIST, SET_PLAYLISTS, UPDATE_PLAYLIST } from './actions';

const initialState = [];

export const playlistsReducer = (state = initialState, action) => {
    const { type, payload } = action;

    if (type === SET_PLAYLISTS) {
        return payload;
    }

    if (type === ADD_PLAYLIST) {
        const playlists = state;
        const newPlaylist = payload;
        return [...playlists, newPlaylist];
    }

    if (type === UPDATE_PLAYLIST) {
        const playlists = state;
        const playlist = payload;
        return playlists.map(p => p.id !== playlist.id ? p : playlist);
    }

    return state;
};
