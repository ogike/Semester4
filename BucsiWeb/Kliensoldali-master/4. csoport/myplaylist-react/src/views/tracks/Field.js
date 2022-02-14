export function Field({ label, placeholder, name, value, onChange, ...args }) {
    return (
        <div className="field">
            <label>{label}</label>
            <input name={name} value={value} onChange={onChange} {...args} type="text" placeholder={placeholder} />
        </div>
    );
}
