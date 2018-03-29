///////////////////////////////////////////////////////////////////////////////////////////////////
var thereIsNewKey = false;
var viewAsLog     = false;


function Back(previous_place)
{
	parent.location.href = previous_place;
}

function Home()
{
  window.top.location.href = 'Refresh.php';
}

function Save(name_action_form)
{
	var $formUpdate = $('#update-form');
	$formUpdate.append("<input type='hidden' name='" + name_action_form + "' value='save' />");
	$formUpdate.submit();
}

function Add(name_action_form)
{
  var $formUpdate = $('#update-form');
  $formUpdate.append("<input type='hidden' name='" + name_action_form + "' value='add' />");
  $formUpdate.submit();
}

function Del(key, name_action_form)
{
	var $formUpdate = $('#update-form');
	$formUpdate.append("<input type='hidden' name='" + name_action_form + "' value='del' />");
	$formUpdate.append("<input type='hidden' name='key' value='" + key + "' />");
	$formUpdate.submit();
}

function Undo()
{
  location.href = location.href;
}

function DelItemConfig(caller, item)
{
  if( confirm('Esta seguro de eliminar [' + item + '] ?') )
  {
    parent.location.href = caller + '?' + 'action=delete_item&item=' + item;
  }
}

function adjustSizeOfConponents()
{
  $(document).ready(function()
  {
    var $entireContainer          = $('#entire-container');
    var $toolbarButtonsContainer  = $('#toolbar-buttons-container');
    var $showConfigTitleContainer = $('#show-config-title-container');
    var $showConfigItemsContainer = $('#show-config-items-container');
    var toolbarContainerH         = 0;
    var toolbarContainerW         = 0;
    var containerH                = 0;
    var containerW                = 0;
    
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

      fitShowConfigContainer();
      
      setTimeout(checkSizeChange, 500);
      
      if( thereIsNewKey )
      {
        $("#class-field-new-key").focus();
        thereIsNewKey = false;
      }
      
      if( viewAsLog )
      {
        $("#view_as_log_at_end_of_file").focus();
        $("#view_as_log_at_end_of_file").css({'display': 'none'});
      }      
    }

    function fitToolbarButtons() 
    {
      var ltoolbarContainerH = $toolbarButtonsContainer.outerHeight();
      var ltoolbarContainerW = $toolbarButtonsContainer.outerWidth();
      if( (ltoolbarContainerH != toolbarContainerH) || (ltoolbarContainerW != toolbarContainerW) )
      {
        toolbarContainerH  = ltoolbarContainerH;
        toolbarContainerW  = ltoolbarContainerW;
        
        var lengthOfButtonsImages = $('.image-button-wrapper').length;
        var newHeightSize = toolbarContainerH-10;
        var newWidthSize  = ((toolbarContainerW/lengthOfButtonsImages));
        
        $toolbarButtonsContainer.each
        (
          function() 
          {
            $(this).find('img').css
                (
                  {
                    'width'  : newWidthSize, 
                    'height' : newHeightSize
                  }
                );
          }
        );
      }
    }

    function fitShowConfigContainer()
    {
      var lContainerH = $entireContainer.outerHeight();
      var lContainerW = $entireContainer.outerWidth();
      if ((lContainerH != containerH) || (containerW != lContainerW))
      {
        containerH = lContainerH;
        containerW = lContainerW;
        
        var newShowConfigContainerHeightSize = (containerH - $toolbarButtonsContainer.outerHeight() - $showConfigTitleContainer.outerHeight());
        
        var lengthOfButtons      = ($('.class-field-2').length);
        var newButtonHeightSize  = (newShowConfigContainerHeightSize / lengthOfButtons)+20;
        
        if( lengthOfButtons <= 7 )
        {
          newButtonHeightSize = '17%';
        }        
        
        $('.class-field-new-key').css
        (
          {
            'width'  : '100%',
            'height' : '100%'
          }
        );
        
        if( viewAsLog )
        {
          $('.class-field-1').css
          (
            {
              'width'  : '3%',
              'height' : newButtonHeightSize
            }
          );        
          
          $('.class-field-2').css
          (
            {
              'width'  : '97%', 
              'height' : newButtonHeightSize
            }
          );
        }
        else
        {
          $('.class-field-1').css
          (
            {
              'width'  : '35%',
              'height' : newButtonHeightSize
            }
          );        
          
          $('.class-field-2').css
          (
            {
              'width'  : '55%', 
              'height' : newButtonHeightSize
            }
          );
        }
        
        $('.class-field-Image-Del').css
        (
          {
            'width'  : '10%', 
            'height' : newButtonHeightSize
          }
        );
        
        $showConfigItemsContainer.each
          (
            function()
            {
              $(this).find('img').css
              (
                {
                  'width'   : 'auto',
                  'height'  : newButtonHeightSize
                }
              );
            }
          );

        $showConfigTitleContainer.css( {'display' : 'inline' } );
        $showConfigItemsContainer.css( {'display' : 'inline' } );
        $entireContainer.css( {'display' : 'inline' } );        
      }
    }
  });
}
///////////////////////////////////////////////////////////////////////////////////////////////////
