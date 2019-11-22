const scrollTrigger = 47
var scrolling = false

function headerScroller () {
    window.addEventListener('scroll', function(){
    if(scrolling && window.scrollY>scrollTrigger) return;
    if(!scrolling && window.scrollY<scrollTrigger) return;
    if (window.scrollY >= scrollTrigger) {
        var element = document.getElementsByTagName("nav")
        element[0].classList.add("scrolled")
        element[0].classList.remove("transparent-background")
        scrolling=true
    } else {
        var element = document.getElementsByTagName("nav")
        element[0].classList.remove("scrolled")
        element[0].classList.add("transparent-background")
        scrolling=false
    }
})
}



