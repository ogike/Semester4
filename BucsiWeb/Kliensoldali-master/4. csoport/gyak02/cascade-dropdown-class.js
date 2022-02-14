class CascadeSelect extends HTMLElement {
    constructor() {
        super(); // A leszármaztatott HTMLElement class konstruktorának meghívása

        // A template alapján készítünk egy instance-ot és ezt a shadow dom-hoz csatoljuk
        const template = document.querySelector('#cascade-select-template');
        const content = template.content.cloneNode(true);
        const shadowRoot = this.attachShadow({mode: 'open'});
        shadowRoot.appendChild(content);

        // Adattagok felvétele
        this.select = document.querySelector('select[name=pets]');
        this.data = this.collectData();
        
        this.select1 = this.shadowRoot.querySelector('select:nth-child(2)');
        this.select2 = this.shadowRoot.querySelector('select:nth-child(3)');
        this.initSelect1();

        this.onChange = this.onChange.bind(this); // Az onChange függvényben a "this" kontextusának beállítása
        this.select1.addEventListener('change', this.onChange);

        this.select2.addEventListener('change', e => {
            this.select.value = e.target.value;
        });

        this.select1.dispatchEvent(new Event('change'));
    }

    collectData() {
        const data = {};
        this.select.querySelectorAll('optgroup').forEach(optg => {
            const key = optg.label;
            const value = [...optg.querySelectorAll('option')].map(o => ({
                value: o.value,
                text: o.text
            }));
            data[key] = value;
        });
        return data;
    }

    initSelect1() {
        Object.keys(this.data).forEach(label => {
            const option = document.createElement('option');
            option.text = label;
            this.select1.appendChild(option);
        });
    }

    onChange(e) {
        const label = e.target.value;
        this.populate(label);
        this.select2.dispatchEvent(new Event('change'));
    }
    
    populate(label) {
        const values = this.data[label];
        this.select2.innerHTML = '';
        values.forEach(val => {
            const option = document.createElement('option');
            option.text = val.text;
            option.value = val.value;
            this.select2.appendChild(option);
        });
    }
}

// A cascade-select tag és a CascadeSelect class "összekötése"
customElements.define('cascade-select', CascadeSelect);
