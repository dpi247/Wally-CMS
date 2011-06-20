(function(jQuery) {
	jQuery.fn.equalHeights = function(minheight) {
		var maxheight = minheight || 0;
		jQuery(this).children().each(function(){
			maxheight = (jQuery(this).height() > maxheight) ? jQuery(this).height() : maxheight;
		});
		jQuery(this).children().css('height', maxheight);
	}
})(jQuery);