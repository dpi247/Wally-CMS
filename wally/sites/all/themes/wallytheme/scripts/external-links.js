// javascript non obtrusif

jQuery(document).ready(function(){
    $("a").each(function(){
        var $a = jQuery(this);
        var href = this.href;
        
        // strip the host name down, removing subdomains or www
        var host = window.location.host.replace(/^(([^\/]+?\.)*)([^\.]{4,})((\.[a-z]{1,4})*)$/, '$3$4');

        //if (this.href != null) {
        //}
        
        if (this.href != null && !$a.is(".exempt")) {
            if ( (this.href.match(/^http/)) && (! this.href.match(host)) && (! this.href.match(/^javascript/)) ) {
                $a.addClass('thickbox').addClass('external');
                $a.attr('href', '#TB_inline?height=220&width=370&inlineId=tb_external');
                tb_init('a.thickbox');
                
                // on click, add external link code to the thickbox
                $a.click(function () {
                  jQuery('#tb_external_thelink')
                   .before('<p id="tb_external_thelink"><a href="' + href + '">' + href + '</a></p>')
                   .remove();
                });
            }
        }
    });
});