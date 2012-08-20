<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

$title = $fields["title"]->content;
$code_dl = $fields["field_lib_dl_value"]->content;
$code_dp = $fields["field_lib_dp_value"]->content;
$id_compiere = $fields["field_lib_compiere_value"]->content;
$code_company = $fields["field_lib_company_value"]->content;
$subscribers = $fields["field_lib_subscribers_value"]->content;
$phone = $fields["field_add_phone_value"]->content;
$mail = $fields["field_add_mail_email"]->content;
$address = $fields["address"]->content;
$city = $fields["city"]->content;
$country = $fields["country"]->content;
//petit patch pas joli
if($country == 'United States'){
	$country = 'Belgium';
}
$postal_code = $fields["postal_code"]->content;
$street = $fields["street"]->content;

$class = "lib ".substr($postal_code,0)." ".substr($postal_code,0,1)." ".substr($postal_code,0,2)." ".substr($postal_code,0,3);

?>

<li class="<?php print $class ; ?>">
  <div class="liblib">
    <h2><?php print $title ; ?></h2>
    <p class="street"><?php print $street; ?></p>
    <p class="cp_city"><?php print $postal_code; ?> <?php print $city; ?></p>
    <p class="map_libraire"><a href="http://maps.google.com/maps?q=<?php print $street; ?>,+<?php print $postal_code; ?>+<?php print $city; ?>, <?php print $country; ?>" target="_blank">localisation</a></p>
    <p class="code_dl"><?php print $code_dl; ?></p>
  </div>
</li>
