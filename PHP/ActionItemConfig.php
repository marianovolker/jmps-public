<?php

  require_once("AbsItemConfig.php");
  require_once("ScriptConfig.php");


  
  /**
   * ActionItemConfig
   *
   * This class allows for parsing of configuration of the module security_module from jmps package.
   * For actions configuration ...
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  class ActionItemConfig extends AbsItemConfig
  {
    /**
     * Constructor
     */
    function __construct()
    {
      parent::__construct();
      
      $this->postPatternUpdateAction = get_class($this) . "_CONFIG_UPDATE";
      
      if( isset( $_GET['action_name_abm'] ) )
      {
        $this->itemConfigName = $_GET['action_name_abm'];
        $filename = PATH_ACTIONS_CONFIG . "/" . $_GET['action_name_abm'];
        $this->config = new ScriptConfig($filename, $this->postPatternUpdateAction);
      }
      else 
      {
        Site::writeHeaderWithoutSession();
          Site::pr('<body><img width="100%" height="100%" align="center" id="imgDisplay" src="' . IMAGES_PATH . 'fondo_jmps.jpg">');
        Site::writeTrailer();

        die("ERROR cargando el archivo de configuracion de actions [" . $_GET['action_name_abm'] . "].");
      }
      
      if( isset($_GET['show_delete_button']) && isset($_GET['caller']) )
      {
        $this->showButtomDel = true;
        $this->caller = $_GET['caller'];
      }      

      $this->internalStorageArray = $this->config->getValuesAsArray();
    }
    
    protected function setMainActionName()
    {
      $this->mainActionName = "action_post_action_form";
    }
    
    protected function setPreviousPage()
    {
      $this->previousPage = "ActionsConfigBackend.php";
    }
  }
  
  $actionItemConfig = new ActionItemConfig();
  $actionItemConfig->evaluateContext();
?>
