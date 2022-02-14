
// Vanilla js
// addEventListener("scroll", ()=> {
//     //toggle force paramétere: csak le, vagy csak feltenni tudja-e
//     document.querySelector("nav").classList.toggle("navbar-scrolled", window.scrollY > 200);
//     console.count("Fgv meghívva");
// })

const toggleNavBarFn = () => {
    document.querySelector("nav").classList.toggle("navbar-scrolled", window.scrollY > 200);
    // console.count("Fgv meghívva");
}
addEventListener("scroll", _.throttle(toggleNavBarFn, 30));

// szokásosan:
// addEventListener("scroll", _.throttle());
// DE: _.throttle egy fgv-t ad vissza visszatérési értéknek, így jó

