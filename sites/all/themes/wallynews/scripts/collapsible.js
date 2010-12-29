function boxToggle(boxId) {
	// *** SETUP VARIABLES ***

	var box = $(boxId);
	// Get the first ahd highest heading (prioritising highest over first)
	var firstHeading = box.find("h1, h2, h3, h4, h5")[0];
	// Select the heading's ancestors
	var headingAncestors = $(firstHeading).parents();
	// Add in the heading
	var headingAncestors  = headingAncestors.add(firstHeading);
	// Restrict the ancestors to the box
	headingAncestors = headingAncestors.not(box.parents());
	headingAncestors = headingAncestors.not(box);
	// Get the siblings of ancestors (uncle, great uncle, ...)
	var boxContents = headingAncestors.siblings();


	// *** HIDE/SHOW LINK ***

	var toggleLink = $("<a href='#'></a>");
	toggleLink.insertAfter(firstHeading);


	// *** TOGGLE FUNCTIONS ***

	var hideBox = function() {
		toggleLink.one("click", function(){
			showBox();
			return false;
		})
		toggleLink.text("+ Discussion")
		toggleLink.attr("class", "box-toggle-show");

		boxContents.attr("style", "display:none");
	}

	var showBox = function() {
		toggleLink.one("click", function(){
			hideBox();
			return false;
		})
		toggleLink.text("- Discussion");
		toggleLink.attr("class", "box-toggle-hide");

		boxContents.removeAttr("style");
	}

	// Initiate
	hideBox();

}
