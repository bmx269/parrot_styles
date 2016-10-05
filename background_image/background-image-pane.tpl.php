
<?php
/**
 * Additional mark up on top of pane template
 * vars:
 * $settings
 * $content
 */
?>
<div class="panel-style-region-background-wrapper <?php print $settings['class']; ?>"<?php print $settings['background_style']; ?>>
  <?php print $content; ?>
</div>