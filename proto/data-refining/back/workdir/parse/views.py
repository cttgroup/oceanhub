# parse
import os

from django.http import HttpResponse

from ocean.models import Region

DATA_DIR = '/data/modis/'
MODIS_SERVER = "MODIS-Terra_L2_"


def parse(request, prefix=None, year=None, day=None, regionSlug=None, suffix=None):
    # Declaring variables
    response = HttpResponse()

    response.write('<h1>Parser</h1>')
    if prefix is not None:
        if prefix.lower() == 'test':
            # Define path to parsing data
            path = '/data/test'
            # Define test file name
            fileName = '2_sea_surface_temperature.nc'
            # Define type of model to parse
            parse = Parser()

            response.write(
                # Call parser and return its result to respone
                parse.parse_test_file(path, fileName, -1)
            )
            response.write("<br /><br />>Task quered<<br />")
        if prefix.lower() == 'bathymetric':
            response.write("> Quered parsing bathymetric <<br />")
            path = '/data'  # test data
            filename = 'bathymetric.20150318.nc'  # test file
            bth = Bathymetric()
            response.write(
                bth.parse_file(path, str(filename))
            )
            return response
    elif suffix is not None:
        # Parsing sea surface temperature
        if suffix.lower() == 'sst':
            parser = Parser()
            response.write(
                parser.parse_dir(
                    year, day, regionSlug, suffix, 'L2_LAC_SST')
            )
        if suffix.lower() == 'chlor_a':
            parser = Parser()
            response.write(
                parser.parse_dir(
                    year, day, regionSlug, suffix, 'L2_LAC_OC')
            )
    else:
        response.write("You need point what characteristic to parse <br />")

    return response

    """
    # Parsing: sea-ice cover, sea surface temperature, 2 metre temperature,
    # total cloud cover
    if attribute.lower() == 'bundle01':
        path = '/data/test/characteristics'  # test data
        filename = 'sea_ice_cover.nc'  # test file
        bundle = BundleOne()
        response.write(
            bundle.parse_file(path, str(filename), limit)
        )
    # Parsing sea surface wind speed
    elif attribute.lower() == 'seawindhdf':
        path = '/data/test/characteristics'  # test data
        filename = 'wind_speed.h5'  # test file
        wsh = WindSpeedHDF()
        response.write(
            wsh.parse_file(path, str(filename), limit)
        )
    # Parsing bathymetric: sea floor height (above mean sea level)
    elif attribute.lower() == 'bathymetric':
        path = '/data'  # test data
        filename = 'bathymetric.20150318.nc'  # test file
        bth = Bathymetric()
        response.write(
            bth.parse_file(path, str(filename), limit)
        )
    # Parsing chlorophyll concentration, OCI algorithm
    elif attribute.lower() == 'chlorophyll':
        path = '/data/modis/MODIS-Terra_L2_2016_244'
        chl = Chlorophyll()
        response.write(
            chl.parse_dir(path, limit)
        )
        response.write("jobs done <br />")
    # Parsing sea surface temperature
    if suffix.lower() == 'modis-terra-l2':
        path = '/data/' + year + '/' + day + '/prefix'
        sst = SST()
        response.write(
            sst.parse_dir(path, limit)
        )
        response.write("jobs done <br />")
    """



def parse_test(request, year, day, prefix, limit=-1):
    limit = int(limit)
    response = HttpResponse()

    response.write('<h1>Testing parser ' + str(prefix) + '...</h1>')

    # Parsing sea surface temperature
    if prefix.lower() == 'test':
        path = '/data/test'
        fileName = '2_sea_surface_temperature.nc'
        sst = SST()
        response.write(
            # sst.parse_test_file(path, fileName, limit)
            sst.parse_test_file(path, fileName)
        )
        response.write("<br />jobs done <br />")

    else:
        pass

    return response


def netcdf_parse(request, year, day):
    start_time = timeit.default_timer()  # execution time 4 testing
    response = HttpResponse()
    path = DATA_DIR + MODIS_SERVER + year + '_' + day
    points = {'added': 0, 'exists': 0, 'skipped_date': 0, 'skipped_region': 0}
    path = '/data/test/'  # test data, wich contains NorwSea points
    zout = ""

    if not os.path.exists(path):
        response.write('directory "' + path + '" not found!')
    else:
        f = []
        for (dirpath, dirnames, filenames) in os.walk(path):
            f.extend(filenames)
            break
        for filename in f:
            # add.delay(7, 8)

            response.write("File: " + str(filename) + "<br />")
            # result = sst_parse(
            # path, str(filename), sst_create, points["added"])
            # print("parsing...")
            zout = sst_parse(path, str(filename), sst_parse, points["added"])
            # print("ZOUT: ", zout)
            response.write("Out: " + str(zout) + "<br />")

            """
            result = {
                'added': 0, 'exists': 0, 'skipped_date': 0, 'skipped_region': 0
            }  # 4 testing
            points["added"] += result["added"]
            points["skipped_region"] += result["skipped_region"]
            points["exists"] += result["exists"]

            response.write(
                "Points added: " + str(result["added"]) + "<br />")
            response.write(
                "Points skipped region " +
                str(result["skipped_region"]) +
                "<br />"
            )
            response.write(
                "Points exists: " + str(result["exists"]) + "<br />")
            """
            response.write("------------------------------<br /><br />")

        """
        response.write(
            "Total points added: " + str(points["added"]) + "<br />")
        response.write(
            "Total points skipped region " +
            str(points["skipped_region"]) +
            "<br />"
        )
        response.write(
            "Total points exists " + str(points["exists"]) + "<br />")
        """

    elapsed = timeit.default_timer() - start_time  # execution time 4 testing
    response.write("Execution time: " + str(elapsed) + "<br />")
    response.write("jobs done!")
    return response
