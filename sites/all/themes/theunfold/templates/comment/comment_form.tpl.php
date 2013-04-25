
<?php $class = '';$text = t('Write a comment');
	if (isset($form['submit'])){
		$class = 'extend';
	}
	if ($form['subject']['#default_value'] != NULL){
		$class = 'extend';
		$text = t('Edit comment');
	}
	if (preg_match('/^\/comment\/reply/', $form['#action'])){
	  $class = 'extend';
	  $text = t('Reply to comment');
	}
?>

<div class="comments-header">
	<h3><?php print t('Comments'); ?></h3>
	<a id="add-comment" class="active" title="<?php print $text?>" href="#"><?php print $text; ?></a>
</div>

<div class="comments-form form <?php print $class;?>" >
	<header><h4><?php print $text;?></h4></header>
	<div class="meta">
		<span class="author">
			<span class="author-name"><?php print $form['_author']['#value']?></span>
		</span>
		<br>
		<time datetime="<?php print date('Y-m-d')?>"><?php print t(date('F')).' '.date('j').', '.date('Y');?></time>
	</div>
	<div class="comments-field">
		<?php print drupal_render($form['subject']);?>
		<?php print drupal_render($form['comment_filter']['comment']);?>
		<?php drupal_render($form['comment_filter']['format']);?>
		<?php drupal_render($form['_author']);?>
		<?php $form['preview']['#attributes'] = array('class' => 'btn blue');?>
		<?php $form['submit']['#attributes'] = array('class' => 'btn blue');?>
		<?php print drupal_render($form);?>
	</div>
	
</div>
