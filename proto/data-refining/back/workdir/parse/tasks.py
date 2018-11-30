import os
import time
import datetime
import timeit
import dateutil.parser
import subprocess

from django.contrib.gis.geos import Point
from celery.decorators import task
from celery.utils.log import get_task_logger

from app.settings import DEBUG
from ocean.models import *


logger = get_task_logger(__name__)


@task(name="Parse using extras")
def parse_using_extras_task(
    model, attribute, input_, output_, shape, dataset, date, logFile,
    commonLogFile, since, till):
    out = ''
    precision = '--precision=1000'
    args = ["./extras/modisnetcdfparser/ncparse", input_, output_,
        shape, dataset, precision, logFile, since, till]
    data = []
    workFile = output_[9:]
    data = []
    chanks = 0
    chankSize = 10000
    taskNumber = 0
    points = 0

    # print('date', date[0:10], '>', model)
    # return out

    # External parser calling
    subResult = subprocess.Popen(args, stdout=subprocess.PIPE)
    subResult.wait()
    subOut, err = subResult.communicate()
    returnCode = subResult.returncode
    subOut = str(subOut)

    if returnCode == 0:
        out += 'Parsing file: ' + workFile + ';'
        if subOut.find('Result=Hit') > 0:
            if os.path.exists(workFile):
                fyle = open(workFile)
                for row in fyle:
                    value = str(row.split('/')[2])
                    strDate =  str(row.split('/')[3][:-2])

                    # Skipping for unset values
                    if value.find('nan') > 0 \
                    or (attribute == 'chlor_a' and value == '-32767.000000'):
                        continue

                    # Convert point data value from string to float
                    fvalue = float(value)

                    # Get date from parsed data if exists,
                    # or use value from function attribute
                    if strDate == '-1' or strDate == 'undefined':
                        datetime_ = date
                    else:
                        datetime_ = strDate

                    # Increase chanks counter
                    chanks += 1

                    data.append({
                        'lon': float(row.split('/')[0]),
                        'lat': float(row.split('/')[1]),
                        'datetime': datetime_,
                        'value': fvalue,
                    })
                    if chanks >= chankSize:
                        create_data_points_task(data, model)  # 4 testing
                        # create_data_points_task.delay(data, model)

                        # print(attribute, "DATA ADDED", value)
                        points += chanks
                        chanks = 0
                        data = []
                        taskNumber += 1

                create_data_points_task(data, model)  # 4 testing
                # create_data_points_task.delay(data, model)

                points += chanks
                taskNumber += 1
                fyle.close()

                # Remove staging data file in production
                if not DEBUG:
                    os.remove(workFile)

                out += 'tasks: ' + str(taskNumber) + ';' + \
                    ' points: ' + str(points)
        elif subOut.find('Result=Miss') > 0:
            # print('missed totally!')
            out += ' missed totally!'
        elif subOut.find('Result=Failure') > 0:
            # print('failure')
            out += ' failure'
        # Writing log
        dateTime = datetime.datetime.now()
        with open(commonLogFile, "a") as text_file:
            text_file.write(
                dateTime.strftime('[%Y%m%d-%H:%M:%S] ') + out + "\n\n")
    else:
        out += 'non-zero exit: ' + str(returnCode) + ';'

    return str(out + '<br />')


@task(name="Creating data points")
def create_data_points_task(data, model):
    logger.info("Creating data points")

    instances = [
        eval(model)(
            point = Point(float(key["lon"]), float(key["lat"])),
            datetime = key["datetime"],
            value = key["value"],
        )
        for key in data
    ]
    eval(model).objects.bulk_create(instances)
