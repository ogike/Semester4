import { NavLink } from 'react-router-dom';

export function Tracklist({ playlist }) {
    return (!playlist ? null :
        <>
            <h3>{playlist.title}</h3>
            <div className="ui very relaxed selection list">
                {playlist.tracks.map((track) => (
                    <NavLink
                        className="item"
                        key={track.id}
                        to={`/playlists/${playlist.id}/${track.id}`}
                    >
                        <i className="large music middle aligned icon"></i>
                        <div className="content">
                            <div className="header">{track.title}</div>
                            <div className="description">{track.artist}</div>
                        </div>
                    </NavLink>
                ))}
            </div>
        </>
    );
}
