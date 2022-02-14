const observer = new IntersectionObserver((entries) => {
    for(const entry of entries) {
        if(entry.isIntersecting) {
            entry.target.classList.add('animate__animated', 'animate__' + entry.target.dataset.scrollAnimation)
        }
    }
}, {threshold: 0.0});

const element = document.querySelectorAll('[data-scroll]').forEach(el => observer.observe(el));