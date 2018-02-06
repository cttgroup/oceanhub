#!/usr/bin/python

from ctypes import *
mylib = CDLL("./libncparse.so")
mylib.test("Hello")
#should print hello

#char* parseMODIS(const char *ncpath,
#                 const char *areaURI,
#                 const int nc_epsg,
#                 const float nc_spres,
#                 const float dx,
#                 const float dy)

ncpath=c_char_p("/home/max/test/T2016244000500.L2_LAC_SST.nc")
#areaURI="file:///home/max/projects/files/GEO/shapes/GSHHG-2.3.4/GSHHS_shp/c/GSHHS_c_L1.shp"
#st_asEWKT(geom)
areaURI=c_char_p("wkt://SRID=4326;POLYGON ((-20 -18,-20 -8,-20 -8,-20 -25,-24 -25))")
nc_epsg=c_int(4326)
nc_spres=c_float(1000.0)
dx=c_float(10000.0)
dy=c_float(10000.0)
output = c_char_p(mylib.parseMODIS(ncpath, areaURI, nc_epsg, nc_spres, dx, dy))
print (output)

ooo = c_char_p("lalala")
print (ooo)


#mylib.clearOutput(output);
