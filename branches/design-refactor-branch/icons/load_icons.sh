#!/bin/bash
echo "loading icons into the key database"
kdb set -t bin -b binary.png system/sw/kdbe/icons/binary
kdb set -t bin -b folder.png system/sw/kdbe/icons/dir
kdb set -t bin -b link.png system/sw/kdbe/icons/link
kdb set -t bin -b lock.png system/sw/kdbe/icons/noaccess
kdb set -t bin -b txt.png system/sw/kdbe/icons/string
kdb set -t bin -b unknown.png system/sw/kdbe/icons/undefined
