/*
 * jquery.pajinate.js - version 0.4
 * Copyright (c) 2010, Wes Nolte (http://wesnolte.com)
 * --------------------------------------------------- */

Drupal.behaviors.pagination = function() {

(function($) {
  $.fn.pajinate = function(options){
    // Set some state information
    var current_page = 'current_page';
    var items_per_page = 'items_per_page';
  
    var meta;

    // Setup default option values
    var defaults = {
      item_container_id : '.page-inner',
      items_per_page : 10,      
      nav_panel_id : '.pagination',
      nav_info_id : '.info_text',
      num_page_links_to_display : 20,     
      start_page : 0,
      wrap_around : false,
      nav_label_first : 'First',
      nav_label_prev : '',
      nav_label_next : '',
      nav_label_last : 'Last',
      nav_order : ["first", "prev", "num", "next", "last"],
      nav_label_info : 'Showing {0}-{1} of {2} results',
      show_first_last: true,
      abort_on_small_lists: false,
      jquery_ui: false,
      jquery_ui_active: "ui-state-highlight",
      jquery_ui_default: "ui-state-default",
      jquery_ui_disabled: "ui-state-disabled"
    };
    
    var options = $.extend(defaults,options);
    var $item_container;
    var $page_container;  
    var $items;
    var $nav_panels;
    var total_page_no_links;
    var jquery_ui_default_class = options.jquery_ui ? options.jquery_ui_default : '';
    var jquery_ui_active_class = options.jquery_ui ? options.jquery_ui_active : '';
    var jquery_ui_disabled_class = options.jquery_ui ? options.jquery_ui_disabled : '';
      
    return this.each(function()
    {
      $page_container = $(this);
      $item_container = $(this).find(options.item_container_id);
      $items = $page_container.find(options.item_container_id).children();

      if (options.abort_on_small_lists && options.items_per_page >= $items.size())
        return $page_container;
              
      meta = $page_container;
    
      // Initialize meta data
      meta.data(current_page,0);
      meta.data(items_per_page, options.items_per_page);
  
      // Get the total number of items
      var total_items = $item_container.children().size();

      // Calculate the number of pages needed
      var number_of_pages = Math.ceil(total_items/options.items_per_page);

      // Construct the nav bar
      var more = '<span class="ellipse more">...</span>';
      var less = '<span class="ellipse less">...</span>';
      var first = !options.show_first_last ? '' : '<a class="first_link '+ jquery_ui_default_class +'" href="">'+ options.nav_label_first +'</a>';
      var last = !options.show_first_last ? '' : '<a class="last_link '+ jquery_ui_default_class +'" href="">'+ options.nav_label_last +'</a>';

      var navigation_html = "";
    
      for(var i = 0; i < options.nav_order.length; i++) 
      {
        switch (options.nav_order[i]) 
        {
          case "first":
            navigation_html += first;
          break;
          case "last":
            navigation_html += last;
          break;
          case "next":
            navigation_html += '<a class="next '+ jquery_ui_default_class +'" href="">'+'<i class="icon-caret-right icon-large"></i>'+ options.nav_label_next +'</a>';
          break;
          case "prev":
            navigation_html += '<a class="prev '+ jquery_ui_default_class +'" href="">'+'<i class="icon-caret-left icon-large"></i>'+ options.nav_label_prev +'</a>';
          break;
          case "num":
            //navigation_html += less;
            var current_link = 0;
            while(number_of_pages > current_link){
            navigation_html += '<a class="link '+ jquery_ui_default_class +'" href="" longdesc="' + current_link +'">'+ (current_link + 1) +'</a>';
            current_link++;
            }
            //navigation_html += more;
          break;
          default:
          break;
        }
      }
    
      // And add it to the appropriate area of the DOM  
      $nav_panels = $page_container.find(options.nav_panel_id);     
      $nav_panels.html(navigation_html).each(function(){
        $(this).find('.link:first').addClass('first');
        $(this).find('.link:last').addClass('last');
      });
    
      // Hide the more/less indicators
      $nav_panels.children('.ellipse').hide();
    
      // Set the active page link styling
      $nav_panels.find('.prev').next().next().addClass('active '+ jquery_ui_active_class);

      /* Setup Page Display */
      // And hide all pages
      $items.hide();
      // Show the first page      
      $items.slice(0, meta.data(items_per_page)).show();

      /* Setup Nav Menu Display */
      // Page number slices

      total_page_no_links = $page_container.children(options.nav_panel_id+':first').children('.link').size();
      options.num_page_links_to_display = Math.min(options.num_page_links_to_display,total_page_no_links);

      $nav_panels.children('.link').hide(); // Hide all the page links

      // And only show the number we should be seeing
      $nav_panels.each(function(){
        $(this).children('.link').slice(0, options.num_page_links_to_display).show();      
      });
    
      /* Bind the actions to their respective links */

      // Event handler for 'First' link
      $page_container.find('.first_link').click(function(e){
        e.preventDefault();

        movePageNumbersRight($(this),0);
        gotopage(0);        
      });     

      // Event handler for 'Last' link
      $page_container.find('.last_link').click(function(e){
        e.preventDefault();
        var lastPage = total_page_no_links - 1;
        movePageNumbersLeft($(this),lastPage);
        gotopage(lastPage);       
      });     
    
      // Event handler for 'Prev' link
      $page_container.find('.prev').click(function(e){
        e.preventDefault();
        showPrevPage($(this));
      });

      // Event handler for 'Next' link
      $page_container.find('.next').click(function(e){
        e.preventDefault();       
        showNextPage($(this));
      });

      // Event handler for each 'Page' link
      $page_container.find('.link').click(function(e){
        e.preventDefault();
        gotopage($(this).attr('longdesc'));
      });     
    
      // Goto the required page
      gotopage(parseInt(options.start_page));
      //toggleMoreLess();
      if(!options.wrap_around)
        tagNextPrev();
    });
  
    function showPrevPage(e){
      new_page = parseInt(meta.data(current_page)) - 1;           

      // Check that we aren't on a boundary link
      if($(e).siblings('.active').prev('.link').length==true)
      {
        movePageNumbersRight(e,new_page);
        gotopage(new_page);
      }
      else if(options.wrap_around)
      {
        gotopage(total_page_no_links-1);   
      }
    };
    
    function showNextPage(e){
      new_page = parseInt(meta.data(current_page)) + 1;

      // Check that we aren't on a boundary link
      if($(e).siblings('.active').next('.link').length==true){    
        movePageNumbersLeft(e,new_page);
        gotopage(new_page);
      } 
      else if (options.wrap_around) 
      {
        gotopage(0);
      }
    };
    
    function gotopage(page_num){
      var ipp = parseInt(meta.data(items_per_page));
      var isLastPage = false;

      // Find the start of the next slice
      start_from = page_num * ipp;

      // Find the end of the next slice
      end_on = start_from + ipp;
      // Hide the current page  
      var items = $items.hide().slice(start_from, end_on);

      items.show();

      // Reassign the active class
      $page_container.find(options.nav_panel_id).children('.link[longdesc=' + page_num +']').addClass('active '+ jquery_ui_active_class)
        .siblings('.active')
        .removeClass('active ' + jquery_ui_active_class);                    

      // Set the current page meta data             
      meta.data(current_page,page_num);

      $page_container.find(options.nav_info_id).html(options.nav_label_info.replace("{0}",start_from+1).
      replace("{1}",start_from + items.length).replace("{2}",$items.length));

      // Hide the more and/or less indicators
      //toggleMoreLess();

      // Add a class to the next or prev links if there are no more pages next or previous to the active page
      tagNextPrev();
    };  
  
    // Methods to shift the diplayed index of page numbers to the left or right
    function movePageNumbersLeft(e, new_p){
      var new_page = new_p;
      var $current_active_link = $(e).siblings('.active');
      if($current_active_link.siblings('.link[longdesc=' + new_page +']').css('display') == 'none')
      {
        $nav_panels.each(function(){
          $(this).children('.link')
            .hide() // Hide all the page links
            .slice(parseInt(new_page - options.num_page_links_to_display + 1) , new_page + 1)
            .show();    
        });
      }
    } 
  
    function movePageNumbersRight(e, new_p){  
      var new_page = new_p;

      var $current_active_link = $(e).siblings('.active');

      if($current_active_link.siblings('.link[longdesc=' + new_page +']').css('display') == 'none')
      {
        $nav_panels.each(function(){
          $(this).children('.link')
            .hide() // Hide all the page links
            .slice( new_page , new_page + parseInt(options.num_page_links_to_display))
            .show();
        });
      }
    }
  
  
  
  /* Add the style class ".no-more" to the first/prev and last/next links to allow custom styling */
  function tagNextPrev() {
    if($nav_panels.children('.last').hasClass('active')){
      $nav_panels.children('.next').add('.last_link').addClass('no-more ' + jquery_ui_disabled_class);
    } else {
      $nav_panels.children('.next').add('.last_link').removeClass('no-more ' + jquery_ui_disabled_class);
    }

    if($nav_panels.children('.first').hasClass('active')){
      $nav_panels.children('.prev').add('.first_link').addClass('no-more ' + jquery_ui_disabled_class);
    } else {
      $nav_panels.children('.prev').add('.first_link').removeClass('no-more ' + jquery_ui_disabled_class);
    }
    if ($nav_panels.children('.first').hasClass('active') & $nav_panels.children('.last').hasClass('active')){
      $nav_panels.hide();
    }
  }

  };

})( jQuery);

// PAGINATION
$(function() {
  $(".page-articles").each(function(){
    var per_page = parseInt($(this).attr('per_page')) || 5;

    $(this).pajinate({
      num_page_links_to_display : 5,
      items_per_page : per_page,
      wrap_around: true,
      show_first_last: false
    })
  });

  $(".block-slidepic").pajinate({
    items_per_page : 1,
    wrap_around: true,
    show_first_last: false
  });
});

} // # Drupal.behaviors.pagination