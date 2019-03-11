#! /bin/bash

for f in ./gfs.t00z.pgrb2.0p50.f*; do
	wgrib2  -match "PRMSL:mean" -ieee "${f//pgrb2/ieee}" \
		-little_endian "$f"
done
