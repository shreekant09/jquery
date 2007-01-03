/* Copyright (c) 2006 Kelvin Luck (kelvin AT kelvinluck DOT com || http://www.kelvinluck.com)
 * Dual licensed under the MIT (http://www.opensource.org/licenses/mit-license.php)
 * and GPL (http://www.opensource.org/licenses/gpl-license.php) licenses.
 *
 * See http://kelvinluck.com/assets/jquery/jScrollPane/
 * $Id$
 */

/**
 * Replace the vertical scroll bars on any matched elements with a fancy
 * styleable (via CSS) version. With JS disabled the elements will
 * gracefully degrade to the browsers own implementation of overflow:auto.
 * If the mousewheel plugin has been included on the page then the scrollable areas will also
 * respond to the mouse wheel.
 *
 * @example jQuery(".scroll-pane").jScrollPane();
 *
 * @name jScrollPane
 * @type jQuery
 * @param Object	settings	hash with options, described below.
 * 								scrollbarWidth - the width of the generated scrollbar in pixels
 * 								scrollbarMargin - the amount of space to leave on the side of the scrollbar in pixels
 * 								wheelSpeed - The speed the pane will scroll in response to the mouse wheel in pixels
 * @return jQuery
 * @cat Plugins/jScrollPane
 * @author Kelvin Luck (kelvin AT kelvinluck DOT com || http://www.kelvinluck.com)
 */
jQuery.fn.jScrollPane = function(settings)
{
	settings = jQuery.extend(
		{
			scrollbarWidth : 10,
			scrollbarMargin : 5,
			wheelSpeed : 18
		}, settings
	);
	return this.each(
		function()
		{
			var $this = jQuery(this);

			if (jQuery(this).parent().is('.jScrollPaneContainer')) {
				var $c = jQuery(this).parent();
				var paneWidth = $c.innerWidth();
				var paneHeight = $c.outerHeight();
				if ($c.unmousewheel) {
					$c.unmousewheel();
				}
				jQuery('>.jScrollPaneTrack', $c).remove();
				$this.css({'top':0});
			} else {
				this.originalPadding = $this.css('padding');
				this.originalSidePaddingTotal = (parseInt($this.css('paddingLeft')) || 0) + (parseInt($this.css('paddingRight')) || 0);
				var paneWidth = $this.innerWidth();
				var paneHeight = $this.innerHeight();
				$this.wrap(
					jQuery('<div>').attr(
						{'className':'jScrollPaneContainer'}
					).css(
						{
							'height':paneHeight+'px',
							'width':paneWidth+'px'
						}
					)
				);
			}
			var p = this.originalSidePaddingTotal;
			$this.css(
				{
					'height':'auto',
					'width':paneWidth - settings.scrollbarWidth - settings.scrollbarMargin - p + 'px',
					'paddingRight':settings.scrollbarMargin + 'px'
				}
			);
			var contentHeight = $this.outerHeight();
			var percentInView = paneHeight / contentHeight;

			if (percentInView < .98) {
				var $container = $this.parent();
				$container.append(
					jQuery('<div>').attr({'className':'jScrollPaneTrack'}).css({'width':settings.scrollbarWidth+'px'}).append(
						jQuery('<div>').attr({'className':'jScrollPaneDrag'}).css({'width':settings.scrollbarWidth+'px'})
					)
				);

				var $pane = jQuery(this).css({'position':'absolute', 'overflow':'visible'});

				var currentOffset;
				var maxY;
				var mouseWheelMultiplier;
				// store this in a seperate variable so we can keep track more accurately than just updating the css property..
				var dragPosition = 0;
				var dragMiddle = percentInView*paneHeight/2;

				// pos function borrowed from tooltip plugin and adapted...
				var getPos = function (event, c) {
					var p = c == 'X' ? 'Left' : 'Top';
					return event['page' + c] || (event['client' + c] + (document.documentElement['scroll' + p] || document.body['scroll' + p])) || 0;
				};

				var ignoreNativeDrag = function() {	return false; };

				var initDrag = function()
				{
					currentOffset = $drag.offset();
					currentOffset.top -= dragPosition;
					maxY = paneHeight - currentOffset.height;
					mouseWheelMultiplier = 2 * settings.wheelSpeed * maxY / contentHeight;
				};

				var onStartDrag = function(event)
				{
					initDrag();
					dragMiddle = getPos(event, 'Y') - dragPosition - currentOffset.top;
					jQuery('body').bind('mouseup', onStopDrag).bind('mousemove', updateScroll);
					if (jQuery.browser.msie) {
						jQuery('body').bind('dragstart', ignoreNativeDrag).bind('selectstart', ignoreNativeDrag);
					}
					return false;
				};
				var onStopDrag = function()
				{
					jQuery('body').unbind('mouseup', onStopDrag).unbind('mousemove', updateScroll);
					dragMiddle = percentInView*paneHeight/2;
					if (jQuery.browser.msie) {
						jQuery('body').unbind('dragstart', ignoreNativeDrag).unbind('selectstart', ignoreNativeDrag);
					}
				};
				var positionDrag = function(destY)
				{
					destY = destY < 0 ? 0 : (destY > maxY ? maxY : destY);
					dragPosition = destY;
					$drag.css({'top':destY+'px'});
					var p = destY / maxY;
					$pane.css({'top':((paneHeight-contentHeight)*p) + 'px'});
				};
				var updateScroll = function(e)
				{
					positionDrag(getPos(e, 'Y') - currentOffset.top - dragMiddle);
				};

				var $drag = jQuery('>.jScrollPaneTrack .jScrollPaneDrag', $container);
				$drag.css(
					{'height':(percentInView*paneHeight)+'px'}
				).bind('mousedown', onStartDrag);

				var trackScrollInterval;
				var trackScrollInc;
				var trackScrollMousePos;
				var doTrackScroll = function()
				{
					if (trackScrollInc > 8 || trackScrollInc%4==0) {
						positionDrag((dragPosition - ((dragPosition - trackScrollMousePos) / 2)));
					}
					trackScrollInc ++;
				};
				var onStopTrackClick = function()
				{
					clearInterval(trackScrollInterval);
					jQuery('body').unbind('mouseup', onStopTrackClick).unbind('mousemove', onTrackMouseMove);
				};
				var onTrackMouseMove = function(event)
				{
					trackScrollMousePos = getPos(event, 'Y') - currentOffset.top - dragMiddle;
				};
				var onTrackClick = function(event)
				{
					initDrag();
					onTrackMouseMove(event);
					trackScrollInc = 0;
					trackScrollInterval = setInterval(doTrackScroll, 100);
					jQuery('body').bind('mouseup', onStopTrackClick).bind('mousemove', onTrackMouseMove);
				};

				var $track = jQuery('>.jScrollPaneTrack', $container);
				$track.bind('mousedown', onTrackClick);

				// if the mousewheel plugin has been included then also react to the mousewheel
				if ($container.mousewheel) {
					$container.mousewheel(
						function (event, delta) {
							initDrag();
							var d = dragPosition;
							positionDrag(dragPosition - delta * mouseWheelMultiplier);
							if (d != dragPosition) {
								if (event.preventDefault) {
									event.preventDefault();
								} else {
									event.returnValue = false;
								}
								return false;
							}
							return true;
						},
						false
					);
				}

				initDrag();

			} else {
				$this.css(
					{
						'height':paneHeight+'px',
						'width':paneWidth-this.originalSidePaddingTotal+'px',
						'padding':this.originalPadding
					}
				);
			}
		}
	)
}