// console.log($('li'));
// console.log(document.querySelectorAll('li'));
// 
// 
// 
// console.log($('.row'));
// console.log(document.querySelectorAll('.row'));
// 
// 
// 
// console.log($('.list-group > .list-group-item:first-child'));
// console.log(document.querySelectorAll('.list-group > .list-group-item:first-child'));1


$('.card-header').each(function() {
    console.log(this);
    const $header = $(this);
    const db = $header.next().children().length;
    $header.append(` (${db})`);
});

document.querySelectorAll('.card-header').forEach(header => {
    const db = header.nextElementSibling.children.length;
    header.innerHTML += ` (${db})`;
});



$('.card-header')
    .append($('<button class="btn btn-primary">Extend</button>'))
    .find('button')
        .on('click', function (e) {
            $(this).closest('.card').parent()
                .toggleClass('col-lg-4 col-lg-12')
        })