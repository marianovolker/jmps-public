<?php
  require_once("AbsMenuButtons.php");
  require_once("Logger.php");

  /**
   * AlarmsConfigBackend
   *
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
  */
  class AlarmsConfigBackend extends AbsMenuButtons
  {
  
    /**
     * Constructor
     */
    function __construct()
    {
      parent::__construct();
  
        $this->marginsOfMainMenu = "top:25%; bottom:5%; left:20%; rigth:20%; width:60%; height:70%";
        $this->buttonsByRow = 2;
    }
    
    /**
     * check if there are alarms to execute before to build the view...
     */
    public function evaluateContext()
    {
      if( isset($_GET['add_new_alarm']) && isset($_GET['name_of_alarm']) )
      {
        $this->header();
    
        $symbolic_link = PATH_BACKEND_CMDS . '/create_new_alarm.sh';
        $target_file = readlink( $symbolic_link );
        $alarm_to_execute = 'sh ' . $target_file . ' ' . $_GET['add_new_alarm'] . ' ' . $_GET['name_of_alarm'];
    
        ob_start();
        passthru($alarm_to_execute, $this->resultExecutedCommandLine);
        $this->outputExecutedCommandLine = ob_get_contents();
        ob_end_clean();
    
        // Erase new line char ...
        $this->outputExecutedCommandLine = trim(preg_replace('/\s+/', ' ', $this->outputExecutedCommandLine));
    
        $log = new Logger("system_alarm_added.log");
        if( $this->resultExecutedCommandLine == 0 )
        {
          $log->log("success executing: " . $alarm_to_execute);
          $log->log("alarm added: [" . $this->outputExecutedCommandLine . "]");
        }
        else
        {
          $log->log("fail to execute: " . $alarm_to_execute);
          $this->outputExecutedCommandLine = "Fallo la creacion de la alarm...";
        }
    
        $this->body();
    
        if( $this->outputExecutedCommandLine != "" )
        {
          $log->log("result: [" . $this->outputExecutedCommandLine . "]");
    
          if( $this->resultExecutedCommandLine == 0 )
          {
            Site::pr("<script> AlarmItemConfig('".$this->outputExecutedCommandLine.".conf"."');\n </script>");
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
    
        $symbolic_link = PATH_BACKEND_CMDS . '/delete_alarm.sh';
        $target_file = readlink( $symbolic_link );
        $alarm_to_execute = 'sh ' . $target_file . ' ' . $_GET['item'];
    
        ob_start();
        passthru($alarm_to_execute, $this->resultExecutedCommandLine);
        $this->outputExecutedCommandLine = ob_get_contents();
        ob_end_clean();
    
        $log = new Logger("system_alarm_added.log");
        if( $this->resultExecutedCommandLine == 0 )
        {
          $this->setMainMenuButtons();
    
          $log->log("success executing: " . $alarm_to_execute);
          $log->log("alarm added: [" . $this->outputExecutedCommandLine . "]");
        }
        else
        {
          $log->log("fail to execute: " . $alarm_to_execute);
          $this->outputExecutedCommandLine = "Fallo la eliminacion de la alarm...";
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
      
      $alarms = Site::getListOfFiles(PATH_ALARMS_CONFIG, ".conf");
      asort( $alarms, SORT_REGULAR );
      
      $i = 0;
      foreach( $alarms as $alarm )
      {
        $item = explode( '.', $alarm, 2 );
        $this->arrayMainMenuButtons[ $i ] = "javascript:AlarmItemConfig('" . $alarm . "')" . $delim . HINT_BUTTON_ALARM_CONFIG . " [" . $item[0] . "]" . $delim . $item[0] . $delim . "green"; $i++;
      }
      
      $this->arrayMainMenuButtons[ $i ] = "javascript:AddAlarmItemConfig()"  . $delim . "Agregar una Alarma" . $delim . "Agregar+" . $delim . "yellow"; $i++;
      
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
      $this->propertiesForClassParameterMainMenu = "col-xs-2 col-md-2 col-lg-2 col-xl-2";
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
      $script .= "   parent.location.href = 'AlarmsSetup.php';";
      $script .= "}\n";
          
      $script .= "function Home()";
      $script .= "{";
      $script .= "   window.top.location.href = 'Refresh.php';";
      $script .= "}\n";
  
      $script .= "function AlarmItemConfig(alarm)";
      $script .= "{";
      $script .= "   parent.location.href = 'AlarmItemConfig.php?alarm_name_abm=' + alarm + '&show_delete_button=1&caller=AlarmsConfigBackend.php';";
      $script .= "}\n";
      
      $script .= "function AddAlarmItemConfig()";
      $script .= "{";
      $script .= "  var alarm = prompt('Seleccione (1) para una ALARMA LOCAL MANUAL, (2) para una ALARMA LOCAL PERIODICA, (3) para una ALARMA LOCAL PERIODICA CON NOTIFICACION, (4) para una ALARMA LOCAL PERIODICA QUE SE EJECUTE A UNA HORA, ";
      $script .=                        "(5) para una ALARMA REMOTA MANUAL, (6) para una ALARMA REMOTA PERIODICA, (7) para una ALARMA REMOTA PERIODICA CON NOTIFICACION, (8) para una ALARMA REMOTA PERIODICA QUE SE EJECUTE A UNA HORA\\n', '1');\n";
      
      $script .= "  if( alarm == null || alarm == '' || alarm < 1 || alarm > 8 )\n";
      $script .= "  {\n";
      $script .= "    myAlert('Tipo de alarma seleccionada NO valida, esta debe ser [1-8] ! ...');";
      $script .= "    return;\n";
      $script .= "  }\n";
      
      $script .= "  var name_alarm = prompt('Nombre de la alarma. Es aconsejable utilizar un nombre descriptivo de la alarma. Ej. \"chequear porton\".', '');\n";
      
      $script .= "  if( name_alarm == null || name_alarm == '' )\n";
      $script .= "  {\n";
      $script .= "    myAlert('El nombre de la alarma no es valido ! ...');";
      $script .= "    return;\n";
      $script .= "  }\n";
      
      $script .= "  parent.location.href = 'AlarmsConfigBackend.php?add_new_alarm=' + alarm + '&name_of_alarm=' + name_alarm;\n";
      $script .= "}\n";      
  
      $this->javascriptToInsert = $script;
    }
  }

  $alarmsConfigBackend = new AlarmsConfigBackend();
  $alarmsConfigBackend->evaluateContext();
?>
