<?php

$plugin = array(
  'parrot_styles' => array(
    'title'              => t('Parrot Styles'),
    'description'        => t('Thin panel pane and region wrappers with custom classes'),
    'render pane'        => 'parrot_styles_render_pane',
    'pane settings form' => 'parrot_styles_settings_form',

    'render region'      => 'parrot_styles_render_region',
    'settings form'      => 'parrot_styles_region_settings_form',

    'css'                => '../../../theme/css/styles.css',

    'hook theme'         => array(
      'parrot_styles_theme_pane' => array(
        'template'  => 'parrot_styles-pane',

        // Change theme name 'parrot' to match changed theme name.
        'path'      => drupal_get_path('module', 'parrot_styles') . '/plugins/styles/parrot_styles',
        'variables' => array(
          'content'  => NULL,
          'settings' => NULL,
        ),
      ),
    ),
  ),
);


function theme_parrot_styles_render_region($vars) {
  $settings = $vars['settings'];
  $prefix = '';
  $suffix = '';
  $attr = $attr_raw = array();
  $node = menu_get_object();
  $display = $vars['display'];

  if (empty($vars['panes'])) {
    return;
  }

  if (!empty($settings)) {
    if (!empty($settings['grid_width'])) {
      $trim_width = trim($settings['float']);
      if (!empty($trim_width)) {
        $attr['class'][] = $trim_width;
      }
    }
    if (!empty($settings['float'])) {
      $trim_float = trim($settings['float']);
      if (!empty($trim_float)) {
        $attr['class'][] = $trim_float;
      }
    }
    if (!empty($settings['left_margin'])) {
      $trim_left_margin = trim($settings['left_margin']);
      if (!empty($trim_left_margin)) {
        $attr['class'][] = $trim_left_margin;
      }
    }
    if (!empty($settings['right_margin'])) {
      $trim_right_margin = trim($settings['right_margin']);
      if (!empty($trim_right_margin)) {
        $attr['class'][] = $trim_right_margin;
      }
    }
    if (!empty($settings['clear'])) {
      $trim_clear = trim($settings['clear']);
      if (!empty($trim_clear)) {
        $attr['class'][] = $trim_clear;
      }
    }
    if (!empty($settings['bg_class'])) {
      $trim_bg_class = trim($settings['bg_class']);
      if (!empty($trim_bg_class)) {
        $attr['class'][] = $trim_bg_class;
      }
    }
    if (!empty($settings['animate'])) {
      $attr['class'][] = 'animate'; // causes delay if set via JS
      $attr_raw[] = 'data-animate="' . $settings['animate'] . '"';

      if (!empty($settings['animate_target'])) {
        $attr_raw[] = 'data-animate-target="' . trim($settings['animate_target']) . '"';
      }
      if (!empty($settings['animate_delay'])) {
        $attr_raw[] = 'data-animate-delay="' . trim($settings['animate_delay']) . '"';
      }
      if (!empty($settings['animate_duration'])) {
        $attr_raw[] = 'data-animate-duration="' . trim($settings['animate_duration']) . '"';
      }
    }
    if (!empty($settings['custom_classes'])) {
      $custom_classes = trim($settings['custom_classes']);
      $custom_classes = ctools_context_keyword_substitute($custom_classes, array(), $display->context);
      $attr['class'][] = $custom_classes;
    }

    // Copy simple settings into template variables.
    if (!empty($settings['custom_attrib'])) {
      $custom_attrib = trim($settings['custom_attrib']);
      $custom_attrib = ctools_context_keyword_substitute($custom_attrib, array(), $display->context);
      $attr_raw[] = filter_xss_admin($custom_attrib);
    }
    $attr['class'] = array_unique($attr['class']);

    // Attributes
    if (!empty($attr) || !empty($attr_raw)) {
      $attributes = !empty($attr) ? drupal_attributes($attr) : '';
      $attributes = !empty($attr_raw) ? $attributes . ' ' . implode(' ', $attr_raw) : $attributes;
      $prefix = '<div' . $attributes .'>';
      $suffix = '</div>';
    }

    // Region containers
    if (!empty($settings['container'])) {
      if ($settings['container'] == 'inner') {
        $prefix .= '<div class="container region-container-inner">';
        $suffix .= '</div>';
      }
      elseif ($settings['container'] == 'outer') {
        $prefix = '<div class="container region-container-outer">' . $prefix;
        $suffix = '</div>' . $suffix;
      }
    }
  }

  $output = $prefix . implode('', $vars['panes']) . $suffix;
  return $output;
}

function theme_parrot_styles_render_pane($vars) {
  //$content = &$vars['content'];
  $settings = $vars['settings'];
  $display = $vars['display'];

  // Add custom css & render pane
  $extra_attrib = array();
  $extra_css = array();
  if (!empty($settings) && !empty($vars['pane'])) {
    if (!empty($settings['grid_width'])) {
      $extra_css[] = $settings['grid_width'];
    }
    if (!empty($settings['left_margin'])) {
      $extra_css[] = $settings['left_margin'];
    }
    if (!empty($settings['right_margin'])) {
      $extra_css[] = $settings['right_margin'];
    }
    if (!empty($settings['float'])) {
      $extra_css[] = $settings['float'];
    }
    if (!empty($settings['gutter'])) {
      $extra_css[] = $settings['gutter'];
    }
    if (!empty($settings['clear'])) {
      $extra_css[] = $settings['clear'];
    }
    if (!empty($settings['padding'])) {
      $extra_css[] = $settings['padding'];
    }
    if (!empty($settings['bg_class'])) {
      $extra_css[] = trim($settings['bg_class']);
    }
    if (!empty($settings['animate'])) {
      $extra_css[] = 'animate'; // causes delay if set via JS
      $extra_attrib[] = 'data-animate="' . $settings['animate'] . '"';

      if (!empty($settings['animate_target'])) {
        $extra_attrib[] = 'data-animate-target="' . trim($settings['animate_target']) . '"';
      }
      if (!empty($settings['animate_delay'])) {
        $extra_attrib[] = 'data-animate-delay="' . trim($settings['animate_delay']) . '"';
      }
      if (!empty($settings['animate_duration'])) {
        $extra_attrib[] = 'data-animate-duration="' . trim($settings['animate_duration']) . '"';
      }
    }

    if (!empty($settings['custom_classes'])) {
      $custom_classes = trim($settings['custom_classes']);
      $custom_classes = ctools_context_keyword_substitute($custom_classes, array(), $display->context);
      $extra_css[] = $custom_classes;
    }

    // @todo - pop into an array & then drupal_attributes
    if (!empty($settings['custom_attrib'])) {
      $custom_attrib = trim($settings['custom_attrib']);
      $custom_attrib = ctools_context_keyword_substitute($custom_attrib, array(), $display->context);
      $extra_attrib[] = filter_xss_admin($custom_attrib);
    }

    if (!empty($extra_css)) {
      // Add to pane settings for reference
      if (empty($vars['pane']->css['css_class'])) {
        $vars['pane']->css['css_class'] = implode(' ', $extra_css);
      }
      else {
        $vars['pane']->css['css_class'] .= ' ' . implode(' ', $extra_css);
      }

      // hijack pane css
      if (empty($vars['content']->css_class)) {
        $vars['content']->css_class = implode(' ', $extra_css);
      }
      else {
        $vars['content']->css_class .= ' ' . implode(' ', $extra_css);
      }
    }
  }

  // Run direct to get default panels functionality too
  template_preprocess_panels_pane($vars);

  // Flattern out arrays ala hook_process, use custom names to prevent other hooks messing with them
  $vars['extra_css'] = !empty($vars['classes_array']) ? implode(' ', $vars['classes_array']) : '';
  $vars['extra_id'] = !empty($vars['id']) ? ' ' . $vars['id'] : '';
  $vars['extra_attrib'] = !empty($extra_attrib) ? ' ' . implode(' ', $extra_attrib) : '';
  $vars['title_attributes'] = !empty($vars['title_attributes_array']) ? drupal_attributes($vars['title_attributes_array']) : '';

  // Containers
  $vars['pane_inner_prefix'] = $vars['pane_inner_suffix'] = '';
  if (!empty($settings['container'])) {
    if ($settings['container'] == 'inner') {
      $vars['pane_inner_prefix'] = '<div class="container pane-container-inner">';
      $vars['pane_inner_suffix'] = '</div>';
    }
    elseif ($settings['container'] == 'outer') {
      $vars['pane_prefix'] = '<div class="container pane-container-outer">' . (!empty($vars['pane_prefix']) ? $vars['pane_prefix'] : '');
      $vars['pane_suffix'] = '</div>' . (!empty($vars['pane_suffix']) ? $vars['pane_suffix'] : '');
    }
  }

  return theme('parrot_styles_theme_pane', $vars);
}

// Region settings form
function parrot_styles_region_settings_form($style_settings) {
  return parrot_styles_settings_form($style_settings, 'region');
}

// Pane settings form
function parrot_styles_pane_settings_form($style_settings) {
  return parrot_styles_settings_form($style_settings);
}

// Default configuration form for the pane/region settings.
function parrot_styles_settings_form($style_settings, $type = 'pane') {

  $form['grid_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Pane Width'),
    '#options'       => array(
      'width-100' => t('100%  width'),
      'width-87'  => t('87% width'),
      'width-84'  => t('84% width'),
      'width-80'  => t('80% width'),
      'width-75'  => t('75% width'),
      'width-66'  => t('66% width'),
      'width-62'  => t('62% width'),
      'width-50'  => t('50% width'),
      'width-37'  => t('37% width'),
      'width-33'  => t('33% width'),
      'width-25'  => t('25% width'),
      'width-20'  => t('20% width'),
      'width-16'  => t('16% width'),
      'width-12'  => t('12% width'),
      ' '       => t('-None-'),
    ),
    '#default_value' => (isset($style_settings['grid_width'])) ? $style_settings['grid_width'] : 'width-100',
  );

  $form['left_margin'] = array(
    '#type'          => 'select',
    '#title'         => t('Left Margin'),
    '#options'       => array(
      '' => t('None'),
      'pre-87'  => t('87%'),
      'pre-84'  => t('84%'),
      'pre-75'  => t('75%'),
      'pre-66'  => t('66%'),
      'pre-62'  => t('62%'),
      'pre-50'  => t('50%'),
      'pre-37'  => t('37%'),
      'pre-33'  => t('33%'),
      'pre-25'  => t('25%'),
      'pre-20'  => t('20%'),
      'pre-16'  => t('16%'),
      'pre-12'  => t('12%'),
    ),
    '#default_value' => (isset($style_settings['left_margin'])) ? $style_settings['left_margin'] : '',
  );

  $form['right_margin'] = array(
    '#type'          => 'select',
    '#title'         => t('Right Margin'),
    '#options'       => array(
      '' => t('None'),
      'post-87'  => t('87%'),
      'post-84'  => t('84%'),
      'post-75'  => t('75%'),
      'post-66'  => t('66%'),
      'post-62'  => t('62%'),
      'post-50'  => t('50%'),
      'post-37'  => t('37%'),
      'post-33'  => t('33%'),
      'post-25'  => t('25%'),
      'post-20'  => t('20%'),
      'post-16'  => t('16%'),
      'post-12'  => t('12%'),
    ),
    '#default_value' => (isset($style_settings['right_margin'])) ? $style_settings['right_margin'] : '',
  );

  $form['float'] = array(
    '#type'          => 'select',
    '#title'         => t('Float'),
    '#options'       => array(
      '' => t('None'),
      'float-left'  => t('float left'),
      'float-right'  => t('float right'),
    ),
    '#default_value' => (isset($style_settings['float'])) ? $style_settings['float'] : '',
  );

  $form['gutter'] = array(
    '#type'          => 'select',
    '#title'         => t('Override Column Gutters'),
    '#options'       => array(
      ''  => t('Default Gutter Width'),
      'no-gutter'  => t('Remove Gutters'),
    ),
    '#default_value' => (isset($style_settings['gutter'])) ? $style_settings['gutter'] : '',
  );

  $form['padding'] = array(
    '#type'          => 'select',
    '#title'         => t('Pane Padding'),
    '#options'       => array(
      'panel-pane-no-padding'  => t('No Padding'),
      'panel-pane-padding-sm'  => t('Small Padding'),
      'panel-pane-padding-md'    => t('Medium Padding'),
      'panel-pane-padding-lg'    => t('Large Padding'),
    ),
    '#default_value' => (isset($style_settings['padding'])) ? $style_settings['padding'] : 'panel-pane-no-padding',
  );


  $form['clear'] = array(
    '#type'          => 'select',
    '#title'         => t('Clear Above Row'),
    '#options'       => array(
      ''  => t('Default Row Clear'),
      'clear-both'  => t('Force Row Clear'),
      'clearfix'    => t('Force Clearfix After'),
    ),
    '#default_value' => (isset($style_settings['clear'])) ? $style_settings['clear'] : '',
  );

  $form['bg_class'] = array(
    '#type' => 'select',
    '#title' => t('Background Overlay'),
    '#options' => array(
      ''  => t('- None -'),
      'bg-white' => t('Solid White'),
      'bg-blue' => t('Transparent Blue'),
      'bg-blue-lt-dk'   => t('Transparent Blue - Light to Dark'),
      'bg-blue-dk-lt'   => t('Transparent Blue - Dark ot Light'),
    ),
    '#default_value' => (isset($style_settings['bg_class'])) ? $style_settings['bg_class'] : '',
    '#description' => t('Background colour to apply to pane.')
  );

  $animate_options = parrot_styles_get_animate_effects(t('- None -'), 'out');
  $milliseconds_options = array('' => t('- Default -')) + drupal_map_assoc(range(0.1,5, 0.1));
  $form['animate'] = array(
    '#type' => 'select',
    '#title' => t('On Load Animation Style'),
    '#options' => $animate_options,
    '#default_value' => (isset($style_settings['animate'])) ? $style_settings['animate'] : '',
    '#description' => t('Animated Style of the pane. This will load when the item is in view.'),
    "#empty_option" => t('- None -'),
  );
  $form['animate_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation target element(s)'),
    '#default_value' => (isset($style_settings['animate_target'])) ? $style_settings['animate_target'] : '',
    '#description' => t('Elements within this pane that should be animated, by default the animation is applied to the pane itself. Should be a jQuery selector ie. .some-class or #some-id '),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[animate]"]' => array('value' => ''),
      ),
    ),
  );
  $form['animate_delay'] = array(
    '#type' => 'select',
    '#title' => t('Animation delay'),
    '#field_suffix' => t('milliseconds'),
    '#options' => $milliseconds_options,
    '#default_value' => (isset($style_settings['animate_delay'])) ? $style_settings['animate_delay'] : '',
    '#description' => t('Optional: A delay in milliseconds to wait before the animation is triggered. If target elements are specified above, this setting is used to "stagger" animation effects. Leave blank for no delay.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[animate]"]' => array('value' => ''),
      ),
    ),
  );
  $form['animate_duration'] = array(
    '#type' => 'select',
    '#title' => t('Animation duration'),
    '#field_suffix' => t('milliseconds'),
    '#options' => $milliseconds_options,
    '#default_value' => (isset($style_settings['animate_duration'])) ? $style_settings['animate_duration'] : '',
    '#description' => t('Optional: A duration in milliseconds the animation should last. Leave blank to use default CSS durations (varies per animation).'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[animate]"]' => array('value' => ''),
      ),
    ),
  );

  $form['custom_classes'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom CSS Classes'),
    '#default_value' => (isset($style_settings['custom_classes'])) ? $style_settings['custom_classes'] : '',
    '#description' => t('A space separated list of custom CSS classes to apply to pane. ie. block-border-bottom-orange my-block etc. Keywords from context are allowed.')
  );

  $form['custom_attrib'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom Attributes'),
    '#default_value' => (isset($style_settings['custom_attrib'])) ? $style_settings['custom_attrib'] : '',
    '#description' => t('A space separated list of custom attributes to apply to pane. ie. data-type="blabla" etc. Keywords from context are allowed.')
  );

  $form['container'] = array(
    '#type'          => 'select',
    '#options'       => array(
      '' => t('-None-'),
      'inner' => t('Inner Container'),
      'outer' => t('Outer Container'),
    ),
    '#title'         => t('Container Options'),
    '#default_value' => (isset($style_settings['container'])) ? $style_settings['container'] : '',
    '#description'   => t('Choose to add a .container div to the inner or outer of the region/pane, useful for full width regions with contained content.')
  );

  return $form;
}

function parrot_styles_get_animate_effects($none = FALSE, $exclude = '') {
  $effects = array(
    '' => $none,
    'bounce' => t('Bounce'),
    'flash' => t('Flash'),
    'pulse' => t('Pulse'),
    'rubberBand' => t('Rubber Band'),
    'shake' => t('Shake'),
    'headShake' => t('Head Shake'),
    'swing' => t('Swing'),
    'tada' => t('Tada'),
    'wobble' => t('Wobble'),
    'jello' => t('Jello'),
    'bounceIn' => t('Bounce In'),
    'bounceInDown' => t('Bounce In Down'),
    'bounceInLeft' => t('Bounce In Left'),
    'bounceInRight' => t('Bounce In Right'),
    'bounceInUp' => t('Bounce In Up'),
    'bounceOut' => t('Bounce Out'),
    'bounceOutDown' => t('Bounce Out Down'),
    'bounceOutLeft' => t('Bounce Out Left'),
    'bounceOutRight' => t('Bounce Out Right'),
    'bounceOutUp' => t('Bounce Out Up'),
    'fadeIn' => t('Fade In '),
    'fadeInDown' => t('Fade In Down'),
    'fadeInDownBig' => t('Fade In Down Big'),
    'fadeInLeft' => t('Fade In Left'),
    'fadeInLeftBig' => t('Fade In Left Big'),
    'fadeInRight' => t('Fade In Right'),
    'fadeInRightBig' => t('Fade In Right Big'),
    'fadeInUp' => t('Fade In Up'),
    'fadeInUpBig' => t('Fade In Up Big'),
    'fadeOut' => t('Fade Out'),
    'fadeOutDown' => t('Fade Out Down'),
    'fadeOutDownBig' => t('Fade Out Down Big'),
    'fadeOutLeft' => t('Fade Out Left'),
    'fadeOutLeftBig' => t('Fade Out Left Big'),
    'fadeOutRight' => t('Fade Out Right'),
    'fadeOutRightBig' => t('Fade Out Right Big'),
    'fadeOutUp' => t('Fade Out Up'),
    'fadeOutUpBig' => t('Fade Out Up Big'),
    'flipInX' => t('Flip In X'),
    'flipInY' => t('Flip In Y'),
    'flipOutX' => t('Flip Out X'),
    'flipOutY' => t('Flip Out Y'),
    'lightSpeedIn' => t('Light Speed In'),
    'lightSpeedOut' => t('Light Speed Out'),
    'rotateIn' => t('Rotate In'),
    'rotateInDownLeft' => t('Rotate In Down Left'),
    'rotateInDownRight' => t('Rotate In Down Right'),
    'rotateInUpLeft' => t('Rotate In Up Left'),
    'rotateInUpRight' => t('Rotate In Up Right'),
    'rotateOut' => t('Rotate Out'),
    'rotateOutDownLeft' => t('Rotate Out Down Left'),
    'rotateOutDownRight' => t('Rotate Out Down Right'),
    'rotateOutUpLeft' => t('Rotate Out Up Left'),
    'rotateOutUpRight' => t('Rotate Out Up Right'),
    'hinge' => t('Hinge'),
    'rollIn' => t('Roll In'),
    'rollOut' => t('Roll Out'),
    'zoomIn' => t('Zoom In'),
    'zoomInDown' => t('Zoom In Down'),
    'zoomInLeft' => t('Zoom In Left'),
    'zoomInRight' => t('Zomm In Right'),
    'zoomInUp' => t('Zoom In Up'),
    'zoomOut' => t('Zoom Out'),
    'zoomOutDown' => t('Zoom Out Down'),
    'zoomOutLeft' => t('Zoom Out Left'),
    'zoomOutRight' => t('Zoom Out Right'),
    'zoomOutUp' => t('Zoom Out Up'),
    'slideInDown' => t('Slide In Down'),
    'slideInLeft' => t('Slide In Left'),
    'slideInRight' => t('Slide In Right'),
    'slideInUp' => t('Slide In Up'),
    'slideOutDown' => t('Slide Out Down'),
    'slideOutLeft' => t('Slide Out Left'),
    'slideOutRight' => t('Slide Out Right'),
    'slideOutUp' => t('Slide Out Up'),
  );

  if (empty($none)) {
    unset($effects['']);
  }

  return $effects;
}
