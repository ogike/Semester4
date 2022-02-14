import { Track } from "./Track";
import { TrackModal } from "./TrackModal";

import { useState } from "react";
import { useDispatch, useSelector } from "react-redux";
import { getTracks } from "../../state/tracks/selectors";
import { addTrack, deleteTrack, updateTrack } from "../../state/tracks/actions";

export function Tracks() {
    const tracks = useSelector(getTracks);
    const dispatch = useDispatch();
    const [open, setOpen] = useState(false);
    const [editedTrack, setEditedTrack] = useState({});

    const handleOpen = () => setOpen(true);
    const handleClose = () => setOpen(false);

    const handleSubmit = (track) => {
        if(!track.id) {
            // új track
            dispatch(addTrack(track));
        } else {
            // track módosítás
            dispatch(updateTrack(track));
        }
    }

    const handleNew = () => {
        setEditedTrack({});
        handleOpen();
    }

    const handleEdit = track => {
        setEditedTrack(track);
        handleOpen();
    }

    const handleDelete = track => {
        dispatch(deleteTrack(track));
    }

    return (
        <>
            <div className="ui container">
                <button
                    href="#"
                    className="ui right floated green button"
                    id="newModal"
                    onClick={handleNew}
                >
                    <i className="plus icon"></i>
                    New track
                </button>
                <h1>Tracks</h1>
                <table className="ui celled striped table">
                    <thead>
                        <tr>
                            <th>Artist</th>
                            <th>Title</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody>
                        {tracks.map(track => (
                            <Track key={track.id} track={track} onEdit={() => handleEdit(track)} onDelete={() => handleDelete(track)} />
                        ))}
                    </tbody>
                </table>
            </div>

            <TrackModal open={open} onClose={handleClose} onSubmit={handleSubmit} track={editedTrack} />
        </>
    );
}
