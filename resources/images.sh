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
  outfile=${ship}${postfix}

  if [[ -z ${postfix:-} ]]; then
    scale_expr="iw*2:ih*2"
  else
    scale_expr="-2:${h_base}"
  fi

  rm "${DEST_DIR}/${outfile}~color.png"
  ffmpeg -i /tmp/temp.png -vf \
"scale=${scale_expr}:flags=neighbor,\
split[s0][s1];\
[s0]palettegen=max_colors=64[p];\
[s1][p]paletteuse=dither=floyd_steinberg" \
  ${DEST_DIR}/${outfile}~color.png

  if [[ -z ${postfix:-} ]]; then
    rm "${DEST_DIR}/${outfile}~bw.png"
    convert "${DEST_DIR}/${outfile}~color.png" \
    -background black -alpha remove -alpha off \
    -colorspace Gray \
    -filter Triangle \
    -sigmoidal-contrast 6x50% \
    -dither FloydSteinberg \
    -remap pattern:gray50 \
    "${DEST_DIR}/${outfile}~bw.png"
  else
    convert -size 1x1 xc:black -type bilevel PNG8:"${DEST_DIR}/${outfile}~bw.png"
  fi

  #p2
  rm ${DEST_DIR}/${outfile}~color~228h.png
  ffmpeg -i /tmp/temp.png -vf \
"scale=-2:${h_228}:flags=neighbor,\
split[s0][s1];\
[s0]palettegen=max_colors=64[p];\
[s1][p]paletteuse=dither=floyd_steinberg" \
  ${DEST_DIR}/${outfile}~color~228h.png
  #round2
  rm ${DEST_DIR}/${outfile}~color~260h.png
  ffmpeg -i /tmp/temp.png -vf \
"scale=-2:${h_260}:flags=neighbor,\
split[s0][s1];\
[s0]palettegen=max_colors=64[p];\
[s1][p]paletteuse=dither=floyd_steinberg" \
  ${DEST_DIR}/${outfile}~color~260h.png
  rm /tmp/temp.png
}

for ship in ${SHIPS}; do
  BASE_FILE=$(find $BASE_DIR -name $ship-big-000.png | head -n1)
  if [[ ! -f $BASE_FILE ]]; then echo "no $ship"; exit 1; fi

  base_h=$(identify -format "%h" "$BASE_FILE")
  h_base=$((base_h * 2))
  h_228=$((h_base * 228 / 168))
  h_260=$((h_base * 260 / 168))

  FILE=$BASE_FILE
  do_convert

  HD4X_FILE=$(find $HD4X_DIR -name $ship-big-000.png | head -n1)
  if [[ ! -f $HD4X_FILE ]]; then echo "no hd4x $ship"; exit 1; fi
  FILE=$HD4X_FILE
  do_convert _hd2x
done
