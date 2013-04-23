<?php
global $base_path;
$object_file = $embededobject->field_objectfile[0];
module_load_include('inc', 'filefield', 'filefield.theme');
?>

<?php print theme_filefield_icon($object_file); ?>
<?php print $embededobject->content['group_digitalobject']['group']['field_other3rdparty']['field']['#children']; ?>
<a href="<?php print $base_path.$object_file['filepath']; ?>"><?php print $object_file['filename']; ?></a>
