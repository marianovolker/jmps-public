<?php

  require_once("IniConfig.php");
  require_once("DBConfig.php");
  require_once("SystemConfig.php");

  
  /**
   * ActionsConfigFrontend
   *
   * This class allows for parsing of configuration of the module security_module from jmps package.
   * For actions configuration ...
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  class ActionsConfigFrontend
  {
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
     * Constructor
     */
    function __construct()
    {
      $this->postPatternUpdateAction = get_class($this) . "_CONFIG_UPDATE";
      
      if( USE_DB_CONFIG )
      {
        $this->config = new DBConfig("jmps", "actions", "", "", $this->postPatternUpdateAction);
      }
      else
      {
        $filename = PATH_SITE_BASE_ABS . "config/actions_configuration.conf";
        touch($filename);
        $this->config = new IniConfig($filename, $this->postPatternUpdateAction, $read_with_sections = true);
      }
      
      $this->internalStorageArray = $this->config->getValuesAsArray();
    }
    
    /**
     * create this object...
     */
    public function create()
    {
      $this->evaluateActions();
    }
    
    /**
     * thisActionIsConfigured
     * Check if the action is configured.
     * 
     * @access public
     * @return true or false
     */
    public function thisActionIsConfigured($number)
    {
      return ( $this->config->getValue( 'action_' . $number, 'script', "not set" ) != "not set" )? (true):(false); 
    }
    
    /**
     * getQuantityOfActions
     * Get quantity of actions configured.
     *
     * @access public
     * @return number of actions configured
     */
    public function getQuantityOfActions()
    {
      return count(array_keys( $this->internalStorageArray ));
    }
    
    /**
     * getActionsAsArray
     * Get all configured actions.
     *
     * @access public
     * @return actions
     */
    public function getActionsAsArray()
    {
      ksort( $this->internalStorageArray, SORT_NATURAL );
      return $this->internalStorageArray;
    }
    
    /**
     * getScriptAction
     * Obtain the script to be executed for this action.
     *
     * @access public
     * @return true or false
     */
    public function getScriptAction($number)
    {
      return $this->config->getValue( 'action_' . $number, 'script', "" );
    }
    
    /**
     * getScriptParameter
     * Obtain the script parameter that will used to execute this action.
     *
     * @access public
     * @return true or false
     */
    public function getScriptParameter($number)
    {
      return $this->config->getValue( 'action_' . $number, 'param', "" );
    }
    
    /**
     * getTextAction
     * Obtain the name for this action.
     *
     * @access public
     * @return true or false
     */
    public function getTextAction($number)
    {
      return $this->config->getValue( 'action_' . $number, 'text', "" );
    }    
    
    
    /**
     * createAvailableActionsList
     * Create a list of the posible actions availables.
     *
     * @access public
     * @return A select tag element with a list of availables actions.
     */    
    public function createAvailableActionsList($name, $current, $class)
    {
      if( trim($current) == "" )
      {
        $current = "--Ninguna--";
      }
            
      $html = '<select title="'.$current.'" class="'.$class.'" name="'.$name.'">';
      
      $combinations = Site::generateCombinations($this->getListOfActions());
      
      Site::appendArrayIntoAnotherArray($combinations, count($combinations), array('--Ninguna--'));
      
      for( $i = 1; $i < count($combinations); $i++ )
      {
        $value = "";

        foreach ( $combinations[$i] as $combination )
        {
          if( strlen($combination) > 0 )
          {
            $value = ( strlen($value) > 0 )? ( $value . ", " . $combination ):( $combination );
          }
        }
        
        if( $value == $current )
        {
          $html.= "<option selected='selected' value='".$value."'>$i) ".$value."</option>";
        }
        else
        {
          $html.= "<option value='".$value."'>$i) ".$value."</option>";
        }
      }

      $html.= "</select>";
    
      Site::pr($html);
    }

    
    
    
    // protected methods ...
    
    
    
    
    protected function createActionsList($name, $current, $class)
    {
      $list = Site::getListOfExecutablesFiles(PATH_ACTIONS);
      
      $html = "";
      $html.= "<select class=\"".$class."\" name=".$name.">";
      
      foreach( $list as $value )
      {
        if( $value == $current )
        {
          $html.= "<option selected=\"selected\" value=".$value.">".$value."</option>";
        }
        else
        {
          $html.= "<option value=".$value.">".$value."</option>";
        }
      }
      
      $html.= "</select>";
      
      Site::pr($html);
    }
    
    protected function writeHeader()
    {
      $cssFiles = array (
                          "users/assets/css/bootstrap.min.css",
                          "ActionsConfigFrontend.css"
                        );
    
      $jsFiles  = array (
                          "libsJS/jquery.imagesloaded.min.js",
                          "ActionsConfigFrontend.js"
                        );
    
      Site::writeHeader("", $cssFiles, "", $jsFiles, "");
    }    

    protected function evaluateActions()
    {
      if( isset( $_POST['actions_config_action'] ) && $_POST['actions_config_action'] == "add" )
      {
        if( $this->getQuantityOfActions() == 0 )
        {
          $this->internalStorageArray['action_1']['script'] = "";
          $this->internalStorageArray['action_1']['text'  ] = "";
          $this->internalStorageArray['action_1']['hint'  ] = "";
          $this->internalStorageArray['action_1']['ask'   ] = "";
          $this->internalStorageArray['action_1']['param' ] = "";
        }
        else
        {
          $keys = array_keys( $this->internalStorageArray );
          $lastKey = $keys[( count( $keys ) - 1 )];
          $arrLastKey = explode( '_', $keys[( count( $keys ) - 1 )], 2 );
          $newKey = $arrLastKey[0] . '_' . ( ( (int) $arrLastKey[1] ) + 1 );
          $tempArray = $this->internalStorageArray[$lastKey];
          Site::addArrayAtTopOfAnotherArray( $this->internalStorageArray, $newKey, $tempArray );
        }
        $this->abm();
        return;
      }
      else if( isset( $_POST['actions_config_action'] ) && $_POST['actions_config_action'] == "del" )
      {
        if( isset( $_POST['key'] ) )
        {
          unset( $this->internalStorageArray[$_POST['key']] );
        }
      }
      else if( isset( $_POST['actions_config_action'] ) && $_POST['actions_config_action'] == "save" )
      {
        $this->config->setValuesToArray($this->internalStorageArray);
        
        if( ! $this->config->save() )
        {
          die("ERROR: Saving to file.");
          return;
        }
      }
      
      if( isset( $_GET['actions_config_action_abm'] ) )
      {
        ksort( $this->internalStorageArray, SORT_NATURAL );
        $this->abm();
      }
    }
    
    protected function getListOfActions()
    {
      $list = array();
      
      $i = 0;
      
      foreach( $this->internalStorageArray as $section => $data )
      {
        $actionText = isset($this->internalStorageArray[$section]['text'])? $this->internalStorageArray[$section]['text'] : "";
        
        $list[$i] = $actionText;
        
        $i++;
      }
      
      return $list;
    }    
    
    protected function buildToolbar()
    {
      Site::pr('<td align="center">');
        Site::pr('<div id="toolbar-buttons-container">');
          $array[ 0 ] = "javascript:Back()" . ";" . HINT_BUTTON_BACK  . ";" . IMAGES_PATH . "return.png";
          $array[ 1 ] = "javascript:Home()" . ";" . HINT_BUTTON_HOME  . ";" . IMAGES_PATH . "home.png";
          $array[ 2 ] = "javascript:Add ()" . ";" . HINT_BUTTON_ADD   . ";" . IMAGES_PATH . "add.png";
          $array[ 3 ] = "javascript:Save()" . ";" . HINT_BUTTON_SAVE  . ";" . IMAGES_PATH . "save.png";
          $array[ 4 ] = "javascript:Undo()" . ";" . HINT_BUTTON_UNDO  . ";" . IMAGES_PATH . "undo.png";
           
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
    }
    
    protected function buildConfigView()
    {
      Site::pr('<td align="center">');
        Site::pr('<div id="show-config-container">');
          Site::pr('<form id="update-form" method="post">');
            Site::pr('<input type="hidden" name="' . $this->postPatternUpdateAction . '" value="' . $this->postPatternUpdateAction . '">');
            Site::pr('<input type="hidden" name="' . $this->postPatternUpdateAction . '_entity" value="' . $this->config->getEntity() . '">');
            Site::pr('<table border="2" align="center" width="100%" height="100%">');
            
              foreach( $this->internalStorageArray as $section => $data )
              {
                $script      = isset($this->internalStorageArray[$section]['script'])? $this->internalStorageArray[$section]['script'] : "";
                $name_script = $this->postPatternUpdateAction . ',' . $section . ',script';

                $text        = isset($this->internalStorageArray[$section]['text'])? $this->internalStorageArray[$section]['text'] : "";
                $name_text   = $this->postPatternUpdateAction . ',' . $section . ',text';
                
                $hint        = isset($this->internalStorageArray[$section]['hint'])? $this->internalStorageArray[$section]['hint'] : "";
                $name_hint   = $this->postPatternUpdateAction . ',' . $section . ',hint';
                
                $ask         = isset($this->internalStorageArray[$section]['ask'])? $this->internalStorageArray[$section]['ask'] : "";
                $name_ask    = $this->postPatternUpdateAction . ',' . $section . ',ask';
                
                $param       = isset($this->internalStorageArray[$section]['param'])? $this->internalStorageArray[$section]['param'] : "";
                $name_param  = $this->postPatternUpdateAction . ',' . $section . ',param';
                
                $seccionId = explode( '_', $section, 2 );
              
                Site::pr('<tr class="record_wrapper">');
                  Site::pr('<td class="class-field-Section-Id" rowspan="5">'.$seccionId[1].'</td>');
                  Site::pr('<td class="class-field-2">script</td>');
                  Site::pr('<td class="class-field-3">');
                    $this->createActionsList($name_script, $script, "class-field-3-select");
                  Site::pr('</td>');
                  Site::pr('<td class="class-field-Image-Del" rowspan="5">');
                    Site::pr('<a href=javascript:Del("'.$section.'")>');
                      Site::pr('<img class="image" title="'.HINT_BUTTON_DEL.' '.$section.'" src="'.IMAGES_PATH.'del.png">');
                    Site::pr('</a>');
                  Site::pr('</td>');
                Site::pr('</tr>');
                
                Site::pr('<tr class="record_wrapper">');
                  Site::pr('<td class="class-field-2">Nombre - texto del boton</td>');
                  Site::pr('<td class="class-field-3">');
                    Site::pr('<input class="class-field-3" title="Sera el texto que aparecera dentro del boton. Tener en cuenta que debera cambiar los permisos a los usuarios para que vean esta accion." type="text" name="' . $name_text . '" value="' . $text . '">' );
                  Site::pr('</td>');
                Site::pr('</tr>');
                
                Site::pr('<tr class="record_wrapper">');
                  Site::pr('<td class="class-field-2">Texto explicativo</td>');
                  Site::pr('<td class="class-field-3">');
                    Site::pr('<input class="class-field-3" title="Sera el texto que aparece arriba del boton." type="text" name="' . $name_hint . '" value="' . $hint . '">' );
                  Site::pr('</td>');
                Site::pr('</tr>');
                
                Site::pr('<tr class="record_wrapper">');
                  Site::pr('<td class="class-field-2">Pregunta antes de ejecutar</td>');
                  Site::pr('<td class="class-field-3">');
                    Site::pr('<input class="class-field-3" title="Si se deja en blanco no se solicitara ninguna confirmacion antes de la ejecucion del script." type="text" name="' . $name_ask . '" value="' . $ask . '">' );
                  Site::pr('</td>');
                Site::pr('</tr>');
                
                Site::pr('<tr class="record_wrapper">');
                  Site::pr('<td class="class-field-2">Parametro del script</td>');
                  Site::pr('<td class="class-field-3">');
                    Site::pr('<input class="class-field-3" title="Sera el parametro a pasarle al script." type="text" name="' . $name_param . '" value="' . $param . '">' );
                  Site::pr('</td>');
                Site::pr('</tr>');
              
                Site::pr('<tr>');
                  Site::pr('<td colspan="5">&nbsp;</td>');
                Site::pr('</tr>');
              }
              
            Site::pr('</table>');
          Site::pr('</form>');
        Site::pr('</div>');
      Site::pr('</td>');
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
      
        Site::pr('<body style="background-color:#' . (new SystemConfig())->getBGColorConfigMenu(). ';">');
          Site::pr('<script>adjustSizeOfConponents();</script>');
          Site::pr('<div id="entire-container">');
            Site::pr('<table>');
              Site::pr('<tr>');
                $this->buildToolbar();
              Site::pr('</tr>');
              Site::pr('<tr>' );
                $this->buildConfigView();
              Site::pr('</tr>');
            Site::pr('</table>');
          Site::pr('</div>');

      Site::writeTrailer();
    }
  }
  
  $actionsConfig = new ActionsConfigFrontend();
  $actionsConfig->create();
?>

