<?php
  require_once("AbsMenuButtons.php");
  require_once("Logger.php");
  
  /**
   * ActionsConfigBackend
   *
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
  */
  class ActionsConfigBackend extends AbsMenuButtons
  {
  
    /**
     * Constructor
     */
    function __construct()
    {
      parent::__construct();
  
      $this->marginsOfMainMenu = "top:25%; bottom:5%; left:20%; rigth:20%; width:60%; height:70%";
      $this->buttonsByRow = 4;
    }
  
    /**
     * check if there are actions to execute before to build the view...
     */
    public function evaluateContext()
    {
      if( isset($_GET['add_new_action']) && isset($_GET['name_of_action']) )
      {
        $this->header();
        
        $symbolic_link = PATH_BACKEND_CMDS . '/create_new_action.sh';
        $target_file = readlink( $symbolic_link );
        $action_to_execute = 'sh ' . $target_file . ' ' . $_GET['add_new_action'] . ' ' . $_GET['name_of_action'];
    
        ob_start();
        passthru($action_to_execute, $this->resultExecutedCommandLine);
        $this->outputExecutedCommandLine = ob_get_contents();
        ob_end_clean();

        // Erase new line char ...
        $this->outputExecutedCommandLine = trim(preg_replace('/\s+/', ' ', $this->outputExecutedCommandLine));
    
        $log = new Logger("system_action_added.log");
        if( $this->resultExecutedCommandLine == 0 )
        {
          $log->log("success executing: " . $action_to_execute);
          $log->log("action added: [" . $this->outputExecutedCommandLine . "]");
        }
        else
        {
          $log->log("fail to execute: " . $action_to_execute);
          $this->outputExecutedCommandLine = "Fallo la creacion de la action...";
        }

        $this->body();

        if( $this->outputExecutedCommandLine != "" )
        {
          $log->log("result: [" . $this->outputExecutedCommandLine . "]");
          
          if( $this->resultExecutedCommandLine == 0 )
          {
            Site::pr("<script> ActionItemConfig('".$this->outputExecutedCommandLine.".conf"."');\n </script>");
          }
          else
          {
            Site::pr("<script> myAlert('".$this->outputExecutedCommandLine."');\n </script>");
          }
        }

        $this->trailer();        
      }
      else if( isset($_GET['action']) && $_GET['action'] == 'delete_item' )
      {
        $this->header();

        $symbolic_link = PATH_BACKEND_CMDS . '/delete_action.sh';
        $target_file = readlink( $symbolic_link );
        $action_to_execute = 'sh ' . $target_file . ' ' . $_GET['item'];
      
        ob_start();
        passthru($action_to_execute, $this->resultExecutedCommandLine);
        $this->outputExecutedCommandLine = ob_get_contents();
        ob_end_clean();
      
        $log = new Logger("system_action_added.log");
        if( $this->resultExecutedCommandLine == 0 )
        {
          $this->setMainMenuButtons();

          $log->log("success executing: " . $action_to_execute);
          $log->log("action added: [" . $this->outputExecutedCommandLine . "]");
        }
        else
        {
          $log->log("fail to execute: " . $action_to_execute);
          $this->outputExecutedCommandLine = "Fallo la eliminacion de la action...";
        }

        $this->body();
        
        if( $this->outputExecutedCommandLine != "" )
        {
          $this->outputExecutedCommandLine = trim(preg_replace('/\s+/', ' ', $this->outputExecutedCommandLine));
          $log->log("result: [" . $this->outputExecutedCommandLine . "]");
      
          if( $this->resultExecutedCommandLine == 0 )
          {
            Site::pr("<script> myAlert('"."Se ha eliminado ".$this->outputExecutedCommandLine."');\n </script>");
          }
        }

        $this->trailer();
      }
      else
      {
        parent::evaluateContext();
      }
    }
    
    
  
    // protected methods ...
    
    // Result of the last executed commandline ...
    protected $resultExecutedCommandLine;
    
    // Output of the last executed commandline ...
    protected $outputExecutedCommandLine;

    
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
      $this->arrayMainMenuButtons = array();

      $delim = $this->delimiterForFieldSeparator;
      
      $actions = Site::getListOfFiles(PATH_ACTIONS_CONFIG, ".conf");
      asort( $actions, SORT_REGULAR );
      
      $i = 0;
      foreach( $actions as $action )
      {
        $item = explode( '.', $action, 2 );
        $this->arrayMainMenuButtons[ $i ] = "javascript:ActionItemConfig('" . $action . "')" . $delim . HINT_BUTTON_ACTION_CONFIG . " [" . $item[0] . "]" . $delim . $item[0] . $delim . "yellow"; $i++;
      }
      
      $this->arrayMainMenuButtons[ $i ] = "javascript:AddActionItemConfig()"  . $delim . "Agregar una Accion" . $delim . "Agregar+" . $delim . "green"; $i++;
      
      $quantityOfButtons=sizeof($this->arrayMainMenuButtons);
      
      if( $quantityOfButtons <= 9 )
      {
        $this->fontSize = '1.3vw';
      }
      else if( $quantityOfButtons <= 15 )
      {
        $this->fontSize = '1.1vw';
      }
      else if( $quantityOfButtons <= 20 )
      {
        $this->fontSize = '0.8vw';
      }
      else
      {
        $this->fontSize = '0.7vw';
      }      
    }
  
    protected function setPropertiesForClassParameterToolbar()
    {
      $this->propertiesForClassParameterToolbar = "";
    }
  
    protected function setPropertiesForClassParameterMainMenu()
    {
      $this->propertiesForClassParameterMainMenu = "col-xs-4 col-md-4 col-lg-4 col-xl-4";
    }
  
    protected function setJavascript()
    {
      $script  = "";
      
      $script .= "toastr.options = {\"debug\": false, \"positionClass\": \"toast-bottom-center\", \"onclick\": null, \"fadeIn\": 300, \"fadeOut\": 1000, \"timeOut\": 3000, \"extendedTimeOut\": 5500};\n";
      
      $script .= "function myAlert(text)\n";
      $script .= "{\n";
      $script .=    (Site::isMobileBrowser())? ("alert(text);\n"):("toastr.info(text);\n");
      $script .= "}\n";

      $script .= "function Back()";
      $script .= "{";
      $script .= "   parent.location.href = 'ActionsSetup.php';";
      $script .= "}\n";
          
      $script .= "function Home()";
      $script .= "{";
      $script .= "   window.top.location.href = 'Refresh.php';";
      $script .= "}\n";
  
      $script .= "function ActionItemConfig(action)";
      $script .= "{";
      $script .= "   parent.location.href = 'ActionItemConfig.php?action_name_abm=' + action + '&show_delete_button=1&caller=ActionsConfigBackend.php';";
      $script .= "}\n";
      
      $script .= "function AddActionItemConfig()";
      $script .= "{";
      $script .= "  var action = prompt('Seleccione (1) para una ACCION LOCAL MANUAL, (2) para una ACCION LOCAL PERIODICA, (3) para una ACCION LOCAL PERIODICA QUE SE EJECUTE A UNA HORA, ";
      $script .=                        "(4) para una ACCION REMOTA MANUAL, (5) para una ACCION REMOTA PERIODICA, (6) para una ACCION REMOTA PERIODICA QUE SE EJECUTE A UNA HORA\\n', '1');\n";

      $script .= "  if( action == null || action == '' || action < 1 || action > 6 )\n";
      $script .= "  {\n";
      $script .= "    myAlert('Tipo de accion seleccionada NO valida, esta debe ser [1-6] ! ...');";
      $script .= "    return;\n";
      $script .= "  }\n";

      $script .= "  var name_action = prompt('Nombre de la accion. Es aconsejable utilizar un nombre descriptivo de la accion. Ej. \"abrir el porton\".', '');\n";

      $script .= "  if( name_action == null || name_action == '' )\n";
      $script .= "  {\n";
      $script .= "    myAlert('El nombre de la accion no es valido ! ...');";
      $script .= "    return;\n";
      $script .= "  }\n";

      $script .= "  parent.location.href = 'ActionsConfigBackend.php?add_new_action=' + action + '&name_of_action=' + name_action;\n";
      $script .= "}\n";      

      $this->javascriptToInsert = $script;
    }
  }
  
  $actionsConfigBackend = new ActionsConfigBackend();
  $actionsConfigBackend->evaluateContext();
?>
