//
// Script.js
//

// Menu Panel

var delayTimer;
$("#nav a").mouseenter(function(){
  var $this = $(this);
  var panelId = "#panel-" + this.id;
  delayTimer = window.setTimeout(function(){
    $('.panel:visible').not(panelId).hide();
    $(panelId).show();
  }, 75);
}).mouseleave(function(){
  window.clearTimeout(delayTimer);
});
$(".nav-primary nav").mouseleave(function(){
  $('.panel:visible').hide();
});
$(".panel").mouseleave(function(){
  $(this).hide();
});

// Show Hide Slide Toggle

$(".h-sections").click(function(){
  $("#nav-wrapper").slideToggle("fast");
  $(this).toggleClass("active"); return false;
});

$("#add-comment").click(function(){
  $(".comments-form").slideToggle("fast");
  $(this).toggleClass("active"); return false;
});

$(".actions-t").click(function(){
  $(this).parents(".panel-edit").find(".actions-list").slideToggle("fast");
  $(this).toggleClass("active"); return false;
});

// Tabs

$(function() {
  // ul.tabs > div.panes
  $("ul.tabs:not(.primary,.secondary)").tabs("div.panes > div");
  $("#tab-nav ul").tabs("#panes > div");
});

// Responsive Carousel

$(function() {
  $('.carousel').elastislide({
    speed   : 450
  });
});

// Scrollable
// initialize scrollable together with the autoscroll plugin

$(function() {
	$(".scrollable").scrollable();
});

// News Ticker

$(function() {
	$("#ticker-news").newsticker();
});

// Modal
var triggers = $("#a-login").overlay({
  // some mask tweaks suitable for modal dialogs
  mask: {
    color: '#000',
    loadSpeed: 200,
    opacity: 0.50
  },
	closeOnClick: false
});
var triggersb = $("#b-login").overlay({
	  // some mask tweaks suitable for modal dialogs
	  mask: {
	    color: '#000',
	    loadSpeed: 200,
	    opacity: 0.50
	  },
		closeOnClick: false
	});