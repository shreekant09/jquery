 $(document).ready(function(){
 	
	jQuery.validator.addMethod("password", function( value, element, param ) {
		var result = this.optional(element) || value.length >= 6 && /\d/.test(value) && /[a-z]/i.test(value);
		if (!result) {
			element.value = "";
			var validator = this;
			setTimeout(function() {
				validator.blockFocusCleanup = true;
				element.focus();
				validator.blockFocusCleanup = false;
			}, 1);
		}
		return result;
	}, "Your password must be at least 6 characters long and contain at least one number and one character.");
	
	jQuery.validator.messages.required = " ";
	$("form").validate({
		focusInvalid: false,
		focusCleanup: true,
		onkeyup: false,
		subformRequired: function(input) {
			return $("#bill_to_co").is(":checked") && input.parents(".subTable").length;
		},
		invalidHandler: function() {
			$("div.error").show();
			var errors = this.numberOfInvalids();
			var message = errors < 2
				? 'You missed 1 field. It has been highlighted below'
				: 'You missed ' + errors + ' fields.  They have been highlighted below';
			$("div.error span").html(message);
		},
		submitHandler: function() {
			$("div.error").hide();
			alert("submit! use link below to go to the other step");
		},
		messages: {
			password2: {
				required: " ",
				equalTo: "Please enter the same password as above"	
			},
			email: {
				required: " ",
				email: "Please enter a valid email address, example: you@yourdomain.com",
				remote: jQuery.format("{0} is already taken, please enter a different address.")	
			}
		}
	});
	
  $(".resize").vjustify();
  $("div.buttonSubmit").hoverClass("buttonSubmitHover");

  if ($.browser.safari) {
    $("body").addClass("safari");
  }
  
  $("input.phone").mask("(999) 999-9999");
  $("input.zipcode").mask("99999");
  var creditcard = $("#creditcard").mask("9999 9999 9999 9999");

  $("#cc_type").change(
    function() {
      switch ($(this).val()){
        case 'amex':
          creditcard.unmask().mask("9999 999999 99999");
          break;
        default:
          creditcard.unmask().mask("9999 9999 9999 9999");
          break;
      }
    }
  );

  $("form select").change(function() {
      $(this).parents("tr:first").removeClass("errorRow");
  });

  $("input.ccNumber").blur(function() {
      hiddenStrValue = $(this).val().replace(new RegExp("[^0-9]{1,}", "gi"), "");
      $(this).siblings("input.hidden").val(hiddenStrValue);
  });

  var subTableDiv = $("div.subTableDiv");
  var toggleCheck = $("input.toggleCheck");
  toggleCheck.is(":checked")
  	? subTableDiv.hide()
	: subTableDiv.show();
  $("input.toggleCheck").click(function() {
      if (this.checked == true) {
        subTableDiv.slideUp("medium");
      } else {
        subTableDiv.slideDown("medium");
      }
  });


});

$.fn.vjustify = function() {
    var maxHeight=0;
    $(".resize").css("height","auto");
    this.each(function(){
        if (this.offsetHeight > maxHeight) {
          maxHeight = this.offsetHeight;
        }
    });
    this.each(function(){
        $(this).height(maxHeight);
        if (this.offsetHeight > maxHeight) {
            $(this).height((maxHeight-(this.offsetHeight-maxHeight)));
        }
    });
};

$.fn.hoverClass = function(classname) {
	return this.hover(function() {
		$(this).addClass(classname);
	}, function() {
		$(this).removeClass(classname);
	});
};