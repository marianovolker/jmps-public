<?php

  require_once("Site.php");
  require_once("AlarmsConfigFrontend.php");
  require_once("SystemConfig.php");
  
  require_once(INSTALLATION_PATH.'config/config.php');
  require_once(INSTALLATION_PATH.'translations/es.php');
  require_once(INSTALLATION_PATH.'classes/Login.php');
  
  /**
   * StatusBar
   *
   * This class allows us to show all alarms availables configured with AlarmsConfig of the module security_module from jmps package.
   * To show alarms configured ...
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  class StatusBar 
  {
    /**
     * @var object $config is the object for system configuration
     */
    protected $systemConfig = null;

    /**
     * Store the information the configuration.
     */
    protected $alarmsConfig;

    /**
     * Store the information about the status of the alarms.
     */
    protected $alarmsChanges;

    /**
     * Store the information about the logged users.
     */
    protected $loggedUsersChanges;



    /**
     * Constructor
     */
    function __construct()
    {
      $this->systemConfig = new SystemConfig();
      $this->alarmsConfig = new AlarmsConfigFrontend();
      
      $this->checkAlarms();
      $this->checkLoggedUsers();
      
    }
    
    /**
     * create this object...
     */
    public function create()
    {
      $this->evaluateActions();
    }
    
    
    
    
    
    // protected methods ...
    
    protected function checkLoggedUsers()
    {
      $this->loggedUsersChanges = false;
      $login = new Login();
      $usersList = "";
      foreach( $login->getLoggedUsers() as $user )
      {
        $usersList .= $user;
      }
      if( (! isset($_COOKIE["usersLoggedList"])) || ($_COOKIE["usersLoggedList"] != $usersList) )
      {
        setcookie("usersLoggedList", $usersList, 0);
        $this->loggedUsersChanges = true;
      }
    }
    
    protected function checkAlarms()
    {
      $this->alarmsChanges = false;
      
      $alarmsArray = $this->alarmsConfig->getAlarmsAsArray();
      
      foreach( $alarmsArray as $alarm => $data )
      {
        $colorPositive = isset($alarmsArray[$alarm]['color_positive' ])? $alarmsArray[$alarm]['color_positive' ]  : "";
        $colorNegative = isset($alarmsArray[$alarm]['color_negative' ])? $alarmsArray[$alarm]['color_negative' ]  : "";
        $textPositive  = isset($alarmsArray[$alarm]['text_positive'  ])? $alarmsArray[$alarm]['text_positive'  ]  : "";
        $statusFile    = PATH_ALARMS . '/' . $alarmsArray[$alarm]['status'];
        $status = file_get_contents($statusFile);
        $rc = preg_match('/'.strtolower($status).'/', strtolower($textPositive));
        $color = ($rc)? $colorPositive:$colorNegative;
        
        if( (! isset($_COOKIE[$alarm])) || ($_COOKIE[$alarm] != $color) )
        {
          setcookie($alarm, $color, 0);
          $this->alarmsChanges = true;
        }
      }
    }
    
    protected function writeHeader()
    {
      $cssFiles = array (
                          "users/assets/css/bootstrap.min.css",
                          "StatusBar.css"
                        );
    
      $jsFiles  = array (
                          "libsJS/jquery.imagesloaded.min.js",
                          "StatusBar.js"
                        );
    
      Site::writeHeader("", $cssFiles, "", $jsFiles, "");
    }
    
    protected function evaluateActions()
    {
      if( isset( $_GET['status_bar_action'] ) && $_GET['status_bar_action'] == 'update_alarms' )
      {
        $this->buildAlarmsStatusBar();
      }
      elseif( isset( $_GET['status_bar_action'] ) && $_GET['status_bar_action'] == 'update_users' )
      {
        $this->buildLoggedUsersInformationBar();
      }
      else
      {
        $this->alarmsChanges = true;
        $this->loggedUsersChanges = true;
        $this->show();
      }
    }
    
    protected function buildAlarmsStatusBar()
    {
      if( $this->alarmsChanges )
      {
        $login = new Login();
        $alarmsArray = $this->alarmsConfig->getAlarmsAsArray();
        foreach( $alarmsArray as $alarm => $data )
        {
          $text           = isset($alarmsArray[$alarm]['text'           ])? $alarmsArray[$alarm]['text'           ]  : "";
          $hint           = isset($alarmsArray[$alarm]['hint'           ])? $alarmsArray[$alarm]['hint'           ]  : "";
          $textColor      = isset($alarmsArray[$alarm]['text_color'     ])? $alarmsArray[$alarm]['text_color'     ]  : "";
          $textPositive   = isset($alarmsArray[$alarm]['text_positive'  ])? $alarmsArray[$alarm]['text_positive'  ]  : "";
          $colorPositive  = isset($alarmsArray[$alarm]['color_positive' ])? $alarmsArray[$alarm]['color_positive' ]  : "";
          $colorNegative  = isset($alarmsArray[$alarm]['color_negative' ])? $alarmsArray[$alarm]['color_negative' ]  : "";

          if( !$login->isAllowedThisAlarm($text) )
          {
            continue;
          }

          $statusFile = PATH_ALARMS . '/' . $alarmsArray[$alarm]['status'];
            
          if( ($status=file_get_contents($statusFile)) && (strtolower($status) != "disabled") )
          {
            $rc = preg_match('/'.strtolower($status).'/', strtolower($textPositive));
             
            $color = ($rc)? $colorPositive:$colorNegative; 

            Site::pr('<button class="btn-lgx round alarm-status-item" style="color:#'.$textColor.'; background-color:#'.$color.'" title="'.$hint.'">');
              Site::pr($text);
            Site::pr('</button>');
          }
        }
        return;
      }
      echo "false";
    }
    
    protected function buildLoggedUsersInformationBar()
    {
      if( $this->loggedUsersChanges )
      {
        $html="";
        $login = new Login();
        $usersList = $login->getLoggedUsers();
        $html.= "<style> .btn-lgx {color : #" . $this->systemConfig->getFontColorMainStatusBar() . " ;} </style>";
          $html.= '<select title="Usuarios logeados" class="btn-info btn-lgx round logged-users-item ">';
            $html.= '<optgroup label="Usuarios Logeados">';
              foreach( $usersList as $user )
              {
                if( $user == $login->getUsername() )
                {
                  $user = $login->getFullUserName($login->user_name)->FullUserName . ' (' . $user . ')';
                  $html .= '<option disabled selected="selected">'.$user.'</option>';
                }
                else
                {
                  $html .= '<option disabled> ' . $login->getFullUserName($user)->FullUserName . ' (' . $user . ') </option>';
                }
              }
            $html.= '</optgroup>';
          $html.= '</select>';
        Site::pr($html);
        return;
      }
      echo "false";
    }
    
    /**
     * show
     * Build the status bar.
     *
     * @access public
     * @return none
     */
    protected function show()
    {
      $this->writeHeader();
        Site::pr('<body style="background-color:#' . $this->systemConfig->getBGColorStatusBar() . ';">');
        
          $login = new Login();
          
          Site::pr('<script>adjustSizeOfConponents();</script>');
          Site::pr('<script>var countdown_timer=' . ($login->getRemainingSessionTime() + 2) . ';</script>');
          
          Site::pr('<div id="entire-container">');
            Site::pr('<table>');
              Site::pr('<tr>');
                Site::pr('<td align="center">');
                  Site::pr('<div id="alarms-status-bar-container">');
                    $this->buildAlarmsStatusBar();
                  Site::pr('</div>');
                Site::pr('</td>');
                Site::pr('<td align="center">');
                  Site::pr('<div id="logged-users-information-bar-container">');
                    $this->buildLoggedUsersInformationBar();
                  Site::pr('</div>');
                Site::pr('</td>');
                Site::pr('<td align="center">');
                  Site::pr('<div id="countdown-timer-container">');
                    Site::pr('<button class="btn-info btn-lgx countdown-timer-item round"  title="Tiempo restante para la finalizacion de la sesion" id="timer"></button>');
                  Site::pr('</div>');
                Site::pr('</td>');
              Site::pr('</tr>');
            Site::pr('</table>');
          Site::pr('</div>');

      Site::writeTrailer();
    }
  }
  
  $statusBar = new StatusBar();
  $statusBar->create();
?>


