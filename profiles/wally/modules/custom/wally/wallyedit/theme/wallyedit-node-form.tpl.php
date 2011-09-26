<style>
<!--
body {
	overflow-y: scroll;
}
#column-main-left {
	width: 65%;
	float: left;
	padding-right: 10px;
	overflow: hidden;
}
#column-side-right{
	width: 33%;
	float: left;
}
#wallyedit_preview_container {
	clear:both;
}
#profile_selector{
	clear:both;
	margin-bottom:30px;
}
.margin {margin-bottom: 10px;}
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
 
#scroller-header, #meta-header{
    /*width:277px;*/
    height:24px;
    padding:35px 0 0 15px;
    font-weight:700;
}
#scroller-header, #meta-header{
	height:36px;
	padding:0px;
	line-height:20px;
	padding: 0 0 10px 0
}
#scroller-header a {
    background: -moz-linear-gradient(center top , #FFFFFF, #EEEEEE) repeat scroll 0 0 transparent;
    border: 1px solid #CCCCCC;
    border-radius: 15px 15px 15px 15px;
    box-shadow: 0 1px 0 rgba(0, 0, 0, 0.1);
    color: #666;
    display: block;
    float: left;
    padding: 4px 15px 6px;
    text-align: center;
    text-decoration: none;
    text-shadow: 0 1px 0 white;
	margin: 0 10px 0 0;
}
#meta-header span{
    background: -moz-linear-gradient(center top , #FFFFFF, #EEEEEE) repeat scroll 0 0 transparent;
    border: 1px solid #CCCCCC;
    /* border-radius: 15px 15px 15px 15px; */
    box-shadow: 0 1px 0 rgba(0, 0, 0, 0.1);
    color: #666666;
    display: block;
    float: left;
    margin: 0 10px 0 0;
    padding: 4px 15px 6px;
    text-align: center;
    text-decoration: none;
    text-shadow: 0 1px 0 white;
 }
#scroller-header .selected {
    background: -moz-linear-gradient(center top , #FFC039, #FF920D) repeat scroll 0 0 transparent;
    border: 1px solid #FF920D;
    color: white !important;
    text-shadow: 0 -1px 0 #DE6E00;
	text-decoration: none !important;
}
#scroller-body {
	background: white;
	padding: 10px;
	border: 1px solid #ccc;
	border-radius: 3px;
}
#mask {
	width: 100%;
	overflow:hidden;
	margin:0 auto;
}
#onglet {
	float: left;
	width: 100000px;
}
#onglet div {
	float: none;
	overflow: hidden;
	clear: both;
}
#onglet .content-tabs { 
	float: left;
	clear: none;
} 
#onglet td div {clear: none;}
body div.column-main {
    float:left;  
}
body div.column-side {
    float:left;
}
.resizable-textarea {
    width: 100%;
}
html.js .resizable-textarea textarea {
    display: block;
    margin-bottom: 0;
    width: 99%;
}
.accordion-tab {margin-bottom: 10px;}
.accordion-tab-title {
	padding: 2px 10px 2px 20px;
	background: #f3f3f3;
	cursor: pointer;
}
.accordion-tab-title:hover {
	background: #ECF8F4;
	color: black;
}
.group {
margin-bottom:10px;
}
.group_content {
border:1px solid #CCCCCC;
padding:5px;
}
.group .title span {
	display: block;
	float: left;
	background: #E1E1E1;
	padding: 5px;
	border-radius: 5px 5px 0 0;
	margin-left:10px;
}
.group .title {clear: both; float: left; width: 100%; border-bottom: 1px solid #ccc;}
.container-inline-date {
    
    margin:auto;
}
-->
table.sticky-header{
position:relative !important;
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

  $tabs=wyditadmin_get_fields_tree($profile_id, $node_type);
  $onglets_struct=$tabs;
  $type=wydit_get_infos_type($node_type);
  $cck_fields = $type['fields'];
?>
$(document).ready(function() { 
 
    //Get the height of the first item
   // $('#mask').css({'height':$('#onglet-<?php print current(array_keys($onglets_struct));?>').height()}); 
     
    //Calculate the total width - sum of all sub-onglets width
    //Width is generated according to the width of #mask * total of sub-onglets
    //$('#onglet').width(parseInt($('#mask').width() * $('#onglet div').length));
     
    //Set the sub-onglet width according to the #mask width (width of #mask and sub-onglet must be same)
    //$('#onglet div').width($('#mask').width());
    
	//calcul idoine des tailles utiles
	var mainColumnWidth = $('#column-main-left').innerWidth() - 32;
	$('.content-tabs').css('width', mainColumnWidth);
	
	//accordion de qualite
	$('#accordion_container .accordion-tab-title').click(function() {
		$(this).next().toggle('slow');
		return false;
	});
	$('#accordion_container .accordion-tab-title.hide').next().hide();
    //Get all the links with rel as onglet
	$('a[rel=onglet]:first').addClass('selected');
    $('a[rel=onglet]').click(function () {
     
        //Get the height of the sub-onglet
        var ongletheight = $($(this).attr('href')).height();
         
        //Set class for the selected item
        $('a[rel=onglet]').removeClass('selected');
        $(this).addClass('selected');
         
        //Resize the height
        $('#mask').animate({'height':ongletheight},{queue:false, duration:500});        
         
        //Scroll to the correct onglet, the onglet id is grabbed from the href attribute of the anchor
        $('#mask').scrollTo($(this).attr('href'), 500);    
         
        //Discard the link default behavior
        return false;
    });
     
});

<?php 
$meta_tab_name="meta_".$profile_id.'_'.$node_type;
$no_tab_name="no_tab";
?>
</script>
<div id="column-main-left">
  <div id="scroller-header">
      <?php foreach($onglets_struct as $onglet=>$onglet_content): ?>
        <?php if($onglet!=$meta_tab_name and $onglet!=$no_tab_name): ?>
          <a href="#onglet-<?php print $onglet; ?>" rel="onglet"><?php print $onglet_content['label']; ?></a>
        <?php endif;?>
      <?php endforeach;?>
  </div>
  <div id="scroller-body">
      <div id="mask">
          <div id="onglet">
          <?php  foreach($onglets_struct as $onglet=>$onglet_content): ?>
            <?php if($onglet!=$meta_tab_name and $onglet!=$no_tab_name): ?>
              <div class="content-tabs" id="onglet-<?php print $onglet; ?>">
             
                <?php if(count($onglets_struct[$onglet]['elements']['no_group']['fields'])>0):?>
                  <div class="group">
                    <div class="group_content">
                      <?php  foreach($onglets_struct[$onglet]['elements']['no_group']['fields'] as $element_name=>$element_content): ?>
                           <?php 
                //  dsm($cck_fields[$element_name],'cck_'.$element_name);
                //  dsm($onglets_struct[$onglet]['elements']['no_group']['fields'][$element_name],'cck_'.$element_name);
                ?>
                        <?php if(isset($cck_fields[$element_name]['display_settings']['parent']) && !empty($cck_fields[$element_name]['display_settings']['parent'])): ?>
                          <?php  print drupal_render($form[$node_type][$cck_fields[$element_name]['display_settings']['parent']][$element_name]); ?>
                        <?php else:?>
                          <?php print drupal_render($form[$node_type][$element_name]); ?>
                        <?php endif;?>
                      <?php endforeach;?>
                    </div>
                  </div>
                <?php endif;?>
             
             <?php  foreach($onglets_struct[$onglet]['elements'] as $group_id=>$group_content): ?>
                <?php if($group_id!='no_group'): ?>
                <div class="group">
                  <h2 class="title  title-group "><span><?php print $onglets_struct[$onglet]['elements'][$group_id]["label"]; ?></span></h2>
                  <div class="group_content">
                  <?php  foreach($onglets_struct[$onglet]['elements'][$group_id]['fields'] as $element_name=>$element_content): ?>
                    <?php if(isset($cck_fields[$element_name]['display_settings']['parent']) && !empty($cck_fields[$element_name]['display_settings']['parent'])): ?>
                      <?php print drupal_render($form[$node_type][$cck_fields[$element_name]['display_settings']['parent']][$element_name]); ?>
                    <?php else:?>
                      <?php print drupal_render($form[$node_type][$element_name]); ?>
                    <?php endif; ?>
                  <?php endforeach; ?>
                  <div class="clear"></div>
                  
                  </div>
                  </div>
                  <?php endif; ?>
                <?php endforeach; ?>
              </div>
              <?php endif; ?>
            <?php endforeach; ?>
          </div>
      </div>
  </div>
</div>
<div id="column-side-right">
  <div id="meta-header">
    <span>Meta</span>
  </div>
  <?php  if(count($onglets_struct[$meta_tab_name]['elements']['no_group']['fields'])>0):?>
  <div class="group">
    <?php foreach($onglets_struct[$meta_tab_name]['elements']['no_group']['fields'] as $element_name=>$element_content):?>
    <?php if(isset($cck_fields[$element_name]['display_settings']['parent']) && !empty($cck_fields[$element_name]['display_settings']['parent'])): ?>
      <?php print drupal_render($form[$node_type][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
    <?php else:?>
      <?php print drupal_render($form[$node_type][$element_name])?>
    <?php endif;?>
    <?php endforeach;?>
  </div>
  <?php endif;?>
  <?php  foreach($onglets_struct[$meta_tab_name]['elements'] as $group_id=>$group_content):?>
  <?php if($group_id!='no_group'):?>
    <div class="group">
      <h2 class="title title-group "><span><?php print $onglets_struct[$meta_tab_name]['elements'][$group_id]["label"]?></span></h2>
      <div class="group_content">
        <?php foreach($onglets_struct[$meta_tab_name]['elements'][$group_id]['fields'] as $element_name=>$element_content): ?>
          <?php if(isset($cck_fields[$element_name]['display_settings']['parent']) && !empty($cck_fields[$element_name]['display_settings']['parent'])): ?>
            <?php print drupal_render($form[$node_type][$cck_fields[$element_name]['display_settings']['parent']][$element_name])?>
          <?php else:?>
            <?php print drupal_render($form[$node_type][$element_name])?>
          <?php endif;?>
        <?php endforeach;?>
        <div class="clear"></div>
      </div>
    </div>
  <?php endif;?>
  <?php endforeach;?>
</div>

<div id="buttons">
  <?php print drupal_render($form['save_global']);?>
  <?php print drupal_render($form['reset_global']);?>
  <?php print drupal_render($form['publish_global']);?>
</div>

<div id="profile_selector">
<?php print drupal_render($form['choose_profile']); ?>
<?php print drupal_render($form['confirm_profile']); ?>
</div>

<div style="display:none">
<?php print drupal_render($form);?>
</div>