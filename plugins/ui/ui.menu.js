/* jQuery UI Menu
 * 
 * m = menu being passed in
 * o = options
 * t = trigger functions
 */

(function($){
	
	$.fn.menu = function(m,o,t) {	// Constructor for the menu method
		return this.each(function() {
			new $.ui.menu(this, m, o, t);	
		});
	}	
	
	$.ui.menu = function(el, m, o, t) {
		var options = $.extend({
			delay: 500,
			timeout: 2000,
			context: 'clickContext',
			show: {opacity:'show'},
			hide: {opacity:'hide'},
			speed: 'normal'
		}, o);
		
		$(m).appendTo(el);
		this.styleMenu(m);	// Pass the menu in to recieve it's makeover
		this[options.context](el, m, options);	// Based on contexted selected, attach to menu parent
		
    	if(t&&t.buttons)
      		$('a',$(m)).click(function(){
	  		if (t.buttons[this.className])
        		t.buttons[this.className]();
      	});
	}
	
	$.extend($.ui.menu.prototype, {
		styleMenu : function(m){
			$(m).addClass('ui-menu-nodes').children('li').addClass('ui-menu-node');
			var nodes = $('ul',m).addClass('ui-menu-nodes')
				.css('MozUserSelect', 'none');
			var node = $('li',m).addClass('ui-menu-node')
				.css('MozUserSelect', 'none');
			return false;
		},
		clickContext : function(a,m,o) {
			var self = this;
			$(a).click(function(){
				x = $(a).position();
				y = x.bottom + ( $(a).height() + 1);
				$(m).css({position:'absolute', top: y, left: x.left})
				.animate(o.show, o.speed);
				$('a', m).hover(
					function(){
						x = $(this).position();
						$(this).parent('li').find('>ul').css({position:'absolute', top:x.top, left:$(m).width()})
								.animate(o.show,o.speed);
					},
					function(){
						$(this).parent('li').find('>ul').animate(o.hide,o.speed);
					});
			});
			return false;
		},
		hoverContext : function(a,m,o) {
			var self = this;
			$(a).hover(function(){
				x = $(a).position();
				y = x.top + ( $(a).height() + 1);
				$(m).css({position:'absolute', top: y, left: x.left})
				.animate(o.show, o.speed);
				$('a', m).hover(
					function(x,y){
						x = $('li', m).position();
						y = x.bottom + ( $('li', m).height() + 1);
						$(this).parent('li').find('>ul').css({position:'absolute', top:y, left:x.left})
								.animate(o.show,o.speed);
					},
					function(){
						$(this).parent('li').find('>ul').animate(o.hide,o.speed);
					});
				},
			function(){
				self.hideMenu(m,o);
			});
			return false;
		},
		context : function (a,m,o) {	
			var self = this;
			$(m).prepend('<span>' + o.title + '</span>');
			$(a).bind('mouseup', function(e){
				if (e.button == 2 || e.button == 3) {
					e.preventDefault();	//FIXME: Not stopping right-click menu in FF
					e.stopPropagation();
					x = $(a).position();
					elBottom = x.top + $(a).height();
					$(m).css({position:'absolute', top: e.clientY, left: e.clientX});
					$(m)[o.show](o.speed, function(){
						self.showChild(m, o);
						$(window).bind('click', function(){
							self.hideMenu(m, o);
							$(window).unbind('click');
						})
					});
					return false;
				} else {
					
				}
			});
			return false;			
		},
		showChild : function(m, o) {
			self = this;
				x = $(this).position();
				y = $(this).width();
				$(m).children('ul').css({position:'absolute', top:x.top, left:y})
				.animate(o.show,o.speed);
		},
		hideMenu : function(m, o){
			$(m).animate(o.hide,o.speed);
			return false;
		}
		
	});
})($);
