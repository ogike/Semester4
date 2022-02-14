class Ingredients extends HTMLElement {
    constructor() {
        super();

        this.template = document.querySelector('#ingredient-template');
        this.attachShadow({mode: 'open'});
        this.textArea = document.querySelector('textarea');

        const style = document.createElement('style');
        style.innerHTML = '@import url("https://cdn.jsdelivr.net/gh/FortAwesome/Font-Awesome@5.10.2/css/all.min.css"); \
                           @import url("https://stackpath.bootstrapcdn.com/bootswatch/4.4.1/cosmo/bootstrap.min.css");';
        this.shadowRoot.appendChild(style);

        this.initData = this.getInitData();
        this.generateInputs();


        this.initButtons();

    }

    getInitData() {
        if(this.textArea.value) {
            return this.textArea.value.split('\n').map(item => {
                return {
                    amount: item.split(' ')[0],
                    ingredient: item.split(' ')[1],
                }
            });
        } else {
            return null;
        }
    }

    generateInputs() {
        const newItemButton = document.createElement('button');
        newItemButton.innerHTML = '<i class="fas fa-plus"></i>';
        newItemButton.classList.add('btn', 'btn-primary');
        newItemButton.addEventListener('click', () => {
            this.newRow();
        });
        this.shadowRoot.appendChild(newItemButton);

        if(this.initData) {
            this.initData.forEach(data => {
                this.newRow(data);
            });
        } else {
            this.newRow();
        }
    }

    newRow(data = null) {
        const temp = this.template.content.cloneNode(true);
        if(data) {
            temp.querySelector('.col:nth-child(1) > input').value = data.amount;
            temp.querySelector('.col:nth-child(2) > input').value = data.ingredient;
        }
        temp.querySelector('button').addEventListener('click', this.deleteRow);
        this.shadowRoot.appendChild(temp);
    }

    deleteRow(e) {
        //console.log(e.target.closest('.ingredients'));
        e.target.closest('.ingredients').remove();
    }

    initButtons() {
        const resetButton = document.querySelector('button[type=reset]');
        resetButton.addEventListener('click', e => {
            e.preventDefault();
            this.shadowRoot.querySelectorAll('input').forEach(input => {
                input.value = '';
                document.querySelector('input').value = '';
            });
        });


        const submitButton = document.querySelector('button[type=submit]');
        submitButton.addEventListener('click', e => {
            e.preventDefault();
            this.textArea.value = this.getIngredients();
            document.querySelector('form').submit();
        });
    }

    getIngredients() {
        const content = this.shadowRoot.querySelectorAll('.ingredients');
        let tmp = Array();
        content.forEach(row => {
            tmp.push(row.querySelector('.col:nth-child(1) > input').value + ' ' +
                     row.querySelector('.col:nth-child(2) > input').value);
        });
        return tmp.join('\n');
    }
}

customElements.define('ingredients-area', Ingredients);





// EXTRA

function randomInt() {
    return Math.floor(Math.random() * (255 - 0) + 0);
}

const intersectionObserver = new IntersectionObserver(onIntersect, {threshold: 1});
function onIntersect(records) {
    console.log(records);
    records.forEach(record => {
        if(record.isIntersecting) {
            record.target.style.color = `rgb(${randomInt()} ${randomInt()} ${randomInt()})`;
        }
    });
}
document.querySelectorAll('label').forEach(l => {
    intersectionObserver.observe(l);
});



const mutationObserver = new MutationObserver(onMutate);
function onMutate(records) {
    records.forEach(record => {console.log(record);});
}
mutationObserver.observe(document.querySelector('ingredients-area').shadowRoot, {
    subtree: true,
    childList: true,
    attributes: true
})