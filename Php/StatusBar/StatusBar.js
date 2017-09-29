///////////////////////////////////////////////////////////////////////////////////////////////////
var hideButtonsUntilResizeIsComplete = true;

function adjustSizeOfConponents()
{
  $(document).ready
    (
      function() 
      {
          var $entireContainer         = $('#entire-container');
          var $alarmsButtonsContainer  = $('#alarms-status-bar-container');
          var $loggedUsersContainer    = $('#logged-users-information-bar-container');
          var $countdownTimerContainer = $('#countdown-timer-container');
          
          var containerH               = 0;
          var containerW               = 0;
          
          $entireContainer.css({'display': 'none'});
          
          var counter=setInterval(timer, 1000);
          function timer()
          {
            countdown_timer = countdown_timer-1;
            if( countdown_timer <= 0 )
            {
               clearInterval(counter);
               return;
            }

            document.getElementById("timer").innerHTML = countdown_timer + " segundos";
          }

          $entireContainer.imagesLoaded().done
          (
            function() 
            {
              checkSizeChange();
              checkAlarmsAndUsersLogged();
              $entireContainer.css({'display'  : 'inline'});
            }
          );
          
          function checkAlarmsAndUsersLogged()
          {
            if( ! hideButtonsUntilResizeIsComplete )
            {
              $.get ( "StatusBar.php?status_bar_action=update_alarms",
                      function (data) 
                      {
                        // to remove non printer chars ...
                        var str = data.replace(/\W/g, '');
                        
                        if( str != "false" )
                        {
                          $('#alarms-status-bar-container').css({'display': 'none'});
                          $('#alarms-status-bar-container').html(data);
                          $('.alarm-status-item').css({'display': 'inline'});
                          $('#alarms-status-bar-container').css({'display': 'inline'});
                          containerH = 0;
                          containerW = 0;
                        }
                      }
                    );
              
              $.get ( "StatusBar.php?status_bar_action=update_users",
                      function (data) 
                      {
                        // to remove non printer chars ...
                        var str = data.replace(/\W/g, '');
                
                        if( str != "false" )
                        {
                          $('#logged-users-information-bar-container').html(data);
                          containerH = 0;
                          containerW = 0;
                        }
                      }
                    );
            }

            setTimeout(checkAlarmsAndUsersLogged, 3000);
          }
          
          function checkSizeChange() 
          {
            fitElementsInContainer();
            setTimeout(checkSizeChange, 300);
          }
          
          function fitElementsInContainer( ) 
          {
            var lContainerH = $alarmsButtonsContainer.outerHeight();
            var lContainerW = $alarmsButtonsContainer.outerWidth();
            if( (lContainerH != containerH) || (containerW != lContainerW) )
            {
              containerH  = lContainerH;
              containerW  = lContainerW;
              
              var alarmsItemW = ( ($alarmsButtonsContainer.outerWidth()-30) / ($('.alarm-status-item').length) );
              $( ".alarm-status-item" ).css
              (
                {
                  'top'     : '7%', 
                  'width'   : alarmsItemW, 
                  'height'  : (containerH-6)
                }
              );
              
              $( ".logged-users-item" ).css
              (
                {
                  'top'     : '7%', 
                  'height'  : (containerH-6)
                }
              );              
              
              $( ".countdown-timer-item" ).css
              (
                {
                  'top'     : '7%', 
                  'height'  : (containerH-6)
                }
              );
            }
            else
            {
              if( hideButtonsUntilResizeIsComplete )
              {
                $('.alarm-status-item').css({'display': 'inline'});
                
                hideButtonsUntilResizeIsComplete = false;
              }
            }
          }
      }
    );
}
///////////////////////////////////////////////////////////////////////////////////////////////////

