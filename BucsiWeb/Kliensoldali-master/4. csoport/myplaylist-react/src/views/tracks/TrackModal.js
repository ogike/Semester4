import { useEffect, useState } from 'react';
import { Modal } from 'semantic-ui-react';
import { Field } from './Field';

const defaultState = {
    artist: '',
    title: '',
    length: '',
    spotifyURL: '',
    chordsURL: '',
    lyricsURL: '',
};

export function TrackModal({ open, onClose, onSubmit, track }) {
    const [formState, setFormState] = useState(defaultState);

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormState({ ...formState, [name]: value });
    };

    const resetFormState = state => setFormState(state);
    useEffect(() => {
        if(open) {
            resetFormState({...defaultState, ...track});
        }
    }, [open, track]);

    const handleSubmit = (e) => {
        e.preventDefault();
        // console.log(formState);
        onSubmit(formState);
        onClose();
    };
    return (
        <Modal
            as="form"
            className="ui modal"
            open={open}
            onClose={onClose}
            onSubmit={handleSubmit}
        >
            <i className="close icon" onClick={onClose}></i>
            <div className="header">Add new Track</div>
            <div className="image content">
                <div className="description">
                    <div className="ui form">
                        <div className="three fields">
                            <Field
                                required
                                name="artist"
                                value={formState.artist}
                                onChange={handleChange}
                                label="Author"
                                placeholder="John Williams"
                            />
                            <Field
                                required
                                name="title"
                                value={formState.title}
                                onChange={handleChange}
                                label="Track name"
                                placeholder="Imperial March"
                            />
                            <Field
                                name="length"
                                value={formState.length}
                                onChange={handleChange}
                                label="Track length"
                                placeholder="3:14"
                            />
                        </div>
                        <div className="three fields">
                            <Field
                                name="spotifyURL"
                                value={formState.spotifyURL}
                                onChange={handleChange}
                                label="Spotify URL"
                                placeholder="https://"
                            />
                            <Field
                                name="chordsURL"
                                value={formState.chordsURL}
                                onChange={handleChange}
                                label="Lyrics URL"
                                placeholder="https://"
                            />
                            <Field
                                name="lyricsURL"
                                value={formState.lyricsURL}
                                onChange={handleChange}
                                label="Guitar tab URL"
                                placeholder="https://"
                            />
                        </div>
                    </div>
                </div>
            </div>
            <div className="actions">
                <button
                    type="button"
                    className="ui black deny button"
                    onClick={onClose}
                >
                    Cancel
                </button>
                <button
                    type="submit"
                    className="ui positive right labeled icon button"
                >
                    Add
                    <i className="plus icon"></i>
                </button>
            </div>
        </Modal>
    );
}
