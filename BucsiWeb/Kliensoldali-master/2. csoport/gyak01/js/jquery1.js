console.log($("li"))

//mostmár natív js megfelelő: (régen nem volt, ezárt kellett jQuery)
// document.querySelector[Akk]

//2.c feladat
// jquery-ben each van forearch helyett
$(".card-header").each(function () {
                 //adott elem elérése jQueryben
    const header = $(this); //fontos!: arrow functionnál nem lehetne thist használni
    //html alapján következő elem gyerekeit kell megszámolni
    const db = header.next().children().length;
    header.append(` (${db})`);
});

//alternatívan natív js-ben:
document.querySelectorAll(".card-header").forEach(header => {
    const db = header.nextElementSibling.children.length
    header.append(` = (${db})`)
})

//4.b FELADAT

//note: append utasítás után is az a három card headert adja vissza jQuery
//          így a most létrehozott buttonokra is be tudok lépni egyből append után
$(".card-header")
    .append("<button class = 'btn btn-primary'>Expand</button")
    .find("button")
    .on("click", function(e){
        //                                       toggle szépen megcseréli a kettőt
        $(this).closest(".card").parent().toggleClass("col-lg-4 col-lg-12")
    })