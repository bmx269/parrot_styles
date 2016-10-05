<?php

// Plugin definition
$plugin = array(
  'background_image' => array(
    // Title and description of the plugin
    'title' => t('Background image'),
    'description' => t('Background image style.'),
    // Define a theme function for this plugin
    'render pane' => 'zenoss_background_handler_background_image_render_pane',
    'render region' => 'zenoss_background_handler_background_image_render_region',
    // Template for rendering
    'hook theme' => array(
      'zenoss_background_handler_background_image_pane' => array(
        'variables' => array(
          'content' => NULL,
          'settings' => array(),
        ),
        'path' => drupal_get_path('module', 'zenoss_background_handler') . '/plugins/styles/background_image',
        'template' => 'background-image-pane',
      ),
      'zenoss_background_handler_background_image_region' => array(
        'variables' => array(
          'content' => NULL,
          'settings' => array(),
        ),
        'path' => drupal_get_path('module', 'zenoss_background_handler') . '/plugins/styles/background_image',
        'template' => 'background-image-region',
      ),
    ),
    // This defines the settings form for the plugin
    'pane settings form' => 'zenoss_background_handler_settings_form',
    'pane settings form validate' => 'zenoss_background_handler_settings_form_validate',
    'pane settings form submit' => 'zenoss_background_handler_settings_form_submit',
    'settings form' => 'zenoss_background_handler_settings_form',
    'settings form submit' => 'zenoss_background_handler_settings_form_submit',
  ),
);

/**
 * Settings form callback. NOTE: Input is not per Drupal's form API (input args)
 */
function zenoss_background_handler_settings_form($settings, $panel_obj, $pane_id, $type, $display_obj) {
  if ($type == 'region') {
    $pane_id = $display_obj['display']->did;
  }
  if (isset($settings['background_image']) && is_numeric($settings['background_image'])) {
    $previous_image_fid = $settings['background_image'];
  }
  else {
    // Get previous fid from file usage, this is needed if style were switched to different style and back to this style
    // default value is gone after that so we need to find it again.
    $query = db_select('file_usage', 'f')
        ->fields('f', array('fid'))
        ->condition('f.id', $pane_id)
        ->condition('f.module', 'zenoss_background_handler')
        ->condition('f.type', $type . '_background_image')
        ->range(0, 1)
        ->execute();
    $result = $query->fetchObject();
    $previous_image_fid = isset($result->fid) ? $result->fid : '';
  }

  $form['background_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Background image'),
    '#size' => 40,
    '#default_value' => $previous_image_fid,
    '#upload_location' => 'public://',
  );

  // Keep a copy of previous image fid, since submit function is not drupal form
  // was not able to find default value from background image field.
  $form['background_image_default_fid'] = array(
    '#type' => 'value',
    '#value' => $previous_image_fid,
  );

  $presetImagesFiles = file_scan_directory(drupal_get_path('module', 'zenoss_background_handler') . '/plugins/styles/background_image/preset_images', '/.*\.(jpg|png|gif)$/');
  $presetImagesOptions = array(
    '' => '<Select preset background>',
    );
  foreach ($presetImagesFiles as $presetImage) {
    $imageName = ucwords(str_replace(array('_', '-'), ' ', $presetImage->name));
    $presetImagesOptions[$presetImage->uri] = $imageName;
  }

  $form['background_image_preset'] = array(
    '#type' => 'select',
    '#title' => t('or Use preset background image'),
    '#description' => t('Choose from the preset images instead'),
    '#options' => $presetImagesOptions,
    '#default_value' => (isset($settings['background_image_preset'])) ? $settings['background_image_preset'] : '',
  );

  $form['text_class'] = array(
    '#type' => 'select',
    '#title' => t('Text color class'),
    '#description' => t('Adds additonal text class to wrapper'),
    '#options' => array('' => '<Select text color shade>', 
      'color-animation' => 'Color Animation', 
      'gradient-blue' => 'Gradient Blue',
      'gradient-green' => 'Gradient Green',
      'gradient-orange' => 'Gradient Orange',
    ),
    '#default_value' => (isset($settings['text_class'])) ? $settings['text_class'] : '',
  );

  // Keep the pane id around, we need to use this later.
  $form['pane_id'] = array(
    '#type' => 'value',
    '#value' => $pane_id,
  );

  // Type could be pane or region.
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  return $form;
}

/**
 * submit handler for settings form, notable: file usage type has region/pane prefix to _background_image, it's used to locate unique id
 */
function zenoss_background_handler_settings_form_submit($form, $settings, $form_state) {
  // If preset is not selected, we use uploaded image, do processing here.
  if (!$settings['background_image_preset']) {
    if ($settings['background_image_default_fid'] != $settings['background_image']) {
      //previous fid is not the same and newly submitted fid, remove old file from server/db
      if (is_numeric($settings['background_image_default_fid'])) {
        //if there is a default value, remove it from usage and delete the file
        $file = file_load($settings['background_image_default_fid']);
        if ($file) {
          file_usage_delete($file, 'zenoss_background_handler', $settings['type'] . '_background_image', $settings['pane_id']);
          file_delete($file);
        }
      }
      $file = file_load($settings['background_image']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      if (!is_numeric($settings['pane_id'])) {
        $next_pid = db_query('select pid from panels_pane order  by pid DESC limit 1')->fetchField();
        $settings['pane_id'] = $next_pid + 1;
      }
      file_usage_add($file, 'zenoss_background_handler', $settings['type'] . '_background_image', $settings['pane_id']);
    }
  }
}

/**
 * Render pane callback
 */
function theme_zenoss_background_handler_background_image_render_pane($vars) {
  $content = $vars['content'];
  $settings['background_style'] = '';
  $pane = $vars['pane'];
  $display = $vars['display'];

  if (empty($content->content)) {
    return;
  }
  // Render with default theme first, for background image, we just need to wrap
  // a background wrapper outside of default.
  $output = theme('panels_pane', array('content' => $content, 'pane' => $pane, 'display' => $display));
  if (strpos($vars['settings']['background_image_preset'], '#') === 0) {
    $settings['background_style'] = ' style="background-color:' . $vars['settings']['background_image_preset'] . ';"';
  }
  else {
    $settings['background_image_url'] = _zenoss_background_handler_get_image_url($vars);
    $settings['background_style'] = ' style="background-image:url(' . $settings['background_image_url'] . ');background-size: cover;background-position: 50% 50%;background-repeat: no-repeat;background-attachment:fixed;display:block;width:100%;"';
  }
  $settings['class'] = $vars['settings']['text_class'];

  // Just stick a wrapper around the standard theme_panels_pane to add background.
  $output = theme('zenoss_background_handler_background_image_pane', array('content' => $output, 'settings' => $settings));
  return $output;
}

/**
 * Render region callback.
 */
function theme_zenoss_background_handler_background_image_render_region($vars) {
  $settings = $vars['settings'];
  $settings['background_style'] = '';
  $settings['class'] = $vars['settings'];

  if (strpos($vars['settings']['background_image_preset'], '#') === 0) {
    $settings['background_style'] = ' style="background-color:' . $vars['settings']['background_image_preset'] . ';"';
  }
  else {
    $settings['background_image_url'] = _zenoss_background_handler_get_image_url($vars);
    $settings['background_style'] = ' style="background-image:url(' . $settings['background_image_url'] . ');background-size: cover;background-position: 50% 50%;background-repeat: no-repeat;background-attachment:fixed;display:inline-block;width:100%;"';
  }
  $content = implode($vars['panes']);
  $output = theme('zenoss_background_handler_background_image_region', array('content' => $content, 'settings' => $settings));
  return $output;
}

/**
 * Process image file id to url, applying image style if necessary
 * @param type $vars: panel variable
 * @return processed image url
 */
function _zenoss_background_handler_get_image_url($vars) {
  $image_url = NULL;
  if (isset($vars['settings']['background_image_preset']) && $vars['settings']['background_image_preset']) {
    $image_uri = $vars['settings']['background_image_preset'];
    $image_url = file_create_url($image_uri);
  }
  else {
    if ($vars['settings']['background_image'] === 0) {
      if ($vars['display']->context['panelizer']->is_type('node')) {
        $nid = $vars['display']->context['panelizer']->get_argument();
        $node_wrapper = entity_metadata_wrapper('node', $nid);
        if (isset($node_wrapper->field_default_background)) {
          $file_val = $node_wrapper->field_default_background->value();
          if (isset($file_val['fid'])) {
            $file = file_load($file_val['fid']);
          }
        }
      }
    }
    else {
      $file = file_load($vars['settings']['background_image']); //file id
    }
    if (isset($file)) {
      $image_uri = $file->uri;
      $image_url = file_create_url($image_uri);
    }

  }

  return $image_url;
}
