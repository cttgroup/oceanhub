import os
import json
import datetime
import timeit
from random import randint, uniform

import numpy as np
# import dateutil.parser
from dateutil.rrule import rrule, DAILY

# from netCDF4 import Dataset
from celery.decorators import task
from celery.utils.log import get_task_logger
from django.contrib.gis.geos import Point
from django.utils.timezone import localtime

from app.settings import DATA_DIR
from ocean.models import Region
from parse.tasks import parse_using_extras_task
from app.settings import DEBUG

logger = get_task_logger(__name__)


def parse_byday(
        dateStart, dateEnd,
        model, tag, storePrefix, fileMask, regionSlug):
    out = ''

    dateEnd = dateEnd if dateEnd else dateStart
    for dt in rrule(DAILY, dtstart=dateStart, until=dateEnd):
        date = str(dt.strftime("%Y-%m-%d"))
        path = DATA_DIR + 'byday/' + date + '/' + storePrefix
        workDir = DATA_DIR + 'extras/byday/' \
            + date + '/' + storePrefix
        commonLogFile = workDir + '/common.log'
        dateTime = datetime.datetime.now()

        out += dateTime.strftime('[%Y%m%d-%H:%M:%S] ')
        # Checking if directory with data to parse exists
        if not os.path.exists(path):
            out += 'directory "' + str(path) + '" not found!'
            out += '\n\n'
            print("No data found for parsing")
        # Do parsing
        else:
            # Check and create working directories
            if not os.path.exists(workDir):
                os.makedirs(workDir)

            f = []
            for (dirpath, dirnames, filenames) in os.walk(path):
                f.extend(filenames)
                break
            for fileName in f:
                if fileName.find(fileMask) > 0 or fileMask == '':
                    print("PARSING FILE: ", fileName)  # 4 testing
                    region = Region.objects.get(slug=regionSlug)
                    input_ = '--input=' + path + '/' + fileName
                    output_ = '--output=' + workDir + '/' \
                        + fileName + '.txt'
                    shape = '--shape=wkt://' + \
                        str(region.mpoly).strip()
                    dataset = '--dataset=' + tag
                    logFile = '--logfile=' + workDir + '/' \
                        + fileName + '.log'
                    since = '--since=' + str(dateStart) + ' ' \
                        + '00:00:00'
                    till = '--till=' + str(dateEnd) + ' ' \
                        + '23:00:00'
                    # out = parse_using_extras_task(  # 4 testing
                    parse_using_extras_task.delay(
                        model, tag, input_, output_, shape,
                        dataset, date, logFile, commonLogFile,
                        since, till,)
            print("Ended with parsing query")
            out += 'job quered for directory "' + str(path) +'"'
            out += '\n\n'

    # print("OUT:", out)
    return out


def parse_set(
        path, dateStart, dateEnd,
        model, tag, storePrefix, fileMask, regionSlug):
    # Checking if file to parse exists
    if not os.path.exists(path):
        print("No file " + path + " found for parsing")
        return "No file " + path + " found for parsing"

    out = ''
    dateEnd = dateEnd if dateEnd else dateStart
    date = str(dateStart.strftime("%Y-%m-%d"))

    # Define working directori
    dateTime = datetime.datetime.now()
    workDir = DATA_DIR + 'extras/sets' \
        + '/' + storePrefix \
        + '/' + model \
        + '/' + str(dateStart) + '--' + str(dateEnd) \
        + '/' + str(dateTime.strftime('%Y%m%d-%H%M%S'))
    # Check and create working directori
    if not os.path.exists(workDir):
        os.makedirs(workDir)

    # Extract fileName from path
    fileName = os.path.basename(path)

    commonLogFile = workDir + '/common.log'

    print("PARSING FILE: ", path)  # 4 testing
    region = Region.objects.get(slug=regionSlug)
    input_ = '--input=' + path
    output_ = '--output=' + workDir + '/' \
        + fileName + '.txt'
    shape = '--shape=wkt://' + \
        str(region.mpoly).strip()
    dataset = '--dataset=' + tag
    logFile = '--logfile=' + workDir + '/' \
        + fileName + '.log'
    since = '--since=' + str(dateStart) + ' ' \
        + '00:00:00'
    till = '--till=' + str(dateEnd) + ' ' \
        + '23:00:00'

    # out = parse_using_extras_task(  # 4 testing
    parse_using_extras_task.delay(
        model, tag, input_, output_, shape,
        dataset, date, logFile, commonLogFile,
        since, till,)

    print("Ended with parsing query")  # 4 testing
    out += 'job quered for file "' + str(path) +'"' + '\n\n'

    return out


def parse_test(
        path, dateStart, dateEnd,
        model, tag, storePrefix, fileMask, regionSlug):
    # Checking if file to parse exists
    if not os.path.exists(path):
        print("No file " + path + " found for parsing")
        return "No file " + path + " found for parsing"

    out = ''
    dateEnd = dateEnd if dateEnd else dateStart
    date = str(dateStart.strftime("%Y-%m-%d"))

    # Define working directori
    dateTime = datetime.datetime.now()
    workDir = DATA_DIR + 'extras/sets' \
        + '/' + storePrefix \
        + '/' + model \
        + '/' + str(dateStart) + '--' + str(dateEnd) \
        + '/' + str(dateTime.strftime('%Y%m%d-%H%M%S'))
    # Check and create working directori
    if not os.path.exists(workDir):
        os.makedirs(workDir)

    # Extract fileName from path
    fileName = os.path.basename(path)

    commonLogFile = workDir + '/common.log'

    print("PARSING FILE: ", path)  # 4 testing
    region = Region.objects.get(slug=regionSlug)
    input_ = '--input=' + path
    output_ = '--output=' + workDir + '/' \
        + fileName + '.txt'
    shape = '--shape=wkt://' + \
        str(region.mpoly).strip()
    dataset = '--dataset=' + tag
    logFile = '--logfile=' + workDir + '/' \
        + fileName + '.log'
    since = '--since=' + str(dateStart) + ' ' \
        + '00:00:00'
    till = '--till=' + str(dateEnd) + ' ' \
        + '23:00:00'

    out = parse_using_extras_task(  # 4 testing
    # parse_using_extras_task.delay(
        model, tag, input_, output_, shape,
        dataset, date, logFile, commonLogFile,
        since, till,)

    print("Ended with parsing query")  # 4 testing
    out += 'job quered for file "' + str(path) +'"' + '\n\n'

    return out
