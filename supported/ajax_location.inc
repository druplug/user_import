<?php


if (module_exists('ajax_location')) {
    /**
     * Implementation of hook_user_import_form_field_match().
     */
    function ajax_location_user_import_form_field_match() {

        $options = array();


        $result = db_query("SELECT * FROM {field_config} WHERE module = :module", array(
            ':module' => 'ajax_location',
        ));

        $fields = array();

        foreach ($result as $record) {
            $options['ajax_location'][$record->id.'|location_1'] = t('Ajax Location 1 (@field_name)', array('@field_name' => $record->field_name));
            $options['ajax_location'][$record->id.'|location_2'] = t('Ajax Location 2 (@field_name)', array('@field_name' => $record->field_name));
            $options['ajax_location'][$record->id.'|location_3'] = t('Ajax Location 3 (@field_name)', array('@field_name' => $record->field_name));
            $options['ajax_location'][$record->id.'|location_4'] = t('Ajax Location 4 (@field_name)', array('@field_name' => $record->field_name));
        }

        return $options;
    }




    /**
     * Implementation of hook_user_import_form_fieldsets().
     */
    function ajax_location_user_import_form_fieldset($import, $collapsed) {
      /*$form = array();
      user_user_import_edit_roles_fields($form, $import, $collapsed);
      user_user_import_edit_email_fields($form, $import, $collapsed);

      return $form;*/
    }

    /**
     * Implementation of hook_user_import_form_update_user().
     */
    function ajax_location_user_import_form_update_user() {

        $result = db_query("SELECT * FROM {field_config} WHERE module = :module", array(
            ':module' => 'ajax_location',
        ));

        foreach ($result as $record) {
            $form[$record->id.'|location_1'] = array('title' => t('Ajax Location 1 (@field_name)', array('@field_name' => $record->field_name)), 'description' => t('Affected: Ajax Location 1.'), 'exclude_add' => TRUE);
            $form[$record->id.'|location_2'] = array('title' => t('Ajax Location 2 (@field_name)', array('@field_name' => $record->field_name)), 'description' => t('Affected: Ajax Location 2.'), 'exclude_add' => TRUE);
            $form[$record->id.'|location_3'] = array('title' => t('Ajax Location 3 (@field_name)', array('@field_name' => $record->field_name)), 'description' => t('Affected: Ajax Location 3.'), 'exclude_add' => TRUE);
            $form[$record->id.'|location_4'] = array('title' => t('Ajax Location 4 (@field_name)', array('@field_name' => $record->field_name)), 'description' => t('Affected: Ajax Location 4.'), 'exclude_add' => TRUE);


        }
      return $form;
    }

    /**
     * Implementation of hook_user_import_data().
     */
    function ajax_location_user_import_data($settings, $update_setting, $column_settings, $module, $field_id, $data, $column_id) {
      if ($module != 'ajax_location') return;
      $value = trim($data[$column_id]);
      return $value;
    }

    /**
     * Implementation of hook_user_import_pre_save().
     */
    function ajax_location_user_import_pre_save($settings, $account, $fields, $errors, $update_setting_per_module) {

      //$options['ajax_location']['location_1']

        $result = db_query("SELECT * FROM {field_config} WHERE module = :module", array(
            ':module' => 'ajax_location',
        ));

        
        foreach ($result as $record) {
            $account_add[$record->id.'|location_1'] = (isset($fields['ajax_location'][$record->id.'|location_1'])) ? $fields['ajax_location'][$record->id.'|location_1'][0] : '';
            $account_add[$record->id.'|location_2'] = (isset($fields['ajax_location'][$record->id.'|location_2'])) ? $fields['ajax_location'][$record->id.'|location_2'][0] : '';
            $account_add[$record->id.'|location_3'] = (isset($fields['ajax_location'][$record->id.'|location_3'])) ? $fields['ajax_location'][$record->id.'|location_3'][0] : '';
            $account_add[$record->id.'|location_4'] = (isset($fields['ajax_location'][$record->id.'|location_4'])) ? $fields['ajax_location'][$record->id.'|location_4'][0] : '';
        }

        return $account_add;
    }

    /**
     * Implementation of hook_user_import_after_save().
     */
    function ajax_location_user_import_after_save($settings, $account, $password, $fields, $updated, $update_setting_per_module) {
        /**
        * @todo change hook_user_import_after_save() so that all changes to data are returned and saved in one hit
        */


        $result = db_query("SELECT * FROM {field_config} WHERE module = :module", array(
            ':module' => 'ajax_location',
        ));

        foreach ($result as $record) {
            $location_arr = array();
            $location = '';
            if (isset($fields['ajax_location'])){
                if (isset($fields['ajax_location'][$record->id.'|location_1'])){
                    if(trim($fields['ajax_location'][$record->id.'|location_1'][0]) != ''){
                        $location_arr[] = $fields['ajax_location'][$record->id.'|location_1'][0];
                    }
                }
                if (isset($fields['ajax_location'][$record->id.'|location_2'])){
                    if(trim($fields['ajax_location'][$record->id.'|location_2'][0]) != ''){
                        $location_arr[] = $fields['ajax_location'][$record->id.'|location_2'][0];
                    }
                }
                if (isset($fields['ajax_location'][$record->id.'|location_3'])){
                    if(trim($fields['ajax_location'][$record->id.'|location_3'][0]) != ''){
                        $location_arr[] = $fields['ajax_location'][$record->id.'|location_3'][0];
                    }
                }
                if (isset($fields['ajax_location'][$record->id.'|location_4'])){
                    if(trim($fields['ajax_location'][$record->id.'|location_4'][0]) != ''){
                        $location_arr[] = $fields['ajax_location'][$record->id.'|location_4'][0];
                    }
                }
                
                $location = implode(', ',$location_arr);
            }


            if ($location != ''){
                $xml_location = _ajax_location_call($location);
                $xml = simplexml_load_string($xml_location);

                $latitude = "";
                $longitude = "";
                $city = "";
                $postcode = "";

                $uid = $account->uid;
                
                if(isset($xml->Result)){
                    if (isset($xml->Result->Latitude)){
                        $latitude = $xml->Result->Latitude;
                    }
                    if (isset($xml->Result->Longitude)){
                        $longitude = $xml->Result->Longitude;
                    }
                    if (isset($xml->Result->City)){
                        $city = $xml->Result->City;
                    }
                    if (trim($latitude) != '' && trim($longitude) != ''){
                        _ajax_location_save($record->id, $uid, $postcode, $city, $latitude, $longitude);
                    }
                }
            }

        }

        return;
    }

}
