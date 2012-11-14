function adtech_generate_tag(placementId,sizeId,kvinfo,keywords,other) {
	tag= 	'<!--  begin adtech  tag: '+placementId+'/'+sizeId+' -->';
	tag+=	'<script language="JavaScript" type="text/javascript">';
	tag+= 	'ord = (typeof(ord)!=\'undefined\') ? ord : Math.round(Math.random()*10000);';
	tag+= 	'grp = (typeof(grp)!=\'undefined\') ? grp : Math.round(Math.random()*10000);';
	tag+= 	'document.write(\'<scr\'+\'ipt language="JavaScript" src="http://adserver.adtech.de/addyn/3.0/927/'+placementId+'/0/'+sizeId+'/ADTECH;loc=100;target=_blank;';

	for(var key in kvinfo ) {
		if (kvinfo[key]!=0) {
			tag+=key+"="+kvinfo[key]+";";
		}
	}

	if (keywords.length>0) {
		tag+= 'key=';
		for (var i=0; i < keywords.length; ++i) {
			tag+=escape(keywords[i])
			if(i < keywords.length -1) {
				tag+='+';
			}
		}
		tag+=';';
	}

	tag+= 	'grp=\'+grp+\';ord=\'+ord+\'?" type="text/javascript"></scr\'+\'ipt>\');';
	tag+=	'</script>';
	tag+=	'<!-- End adtech tag -->';

	document.write(tag);
}
