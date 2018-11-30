# dump
from itertools import chain
import json
import os
import time

from django.core.serializers import serialize
from celery.decorators import task
from celery.utils.log import get_task_logger
from celery.result import AsyncResult
from hurry.filesize import size

from app.settings import DEBUG
from ocean.models import *
from ocean.serializers import *
from dump.models import *


logger = get_task_logger(__name__)

# Define storage directory for dump files
storagePath = '/data/json/'
if not os.path.exists(storagePath):
    os.makedirs(storagePath)


@task(bind=True, name="Creating query dump")
def create_dump_task(
        self, date, region, models=[], forceUpdate=True):
    data = {}
    start_time = time.time()
    logger.info("creating query dump")

    # Getting the region value
    try:
        region = Region.objects.get(slug=region)
    except:
        region = Region.objects.get(slug='default')
    # print("region", str(region.mpoly))

    # Define json dump file name
    flatModels = ''
    for model in models:
        flatModels += model + '-'
    fyleName = str(date) + '-' + str(region.slug) + '-' + flatModels \
        + '.json'
    jsonFile = storagePath + fyleName

    # Check if file not exists, or forcing update
    # then making query and dump file
    if not os.path.exists(jsonFile) or forceUpdate:
        # Remove file if exists
        if os.path.exists(jsonFile):
            os.remove(jsonFile)

        # Open file for append writing
        fyle = open(jsonFile, 'a')
        fyle.write('[')

        for key, model in enumerate(models):
            # Open model array tag in json file
            fyle.write('\n{"' + model + '": [')

            # Models types exceptions
            if model == 'Bathymetric':
                day = '18.03.2015'
            else:
                day = date[:10]

            # print('day:' + str(day))  # 4 testing
            startDate = day + ' 00:00:00'
            endDate = day + ' 23:59:59'

            # Declare queryset to execute later
            qs = eval(model).objects.filter(
                datetime__range=(startDate, endDate),
                point__intersects=str(region.mpoly),
            )
            # )[0:3]  # 4 testing

            # Writing queryset data to file using iterator method
            # to stop Django from caching database query data
            firstRow = True
            firstRowData = ''
            for row in qs.iterator():
                rowData = '{' \
                    + '"pk": ' + str(row.pk) + ', ' \
                    + '"datetime": "' + str(row.datetime) + '", ' \
                    + '"point": "' + str(row.point) + '", ' \
                    + '"value": ' + str(row.value) + '}'
                if firstRow:
                    firstRow = False
                    firstRowData = rowData
                else:
                    fyle.write(rowData + ',')
            fyle.write(firstRowData)

            # Close model array tag in json file
            fyle.write(']}')
            if key != (len(models) - 1):
                fyle.write(',')
            fyle.write('\n')

        fyle.write(']')
        # Close file for writing
        fyle.close()

    data["file"] = fyleName
    data["elapsedTime"] = round((time.time() - start_time), 1)

    # Update corresponding dump task
    try:
        update_task(self.request.id, data)
    except:
        print('No proper dump task record found in database')

    return (data)


def update_task(id, result):
    dumpTask = DumpTask.objects.get(id=id)
    dumpTask.status = 2
    dumpTask.file_name = result["file"]
    dumpTask.file_size = size(os.path.getsize(
        storagePath + result["file"]))
    dumpTask.elapsed_time = result["elapsedTime"]
    dumpTask.save()


def update_tasks():
    # Get all dump tasks in progress: status = 1
    qs = DumpTask.objects.filter(status=1)
    for dumpTask in qs:
        try:
            celeryTask = AsyncResult(dumpTask.id)
            # print('celeryTask', celeryTask.status)
            # If celery task is accomplished, update according data
            if celeryTask.successful():
                result = celeryTask.result
                dumpTask.status = 2  # celery task is ready
                dumpTask.file_name = result["file"]
                dumpTask.file_size = size(os.path.getsize(
                    storagePath + result["file"]))
                dumpTask.elapsed_time = result["elapsedTime"]
                dumpTask.save()
        except:
            pass
            dumpTask.delete()

    # Get all ready tasks, delete task if dump file does not exists
    qs = DumpTask.objects.filter(status=2)
    for dumpTask in qs:
        if not os.path.exists(storagePath + dumpTask.file_name):
            dumpTask.delete()
