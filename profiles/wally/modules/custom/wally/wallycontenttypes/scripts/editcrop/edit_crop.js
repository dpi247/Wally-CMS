
    Drupal.behaviors.wallycontenttypes = function (context) {
      var currentAc;
      var ac = Drupal.settings.js_usablecrops;

      function showCoordsAndSave(c) {
        if (currentAc) {
          // 1 ajouté la fin de l'array pour flager la modification de ce preset
          var serialCoord = [c.x,c.y,c.w,c.h,1];
          $("#"+currentAc+"_serialCoord").val(serialCoord);
          ac[currentAc][0] = c.x;
          ac[currentAc][1] = c.y;
          ac[currentAc][2] = c.x2;
          ac[currentAc][3] = c.y2;
        }
      };

      $("#wallycontenttypes-form-edit-crop-form").ready(function() {
        if (typeof $("#cropbox").attr("id") != "undefined") {
          var api = $.Jcrop($("#cropbox"),{
            onSelect: showCoordsAndSave,
            bgColor: "black",
            bgOpacity: .4,
            boxWidth: 800
          });

          var i;

          // A handler to kill the action
          function nothing(e) {
            e.stopPropagation();
            e.preventDefault();
            return false;
          };

          var ratio = {};
          for(i in ac) {
            ratio[i] = ac[i][4];
          }

          // Returns event handler for animation callback
          function anim_handler(ac, ratio, i) {
            return function(e) {
              $("#crop-current-name").html(i);
              api.setSelect(ac);
              api.setOptions({ aspectRatio: ratio });
              // Change class of selected button
              if (currentAc) {
                $("#"+currentAc).removeClass("crop_selected");
              }
              $("#"+i).addClass("crop_selected");	
              currentAc = i;

              return nothing(e);
            };
          };

          // Attach respective event handlers and charge default values in the form
          for(i in ac) {
            jQuery("#"+i).click(anim_handler(ac[i], ratio[i], i));
          }

          // Select the first preset
          var first = true;
          for(i in ac) {
            if (first) {
              $("#"+i).trigger("click");
              first = false;
            }
          }
        }
      });
    };
  