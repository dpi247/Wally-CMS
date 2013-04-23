/*
News ticker plugin (typewriter style)
Matt Hamilton,2011,Version 1.0 (25-04-2011)
http://www.junasoftware.com/jquery.aspx
Use and distribute freely with this header
 */
(function($){

    $.fn.newsticker = function(options){
    
        var opts = $.extend({}, $.fn.newsticker.defaults, options);
        
        return this.each(function(){
            $this = $(this);
            var o = $.meta ? $.extend({}, opts, $this.data()) : opts;
            
            var items = new Array()
            
            $this.children().each(function(index){
                items.push({
                    'text': $(this).text(),
                    'link': $(this).attr('href')
                });
            });
            var e = $("<div />");
            $this.replaceWith(e);
			e.items = items;
            e.addClass(o.className);
            e.append($("<a href='#' />").fadeOut(0).attr('target',o.target));
			showItem(e,o,0);
        });
    };
    
    function showItem(e,o,i){
		var item = e.children().filter(':first');
        var text = item.text();
        var j = text.length;
        if (j == 0) {
        	if(e.items[i].link!=''){
            	item.attr('href', e.items[i].link);
            }
            else{
            	item.attr('href', 'javascript:void(0)');
            }
            item.fadeIn(0);
        }
        if (j < e.items[i].text.length) {
            item.text(text + e.items[i].text[j]);
            setTimeout(function(){
                showItem(e,o,i)
            }, o.typeDelay);
        	j++;
        }
        else {
            i++;
            if (i == e.items.length) {
                i = 0;
            }
            setTimeout(function(){
                item.fadeOut(o.fadeOutSpeed, function(){
                    item.text('');
                    showItem(e,o,i);
                })
            }, o.fadeOutDelay);
        }
    };
    
    $.fn.newsticker.defaults = {
        typeDelay: 25,
        fadeOutDelay: 5000,
        fadeOutSpeed: 750,
        target: '_self',
        className: 'newsticker'
    };
})(jQuery);


