import { useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { Dropdown } from 'semantic-ui-react';
import { addTrackToPlaylist } from '../../state/playlists/actions';
import { getPlaylists } from '../../state/playlists/selectors';
// import { PlaylistsContext } from '../../state/PlaylistsProvider';

export function Track({ track, onEdit, onDelete }) {
    // const { playlists } = useContext(PlaylistsContext);
    const playlists = useSelector(getPlaylists);
    const [open, setOpen] = useState(false);

    const handleOpen = () => setOpen(true);
    const handleClose = () => setOpen(false);

    const dispatch = useDispatch();
    const handleAddTrackToPlaylist = (playlistId, trackId) => {
        dispatch(addTrackToPlaylist(playlistId, trackId));
    };
    return (
        <tr>
            <td>
                <i className="user icon"></i> {track.artist}
            </td>
            <td>
                <i className="music icon"></i> {track.title}
            </td>
            <td className="right aligned collapsing">
                <Dropdown
                    onOpen={handleOpen}
                    onClose={handleClose}
                    open={open}
                    icon="plus"
                    className="ui icon top right pointing dropdown button"
                >
                    <Dropdown.Menu className="menu">
                        <div className="header">Add to playlist</div>
                        <div className="ui search icon input">
                            <i className="search icon"></i>
                            <input
                                type="text"
                                name="search"
                                placeholder="Search playlists..."
                            />
                        </div>
                        {playlists.map((playlist) => (
                            <div
                                onClick={() =>
                                    handleAddTrackToPlaylist(
                                        playlist.id,
                                        track.id
                                    )
                                }
                                key={playlist.id}
                                className="item"
                            >
                                {playlist.title}
                            </div>
                        ))}
                    </Dropdown.Menu>
                </Dropdown>
                <button onClick={onEdit} className="ui icon button">
                    <i className="edit icon"></i>
                </button>
                <button onClick={onDelete} className="ui icon button red">
                    <i className="trash icon"></i>
                </button>
            </td>
        </tr>
    );
}
