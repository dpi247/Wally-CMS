$(document).ready(function(){
	var browser = "other";
	$.each($.browser, function(i, val) {
		if (i == "mozilla" && val == true) {
			browser = "mozilla";
		}
	});
	
	$('<div id="follower" style="display:none;"></div>').insertAfter('body');
	var tstamp = 0;
	var nid, cur_nid;

	if (browser == "mozilla") {
		$("span.wallystat_follower").mouseover(function(e) {
			nid = this.id;
			if (updateFollower(e, tstamp, nid, cur_nid)) {
				cur_nid = nid;
				tstamp = e.timeStamp;
			}
		});

		$("#follower").mouseover(function(e) {
			if (updateFollower(e, tstamp, nid, cur_nid)) {
				cur_nid = nid;
				tstamp = e.timeStamp;
			}
		});
	} else {
		$("span.wallystat_follower").mousemove(function(e) {
			nid = this.id;
			if (updateFollower(e, tstamp, nid, cur_nid)) {
				cur_nid = nid;
				tstamp = e.timeStamp;
			}
		});

		$("#follower").mousemove(function(e) {
			if (updateFollower(e, tstamp, nid, cur_nid)) {
				cur_nid = nid;
				tstamp = e.timeStamp;
			}
		});
	}

	$("span.wallystat_follower").mouseout(function(e){
		$("#follower").hide();
	});
});

function updateFollower(e, tstamp, nid, cur_nid){
	var update = false;
	if ((e.timeStamp > (tstamp + 5*1000)) || (nid != cur_nid)) {
		update = true;
		$.ajax({
			url: "/wallystat/showstats/"+nid,
			cache: false,
			async: false,
			complete: function(data) {
				$("#follower").html(data.responseText);
			}
		});
	}
	$("#follower").show();
	$("#follower").css({
		position: "absolute",
		display: "block",
		top: e.pageY + "px",
		left: e.pageX + "px"
	});
	return update;
}
