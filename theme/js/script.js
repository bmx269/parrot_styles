(function($) {

  /**
   * Element animation effects
   *
   * Usage:
   * 1. Via Parrot Panels Styles
   * 2. Manully: <div data-animate="EFFECT" data-animate-target=".SELECTOR1,#sector2,.etc" data-animate-delay="MILLISECONDS" data-animate-duration="MILLISECONDS">Element content</div>
   */
  Drupal.behaviors.parrotStylesAnimation = {
    attach: function(context, settings) {
      var hasWaypoints = jQuery().waypoint;

      // CSS based animations
      if (hasWaypoints) {
        $('body').waypoint(function(direction) {
          window.setTimeout(function(){
            $('html').addClass('animate-onload');
          },1000);
        });
      }
      else {
        window.setTimeout(function(){
          $('html').addClass('animate-onload');
        },1000);
      }

      // data based animation
      var $animates= $('[data-animate]', context);
      if ($animates.length > 0) {
        var elementLoaded = function(elementPath, callBack){
          window.setTimeout(function(){
            if($(elementPath, context).length){
              callBack(elementPath, $(elementPath, context));
            }else{
              waitForElement(elementPath, callBack);
            }
          },500)
        }

        var animateInit = function() {
          $animates.each(function(i) {
            var $animate      = $(this),
              $animateTargets = $animate,
              animateTargets  = $animate.data('animate-target') || null,
              animateEffect   = $animate.data('animate') || null,
              animateDelay    = $animate.data('animate-delay') || null,
              animateDuration = $animate.data('animate-duration') || null;

            // Check effect valid effect
            if (animateEffect === null) {
              $animateTargets.removeClass('animate');
              return;
            }

            // Delay in triggering animation
            if (animateDelay !== null) {
              $animateTargets.css({
                '-webkit-animation-delay': animateDelay +'s',
                '-moz-animation-delay': animateDelay +'s',
                'animation-delay': animateDelay +'s'
              });
            }
            // Duration of animation
            if (animateDuration !== null) {
              $animateTargets.css({
                '-webkit-animation-duration': animateDuration +'s',
                '-moz-animation-duration': animateDuration +'s',
                'animation-duration': animateDuration +'s'
              });
            }

            // Target(s)
            if (animateTargets !== null) {
              var $tempAnimateTargets = $animateTargets.find(animateTargets);
              if ($tempAnimateTargets.length > 0) {
                $tempAnimateTargets.addClass('animate');
                $animateTargets.removeClass('animate');

                // Multiple targets so stagger with delay
                if (animateDelay !== null) {
                  $animateTargets.css({
                    '-webkit-animation-delay': '0s',
                    '-moz-animation-delay': '0s',
                    'animation-delay': '0s'
                  });

                  $tempAnimateTargets.each(function(e) {
                    var delay = e === 0 ? '0s' : animateDelay*e +'s';
                    $(this).css({
                      '-webkit-animation-delay': delay,
                      '-moz-animation-delay': delay,
                      'animation-delay': delay
                    });
                  });
                }

                $animateTargets = $tempAnimateTargets;
                $animate.data('animate-targets', $animateTargets);
              }
            }
            $animateTargets.addClass('animate-target');

            // Trigger when item reaches top of view port
            if (hasWaypoints) {
              $animateTargets.each(function() {
                var $animateTarget = $(this);

                var waypoint = $animateTarget.waypoint(function(direction) {
                  var animateEffect = $animate.data('animate');
                  $animateTarget.animateCss(animateEffect, function() {
                    // Flag as animation done
                    $animateTarget.data('animate-done', true).addClass('animate-done').removeClass('animate animated '+ animateEffect);

                    // All targets animated so flag on parent
                    var $tempAnimateTargets = $animate.data('animate-targets') || null;
                    var animateDoneCount = $animate.find('.animate-done').length;
                    if ($tempAnimateTargets !== null && $tempAnimateTargets.length > 0 && animateDoneCount === $tempAnimateTargets.length) {
                      $animate.data('animate-done', true).addClass('animate-done');
                    }

                    $(window).trigger('resize');
                  });
                  this.destroy(); // only once
                },
                {
                  offset: (i === 0 ? '100%' : '80%')
                });
              });
            }
            else {
              // No waypoints for whatever reason so just apply effects directly
              $animateTargets.animateCss(animateEffect);
            }
          });
        };
      }
    }
  };
})(jQuery);
