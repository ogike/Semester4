export function Field({ name, label, placeholder, value, onChange, ...props }) {
    return (
        <div className="field">
            <label>{label}</label>
            <input name={name} value={value} onChange={onChange} {...props} type="text" placeholder={placeholder} />
        </div>
    );
}
