#run logrotate every 5 minutes
2,7,12,17,22,27,32,37,42,47,52,57 * * * * root /usr/sbin/logrotate /etc/logrotate.conf
