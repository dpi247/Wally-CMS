<?php


/**
 * Flush all caches remotely
 *
 * The main goal is to allow to remotely flushing the cache anonymously.
 * Access is granted by challenge/answer in the callback based on a secret passphrase.
 * 
 * @see wallytoolbox_page_remote_cache_clear_access()
 * @see _wallyscheduler_process_switch()
 *
 * @param string $challenge
 *   the challenge is the timestamp when the request url was built.
 *   We need a time stamp cause a challenge need to expire after two minutes.
 *
 * @return string 
 *   simple 'CACHE CLEAR' string
 *
 */
function wallytoolbox_page_remote_cache_clear($challenge,$answer){
	watchdog('Wallytoolbox','remote cache clear processed');
	drupal_flush_all_caches();
    echo 'CACHE CLEAR';

}