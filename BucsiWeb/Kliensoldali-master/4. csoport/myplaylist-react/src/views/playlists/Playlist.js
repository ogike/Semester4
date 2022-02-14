import { PlaylistForm } from './PlaylistForm';
import { PlaylistList } from './PlaylistList';
import { TrackDetails } from './TrackDetails';
import { Tracklist } from './Tracklist';

import { useParams } from 'react-router';
// import { useContext } from 'react';
// import { PlaylistsContext } from '../../state/PlaylistsProvider';
import { useSelector } from 'react-redux';
import { getPlaylists } from '../../state/playlists/selectors';
import { getPlaylistsWithTracks } from '../../state/selectors';

export function Playlist() {
    const { playlistId, trackId } = useParams();

    // const { playlists, addNewPlaylist } = useContext(PlaylistsContext);
    const playlists = useSelector(getPlaylists);

    const playlistsWithTracks = useSelector(getPlaylistsWithTracks);

    const selectedPlaylist = playlistsWithTracks.find((p) => p.id === playlistId);
    const selectedTrack = selectedPlaylist && selectedPlaylist.tracks.find((t) => t.id === trackId);
    return (
        <div className="ui container">
            <h1>My Playlists</h1>
            <div className="ui stackable two column grid">
                <div className="ui six wide column">
                    <h3>Playlists</h3>
                    <PlaylistForm />
                    <PlaylistList playlists={playlists} />
                </div>
                <div className="ui ten wide column">
                    <Tracklist playlist={selectedPlaylist} />
                </div>
            </div>
            <div className="ui divider"></div>
            <TrackDetails track={selectedTrack} />
        </div>
    );
}
