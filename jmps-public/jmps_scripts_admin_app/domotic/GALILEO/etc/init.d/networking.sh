#!/bin/sh
ip_address="192.168.1.12"
mask_address="255.255.255.0"

iface="enp0s20f6"

while test 1 -eq 1
do
        ip_configured=$(ifconfig "$iface" | grep "inet" | awk -F':' '{printf "%s", $2}' | awk '{printf "%s", $1}')
        if test -z "$ip_configured" -o "$ip_configured" != "$ip_address"
        then
                echo "trying to set ipaddress from [$ip_configured] to [$ip_address]..."
                echo "ifconfig \"$iface\" \"$ip_address\" netmask \"$mask_address\" up"
                ifconfig "$iface" "$ip_address" netmask "$mask_address" up
        else
                echo "ok [$ip_address]."
        fi
        sleep 2
done

exit 0
