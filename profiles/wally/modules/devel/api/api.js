/**
 * @file
 * JavaScript for the API module.
 */

Drupal.behaviors.apiAutoComplete = function (context) {
  $('form.api-search-form:not(.apiAutoCompleteProcessed)', context).addClass('apiAutoCompleteProcessed').each(function () {
    // On the first focus.
    $('input.api-search-keywords', this).attr('autocomplete', 'off').one('focus', function () {
      var $this = $(this);
      // Prefetch list of objects for this branch.
      $.getJSON(Drupal.settings.apiAutoCompletePath, function (data) {
        // Attach to autocomplete.
        $this.autocomplete(data, {
          sort: function (a, b) {
            return a.value.length - b.value.length;
          },
          matchContains: true,
          max: 200,
          scroll: true,
          scrollHeight: 360,
          width: 300
        }).result(function () {
          $this.get(0).form.submit();
        }).focus();
      });
    });
  });
};
