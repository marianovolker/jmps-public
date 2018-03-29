<?php

  require_once("Site.php");
  require_once("ActionsConfigFrontend.php");
  require_once("AbsMenuButtons.php");
  require_once("Logger.php");
  require_once("SystemConfig.php");
  
  require_once(INSTALLATION_PATH.'config/config.php');
  require_once(INSTALLATION_PATH.'translations/es.php');
  require_once(INSTALLATION_PATH.'classes/Login.php');
  
  /**
   * ActionsBar
   * This class allows us to show all actions availables configured with ActionsConfig of the module security_module from jmps package.
   * To show actions configured ...
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
  */
  class ActionsBar extends AbsMenuButtons
  {

    /**
     * Store the information the configuration.
     */
    protected $actionsConfig;    
  
    /**
     * Constructor
     */
    function __construct()
    {
      $systemConfig = new SystemConfig();
      $this->actionsConfig = new ActionsConfigFrontend();
      $this->cssStyleToInsert = ".btn-lgx {color : #" . $systemConfig->getFontColorMainActionBar() . " ;}";
      parent::__construct();
      $this->backgroundColor = $systemConfig->getBGColorActionBar();
      $this->marginsOfMainMenu = "top:7%; bottom:0%; left:2%; rigth:2%; width:96%; height:92%";
    }
  
  
    
    // protected methods ...
    
    protected function evaluateActions()
    {
      if( isset( $_GET['actions_action'] ) && $this->actionsConfig->thisActionIsConfigured($_GET['actions_action']) )
      {
        $symbolic_link = PATH_ACTIONS . '/' . $this->actionsConfig->getScriptAction($_GET['actions_action']);
        $target_file = readlink( $symbolic_link );
        $action_to_execute = $target_file . ' ' . $this->actionsConfig->getScriptParameter($_GET['actions_action']);
        passthru($action_to_execute, $result);
        $name = $this->actionsConfig->getTextAction($_GET['actions_action']);
        $log = new Logger("actions_executed.log");
        if( $result == 0 )
        {
          $log->log(" Accion: [ " . $name . " ] ");
        }
        else
        {
          $log->log(" Fallo la ejecucion de : [" . $name . "] [" . $action_to_execute . "] ");
        }
      }

      $this->show();
    }

    protected function setDelimiterForFieldSeparator()
    {
      $this->delimiterForFieldSeparator = ";";
    }
  
    protected function setToolbarButtons()
    {
    }
  
    protected function setMainMenuButtons()
    {
      $delim = $this->delimiterForFieldSeparator;
      $actionsArray = $this->actionsConfig->getActionsAsArray();
      
      $index = 0;
      $login = new Login();

      foreach( $actionsArray as $action => $data )
      {
        $script  = PATH_ACTIONS . '/' . (isset($actionsArray[$action]['script'])? $actionsArray[$action]['script'] : "");
        $text    = isset($actionsArray[$action]['text'])  ? $actionsArray[$action]['text']   : "";
        $hint    = isset($actionsArray[$action]['hint'])  ? $actionsArray[$action]['hint']   : "";
        $ask     = isset($actionsArray[$action]['ask'])   ? $actionsArray[$action]['ask']    : "";
        $param   = isset($actionsArray[$action]['param']) ? $actionsArray[$action]['param']  : "";
        
        // Check if the current user could execute this action.      
        if( !$login->isAllowedThisAction($text) )
        {
          continue;
        }
              
        $actionId = explode( '_', $action, 2 );
        if( is_executable($script) && is_file($script) )
        {
          if( strlen($ask) <= 0 )
          {
            $this->arrayMainMenuButtons[ $index ] = "javascript:executeAction('" . $actionId[1] . "', '" . $text . "')"  . $delim . $hint   . $delim . $text;
          }
          else
          {
            $this->arrayMainMenuButtons[ $index ] = "javascript:executeActionWithAskConfirmation('" . $ask . "', '" . $actionId[1] . "', '" . $text . "')"  . $delim . $hint   . $delim . $text;
          }
          $index++;
        }
      }

      $this->fontSize = '2vw';
      $this->buttonsByRow = sizeof($this->arrayMainMenuButtons);
    }
  
    protected function setPropertiesForClassParameterToolbar()
    {
      $this->propertiesForClassParameterToolbar = "";
    }
  
    protected function setPropertiesForClassParameterMainMenu()
    {
      $this->propertiesForClassParameterMainMenu = "";
    }
  
    protected function setJavascript()
    {
      $script  = "";
      
      $script .= "toastr.options = {\"debug\": false, \"positionClass\": \"toast-top-center\", \"onclick\": null, \"fadeIn\": 300, \"fadeOut\": 1000, \"timeOut\": 4000, \"extendedTimeOut\": 3500};\n";

      $script .= "function myAlert(text)\n";
      $script .= "{\n";
      $script .=    (Site::isMobileBrowser())? ("alert(text);\n"):("toastr.info(text);\n");
      $script .= "}\n";      
      
      $script .= "function executeAction(script, text)\n";
      $script .= "{\n";
      $script .= "   var action_link  = 'ActionsBar.php?actions_action=' + script;\n";
      $script .= "   $.get ( action_link, function(data, status) { } );\n";
      $script .= "   myAlert('Se ejecuto [' + text + '].');\n";
      $script .= "}\n";
          
      $script .= "function executeActionWithAskConfirmation(ask_before_execute, script, text)\n";
      $script .= "{\n";
      $script .= "   if( confirm(ask_before_execute) )\n";
      $script .= "   {\n";
      $script .= "     var action_link = 'ActionsBar.php?actions_action=' + script;\n";
      $script .= "     $.get ( action_link, function (data, status) {} );\n";
      $script .= "   }\n";
      $script .= "}\n";

      $this->javascriptToInsert = $script;
    }
  }

  $actionsBar = new ActionsBar();
  $actionsBar->evaluateContext();
?>

