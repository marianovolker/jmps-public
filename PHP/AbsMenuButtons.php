<?php

  require_once("Site.php");
  require_once("SystemConfig.php");
  
  
  /**
   * AbsMenuButtons
   *
   * This class build a generic buttons menu with a toolbar, the module security_module from jmps package.
   *
   * @author   Mariano Volker <marianovolker@yahoo.com.ar>
   * @package  Security Module
   */
  abstract class AbsMenuButtons 
  {
    /**
     * Constructor
     */
    function __construct()
    {
      $this->typesOfButtons = array("btn-info", "btn-danger", "btn-warning", "btn-primary", "btn-success");
      $this->marginsOfMainMenu = "top:25%; bottom:10%; left:20%; rigth:20%; width:70%; height:65%";
      $this->buttonsByRow = 3;
      $this->fontSize = '3vw';
      $this->maxButtonsByPage = 4;
      $this->lengthOfTextInButtons = 21;
      $this->backgroundColor = (new SystemConfig())->getBGColorConfigMenu();
      
      $this->setDelimiterForFieldSeparator();
      $this->setToolbarButtons();
      $this->setMainMenuButtons();
      $this->setPropertiesForClassParameterToolbar();
      $this->setPropertiesForClassParameterMainMenu();
      $this->setJavascript();      
    }
    
    /**
     * check if there are actions to execute before to build the view...
     */
    public function evaluateContext()
    {
      $this->evaluateActions();
    }

    
    /*
     * array with types of buttons to use in the main menu.
     */
    protected $typesOfButtons;    
    
    /*
     * array with buttons and actions for the toolbar.
     */
    protected $arrayToolbarButtons;
    
    /*
     * array  with buttons and actions for the main menu.
     */
    protected $arrayMainMenuButtons;
    
    /*
     * properties to set in the class parameter of the tag button
     * in the toolbar section.
     */
    protected $propertiesForClassParameterToolbar;    
    
    /*
     * properties to set in the class parameter of the tag button 
     * in the main menu section.
     */
    protected $propertiesForClassParameterMainMenu;
    
    /*
     * properties to set in the class parameter of the tag button
     * in the main menu section.
     */
    protected $delimiterForFieldSeparator;    
    
    /*
     * javascript string to insert into the page.
     * 
     */
    protected $javascriptToInsert;    
    
    /*
     * css style string to insert into the page.
     *
     */
    protected $cssStyleToInsert;    
    
    /*
     * margins of the main menu.
     *
     */
    protected $marginsOfMainMenu;

    /*
     * quantity of buttons by row at main menu.
     *
     */
    protected $buttonsByRow;
    
    /*
     * fontsize for buttons at main menu.
     *
     */    
    protected $fontSize;
    
    /*
     * Max buttons at main menu by page when the quantity of rows is equal to one.
     *
     */
    protected $maxButtonsByPage;
    
    /*
     * Background color.
     *
     */
    protected $backgroundColor;
    
    /*
     * Length of the text into buttons, to truncate if this is exceded.
     *
     */
    protected $lengthOfTextInButtons;
    
    
    // Abstracts methods ...
    
    /*
     * force that the child define this method.
     */
    abstract protected function setToolbarButtons();

    
    /*
     * force that the child define this method.
     */
    abstract protected function setMainMenuButtons();
    

    /*
     * force that the child define this method.
     */
    abstract protected function setPropertiesForClassParameterToolbar();    
    
    
    /*
     * force that the child define this method.
     */
    abstract protected function setPropertiesForClassParameterMainMenu();    
    
    
    /*
     * force that the child define this method.
     */
    abstract protected function setDelimiterForFieldSeparator();    
    
    
    /*
     * force that the child define this method.
     */
    abstract protected function setJavascript();    
    
        

    
    
    
    
    
    // protected methods ...
    
    
    protected function evaluateActions()
    {
      $this->show();
    }
    
    protected function getBootstrapClassFromColor($color)
    {
      switch(strtolower($color))
      {
        case 'lightblue':
          return 'btn-info';
          
        case 'red':
          return 'btn-danger';
          
        case 'yellow':
          return 'btn-warning';
          
        case 'blue':
          return 'btn-primary';
          
        case 'green':
          return 'btn-success';
      }
      return 'btn-success';
    }    
    
    protected function buildToolbar()
    {
      $sizeOfButtons = sizeof($this->arrayToolbarButtons);
      
      if( $sizeOfButtons )
      {
        Site::pr('<td align="center">');
          Site::pr('<div id="toolbar-buttons-container">');
            Site::pr('<table>');
              Site::pr('<tr>');
                for( $i = 0; $i < $sizeOfButtons; $i++ )
                {
                  $item = explode( $this->delimiterForFieldSeparator, $this->arrayToolbarButtons[$i], 3 );
                  
                  Site::pr('<td>');
                    Site::pr('<a href="'. $item[0] .'">');
                      Site::pr('<div class="image-button-wrapper">');
                        Site::pr('<img class="image ' . $this->propertiesForClassParameterToolbar . ' " id="'.$item[0].'" title="'.$item[1].'" src="'.$item[2].'">');
                      Site::pr('</div>');
                    Site::pr('</a>');
                  Site::pr('</td>');
                }
              Site::pr('<tr>');
            Site::pr('</table>');
          Site::pr('</div>');
        Site::pr('</td>');
      }
    }
    
    protected function buildMainMenu()
    {
      Site::pr('<td');
        Site::pr('<div id="main-menu-buttons-container">');
        
          $typesOfButtons = $this->typesOfButtons;
          
          $quantityOfButtons=sizeof($this->arrayMainMenuButtons);

          if( $quantityOfButtons <= $this->buttonsByRow )
          {
            $maxByPage = $this->maxButtonsByPage;
            
            if( $quantityOfButtons > $maxByPage )
            {
              $page = ( isset($_GET['page'])? ($_GET['page']):(0) );
              $truncatePage = false;
              
              for( $c = 0; $c < $maxByPage; $c++ )
              {
                $i = ( ($page * $maxByPage) + $c );
                
                if( $i >= $quantityOfButtons )
                {
                  $i = ( $i % $quantityOfButtons );
                  $truncatePage = true;
                }
                
                $item = explode( $this->delimiterForFieldSeparator, $this->arrayMainMenuButtons[$i], 4 );
                
                // check if the color of the button was set ...    
                $colorOfButton = (isset($item[3]))? ($this->getBootstrapClassFromColor($item[3])):($typesOfButtons[($i % sizeof($typesOfButtons))]);
                
                $textButton = strtolower(str_replace('_', ' ', $item[2]));
                Site::pr('<a href="'.$item[0].'">');
                  Site::pr('<button type="button" class="btn-lgx round image action-button-wrapper ' . $this->propertiesForClassParameterMainMenu . ' text-center '.$colorOfButton.'" id="'.$item[0].'" title="'.$item[1].'">');
                    Site::pr(substr($textButton,0,$this->lengthOfTextInButtons));
                  Site::pr('</button>');
                Site::pr('</a>');
              }
              
              if( $truncatePage == true ) $page=0;
              else $page++;
              
              $url = "http://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
              $url = explode('?', $url)[0];

              Site::pr('<a href="javascript:location.href = \''.$url.'?page='.$page.'\'">');
                Site::pr('<img class="image" id="next_page_button" title="'.HINT_BUTTON_NEXT_PAGE.'" src="images/more_buttons.png">');
              Site::pr('</a>');
            }
            else
            {
              for( $i = 0; $i < $quantityOfButtons; $i++ )
              {
                $item = explode( $this->delimiterForFieldSeparator, $this->arrayMainMenuButtons[$i], 4 );
                
                // check if the color of the button was set ...
                $colorOfButton = (isset($item[3]))? ($this->getBootstrapClassFromColor($item[3])):($typesOfButtons[($i % sizeof($typesOfButtons))]);
                
                $textButton = strtolower(str_replace('_', ' ', $item[2]));
                Site::pr('<a href="'.$item[0].'">');
                  Site::pr('<button type="button" class="btn-lgx round image action-button-wrapper ' . $this->propertiesForClassParameterMainMenu . ' text-center '.$colorOfButton.'" id="'.$item[0].'" title="'.$item[1].'">');
                    Site::pr(substr($textButton,0,$this->lengthOfTextInButtons));
                  Site::pr('</button>');
                Site::pr('</a>');
              }
            }
          }
          else
          {
            Site::pr('<table>');
              Site::pr('<tr>');
                Site::pr('<td>');
                for( $i = 0, $j = 1; $i < $quantityOfButtons; $i++, $j++ )
                {
                  if( $j > $this->buttonsByRow )
                  {
                    $j = 1;
                    Site::pr('</td>');
                    Site::pr('</tr>');
                    
                    Site::pr('<tr><td><p>&nbsp;</p></td></tr>');
                    
                    Site::pr('<tr>');
                    Site::pr('<td>');
                  }
                  
                  $item = explode( $this->delimiterForFieldSeparator, $this->arrayMainMenuButtons[$i], 4 );

                  // check if the color of the button was set ...
                  $colorOfButton = (isset($item[3]))? ($this->getBootstrapClassFromColor($item[3])):($typesOfButtons[($i % sizeof($typesOfButtons))]);                  
                  
                  $textButton = strtolower(str_replace('_', ' ', $item[2]));                
  
                  Site::pr('<a href="'.$item[0].'">');
                    Site::pr('<button type="button" class="btn-lgx round image action-button-wrapper ' . $this->propertiesForClassParameterMainMenu . ' text-center '.$colorOfButton.'" id="'.$item[0].'" title="'.$item[1].'">');
                      Site::pr(substr($textButton,0,$this->lengthOfTextInButtons));
                    Site::pr('</button>');
                  Site::pr('</a>');
                }
                Site::pr('</td>');
              Site::pr('</tr>');
            Site::pr('</table>');
          }
          
        Site::pr('</div>');
      Site::pr('</td>');
    }
    
    
    /**
     * header
     * Build the header of the main view.
     *
     * @access public
     * @return none
     */
    protected function header()
    {
      $cssFiles = array (
                          "users/assets/css/bootstrap.min.css",
                          "libsJS/toastr.min.css",
                          "AbsMenuButtons.css"
                        );
    
      $jsFiles  = array (
                          "libsJS/jquery.imagesloaded.min.js",
                          "libsJS/toastr.min.js",
                          "AbsMenuButtons.js"
                        );
    
      Site::writeHeader("", $cssFiles, $this->cssStyleToInsert, $jsFiles, $this->javascriptToInsert);
    }

    
    /**
     * body
     * Build the body of the main view.
     *
     * @access public
     * @return none
     */
    protected function body()
    {
      Site::pr('<body style="background-color:#' . $this->backgroundColor . ';">');
    
      list($top, $bottom, $left, $right, $width, $height) = explode(";", $this->marginsOfMainMenu);
      $top    = "'" . explode(":", $top   , 2 )[1] . "'";
      $bottom = "'" . explode(":", $bottom, 2 )[1] . "'";
      $left   = "'" . explode(":", $left  , 2 )[1] . "'";
      $right  = "'" . explode(":", $right , 2 )[1] . "'";
      $width  = "'" . explode(":", $width , 2 )[1] . "'";
      $height = "'" . explode(":", $height, 2 )[1] . "'";
    
      $script = '<script>';
      $script.= 'adjustSizeOfConponents(';
      $script.= $top                . ',' . $bottom         . ',';
      $script.= $left               . ',' . $right          . ',';
      $script.= $width              . ',' . $height         . ',';
      $script.= '\'' . $this->buttonsByRow . '\',\'' . $this->fontSize . '\'';
      $script.= ');</script>';
      Site::pr($script);
    
      Site::pr('<div id="entire-container">');
        Site::pr('<table>');
          if( sizeof($this->arrayToolbarButtons) > 0 )
          {
            Site::pr('<tr>');
              $this->buildToolbar();
            Site::pr('</tr>');
          }
          if( sizeof($this->arrayMainMenuButtons) > 0 )
          {
            Site::pr('<tr>');
              $this->buildMainMenu();
            Site::pr('</tr>');
          }
        Site::pr('</table>');
      Site::pr('</div>');
    }

    /**
     * trailer
     * Build the trailer of the main view.
     *
     * @access public
     * @return none
     */
    protected function trailer()
    {
      Site::writeTrailer();
    }    
    
    /**
     * show
     * Build the main view.
     *
     * @access public
     * @return none
     */
    protected function show()
    {
      $this->header();
      $this->body();
      $this->trailer();
    }
  }
?>
