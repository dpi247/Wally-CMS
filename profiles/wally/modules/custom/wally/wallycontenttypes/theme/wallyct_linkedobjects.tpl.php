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
    <span class="linklistblock">
    <h3 class="linklist"><?php print $linkedobject->title; ?></h3>
    <ul>
      <?php
        foreach ($linkedobject->field_links_list_nodes as $link) {

//          $attributes_html = ""; 
//          foreach ($link->field_link_item[0]['attribute'] as $attribute->$value) {
//            $attribute_html .= $attribute."= '".$value."' "; 
//          }
        ?>
        
        <li>
          <a class="link" <?php print $attribute_html; ?>$href="<?php print linkURI($link->field_link_item[0]['url']); ?>" >
            <?php print $link->field_link_item[0]['title']; ?>
          </a>
        <?php if ($link->field_summary[0]['value']) {
            print "<span class='link-description'>".$link->field_summary[0]['value']."</span>";
          } ?>
        </li>
      <?php
        }
      ?>
    </ul>
    </span>
<?php
    }
  }
?>
