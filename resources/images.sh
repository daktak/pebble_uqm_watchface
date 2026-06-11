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

do_convert() {
  postfix=$1
  convert $FILE -define png:preserve-colormap=true -alpha set /tmp/temp.png

  rm "${DEST_DIR}/${ship}~color.png"
  ffmpeg -i /tmp/temp.png -vf \
  "scale=-2:76:flags=lanczos,\
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

  #p2
  rm ${DEST_DIR}/${ship}~color~228h.png
  ffmpeg -i /tmp/temp.png -vf \
  "scale=-2:103:flags=lanczos,\
  unsharp=7:7:1.0:3:3:0.5,\
  eq=contrast=1.2" \
  ${DEST_DIR}/${ship}~color~228h.png
  #round2
  rm ${DEST_DIR}/${ship}~color~260h.png
  ffmpeg -i /tmp/temp.png -vf \
  "scale=-2:117:flags=lanczos,\
  unsharp=7:7:1.0:3:3:0.5,\
  eq=contrast=1.2" \
  ${DEST_DIR}/${ship}~color~260h.png
  rm /tmp/temp.png
}

for ship in ${SHIPS}; do
  FILE=$(find $HD4X_DIR -name $ship-big-000.png)
  do_convert _hd2x
  FILE=$(find $BASE_DIR -name $ship-big-000.png)
  do_convert
done
