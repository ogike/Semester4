import { tracksApi } from "../../api/rest";
import { sendMessage } from "../messages/actions";
import { deleteTrackFromAllPlaylists } from "../playlists/actions";
import { getToken } from '../auth/selectors'
import { addToken } from "../auth/actions";

export const SET_TRACKS = 'SET_TRACKS';
export const ADD_TRACK = 'ADD_TRACK';
export const UPDATE_TRACK = 'UPDATE_TRACK';
export const DELETE_TRACK = 'DELETE_TRACK';

// Sync
export const setTracks = (tracks) => ({
    type: SET_TRACKS,
    payload: tracks,
});

export const addTrackToStore = (track) => ({
    type: ADD_TRACK,
    payload: {...track, id: Date.now().toString()},
});

export const updateTrackToStore = (track) => ({
    type: UPDATE_TRACK,
    payload: track,
});

export const deleteTrackFromStore = (track) => ({
    type: DELETE_TRACK,
    payload: track,
});

// Async
export const fetchTracks = () => addToken(async (dispatch, getState, _, token) => {
    // const token = getToken(getState());
    const tracks = await tracksApi.getAll(token)
    dispatch(setTracks(tracks));
});

export const addTrack = (track) => addToken(async (dispatch, getState, _, token) => {
    const newTrack = await tracksApi.create(track, token);
    dispatch(sendMessage(`Track created: ${track.artist} | ${track.title}`));
    dispatch(addTrackToStore(newTrack));
});

export const updateTrack = (track) => addToken(async (dispatch, getState, _, token) => {
    const updatedTrack = await tracksApi.update(track, token);
    dispatch(sendMessage(`Track updated: ${track.artist} | ${track.title}`));
    dispatch(updateTrackToStore(updatedTrack));
});

export const deleteTrack = (track) => addToken(async (dispatch, getState, _, token) => {
    await tracksApi.delete(track.id, token);
    dispatch(sendMessage(`Track deleted: ${track.artist} | ${track.title}`));
    dispatch(deleteTrackFromStore(track));
    dispatch(deleteTrackFromAllPlaylists(track));
});
