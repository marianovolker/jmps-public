<?php

  require_once("AbsMenuButtons.php");
  require_once("EtcConfig.php");
  require_once("Logger.php");
  
  /**
   * DevicesBackendConfig
   *
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
  */
  class DevicesBackendConfig extends AbsMenuButtons
  {
  
    /**
     * Constructor
     */
    function __construct()
    {
      parent::__construct();
  
      $this->marginsOfMainMenu = "top:25%; bottom:5%; left:20%; rigth:20%; width:60%; height:70%";
      $this->buttonsByRow = 6;
    }
  
    /**
     * check if there are actions to execute before to build the view...
     */
    public function evaluateContext()
    {
      parent::evaluateContext();
      
      if( isset($_GET['exec_start_devices']) || isset($_GET['exec_stop_devices']) )
      {
        $symbolic_link = PATH_BACKEND_CMDS . '/webcam-server';
        $target_file = readlink( $symbolic_link );
        $action_to_execute = (isset($_GET['exec_start_devices'])? ($target_file . ' start'):($target_file . ' stop'));

        ob_start();
        passthru($action_to_execute, $result);
        $outputExecutedCommandLine = ob_get_contents();
        ob_end_clean();

        $log = new Logger("webcam_server.log");
        $log->log("executed action: " . $action_to_execute);
        if( $result == 0 )
        {
          $log->log("success executing: " . $action_to_execute);
        }
        else
        {
          $log->log("fail to execute: " . $action_to_execute);
        }
        
        if( $outputExecutedCommandLine != "" )
        {
          $outputExecutedCommandLine = trim(preg_replace('/\s+/', ' ', $outputExecutedCommandLine));
          Site::pr("<script> myAlert('".$outputExecutedCommandLine."');\n </script>");
          $log->log("result: [" . $outputExecutedCommandLine . "]");
        }
      }      
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
    
    protected function array_find($needle, array $haystack)
    {
      foreach ( $haystack as $key => $value )
      {
        if ( stripos($value, $needle) !== false )
        {
          return true;
        }
      }
      return false;
    }    
  
    protected function setMainMenuButtons()
    {
      $delim = $this->delimiterForFieldSeparator;
      
      $path  = PATH_SITE_BASE_ABS . "config/video/";
  
      $driverCfgFilename = $path . "bttv.conf";
      $motionCfgFilename = $path . "motion.conf";
  
      $motionConfig = new EtcConfig($motionCfgFilename, null);
      
      $threads = Site::getListOfFiles($path, "thread");
      
      $sorted_array = array();
      foreach( $threads as $thread )
      {
        if( $this->array_find( $thread, $motionConfig->getValuesAsArray() ) )
        {
          $key = filter_var($thread, FILTER_SANITIZE_NUMBER_INT);
          
          if( strlen($key) > 0 )
          {
            if( isset($sorted_array[$key]) )
            {
              $key = $key . "_" . rand ( 1 , 10000 );
            }
            $sorted_array[$key] = $thread;
          }
        }
      }
  
      asort( $sorted_array, SORT_ASC );
      
      $i = 0;
      
      $this->arrayMainMenuButtons[ $i ] = "javascript:VideoItemConfig('"  . urlencode(base64_encode($driverCfgFilename)) . "')"                     . $delim . HINT_BUTTON_DEVICE_CONFIG . " [" . "bttv.conf"   . "]" . $delim . "Config Driver"        . $delim . "blue"  ; $i++;
      $this->arrayMainMenuButtons[ $i ] = "javascript:VideoItemConfig('"  . urlencode(base64_encode($motionCfgFilename)) . "')"                     . $delim . HINT_BUTTON_DEVICE_CONFIG . " [" . "motion.conf" . "]" . $delim . "Config Video"         . $delim . "blue"  ; $i++;
      $this->arrayMainMenuButtons[ $i ] = "javascript:StopDevices()"                                                                                . $delim . "Parar la reproduccion"                                . $delim . "Stop Video"           . $delim . "blue"  ; $i++;      
      $this->arrayMainMenuButtons[ $i ] = "javascript:StartDevices()"                                                                               . $delim . "Comenzar con la reproduccion"                         . $delim . "Start Video"          . $delim . "blue"  ; $i++;
            
      foreach( $sorted_array as $key => $thread )
      {
        $item = explode( '.', $thread, 2 );
        $this->arrayMainMenuButtons[ $i ] = "javascript:VideoItemConfig('" . urlencode(base64_encode($path . $thread)) . "')"                       . $delim . HINT_BUTTON_DEVICE_CONFIG . " [" . $item[0]      . "]" . $delim . 'Video [' . $key . ']' . $delim . "yellow" ; $i++;
      }

      $this->arrayMainMenuButtons[ $i ] = "javascript:VideoItemConfig('"  . urlencode(base64_encode($path  . "thread"  . ($i-3) . ".conf")) . "')"  . $delim . "Agregar una camara"      . $delim . "Agregar+"      . $delim . "green"; $i++;      
      
      $quantityOfButtons=sizeof($this->arrayMainMenuButtons);
      if( $quantityOfButtons >= 9 && $quantityOfButtons < 20 )
      {
        $this->fontSize = '1.7vw';
      }
      else if( $quantityOfButtons >= 20 )
      {
        $this->fontSize = '1vw';
      }    
      else
      {
        $this->fontSize = '2vw';
      }
    }
  
    protected function setPropertiesForClassParameterToolbar()
    {
      $this->propertiesForClassParameterToolbar = "";
    }
  
    protected function setPropertiesForClassParameterMainMenu()
    {
      $this->propertiesForClassParameterMainMenu = "col-xs-6 col-md-6 col-lg-6 col-xl-6";
    }
  
    protected function setJavascript()
    {
      $script  = "";
      
      $script .= "toastr.options = {\"debug\": false, \"positionClass\": \"toast-bottom-center\", \"onclick\": null, \"fadeIn\": 300, \"fadeOut\": 1000, \"timeOut\": 2000, \"extendedTimeOut\": 3500};\n";
      
      $script .= "function myAlert(text)\n";
      $script .= "{\n";
      $script .=    (Site::isMobileBrowser())? ("alert(text);\n"):("toastr.info(text);\n");
      $script .= "}\n";      
  
      $script .= "function Back()";
      $script .= "{";
      $script .= "   parent.location.href = 'DevicesSetup.php';";
      $script .= "}\n";
          
      $script .= "function Home()";
      $script .= "{";
      $script .= "   window.top.location.href = 'Refresh.php';";
      $script .= "}\n";
  
      $script .= "function VideoItemConfig(item)";
      $script .= "{";
      $script .= "   parent.location.href = 'VideoItemConfig.php?name=' + item;";
      $script .= "}\n";
      
      $script .= "function StartDevices()";
      $script .= "{";
      $script .= "   parent.location.href = 'DevicesBackendConfig.php?exec_start_devices';";
      $script .= "}\n";
      
      $script .= "function StopDevices()";
      $script .= "{";
      $script .= "   parent.location.href = 'DevicesBackendConfig.php?exec_stop_devices';";
      $script .= "}\n";
      
      $this->javascriptToInsert = $script;
    }
  }
  
  $devicesBackendConfig = new DevicesBackendConfig();
  $devicesBackendConfig->evaluateContext();
?>
