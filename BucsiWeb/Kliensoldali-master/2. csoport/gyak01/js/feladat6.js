const waypoint = new Waypoint({
    element: document.querySelector("#incNumber"),
    handler: () => {
        // jQuery selector-ból az első HTML elemmel dolgozunk
        $("#incNumber").html('0').animateNumber({ number: 69420}, {duration: 1000})
    },
    offset: "100%" //ugyanaz mint IntersectionObserver threshold
})