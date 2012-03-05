function getCookieVal(offset)
{
  var endstr=document.cookie.indexOf (";", offset);
  if (endstr==-1) endstr=document.cookie.length;
    return unescape(document.cookie.substring(offset, endstr));
}
function LireCookie(nom)
{
  var arg=nom+"=";
  var alen=arg.length;
  var clen=document.cookie.length;
  var i=0;
  while (i<clen)
  {
    var j=i+alen;
    if (document.cookie.substring(i, j)==arg) return getCookieVal(j);
    i=document.cookie.indexOf(" ",i)+1;
    if (i==0) break;

  }
  return null;
}

function createCookie(name,value,days,timespan) {
	defaultTime = days*24*60*60*1000 ;
	var date = new Date();
	date.setTime(date.getTime()+(defaultTime));
	var expires = "; expires="+date.toGMTString();
	document.cookie = name+"="+value+expires+"; path=/";
}

var path_splash = encodeURI(document.URL);
var cC = LireCookie('splash_active');
if(cC == null) {
	createCookie('splash_active',path_splash,1,true);
	document.location.href = '/splash' ;
}

