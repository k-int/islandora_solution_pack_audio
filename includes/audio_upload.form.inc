<?php

/**
 * @file
 * Handles the uploading of the audio file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the audio file.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_audio_audio_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));

  $extensions = variable_get('islandora_audio_allowed_types', '');
  return array(
    'file' => array(
      '#title' => t('Audio File'),
      '#type' => 'managed_file',
      '#required' => TRUE,
      '#description' => t('Select a file to upload.<br />Files must be less than <strong>!size MB.</strong><br/>Allowed file types: <strong>!ext.</strong>', array('!size' => $upload_size, '!ext' => $extensions)),
      '#default_value' => isset($form_state['values']['file']) ? $form_state['values']['file'] : NULL,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => array ($extensions),
         // Assume its specified in MB.
        'file_validate_size' => array($upload_size * 1024 * 1024),
      ),
    ),
  );
}

/**
 * Addes the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_audio_audio_upload_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  if (empty($object['OBJ'])) {
    $ds = $object->constructDatastream('OBJ', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['OBJ'];
  }
  $file = file_load($form_state['values']['file']);
  $path = drupal_realpath($file->uri);
  $ds->setContentFromFile($path, FALSE);
  // Changed this
  // $ds->label = $file->filename;
  $filename = preg_replace("/\\.[^.\\s]{3,4}$/", "", $file->filename);
  $ds->label = $filename;
  $ds->mimetype = $file->filemime;
}
