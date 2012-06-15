Edge Side Includes
==================


CONTENTS OF THIS FILE
---------------------

 * About ESI
 * Requirements
 * Configuration
 * API Overview
 * Technical Details


ABOUT ESI
---------

ESI is a high performance caching solution for Authenticated users. Typically,
pages which are personalized for authenticated users (even minor
personalizations, such as a block which says "Logged in as Admin") will
prevent high-performance reverse-proxies from efficiently caching the page,
because messages intended for one user could then be seen by another. ESI
addresses this issue by allowing personalized sections to be replaced with a
special tag which is interpreted by the proxy or server. These sections can be
a block or a panel pane currently.


REQUIREMENTS
------------

Use of the ESI module requires a reverse proxy with ESI support (Varnish); or a
server that supports Server Side Includes (Nginx); or a browser that supports
JavaScript (AJAX).


CONFIGURATION
-------------

URI: admin/settings/esi
Note that some of these options may not be available depending on your
selections. Form is JavaScripted so it will auto hide settings that do not
matter.

### Global Settings

These are settings that change how ESI works on your entire site.
* Mode: Disabled, ESI, SSI, AJAX.
* Use AJAX if ESI is disabled: If checked server will use AJAX calls as a
  fallback in case ESI didn't include the HTML snippet.
* Use a CDN for AJAXed fragments: Have AJAX requests be routed through the CDN.
* Use Core's page cache to store ESI fragments: Store the HTML snippets inside
  of the cache_page table and serve it during the hook_boot phase.
* Default seed key rotation interval: How often the seed key should change
  (in seconds). Defaults to daily.
* Place user & roles keys in the URL: By default CACHE=* will get replaced
  client side (if ajax) or server side (if using varnish rules). If dynamic
  replacement is not an option; this can be done when the page is generated.
  Keeping this disabled allows for the same page to be served to different
  users (Authenticated Page Caching).


### Block settings

These are settings that change how ESI works by default for blocks.
* Cache Maximum Age (TTL): External page caches (proxy/browser) will not
  deliver cached paged older than this setting; time to live in short.
* Default Block Cache Scope:
  * Disabled - Do not use ESI.
  * Not Cached - Use ESI, but never cache the content.
  * Global - Content is same on every page.
  * Page - Content changes based on the URL.
  * User Role - Content changes based on the user role.
  * User Role/Page - Content changes based on the user role as well as the URL.
  * User ID - Content changes based on the UID; otherwise it is the same as
    global.
  * User ID/Page - Content changes based on the UID and based on the URL.

### Panels settings

These are settings that change how ESI works by default for Panels.
* Cache Maximum Age (TTL): External page caches (proxy/browser) will not
  deliver cached paged older than this setting; time to live in short.
* Default Panel Cache Scope:
  * Disabled - Do not use ESI.
  * Not Cached - Use ESI, but never cache the content.
  * Global - Content is same on every page.
  * Page - Content changes based on the URL.
  * User Role - Content changes based on the user role.
  * User Role/Page - Content changes based on the user role as well as the URL.
  * User ID - Content changes based on the UID; otherwise it is the same as
    global.
  * User ID/Page - Content changes based on the UID and based on the URL.


### Role settings

* Roles included in User Role scope: Choose the roles that will be included in
  the roles hash. Only select roles that may affect caching. If you select no
  roles, all roles will be candidates.


### Clear ESI Cache (cache_page cache)

* Flush ESI: cache_page Cache: This only clears out the cache in the cache_page
  cache. Varnish and browser caches are not cleared by this action.


API OVERVIEW
------------

If you are using a custom panel pane style you need to add this to the top of
your render pane functions.

    // Return ESI Tag if it exists in the content.
    if (module_exists('esi')) {
      if (esi_theme_is_esied($content->content)) {
        return;
      }
    };


TECHNICAL DETAILS
-----------------

### ESI Output

The ESI module will replace blocks/panes with (ESI/SSI) include tags, or with a
div for AJAX replacement. The tag will be replaced with content after it has
left Drupal's PHP instance. Replacement happens on the server with ESI/SSI and
in the browser with AJAX. The proxy (ESI) or server (SSI) should be configured
to cache the content appropriately for the cache configuration, so that
blocks/panes which change per-user/role/page have separate caches for each
context. The example VCL demonstrates how this is done with Varnish. AJAX has
the correct expires headers set so you shouldn't have to worry about this.


#### ESI include tags

Look like this:
    <esi:include src="/esi/..." />


#### ESI include tags with AJAX fallback

Look like this:
    <esi:include src="/esi/..." />
    <esi:remove><div id="..." class="esi-ajax"></div></esi:remove>


#### SSI include tags

Look like this:
    <!--# include virtual="/esi/..."  -->


#### AJAX divs

Look like this:
    <div id="..." class="esi-ajax"></div>


### Cookies

To support caching of user & role based content, the proxy/server/browser needs
a way of recognizing which roles a user has, or what user is making the request.
In esi_init() 2 cookies are checked to see if they exist. If they do not exist
and the user is logged in, they will be created. The role cookie is created by
using a unique hash for each combination of roles; for example, all users who
have no role will have hash a; users who are in role foo (and only role foo)
will have hash b; users who are in role foo and role bar will have hash c; etc.
The proxy has no way of interpreting which roles a user has, but can distinguish
each unique combination of roles. The user cookie is unique to that user ID.
Both cookies are salted and the salt is changed on cron.


### Varnish VCLs

Five VCLs are currently provided inside of the docs folder:

* esi_blocks.vcl
  This VCL provided custom sub-routines to handle ESI-block integration.
  This is designed to be included from another VCL.
  * esi_blocks-2_0.vcl for people that are using varnish < 2.1
  * esi_blocks-3_0.vcl for people that are using varnish >= 3
* default.vcl
  This is an example default.vcl, showing how the ESI-blocks VCL can be
  included.
  * default-3_0.vcl for people that are using varnish >= 3

### Nginx Configuration

http://groups.drupal.org/node/197478

