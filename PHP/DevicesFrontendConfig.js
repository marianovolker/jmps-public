///////////////////////////////////////////////////////////////////////////////////////////////////
function Back()
{
	parent.location.href = "DevicesSetup.php";
}

function Home()
{
  window.top.location.href = 'Refresh.php';
}

function Save()
{
	var $formUpdate = $('#update-form');
	$formUpdate.append("<input type='hidden' name='devices_action' value='save' />");
	$formUpdate.submit();
}

function Add()
{
	var $formUpdate = $('#update-form');
	$formUpdate.append("<input type='hidden' name='devices_action' value='add' />");
	$formUpdate.submit();
}

function Del(section)
{
	var $formUpdate = $('#update-form');
	$formUpdate.append("<input type='hidden' name='devices_action' value='del' />");
	$formUpdate.append("<input type='hidden' name='section' value='" + section + "' />");
	$formUpdate.submit();
}

function Undo()
{
  location.href = location.href;
}

function adjustSizeOfConponents()
{
  $(document).ready
    (
      function()
      {
        var $entireContainer           = $('#entire-container');
        var $toolbarButtonsContainer   = $('#toolbar-buttons-container');
        var $showConfigContainer       = $('#show-config-container');
        var imageButtonsContainerH     = 0;
        var imageButtonsContainerW     = 0;
        var containerH                 = 0;
        var containerW                 = 0;

        $entireContainer.each
        (
            function() 
            {
              $(this).find('img').css
              (
                  {
                    'display' : 'none' ,
                    'width'   : '0'    ,
                    'height'  : '0'
                  }
              );
            }
        );

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

          setTimeout(checkSizeChange, 300);
        }
    
        function fitToolbarButtons() 
        {
          var lImageButtonsContainerH = $toolbarButtonsContainer.outerHeight();
          var lImageButtonsContainerW = $toolbarButtonsContainer.outerWidth();
          if( (lImageButtonsContainerH != imageButtonsContainerH) || (lImageButtonsContainerW != imageButtonsContainerW) )
          {
            imageButtonsContainerH  = lImageButtonsContainerH;
            imageButtonsContainerW  = lImageButtonsContainerW;

            var lengthOfButtonsImages = $('.image-button-wrapper').length;
            var newHeightSize = imageButtonsContainerH-5;
            var newWidthSize  = ((imageButtonsContainerW/lengthOfButtonsImages)-5);

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
          if( (lContainerH != containerH) || (containerW != lContainerW) )
          {
            containerH  = lContainerH;
            containerW  = lContainerW;
            var newShowConfigContainerHeightSize = (containerH-$toolbarButtonsContainer.outerHeight());
            var newShowConfigContainerWidthSize  = (containerW-100);

            var lengthOfRecords      = $('.record_wrapper').length;
            var newRecordsHeightSize = (newShowConfigContainerHeightSize/lengthOfRecords);
            
            if( (lengthOfRecords/6) <= 5 ) // el valor de 6 es porque cada registro se compone por 6 valores ...
            {
              newRecordsHeightSize = '30';
            }
            
            $showConfigContainer.css
            (
              {
                'left'    : '5%'
              }
            );            

            $(".class-field-Section-Id").css
            (
                {
                  'width' : ((newShowConfigContainerWidthSize - 10) / 4)
                }
            );

            $(".class-field-Image-Del").css
            (
                {
                  'width' : ((newShowConfigContainerWidthSize - 10) / 4)
                }
            );

            $(".class-field-2").css
            (
                {
                  'width' : ((newShowConfigContainerWidthSize - 10) / 3)
                }
            );

            $(".class-field-3").css
            (
                {
                  'width' : ((newShowConfigContainerWidthSize - 10) / 2)
                }
            );

            $(".class-field-3-select").css
            (
                {
                  'width' : ((newShowConfigContainerWidthSize - 10) / 2)
                }
            );

            $showConfigContainer.each
            (
                function() 
                {
                  $(this).find('img').css
                  (
                      {
                        'width'  : 'auto', 
                        'height' : (newRecordsHeightSize*6) // el valor de 6 es porque cada registro se compone por 6 valores ...
                      }
                  );
                }
            );

            $entireContainer.each
            (
                function() 
                {
                  $(this).find('img').css
                  (
                      {
                        'display'  : 'inline'
                      }
                  );
                }
            )

            $entireContainer.css
            (
                {
                  'display'  : 'inline'
                }
            )
          }
        }
      }
    );
}
///////////////////////////////////////////////////////////////////////////////////////////////////
