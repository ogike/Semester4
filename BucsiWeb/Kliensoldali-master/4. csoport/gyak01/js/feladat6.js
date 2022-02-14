var waypoint = new Waypoint({
    element: document.querySelector('footer span'),
    handler: function() {
      $('footer span')
      .html('0')
      .animateNumber({number: 2019})
    },
    offset: '100%'
})