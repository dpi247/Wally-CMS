$Id: README.txt,v 1.2 2010/03/18 22:57:45 63reasons Exp $

SuperCron must be installed in three steps.

* First, install the module as usual and enable it (the module is found in the
  Other section of the Modules page).
* Once installed, copy the supercron.php file from the module directory to the
  public root of your web installation (in the same directory as the existing
  cron.php)
* Update your crontab definitions to call supercron.php instead of the regular
  cron.php
* You can delete the original cron.php file if desired; supercron.php is a safer
  entry point

Cheers,

The 63reasons Team
