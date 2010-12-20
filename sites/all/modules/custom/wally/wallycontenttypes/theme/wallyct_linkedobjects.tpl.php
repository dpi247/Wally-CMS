qdjhfjdhkfjhdlfhsdfjlkhsdqlfjhsdqlkjfhqsjfhqsljkfhqsjklfhqsdlkfhqsdlkjfhdsqlkfqsd
<hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr><hr>
<?php

  drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/linkslist/linkslist.css');

	// Links list
  if (!function_exists('linkURI')) {
    function linkURI($URL = '') {
      $parsedURL = array();
      $parsedURL = parse_url($URL);
      switch ($parsedURL['scheme']) {
        case 'http':
          return $URL;
          break;
        case 'internal':
          return drupal_get_path_alias("/node/".$parsedURL['host']);
          break;
        default:
          return '';
          break;
      }
      return '';
    }
  }

?>
<?php
  if (count($linkedobjects)) {
    foreach ($linkedobjects as $linkedobject) {
?>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
zaezaeazeaz<br/>
    <h2><?php print $linkedobject->title; ?></h2>
    <ul>
      <?php
        foreach ($linkedobject->field_links_list_nodes as $link) {
      ?>
        <li>
          <a href="<?php print linkURI($link->field_link_item[0]['url']); ?>" >
            <?php print $link->field_link_item[0]['title']; ?>
          </a>
        </li>
      <?php
        }
      ?>
    </ul>
<?php
    }
  }
?>
