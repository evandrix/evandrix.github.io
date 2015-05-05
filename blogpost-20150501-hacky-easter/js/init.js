/*
	Dopetrope by HTML5 UP
	html5up.net | @n33co
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
*/

(function($) {

	skel.init({
		reset: 'full',
		ignoreOrientation: true,
		breakpoints: {
			'global':	{ range: '*', href: 'css/style.css' },
			'desktop':	{ range: '581-', href: 'css/style-desktop.css', containers: 1200, grid: { gutters: 50 } },
			'1000px':	{ range: '581-1200', href: 'css/style-1000px.css', containers: 960, grid: { gutters: 25 }, viewportWidth: 1080 },
			'mobile':	{ range: '-580', href: 'css/style-mobile.css', containers: '100%', grid: { collapse: true, gutters: 10 }, lockViewport: true }
		}
	}, {
	});

	$(function() {

		var	$window = $(window),
			$body = $('body');
			
		// Disable animations/transitions until the page has loaded.
			$body.addClass('is-loading');
			
			$window.on('load', function() {
				$body.removeClass('is-loading');
			});
			
		// Dropdowns.
			$('#nav > ul').dropotron({ 
				mode: 'fade',
				noOpenerFade: true,
				alignment: 'center'
			});
			
	});

})(jQuery);