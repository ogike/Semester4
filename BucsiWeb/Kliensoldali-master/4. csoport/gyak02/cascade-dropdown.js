const select = document.querySelector('select[name=pets]');
const data = {};
select.hidden = true;
select.removeAttribute('name');

/*
{
    "4-legged pets": [
        {
            value: dog,
            text: Dog
        },
        {
            value: cat,
            text: Cat
        }
    ],
    "Flying pets": [
        {
            value: parrot,
            text: Parrot
        },
        {
            value: macaw,
            text: Macaw
        }
    ]
}
*/

select.querySelectorAll('optgroup').forEach(optg => {
    //console.log(optg);
    const key = optg.label;
    const value = Array.from(optg.querySelectorAll('option')).map(o => ({
        value: o.value,
        text: o.text
    }));
    data[key] = value;
});


const select1 = document.createElement('select');
Object.keys(data).forEach(label => {
    const option = document.createElement('option');
    option.text = label;
    select1.appendChild(option);
});
select.parentElement.appendChild(select1);


const select2 = document.createElement('select');
select2.setAttribute('name', 'pets')
select.parentElement.appendChild(select2);

select1.addEventListener('change', onChange);

function onChange(e) {
    const label = this.value;
    populate(label);
}

function populate(label) {
    const values = data[label];
    select2.innerHTML = '';
    values.forEach(val => {
        const option = document.createElement('option');
        option.text = val.text;
        option.value = val.value;
        select2.appendChild(option);
    });
}

// populate(Object.keys(data)[0]);
// onChange.call(select1);
select1.dispatchEvent(new Event('change'));

const button = document.querySelector('button');
button.addEventListener('click', function(e){
    console.log(e.clientX);
})

button.dispatchEvent(new Event('click'));