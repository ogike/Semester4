export function TrackDetails({ track }) {
    return (!track ? null :
        <div className="ui segment">
            <div className="ui items">
                <div className="item">
                    <div className="image">
                        <img src={track.thumbnailURL} alt="" />
                    </div>
                    <div className="content">
                        <div className="header">{track.title}</div>
                        <div className="meta">
                            <span>{track.artist}</span>
                            <span>{track.length}</span>
                        </div>
                        <div className="extra">
                            {track.spotifyURL && (
                                <div
                                    href={track.spotifyURL}
                                    className="ui button tiny green button"
                                    target="_blank"
                                >
                                    <i className="spotify icon"></i>
                                    Listen on Spotify
                                </div>
                            )}
                            {track.lyricsURL && (
                                <div
                                    href={track.lyricsURL}
                                    className="ui button tiny teal button"
                                    target="_blank"
                                >
                                    <i className="microphone icon"></i>
                                    Show lyrics
                                </div>
                            )}
                            {track.chordsURL && (
                                <div
                                    href={track.chordsURL}
                                    className="ui button tiny orange button"
                                    target="_blank"
                                >
                                    <i className="guitar icon"></i>
                                    Show chords
                                </div>
                            )}
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
}
