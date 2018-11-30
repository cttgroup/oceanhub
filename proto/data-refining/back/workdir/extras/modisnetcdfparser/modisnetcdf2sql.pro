CONFIG -= qt
CONFIG += c++11
CONFIG += console
CONFIG -= import_qpa_plugin import_plugins qpa
#CONFIG += shared
#TEMPLATE += plugin
#DEFINES+= BUILDLIB

INCLUDEPATH+=/home/max/git/tamarin/ext_libs_build/gdal/NetCDF/netcdf-4.3.2/install/include
INCLUDEPATH+=/home/max/git/tamarin/ext_libs_build/gdal/gdal-2.1.0/install/release/include
LIBS+=-L/home/max/git/distr/tamarin/lib -lgdal -lnetcdf -lhdf5_hl -lhdf5

QMAKE_CXXFLAGS += -Wno-unused-function
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
QMAKE_CXXFLAGS += -Wno-unused-parameter

TEMP_DIR = build-temp
OBJECTS_DIR = $$TEMP_DIR
TARGET = ncparse

SOURCES += \
    main.cpp \
    logger.cpp \
    parsers/DatasetValue4Time1Depth1Lat1Lon1.cpp \
    parsers/DatasetAbstract.cpp \
    parsers/DatasetElevation.cpp \
    parsers/DatasetTotalCloudCoverage.cpp \
    parsers/DatasetTwoMetreTemperature.cpp \
    parsers/DatasetSwath.cpp \
    parsers/DatasetSeaSurfaceTemperatureSwath.cpp \
    parsers/DatasetWindSpeed.cpp \
    parsers/DatasetIron.cpp \
    parsers/DatasetCarbon.cpp \
    parsers/DatasetSilicate.cpp \
    parsers/DatasetNitrate.cpp \
    parsers/DatasetChlorophyllSwath.cpp \
    parsers/DatasetChlorophyll41111.cpp \
    parsers/DatasetPhytoplanktonCarbon.cpp \
    parsers/DatasetPhosphate.cpp \
    parsers/DatasetOxygen.cpp \
    parsers/DatasetSeaSurfaceTemperature41111.cpp \
    parsers/DatasetSalinity.cpp \
    parsers/DatasetIceThickness.cpp \
    parsers/DatasetPotentialTemperature.cpp \
    parsers/DatasetIceConcentration.cpp \
    parsers/DatasetEastwardVelocity.cpp \
    parsers/DatasetRadiativeAttenuation.cpp \
    parsers/DatasetPotentialSeaFloorTemperature.cpp \
    parsers/DatasetHeightAboveGeoid.cpp \
    parsers/DatasetDensityOceanMixedLayerThickness.cpp \
    parsers/DatasetNorthwardVeclocity.cpp \
    parsers/DatasetIceNorthwardVelocity.cpp \
    parsers/DatasetIceEastwardVelocity.cpp \
    parsers/DatasetIceCover.cpp

HEADERS += \
    OGRReader.h \
    testHit.h \
    GdalTransform.h \
    attributes.h \
    ComponentNT.h \
    floatops.h \
    VectorLayerShapeType.h \
    stringsplit.h \
    coordcvt.h \
    Merger.h \
    readVariable.h \
    logger.h \
    interpolation.h \
    square.h \
    timefun.h \
    parsers/DatasetValue4Time1Depth1Lat1Lon1.h \
    parsers/DatasetAbstract.h \
    parsers/DatasetElevation.h \
    parsers/DatasetTotalCloudCoverage.h \
    parsers/DatasetTwoMetreTemperature.h \
    parsers/DatasetSwath.h \
    parsers/DatasetSeaSurfaceTemperatureSwath.h \
    parsers/DatasetWindSpeed.h \
    parsers/DatasetIron.h \
    parsers/DatasetCarbon.h \
    parsers/DatasetSilicate.h \
    parsers/DatasetNitrate.h \
    parsers/DatasetChlorophyllSwath.h \
    parsers/DatasetChlorophyll41111.h \
    parsers/DatasetPhytoplanktonCarbon.h \
    parsers/DatasetPhosphate.h \
    parsers/DatasetOxygen.h \
    parsers/DatasetSeaSurfaceTemperature41111.h \
    parsers/DatasetSalinity.h \
    parsers/DatasetIceThickness.h \
    parsers/DatasetPotentialTemperature.h \
    parsers/DatasetIceConcentration.h \
    parsers/DatasetEastwardVelocity.h \
    parsers/DatasetRadiativeAttenuation.h \
    parsers/DatasetPotentialSeaFloorTemperature.h \
    parsers/DatasetHeightAboveGeoid.h \
    parsers/DatasetDensityOceanMixedLayerThickness.h \
    parsers/DatasetNorthwardVeclocity.h \
    parsers/DatasetIceNorthwardVelocity.h \
    parsers/DatasetIceEastwardVelocity.h \
    parsers/DatasetIceCover.h
