import { useState } from "react";
import { useDispatch } from "react-redux";
import { addPlaylist } from "../../state/playlists/actions";

export function PlaylistForm() {
    // Referenciával
    // const inputRef = useRef(null);

    // State-tel
    const [value, setValue] = useState('');
    const dispatch = useDispatch();

    const handleSubmit = e => {
        e.preventDefault();
        setValue('');
        // console.log(inputRef.current.value); // referencia

        dispatch(addPlaylist(value));
    }
    return (
        <details open>
            <summary>Add new playlist</summary>
            <form className="ui action input container" onSubmit={handleSubmit}>
                {/* <input defaultValue="" ref={inputRef} /> */}
                <input value={value} onChange={e => setValue(e.target.value)} />
                <button className="ui primary button">Add</button>
            </form>
        </details>
    );
}
