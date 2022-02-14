import { useState } from "react";
import cn from 'classnames';
import { useDispatch } from "react-redux";
import { addPlaylist } from "../../state/playlists/actions";

export function PlaylistForm() {
    // Referenciával
    // const inputRef = useRef(null);

    const [value, setValue] = useState('');
    const [error, setError] = useState(false);
    const dispatch = useDispatch();


    const handleSubmit = e => {
        e.preventDefault();

        if(value.trim().length === 0) {
            setError(true);
            return;
        }
        setError(false);

        dispatch(addPlaylist(value));
        setValue('');
    }

    return (
        <details open>
            <summary>Add new playlist</summary>
            <form className={cn("ui action input container", {error})} onSubmit={handleSubmit}>
                {/* <input defaultValue="" ref={inputRef} /> */}
                <input value={value} onChange={e => {setValue(e.target.value)}} />
                <button className={cn("ui primary button", {negative: error})}>Add</button>
            </form>
        </details>
    );
}
