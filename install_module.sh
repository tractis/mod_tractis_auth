#!/bin/sh
APACHE_HOME=/usr/local/apache2/
sudo $APACHE_HOME/bin/apxs -c -i -n mod_tractis_auth mod_tractis_auth.c
