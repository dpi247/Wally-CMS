<style>
#column-main-left{
width:60%;
float:left;
}

#column-side-right{
width:30%;
float:right;

}
#wallyedit_preview_container{
clear:both;
}
#profile_selector{
clear:both;
margin-bottom:30px;
}
#scroller-header a {
    text-decoration:none;
    color:#867863;
    padding:0 2px;
}
 
#scroller-header a:hover {
    text-decoration:none;
    color:#4b412f
}
 
a.selected {
    text-decoration:underline !important;
    color:#4b412f !important;
}
 
#scroller-header {
    background:url(images/header.gif) no-repeat;
    /*width:277px;*/
    height:24px;
    padding:35px 0 0 15px;
    font-weight:700;
}
#scroller-header{
  height:36px;
  padding:0px;
  line-height:20px;
}
#scroller-header a{
    background: url("../images/bleeds.png") repeat-x scroll 0 -41px #F4F4F4;
    border-bottom: 0 none #FFFFFF;
    border-color: #FFFFFF;
    border-radius: 3px 3px 0 0;
    color: #333333;
    padding: 4px 14px 11px;
}
 
#scroller-body {
    background:url(images/body.gif) no-repeat bottom center;
   /* width:277px; */
    padding-bottom:30px;
 
}
 
#mask {
   /* width:250px; */
    overflow:hidden;
    margin:0 auto;
}
 
#onglet {
 
}
 
#onglet div {
float:left;
 
}

#onglet input.fluid {
    -moz-box-sizing: border-box;
    width: 95%;
}

 
/* extra optional styling for each tab content */
#onglet-1 {
}
 
#onglet-2 {
}
 
#onglet-3 {
}



body div.column-main {
    float:left;
    
    
}
body div.column-side {
    float:left;
    
    }
    
    
    
</style>



<script>
/**
* jQuery.ScrollTo
* Copyright (c) 2007-2009 Ariel Flesler - aflesler(at)gmail(dot)com | http://flesler.blogspot.com
* Dual licensed under MIT and GPL.
* Date: 5/25/2009
*
* @projectDescription Easy element scrolling using jQuery.
* http://flesler.blogspot.com/2007/10/jqueryscrollto.html
* Works with jQuery +1.2.6. Tested on FF 2/3, IE 6/7/8, Opera 9.5/6, Safari 3, Chrome 1 on WinXP.
*
* @author Ariel Flesler
* @version 1.4.2
*
* @id jQuery.scrollTo
* @id jQuery.fn.scrollTo
* @param {String, Number, DOMElement, jQuery, Object} target Where to scroll the matched elements.
* The different options for target are:
* - A number position (will be applied to all axes).
* - A string position ('44', '100px', '+=90', etc ) will be applied to all axes
* - A jQuery/DOM element ( logically, child of the element to scroll )
* - A string selector, that will be relative to the element to scroll ( 'li:eq(2)', etc )
* - A hash { top:x, left:y }, x and y can be any kind of number/string like above.
* - A percentage of the container's dimension/s, for example: 50% to go to the middle.
* - The string 'max' for go-to-end.
* @param {Number} duration The OVERALL length of the animation, this argument can be the settings object instead.
* @param {Object,Function} settings Optional set of settings or the onAfter callback.
* @option {String} axis Which axis must be scrolled, use 'x', 'y', 'xy' or 'yx'.
* @option {Number} duration The OVERALL length of the animation.
* @option {String} easing The easing method for the animation.
* @option {Boolean} margin If true, the margin of the target element will be deducted from the final position.
* @option {Object, Number} offset Add/deduct from the end position. One number for both axes or { top:x, left:y }.
* @option {Object, Number} over Add/deduct the height/width multiplied by 'over', can be { top:x, left:y } when using both axes.
* @option {Boolean} queue If true, and both axis are given, the 2nd axis will only be animated after the first one ends.
* @option {Function} onAfter Function to be called after the scrolling ends.
* @option {Function} onAfterFirst If queuing is activated, this function will be called after the first scrolling ends.
* @return {jQuery} Returns the same jQuery object, for chaining.
*
* @desc Scroll to a fixed position
* @example $('div').scrollTo( 340 );
*
* @desc Scroll relatively to the actual position
* @example $('div').scrollTo( '+=340px', { axis:'y' } );
*
* @dec Scroll using a selector (relative to the scrolled element)
* @example $('div').scrollTo( 'p.paragraph:eq(2)', 500, { easing:'swing', queue:true, axis:'xy' } );
*
* @ Scroll to a DOM element (same for jQuery object)
* @example var second_child = document.getElementById('container').firstChild.nextSibling;
* $('#container').scrollTo( second_child, { duration:500, axis:'x', onAfter:function(){
* alert('scrolled!!');
* }});
*
* @desc Scroll on both axes, to different values
* @example $('div').scrollTo( { top: 300, left:'+=200' }, { axis:'xy', offset:-20 } );
*/
;(function( $ ){
var $scrollTo = $.scrollTo = function( target, duration, settings ){
$(window).scrollTo( target, duration, settings );
};
$scrollTo.defaults = {
axis:'xy',
duration: parseFloat($.fn.jquery) >= 1.3 ? 0 : 1
};
// Returns the element that needs to be animated to scroll the window.
// Kept for backwards compatibility (specially for localScroll & serialScroll)
$scrollTo.window = function( scope ){
return $(window)._scrollable();
};
// Hack, hack, hack :)
// Returns the real elements to scroll (supports window/iframes, documents and regular nodes)
$.fn._scrollable = function(){
return this.map(function(){
var elem = this,
isWin = !elem.nodeName || $.inArray( elem.nodeName.toLowerCase(), ['iframe','#document','html','body'] ) != -1;
if( !isWin )
return elem;
var doc = (elem.contentWindow || elem).document || elem.ownerDocument || elem;
return $.browser.safari || doc.compatMode == 'BackCompat' ?
doc.body :
doc.documentElement;
});
};
$.fn.scrollTo = function( target, duration, settings ){
if( typeof duration == 'object' ){
settings = duration;
duration = 0;
}
if( typeof settings == 'function' )
settings = { onAfter:settings };
if( target == 'max' )
target = 9e9;
settings = $.extend( {}, $scrollTo.defaults, settings );
// Speed is still recognized for backwards compatibility
duration = duration || settings.speed || settings.duration;
// Make sure the settings are given right
settings.queue = settings.queue && settings.axis.length > 1;
if( settings.queue )
// Let's keep the overall duration
duration /= 2;
settings.offset = both( settings.offset );
settings.over = both( settings.over );
return this._scrollable().each(function(){
var elem = this,
$elem = $(elem),
targ = target, toff, attr = {},
win = $elem.is('html,body');
switch( typeof targ ){
// A number will pass the regex
case 'number':
case 'string':
if( /^([+-]=)?\d+(\.\d+)?(px|%)?$/.test(targ) ){
targ = both( targ );
// We are done
break;
}
// Relative selector, no break!
targ = $(targ,this);
case 'object':
// DOMElement / jQuery
if( targ.is || targ.style )
// Get the real position of the target
toff = (targ = $(targ)).offset();
}
$.each( settings.axis.split(''), function( i, axis ){
var Pos = axis == 'x' ? 'Left' : 'Top',
pos = Pos.toLowerCase(),
key = 'scroll' + Pos,
old = elem[key],
max = $scrollTo.max(elem, axis);
if( toff ){// jQuery / DOMElement
attr[key] = toff[pos] + ( win ? 0 : old - $elem.offset()[pos] );
// If it's a dom element, reduce the margin
if( settings.margin ){
attr[key] -= parseInt(targ.css('margin'+Pos)) || 0;
attr[key] -= parseInt(targ.css('border'+Pos+'Width')) || 0;
}
attr[key] += settings.offset[pos] || 0;
if( settings.over[pos] )
// Scroll to a fraction of its width/height
attr[key] += targ[axis=='x'?'width':'height']() * settings.over[pos];
}else{
var val = targ[pos];
// Handle percentage values
attr[key] = val.slice && val.slice(-1) == '%' ?
parseFloat(val) / 100 * max
: val;
}
// Number or 'number'
if( /^\d+$/.test(attr[key]) )
// Check the limits
attr[key] = attr[key] <= 0 ? 0 : Math.min( attr[key], max );
// Queueing axes
if( !i && settings.queue ){
// Don't waste time animating, if there's no need.
if( old != attr[key] )
// Intermediate animation
animate( settings.onAfterFirst );
// Don't animate this axis again in the next iteration.
delete attr[key];
}
});
animate( settings.onAfter );
function animate( callback ){
$elem.animate( attr, duration, settings.easing, callback && function(){
callback.call(this, target, settings);
});
};
}).end();
};
// Max scrolling position, works on quirks mode
// It only fails (not too badly) on IE, quirks mode.
$scrollTo.max = function( elem, axis ){
var Dim = axis == 'x' ? 'Width' : 'Height',
scroll = 'scroll'+Dim;
if( !$(elem).is('html,body') )
return elem[scroll] - $(elem)[Dim.toLowerCase()]();
var size = 'client' + Dim,
html = elem.ownerDocument.documentElement,
body = elem.ownerDocument.body;
return Math.max( html[scroll], body[scroll] )
- Math.min( html[size] , body[size] );
};
function both( val ){
return typeof val == 'object' ? val : { top:val, left:val };
};
})( jQuery );

<?php
  module_load_include("inc",'wallyedit','includes/page_form_display_tabs');
  
  //TODO: Adapt to get used profile
  $tabs=wyditadmin_get_fields_tree(wallyedit_get_default_profile(), $form["type"]["#value"]);
  $onglets_struct=$tabs;
  $type=wydit_get_infos_type($form["type"]["#value"]);
  $cck_fields = $type['fields'];
?>
$(document).ready(function() { 
 
    //Get the height of the first item
   // $('#mask').css({'height':$('#onglet-<?php print current(array_keys($onglets_struct));?>').height()}); 
     
    //Calculate the total width - sum of all sub-onglets width
    //Width is generated according to the width of #mask * total of sub-onglets
    $('#onglet').width(parseInt($('#mask').width() * $('#onglet div').length));
     
    //Set the sub-onglet width according to the #mask width (width of #mask and sub-onglet must be same)
    $('#onglet div').width($('#mask').width());
     
    //Get all the links with rel as onglet
    $('a[rel=onglet]').click(function () {
     
        //Get the height of the sub-onglet
        var ongletheight = $($(this).attr('href')).height();
         
        //Set class for the selected item
        $('a[rel=onglet]').removeClass('selected');
        $(this).addClass('selected');
         
        //Resize the height
        $('#mask').animate({'height':ongletheight},{queue:false, duration:500});        
         
        //Scroll to the correct onglet, the onglet id is grabbed from the href attribute of the anchor
        $('#mask').scrollTo($(this).attr('href'), 800);    
         
        //Discard the link default behavior
        return false;
    });
     
});

<?php 
$meta_tab_name="meta_".$profile_id.'_'.$node_type;
$no_tab_name="no_tab";
?>
</script>
<div id="profile_selector">
<?php print drupal_render($form['choose_profile']); ?>
<?php print drupal_render($form['confirm_profile']); ?>
</div>
<div id="column-main-left">
  <div id="scroller-header">
      <?php foreach($onglets_struct as $onglet=>$onglet_content):?>
        <?php if($onglet!=$meta_tab_name and $onglet!=$no_tab_name):?>
          <a href="#onglet-<?php print $onglet?>" rel="onglet" class="selected"><?php print $onglet_content['label']?></a>
        <?php endif;?>
      <?php endforeach;?>
  </div>
  <div id="scroller-body">
      <div id="mask">
          <div id="onglet">
          <?php  foreach($onglets_struct as $onglet=>$onglet_content):?>
            <?php if($onglet!=$meta_tab_name and $onglet!=$no_tab_name):?>
              <div id="onglet-<?php print $onglet?>">
                
                
                  <div class="group">
                  <h2><?php print $onglets_struct[$onglet]['elements']['no_group']["label"]?></h2>
                    <?php  foreach($onglets_struct[$onglet]['elements']['no_group']['fields'] as $element_name=>$element_content):?>
                      <?php if(isset($cck_fields[$element_name]['display_settings']['parent'])):?>
                       <?php  print drupal_render($form[$form['type']['#value']][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
                      <?php else:?>
                        <?php print drupal_render($form[$form['type']['#value']][$element_name])?>
                      <?php endif;?>
                  
                <?php endforeach;?>
             </div>
              <?php  foreach($onglets_struct[$onglet]['elements'] as $group_id=>$group_content):?>
                <?php if($group_id!='no_group'):?>
                <div class="group">
                  <h2><?php print $onglets_struct[$onglet]['elements'][$group_id]["label"]?></h2>
                  
                  <?php  foreach($onglets_struct[$onglet]['elements'][$group_id]['fields'] as $element_name=>$element_content):?>
                    <?php if(isset($cck_fields[$element_name]['display_settings']['parent'])):?>
                      <?php print drupal_render($form[$form['type']['#value']][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
                    <?php else:?>
                      <?php print drupal_render($form[$form['type']['#value']][$element_name])?>
                    <?php endif;?>
                  <?php endforeach;?>
                  </div>
                  
                  <?php endif;?>
                  
                <?php endforeach;?>
           
              </div>
              <?php endif;?>
            <?php endforeach;?>
          </div>
      </div>
  </div>
</div>
<div id="column-side-right">
<div class="group">
                  <h2><?php print $onglets_struct[$onglet]['elements']['no_group']["label"]?></h2>
                  
                  <?php  foreach($onglets_struct[$meta_tab_name]['elements']['no_group']['fields'] as $element_name=>$element_content):?>
                    <?php if(isset($cck_fields[$element_name]['display_settings']['parent'])):?>
                      <?php  print drupal_render($form[$form['type']['#value']][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
                    <?php else:?>
                      <?php print drupal_render($form[$form['type']['#value']][$element_name])?>
                    <?php endif;?>
         
                  
                <?php endforeach;?>
                </div>
              <?php  foreach($onglets_struct[$meta_tab_name]['elements'] as $group_id=>$group_content):?>
                <?php if($group_id!='no_group'):?>
                 
                 <div class="group">
                  <h2><?php print $onglets_struct[$onglet]['elements'][$group_id]["label"]?></h2>
                   <?php  foreach($onglets_struct[$meta_tab_name]['elements'][$group_id]['fields'] as $element_name=>$element_content):?>
                    <?php if(isset($cck_fields[$element_name]['display_settings']['parent'])):?>
                      <?php print drupal_render($form[$form['type']['#value']][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
                    <?php else:?>
                      <?php print drupal_render($form[$form['type']['#value']][$element_name])?>
                    <?php endif;?>
                  <?php endforeach;?>
                  
                  </div>
                  <?php endif;?>
                  
                <?php endforeach;?>
           
  
</div>

<div style="display:none">
<?php print drupal_render($form);?>
</div>
