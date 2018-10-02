var slider;
function slider_hide(czas) {
    slider.slideUp(czas);
    sessionStorage.hiddenSlider = true;
}

$(document).ready(function () {
    slider = $("#slider");
    if (sessionStorage.hiddenSlider == 'true') {
        slider.slideUp(1);
    } else {
        slider.slideDown(1500);
    }
    var logo = $("#slider_switch");
    logo.click(function(){
        slider_hide(1500);
    })
	
	
})

