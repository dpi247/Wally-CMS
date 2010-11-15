<VirtualHost *:80>
	ServerName 3ptest.3pommes.org:80
	ServerAdmin admin@example.com
	DocumentRoot "/Library/WebServer/Documents/3ptest"
	DirectoryIndex "index.html" "index.php"
	CustomLog "/var/log/apache2/access_log" "%h %l %u %t \"%r\" %>s %b"
	ErrorDocument 404 /error.html
	<IfModule mod_ssl.c>
		SSLEngine Off
		SSLCipherSuite "ALL:!aNULL:!ADH:!eNULL:!LOW:!EXP:RC4+RSA:+HIGH:+MEDIUM"
		SSLProtocol -ALL +SSLv3 +TLSv1
		SSLProxyProtocol -ALL +SSLv3 +TLSv1
	</IfModule>
	<IfModule mod_dav.c>
		DAVLockDB "/var/run/davlocks/.davlock100"
		DAVMinTimeout 600
	</IfModule>
	<IfModule mod_mem_cache.c>
		CacheEnable mem /
		MCacheSize 4096
	</IfModule>
	<Directory "/Library/WebServer/Documents/3ptest">
		Options All -Includes -ExecCGI -Indexes +MultiViews
		<IfModule mod_dav.c>
			DAV Off
		</IfModule>
		AllowOverride All
		AuthName "3P TEST ZONE"
		AuthType Basic
		<Limit PUT DELETE PROPPATCH PROPFIND MKCOL COPY MOVE LOCK UNLOCK>
			Require no-user
		</Limit>
		<Limit GET HEAD OPTIONS CONNECT POST>
			Require user  3ptestman
		</Limit>
		<Limit PROPFIND>
			Require no-user
		</Limit>
	</Directory>
	<IfModule mod_proxy_balancer.c>
#		ProxyPass / balancer://balancer-group/
#		ProxyPassReverse / balancer://balancer-group
		<Proxy "balancer://balancer-group">
		</Proxy>
	</IfModule>
	<IfModule mod_alias.c>
		Alias "/collaboration" "/usr/share/collaboration"
		Alias "/icons/" "/usr/share/httpd/icons/"
		Alias "/error/" "/usr/share/httpd/error/"
	</IfModule>
#	Include /etc/apache2/httpd_users.conf
#	Include /etc/apache2/httpd_groups.conf
#	Include /etc/apache2/httpd_webcalendar.conf
#	Include /etc/apache2/httpd_pwdchange_required.conf
#	Include /etc/apache2/httpd_emailrules_required.conf
#	Include /etc/apache2/httpd_teams_required.conf
	LogLevel warn
#	ProxyPass /webcal http://127.0.0.1:8008/webcal retry=5
#	ProxyPass /calendars http://127.0.0.1:8008/calendars retry=5
#	ProxyPass /principals http://127.0.0.1:8008/principals retry=5
#	ProxyPass /timezones http://127.0.0.1:8008/timezones retry=5
#	ProxyPass /EMAILRULES http://127.0.0.1:8089/RPC2 retry=5
#	ProxyPass /emailrules-config http://127.0.0.1:8089/config retry=5
#	ProxyPassReverse /webcal http://127.0.0.1:8008/webcal
#	ProxyPassReverse /calendars http://127.0.0.1:8008/calendars
#	ProxyPassReverse /principals http://127.0.0.1:8008/principals
#	ProxyPassReverse /timezones http://127.0.0.1:8008/timezones
#	ProxyPassReverse /emailrules-config http://127.0.0.1:8089/config
</VirtualHost>
