#!/bin/bash
DEST_DIR=~/git/pebble_uqm_watchface/resources/images
HD4X_DIR=~/Downloads/hires4x/hires4x/ships/
BASE_DIR=~/Downloads/base/ships
SHIPS="avatar
avenger
blade
broodhome
cruiser
dreadnought
drone
eluder
fury
guardian
intruder
jugger
marauder
mauler
nemesis
penetrator
podship
probe
scout
skiff
stinger
terminator
torch
trader
turret
xform
ywing"
for ship in ${SHIPS}; do
  FILE=$(find $BASE_DIR -name $ship-big-000.png)
  rm "${DEST_DIR}/${ship}~color.png"
  convert $FILE -define png:preserve-colormap=true -alpha set /tmp/temp.png
  ffmpeg -i /tmp/temp.png -vf \
  "scale=iw*2:ih*2:flags=lanczos, \
 unsharp=7:7:1.0:3:3:0.5, \
 eq=contrast=1.2" \
  ${DEST_DIR}/${ship}~color.png
  rm "${DEST_DIR}/${ship}~bw.png"
  convert "${DEST_DIR}/${ship}~color.png" \
  -colorspace Gray \
  -filter Triangle \
  -sigmoidal-contrast 6x50% \
  -dither FloydSteinberg \
  -remap pattern:gray50 \
  "${DEST_DIR}/${ship}~bw.png"
  rm /tmp/temp.png
done
