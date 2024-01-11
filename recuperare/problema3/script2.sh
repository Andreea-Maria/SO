#!/bin/bash

mac_address="001122334455"
formatted_mac=$(echo $mac_address | sed 's/../&:/g; s/:$//')

echo "Adresa MAC originala: $mac_address"
echo "Adresa MAC formata: $formatted_mac"
