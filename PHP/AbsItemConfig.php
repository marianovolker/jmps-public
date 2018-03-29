<?php

  require_once("EtcConfig.php");
  require_once("SystemConfig.php");


  
  /**
   * AbsItemConfig
   *
   * This class allows for parsing of configuration of the module security_module from jmps package.
   * For generic configuration ...
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  abstract class AbsItemConfig
  {
    /**
     * @var object $config is the object for system configuration
     */
    protected $systemConfig = null;
    
    /**
     * @var if is readonly
     */
    protected $readonly = false;    
    
    
    /**
     * @var need color pallete
     */
    protected $needColorPalette = true;    
    
    
    /**
     * @var view as log
     */
    protected $viewAsLog = false;    
    
    
    /**
     * @var maxLengthToShowForTheKey
     */
    protected $maxLengthToShowForTheKey = 55;    

    
    /**
     * constructor ...
     */
    function __construct()
    {
      $this->systemConfig  = new SystemConfig();
      $this->translateKeys = true;
      $this->showButtomDel = false;
      $this->caller        = "";
      
      $this->setMainActionName();
      $this->setPreviousPage();
      
      $filename = PATH_SITE_BASE_ABS . "config/key_mapping_for_configfiles.conf";
      touch($filename);
      $this->translationConfig = new ScriptConfig($filename, null);
    }
    
    /**
     * create this object...
     */
    public function evaluateContext()
    {
      $this->evaluateActions();
    }
    
    /**
     * translate keys at view...
     */
    public function setTranslateKeys( $translateKeys )
    {
      $this->translateKeys = $translateKeys;
    }

    
    
    
    
    /**
     * Store the information the configuration.
     */
    protected $translationConfig;    
    
    /**
     * Store the information the configuration.
     */
    protected $config;

    /**
     * Data member to store the entire content of config in the form as "[section][key] = value"
     */
    protected $internalStorageArray;
    
    /**
     * Post pattern for update action...
     */
    protected $postPatternUpdateAction;
    
    /**
     * Item config name...
     */
    protected $itemConfigName;
    
    /**
     * Show button Delete to erase this config item...
     */
    protected $showButtomDel;    
    
    /**
     * Caller that has been iniated the ItemConfigurationAction...
     */
    protected $caller;    

    /**
     * Main action for post...
     */
    protected $mainActionName;
    
    /**
     * Previous Page...
     */
    protected $previousPage;
    
    /**
     * Do not translate keys...
     */
    protected $translateKeys;    
    
    

    // protected methods ...
    
    // force that the child define this method.
    abstract protected function setMainActionName();
    
    // force that the child define this method.
    abstract protected function setPreviousPage();

    
    protected function writeHeader()
    {
      $cssFiles = array (
                          "users/assets/css/bootstrap.min.css",
                          "AbsItemConfig.css"
                        );
    
      $jsFiles  = array (
                          "libsJS/jquery.imagesloaded.min.js",
                          "libsJS/bootstrap.min.js",
                          "libsJS/jscolor.min.js",
                          "AbsItemConfig.js"
                        );
    
      Site::writeHeader("", $cssFiles, "", $jsFiles, "");
    }

    
    protected function evaluateActions()
    {
      if( isset( $_POST[$this->mainActionName] ) && $_POST[$this->mainActionName] == "add" )
      {
        $tempArray = array('new_key' => '');
        
        $this->internalStorageArray = array_merge ($this->internalStorageArray, $tempArray );
      }
      else if( isset( $_POST[$this->mainActionName] ) && $_POST[$this->mainActionName] == "del" )
      {
        if( isset( $_POST['key'] ) )
        {
          unset( $this->internalStorageArray[$_POST['key']] );
        }
      }
      else if( isset( $_POST[$this->mainActionName] ) && $_POST[$this->mainActionName] == "save" )
      {
        $tempArray = array();
        
        foreach( $this->internalStorageArray as $key => $data )
        {
          $key_new = str_replace(' ', '_', $key);
          
          $key_new = preg_replace('/[^A-Za-z0-9\_]/', '', $key_new);
          
          $tempArray[$key_new] = $data;
        }
        
        $this->internalStorageArray = $tempArray;
        
        $this->config->setValuesToArray($this->internalStorageArray);
        
        if( ! $this->config->save() )
        {
          die("ERROR: Saving to file.");
          return;
        }
      }
      
      $this->abm();
    }
    
    protected function buildToolbar()
    {
      Site::pr('<tr>' );
        Site::pr('<td align="center">');
          Site::pr('<div id="toolbar-buttons-container">');
            $i=0;
            $array[ $i ] = "javascript:Back('"  . $this->previousPage        . "')"   . ";" . HINT_BUTTON_BACK              . ";" . IMAGES_PATH . "return.png"; $i++;
            $array[ $i ] = "javascript:Home()"                                        . ";" . HINT_BUTTON_HOME              . ";" . IMAGES_PATH . "home.png"; $i++;
            
            if( ! $this->readonly )
            {
              $array[ $i ] = "javascript:Add ('"  . $this->mainActionName      . "')" . ";" . HINT_BUTTON_ADD               . ";" . IMAGES_PATH . "add.png"; $i++;
              $array[ $i ] = "javascript:Save('"  . $this->mainActionName      . "')" . ";" . HINT_BUTTON_SAVE              . ";" . IMAGES_PATH . "save.png"; $i++;
            }
            
            if( $this->viewAsLog )
            {
              $array[ $i ] = "javascript:Undo()"                                      . ";" . "Releer el archivo de log"    . ";" . IMAGES_PATH . "undo.png"; $i++;
            }
            else
            {
              $array[ $i ] = "javascript:Undo()"                                      . ";" . HINT_BUTTON_UNDO              . ";" . IMAGES_PATH . "undo.png"; $i++;
            }
            
            if( $this->showButtomDel )
            {
              $array[ $i ] = "javascript:DelItemConfig('".$this->caller."', '".$this->itemConfigName."')" . ";" . "Eliminar este item" . ";" . IMAGES_PATH . "del.png"; $i++;
            }
            
            Site::pr('<table>');
              Site::pr('<tr>');
              for( $i = 0; $i < sizeof($array); $i++ )
              {
                $item = explode( ';', $array[$i], 3 );
                Site::pr('<td>');
                  Site::pr('<a href="'. $item[0] .'">');
                    Site::pr('<div class="image-button-wrapper">');
                      Site::pr('<img class="image" id="'.$item[0].'" title="'.$item[1].'" src="'.$item[2].'">');
                    Site::pr('</div>');
                  Site::pr('</a>');
                Site::pr('</td>');
              }
              Site::pr('<tr>');
            Site::pr('</table>');
          Site::pr('</div>');
        Site::pr('</td>');
      Site::pr('</tr>' );
    }
    
    protected function findKeyInArrayTraduction( $key )
    {
      // Erase internals keys from the internal array when are duplicated keys.
      ScriptConfig::extractInternalKey($key);
      EtcConfig::extractInternalKey($key);
      FSConfig::extractInternalKey($key);
      
      $value = $key;

      if( $this->translateKeys && $this->systemConfig->getTranslateKeyFromConfigFiles() == "1" )
      {
        if( isset($this->translationConfig->getValuesAsArray()[$key]) )
        {
          $value = $this->translationConfig->getValuesAsArray()[$key];
        }
      }

      return $value;
    }
    
    protected function buildConfigView()
    {
      Site::pr('<tr>' );
        Site::pr('<td align="center">');
        
            Site::pr('<div id="show-config-title-container">');
              Site::pr('<pr> ' . $this->itemConfigName . ' </pr>');
            Site::pr('</div>');
            
            Site::pr('<div id="show-config-items-container">');
              Site::pr('<form id="update-form" method="post">');
                Site::pr('<input type="hidden" name="' . $this->postPatternUpdateAction . '" value="' . $this->postPatternUpdateAction . '">');
                Site::pr('<input type="hidden" name="' . $this->postPatternUpdateAction . '_entity" value="' . $this->config->getEntity() . '">');
                Site::pr('<table border="2" align="center" width="100%" height="100%">');
                  foreach( $this->internalStorageArray as $key => $value )
                  {
                    Site::pr('<tr class="record_wrapper">');
                    
                      if( $this->viewAsLog && strlen($value) <= 0 )
                      {
                        continue;
                      }
                    
                      if( ! $this->viewAsLog && $key == "new_key" )
                      {
                        $new_key = $this->postPatternUpdateAction . '_new_key';
                        $title_new_key = "El nombre de la nueva clave sin espacios ni caracteres especiales.";
                        $value = "ingrese el valor para la nueva clave";
                        Site::pr('<td class="class-field-1">');
                          Site::pr('<input id="class-field-new-key" class="class-field-new-key" title="' . $title_new_key . '" type="text" name="' . $new_key . '" value="ingrese la nueva clave, sin espacios">');
                        Site::pr('</td>');
                        Site::pr('<script>thereIsNewKey = true;</script>');
                      }
                      else
                      {
                        Site::pr('<td>');
                          if( $this->viewAsLog ) Site::pr('<script>viewAsLog = true;</script>');
                          $keyToShow = $this->findKeyInArrayTraduction($key);
                          if( strlen($keyToShow) > $this->maxLengthToShowForTheKey )
                          {
                            $keyToShow = substr($keyToShow, 0, $this->maxLengthToShowForTheKey) . " ...";
                          }
                          Site::pr('<pr class="class-field-1" title="' . $this->findKeyInArrayTraduction($key) . '">' . $keyToShow . '</pr>');
                        Site::pr('</td>');
                      }
                      
                      Site::pr('<td class="class-field-2">');
                        $name_key = $this->postPatternUpdateAction . ',section=none,' . $key;
                        $color = ( ($this->needColorPalette) && (preg_match('/color/', strtolower($key))) )? "jscolor":"";
                        if( $this->readonly )
                        {
                          Site::pr('<pr class="class-field-2-log" title="' . $value . '">' . $value . '</pr>');
                        }
                        else
                        {
                          Site::pr('<input class="class-field-2-input '.$color.'" title="El valor de la clave." type="text" name="' . $name_key . '" value="' . $value . '">');
                        }
                      Site::pr('</td>');
                      
                      if( ! $this->readonly )
                      {
                        Site::pr('<td class="class-field-Image-Del">');
                          Site::pr("<a href=javascript:Del('" . $key . "','" . $this->mainActionName . "');>");
                            Site::pr('<img class="image class-field-Image-Del" title="'.HINT_BUTTON_DEL.' '.$key.'" src="'.IMAGES_PATH.'del.png">');
                          Site::pr('</a>');
                        Site::pr('</td>');
                      }
                      
                    Site::pr('</tr>');
                  }
                  if( $this->viewAsLog ) Site::pr('<tr><td><input id="view_as_log_at_end_of_file"></td></tr>');
                Site::pr('</table>');
              Site::pr('</form>');
            Site::pr('</div>');
            
        Site::pr('</td>');
      Site::pr('</tr>' );
    }
    
    /**
     * abm
     * Build an abm form to do changes.
     *
     * @access public
     * @return none
     */
    protected function abm()
    {
      $this->writeHeader();
        Site::pr('<body style="background-color:#' . $this->systemConfig->getBGColorConfigMenu() . ';">');
          Site::pr('<div id="entire-container">');
            Site::pr('<table border="1">');
              $this->buildToolbar();
              $this->buildConfigView();
            Site::pr('</table>');
          Site::pr('</div>');
          Site::pr('<script>adjustSizeOfConponents();</script>');
      Site::writeTrailer();
    }
  }
?>


