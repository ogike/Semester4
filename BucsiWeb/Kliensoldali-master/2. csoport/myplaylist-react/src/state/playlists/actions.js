import { playlistsApi } from '../../api/rest';
import { getPlaylists } from './selectors';
import { getToken } from '../auth/selectors'
import { addToken, addUserId } from '../auth/actions';

export const SET_PLAYLISTS = 'SET_PLAYLISTS';
export const ADD_PLAYLIST = 'ADD_PLAYLIST';
export const UPDATE_PLAYLIST = 'UPDATE_PLAYLIST';

// Sync
export const setPlaylists = (playlists) => ({
    type: SET_PLAYLISTS,
    payload: playlists,
});

export const addPlaylistToStore = (track) => ({
    type: ADD_PLAYLIST,
    payload: track,
});

export const updatePlaylist = (updatedPlaylist) => ({
    type: UPDATE_PLAYLIST,
    payload: updatedPlaylist,
});

// Async
export const fetchPlaylists = () => addToken(addUserId(async (dispatch, getState, _, token, userId) => {
    // const token = getToken(getState());
    const playlists = await playlistsApi.getAll(token, userId);
    dispatch(setPlaylists(playlists));
}));

export const addPlaylist = (title) => addToken(async (dispatch, getState, _, token) => {
    const newPlaylist = await playlistsApi.create({ title, tracks: [] }, token);
    dispatch(addPlaylistToStore(newPlaylist));
});

export const addTrackToPlaylist = (playlistId, trackId) => addToken(async (dispatch, getState, _, token) => {
    const playlists = getPlaylists(getState());
    const playlist = playlists.find((p) => p.id === playlistId);
    if (!playlist) return;
    if (playlist.tracks.includes(trackId)) return;
    const modifiedPlaylist = {
        ...playlist,
        tracks: playlist.tracks.concat(trackId),
    };
    const updatedPlaylist = await playlistsApi.update(modifiedPlaylist, token);
    dispatch(updatePlaylist(updatedPlaylist));
});

export const deleteTrackFromAllPlaylists = track => addToken(async (dispatch, getState, _, token) => {
    const playlists = getPlaylists(getState());
    for (const playlist of playlists) {
        if(playlist.tracks.includes(track.id)) {
            const modifiedPlaylist = {
                ...playlist,
                tracks: playlist.tracks.filter(id => id !== track.id),
            };
            const updatedPlaylist = await playlistsApi.update(modifiedPlaylist, token);
            dispatch(updatePlaylist(updatedPlaylist));
        }
    }
});
