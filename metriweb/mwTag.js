/* MetriWeb Javascript file */
/* Version 5.1 -- release 1.0 -- 2010-01-06 */

/* Please, do NEVER modify this monitored file! */

var mw=new Object({
V:"5.1",
R:"1.0",
i:new Image(),
p:0,
l:0,
D:".metriweb.be",
belgian:1,
r:function(u1,u2){
	if(mw.i.width){
		if(mw.i.width>1){
			var nS=document.createElement("script");
			nS.setAttribute("src", u1+"td"+u2+"&w="+mw.i.width+'&T='+new Date().getTime());
			document.getElementsByTagName("head").item(0).appendChild(nS);
		}
	}
	else
		setTimeout("mw.r('"+u1.replace(/'/,"\\'")+"','"+u2.replace(/'/,"\\'")+"')",1000);
},
W:function(u1,u2){
	if(mw.l){
		if(mw.l++>1){
			var nS=document.createElement("script");
			nS.setAttribute("src", u1+"sd"+u2+'&T='+new Date().getTime());
			document.getElementsByTagName("head").item(0).appendChild(nS);
		}else mw.r(u1,u2)
	}else{
		mw.i.src=u1+"dyn"+u2;
		if(!mw.p){
			if(window.addEventListener)window.addEventListener('load',function(){mw.l++;mw.W(u1,u2)},false);
			else if(window.attachEvent)window.attachEvent('onload',function(){mw.l++;mw.W(u1,u2)});
		}
	}
},
n:function(d){
	var k="0000";
	var t=d.split(".");
	if(parseInt(k+t[t.length-1]))mw.d=d;
	else mw.d="."+t[t.length-2]+"."+t[t.length-1];
	t="charCodeAt(0)";
	var c=(k+eval("\""+mw.d.split("").join("\"."+t+"+\"")+"\"."+t));
	t=k+k+k+k+new Date().getTime();
	t=t.substr(t.length-16,16);
	return t.substr(0,13)+"."+t.substr(13,3)+c.substr(c.length-3,3)+".5";
},
g:function(s,f){
	var c;
	var o=document.cookie.indexOf(s+"=");
	if(o<0){
		c=f(document.domain);
		document.cookie=s+"="+c+"; domain="+mw.d+"; path=/; expires="+
			new Date(new Date().getTime()+365*24*3600000).toGMTString()+";";
	}else{
		var e=document.cookie.indexOf(";",o);
		if(e<0)e=document.cookie.length;
		c=document.cookie.substring(o+4,e);
	}
	return c;
},
H:function(){
	var e=document.getElementById("metriwebLayer");
	if (e)
		e.style.visibility="hidden";
},
e:window.screen?window.screen.width+"x"+window.screen.height+"/"+window.screen.colorDepth:"-",
m:"0x0",
t:undefined,
k:undefined,
x:undefined,
s:undefined,
PR:0,
PV:0,
to:0,
pr:document.referrer?escape(document.referrer):"-",
RIA:function(action,keyword,origin,into){
	mw.W('http://'+mw.t+mw.D+'/','/'+mw.t+'/mw.cgi?page='+keyword+
		(mw.x?('&q='+mw.x):'')+(mw.s?('&s='+mw.s):'')+
		'&c='+mw.c+'&v='+mw.V+"."+mw.R+'&p='+mw.p+'&d='+Math.round(new Date().getTime())+
		'&e='+mw.e+'&a='+action+'&r='+mw.pr+'&f='+mw.f+
		'&m='+mw.m+'&o='+origin+'&i='+into+'&R='+Math.random());
},
em:function(e) {
	if(!e)var e=window.event;
	mw.m=e.screenX+"x"+e.screenY;
},
eb:function() {
	mw.PV=new Date().getTime();
	if(mw.to){clearTimeout(mw.to);mw.to=0};
},
ef:function() {
	if(!mw.PV){
		if(new Date().getTime()-mw.PR>15e3){
			if(mw.to){clearTimeout(mw.to);mw.to=setTimeout("mw.to=0;mw.RIA('page.idle','"+mw.k+"','','')",3e4);};
			mw.RIA("page.view",mw.k,"","");
		}
		mw.PV=new Date().getTime();
	}
}
});
mw.c=mw.g("mwc",mw.n);
if(window.addEventListener){
	window.addEventListener('focus',mw.ef,false);
	window.addEventListener('blur',mw.eb,false);
	window.addEventListener('mousemove',mw.em,false);
}else if(window.attachEvent){
	window.attachEvent('onfocus',mw.ef);
	window.attachEvent('onblur',mw.eb);
	window.attachEvent('onmousemove',mw.em);
}
function metriwebTag (tag,keyword,extra,refresh){
	if(mw.belgian&&!mw.p){
		try{
			mw.t=tag.replace(/[^a-zA-Z0-9\-/]/g,"").substring(0,24);
			mw.k=keyword.replace(/[^a-zA-Z0-9_/]/g,"").substring(0,24);
			if(extra.match(/en|fr|nl|ge/)){mw.x=extra}else mw.x="na";
			mw.s=refresh?parseInt(refresh):undefined;
			mw.PR=new Date().getTime();
			mw.to=setTimeout("mw.to=0;mw.RIA('page.idle','"+mw.k+"','','')",3e4);
			mw.f=(mw.t==tag?0:1)+(mw.k==keyword?0:2)+(mw.x==extra?0:4)+(mw.s==refresh?0:8);
			mw.RIA("page.request",mw.k,"","");
			mw.p++;
		}catch (e){}
	}
}
function metriwebRIA (action,keyword,origin)
{
	if(mw.belgian&&mw.PR){
		try{
			a=action.replace(/[^a-zA-Z0-9\./]/g,"").substring(0,24);
			k=keyword?keyword.replace(/[^a-zA-Z0-9_/]/g,"").substring(0,24):mw.k;
			o=origin?origin.replace(/[^a-zA-Z0-9\-/]/g,"").substring(0,24):"";
			if(mw.to){clearTimeout(mw.to);mw.to=0};
			mw.RIA(a,k,o,mw.k);
			mw.p++;
		}catch (e){}
	}
}

/* (c) 2000-2010, DouWère/MetriWare */
