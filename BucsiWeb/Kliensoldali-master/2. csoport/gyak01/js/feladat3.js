//interception observer
//TODO: nem működik

const intersectionCallback = entities => {
    // console.log(elems);
    for (const entity of entities){
        if(entity.isIntersecting){
            //maga a target a HTML elem amit intersectelünk
            entity.target.classList.add('animete__animeted', `animete__${ entity.target.dataset.scrollAnimation}`);
        }
    }
}

const observerOptions = {
    threshold : 0.0
}

//minden elem ami épp intersecttel a viewportommal
const obs = new IntersectionObserver(intersectionCallback, observerOptions);

//azok az elemek, amiken van a data-scrolll paraméter
document.querySelectorAll("[data-scroll]").forEach( elem => {
    obs.observe(elem);
})

