Drupal.behaviors.officeHours = function (context) {
  $(".oh-hide").parent().hide();
  $(".oh-add-more-link").each(function (i) {
   $(this).parent().children("div.office-hours-block").hide();
    })
    .click(function () {
    $(this).hide();
    $(this).parent().children("div.office-hours-block").fadeIn("slow");
    return false;
  });
}; 
