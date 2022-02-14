import React, { useEffect, useState } from 'react';
import { playlistsStorage } from '../api/PlaylistsStorage';

const usePlaylistsService = () => {
    // Data
    const [playlists, setPlaylists] = useState([]);

    useEffect(() => {
        const getAll = async () => setPlaylists(await playlistsStorage.getAll());
        getAll();
    }, [])

    // Operations
    const addNewPlaylist = async (title) => {
        const newPlaylist = await playlistsStorage.create({
            title,
            tracks: [],
        });
        setPlaylists([...playlists, newPlaylist]);
    };

    // Service
    const playlistsService = { playlists, addNewPlaylist };

    return playlistsService;
};

export const PlaylistsContext = React.createContext();
export const PlaylistsProvider = ({ children }) => {
    const playlistsService = usePlaylistsService();
    return (
        <PlaylistsContext.Provider value={playlistsService}>
            {children}
        </PlaylistsContext.Provider>
    );
};
