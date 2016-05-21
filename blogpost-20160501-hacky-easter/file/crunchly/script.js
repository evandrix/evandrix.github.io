
var serviceUrl = "http://hackyeaster.hacking-lab.com/hackyeaster/crunch?";
var urlstep;
var shorturl;
var ticket;

function showPanel2() {
  urlstep = 1;
  ticket = '';
  $("#url").val('');
  $("#url").prop("disabled", false);
  $("#panel1").hide();
  $("#panel2").show();
}

function showPanel1(passShortUrl) {
  $("#shorturl").val(passShortUrl);
  $("#panel1").show();
  $("#panel2").hide();
}

function urlButtonPressed() {
  if (urlstep == 1) {
	 // CHECK
   shorturl = $("#url").val();
	 $.getJSON(serviceUrl + "service=check&url=" + encodeURIComponent(shorturl), function(data) {
	  if (data.status == 2) {
		  alert("that's not a URL");
	  } else if (data.status == 1) {
		  alert("sorry, cannot shorten this url - the short url is already taken!");
	  } else if (data.status == 0) {
      shorturl = data.shorturl;
      ticket = data.ticket;
	    $("#url").val(shorturl);
	    $("#url").prop("disabled", true);
		  $("#urlbutton").val("save");
		  urlstep = 2;
	  }
	});	
  } else {
	// SAVE  
	 $.getJSON(serviceUrl + "service=save&ticket=" + encodeURIComponent(ticket), function(data) {
	  if (data.status == 3) {
		  alert("something went wrong, sorry");
	  } else if (data.status == 0) {
		  alert("your url was shortened, have fun!");
			showPanel1(shorturl.substring(17, shorturl.length));
	  }
	});
  }
}

function goButtonPressed() {
  var shorturl = $("#shorturl").val();
  if (shorturl && shorturl.length == 7) {
	shorturl = "http://crunch.ly/" + shorturl;
	$.getJSON(serviceUrl + "service=go&shorturl=" + encodeURIComponent(shorturl), function(data) {
	  if (data.status == 4) {
		  alert("short url not found");
	  } else if (data.status == 0) {
		  window.location.href = decodeURIComponent(data.url);
	  }
	
	
	});
  } else {
	alert("invalid short url");
  }
}
