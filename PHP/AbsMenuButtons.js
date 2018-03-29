///////////////////////////////////////////////////////////////////////////////////////////////////
var hideButtonsUntilResizeIsComplete = true;

function adjustSizeOfConponents(top, bottom, left, right, width, height, qElementsByRow, fontSize)
{
  $(document).ready
    (
      function() 
      {
          var $entireContainer         = $('#entire-container');
          var $toolbarButtonsContainer = $('#toolbar-buttons-container');
          var $actionsButtonsContainer = $('#main-menu-buttons-container');
          var toolbarContainerH        = 0;
          var toolbarContainerW        = 0;
          var actionsButtonsContainerH = 0;
          var actionsButtonsContainerW = 0;
          
          $entireContainer.css({'display': 'none'});
        
          $entireContainer.imagesLoaded().done
          (
            function() 
            {
              checkSizeChange();
            }
          );
          
          function checkSizeChange() 
          {
            fitToolbarButtons();
            
            fitActionsButtonsContainer();
            
            setTimeout(checkSizeChange, 100);
          }
          
          function fitToolbarButtons() 
          {
            if( $toolbarButtonsContainer )
            {
              var ltoolbarContainerH = $toolbarButtonsContainer.outerHeight();
              var ltoolbarContainerW = $toolbarButtonsContainer.outerWidth();
              if( (ltoolbarContainerH != toolbarContainerH) || (ltoolbarContainerW != toolbarContainerW) )
              {
                toolbarContainerH  = ltoolbarContainerH;
                toolbarContainerW  = ltoolbarContainerW;
                
                var quantityOfButtonsImages = $('.image-button-wrapper').length;
                var newHeightSize = toolbarContainerH;
                var newWidthSize  = (toolbarContainerW/quantityOfButtonsImages);
                
                $toolbarButtonsContainer.each
                (
                  function() 
                  {
                    $(this).find('img').css
                        (
                          {
                            'top'    : 10,
                            'bottom' : 10,
                            'width'  : newWidthSize, 
                            'height' : newHeightSize
                          }
                        );
                  }
                );
              }
            }
          }

          function fitActionsButtonsContainer() 
          {
            var lactionsButtonsContainerH = $actionsButtonsContainer.outerHeight();
            var lactionsButtonsContainerW = $actionsButtonsContainer.outerWidth ();
            if( (lactionsButtonsContainerH != actionsButtonsContainerH) || (actionsButtonsContainerW != lactionsButtonsContainerW) )
            {
              actionsButtonsContainerH  = lactionsButtonsContainerH;
              actionsButtonsContainerW  = lactionsButtonsContainerW;
              
              var quantityOfButtons = ($('.action-button-wrapper').length);
              
              var buttonHeight = 0;
              var buttonWidth  = 0;
              var deltaH = 10;
              var deltaW = 10;
              
              if( quantityOfButtons <= qElementsByRow )
              {
                qRows = 1;
                if( quantityOfButtons == 1 )
                {
                  deltaW = 0;
                }
                else if( quantityOfButtons == 2 )
                {
                  deltaW = 5;
                }
                else if( quantityOfButtons == 3 )
                {
                  deltaW = 8;
                }
                else if( quantityOfButtons == 4 )
                {
                  deltaW = 80;
                }
                
                buttonWidth  = (actionsButtonsContainerW-deltaW)/quantityOfButtons;
              }
              else
              {
                qRows = (quantityOfButtons / qElementsByRow) + (quantityOfButtons % qElementsByRow);
                buttonWidth  = (actionsButtonsContainerW-deltaW)/qElementsByRow;
                
                deltaH = deltaH + 20;
              }
              
              deltaH = deltaH * qRows;
              buttonHeight = (actionsButtonsContainerH-deltaH)/qRows;              
              
              $actionsButtonsContainer.css
              (
                {
                  'top'    : top,
                  'bottom' : bottom,
                  'left'   : left,
                  'right'  : right,
                  'width'  : width,
                  'height' : height
                }                
              );              
              
              $('.btn-lgx').css
              (
                {
                  'font-size' : fontSize
                }
              );
              
              $('.action-button-wrapper').css
              (
                {
                  'width'  : buttonWidth,
                  'height' : buttonHeight,
                }                
              );
              
              if( quantityOfButtons >= 4 )
              {
                $('#next_page_button').css
                (
                  {
                    'width'  : '60',
                    'height' : (buttonHeight)
                  }
                );
              }
              
              $entireContainer.css({'display' : 'inline'});
            }
            else
            {
              if( hideButtonsUntilResizeIsComplete )
              {
                $('.action-button-wrapper').css({'display': 'inline'});
                
                hideButtonsUntilResizeIsComplete = false;
              }
            }
          }
      }
    );
}
///////////////////////////////////////////////////////////////////////////////////////////////////

