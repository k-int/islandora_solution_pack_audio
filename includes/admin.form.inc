<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_audio_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  
	$ffmpeg = $get_default_value('islandora_ffmpeg_url', '/usr/bin/ffmpeg');
  $form = array(
    'islandora_ffmpeg_url' => array(
      '#type' => 'textfield',
      '#title' => t("Path to ffmpeg"),
      '#description' => t('!ffmpeg is required to create derivatives files.<br/>!msg',
                      array(
                        '!ffmpeg' => t('ffmpeg'),
                        '!msg' => islandora_executable_available_message($ffmpeg),
                      )),
      '#default_value' => $ffmpeg,
      '#size' => 20,
      '#prefix' => '<div id="lame-wrapper">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'islandora_audio_admin_form_lame_ajax_callback',
        'wrapper' => 'lame-wrapper',
        'effect' => 'fade',
        'event' => 'change',
      ),
    ),
  );
  
  // Add supported extensions for file upload.
  $form['islandora_audio_allowed_types'] = array(
  		'#type' => 'textfield',
  		'#title' => t('Allowed file extensions'),
  		'#description' => t('Enter a list of allowed file extensions separated by a comma. Leave blank to not limit by extension.'),
  		'#default_value' => variable_get('islandora_audio_allowed_types', ''),
  		'#size' => 100,
  );
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_audio_viewers', 'audio/mpeg');
  return system_settings_form($form);
}

/**
 * Ajax callback for the Lame textfield.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_audio_admin_form_lame_ajax_callback(array $form, array $form_state) {
  return $form['islandora_ffmpeg_url'];
}
