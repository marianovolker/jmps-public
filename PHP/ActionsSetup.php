<?php

  require_once("AbsMenuButtons.php");
  
  /**
   * ActionsSetup
   *
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  class ActionsSetup extends AbsMenuButtons
  {
    
    /**
     * Constructor
     */
    function __construct()
    {
      parent::__construct();
      
      $this->marginsOfMainMenu = "top:30%; bottom:20%; left:25%; rigth:25%; width:50%; height:50%";
      $this->buttonsByRow = 2;
      $this->typesOfButtons = array("btn-success", "btn-danger", "btn-primary", "btn-info", "btn-warning");
    }

    
    // protected methods ...

    protected function setDelimiterForFieldSeparator()
    {
      $this->delimiterForFieldSeparator = ";";
    }
    
    protected function setToolbarButtons()
    {
      $delim = $this->delimiterForFieldSeparator;
      
      $this->arrayToolbarButtons [ 0 ] = "javascript:Back()" . $delim . HINT_BUTTON_BACK  . $delim . IMAGES_PATH . "return.png";
      $this->arrayToolbarButtons [ 1 ] = "javascript:Home()" . $delim . HINT_BUTTON_HOME  . $delim . IMAGES_PATH . "home.png";
    }
    
    protected function setMainMenuButtons()
    {
      $delim = $this->delimiterForFieldSeparator;
      
      $this->arrayMainMenuButtons[ 0 ] = "javascript:ActionsFrontendConfig()" . $delim . HINT_BUTTON_ACTIONS_FRONTEND_CONFIG . $delim . "Acciones Frontend" ;
      $this->arrayMainMenuButtons[ 1 ] = "javascript:ActionsBackendConfig()"  . $delim . HINT_BUTTON_ACTIONS_BACKEND_CONFIG  . $delim . "Acciones Backend"  ;
    }
    
    protected function setPropertiesForClassParameterToolbar()
    {
      $this->propertiesForClassParameterToolbar = "";
    }

    protected function setPropertiesForClassParameterMainMenu()
    {
      $this->propertiesForClassParameterMainMenu = "col-xs-2 col-md-2 col-lg-2 col-xl-2";
    }
    
    protected function setJavascript()
    {
      $script  = "";
      
      $script .= "function Back()";
      $script .= "{";
      $script .= "   parent.location.href = 'Setup.php';";
      $script .= "}\n";
      
      $script .= "function Home()";
      $script .= "{";
      $script .= "   window.top.location.href = 'Refresh.php';";
      $script .= "}\n";
      
      $script .= "function ActionsFrontendConfig()";
      $script .= "{";
      $script .= "   parent.location.href = 'ActionsConfigFrontend.php?actions_config_action_abm=1';";
      $script .= "}\n";
      
      $script .= "function ActionsBackendConfig()";
      $script .= "{";
      $script .= "  parent.location.href = 'ActionsConfigBackend.php?actions_config_action_abm=1';";
      $script .= "}\n";
      
      $this->javascriptToInsert = $script;
    }
  }
  
  $actionsSetup = new ActionsSetup();
  $actionsSetup->evaluateContext();
?>

