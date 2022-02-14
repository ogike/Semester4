// window.addEventListener('scroll', e => {
//     document.querySelector('nav').classList.toggle('navbar-scrolled', window.scrollY > 200);
//     console.log('asdf');
// });


// let waiting = false;
// window.addEventListener('scroll', e => {
//     document.querySelector('nav').classList.toggle('navbar-scrolled', window.scrollY > 200);
//     console.log('asdf');
//     window.requestAnimationFrame(() => {
//         waiting = false;
//     });
//     waiting = true;
// })


window.addEventListener('scroll', _.throttle(() => {
    document.querySelector('nav').classList.toggle('navbar-scrolled', window.scrollY > 200);
    console.log('asdf');
}), 30);