import datetime
import os
import re
import requests
import subprocess
import time

from celery.decorators import task
from celery.utils.log import get_task_logger

from app.settings import zredis


logger = get_task_logger(__name__)


@task(name="Create modis quere")
def create_download_quere_task(
    serverURL, serverMaxConnections, serverPasses,
    filesNamesGenerator,
    storePath, date):
    logger.info("Create modis quere")
    out = ''
    count = 0

    # Executing task in several passes for better file
    # download result, because server not always responses correctly
    # for passes in range(1):  # 4 testing
    for passes in range(serverPasses):
        print("DOWNLOADING")
        for fileName in eval(filesNamesGenerator, {
                "date": datetime.datetime.strptime(date, "%Y-%m-%d"),
            }):
            out += download_task(serverURL, fileName, storePath)
    return out


@task(name="Download MODIS data")
def download_task(serverURL, fileName, storePath):
    logger.info("Downloading MODIS file")
    out = ''
    url = serverURL + fileName
    filePath = storePath + fileName

    if not os.path.exists(filePath):
    # if 1:
        out += get_file(url, storePath)
    else:
        out += datetime.datetime.now().strftime('[%H:%M:%S-%Y%m%d] ')
        out += 'skipping, file already exists: ' + filePath + '\n\n'

    return out


def get_file(url,  storePath):
    out = ''

    args = ["wget", url]
    subResult = subprocess.Popen(
        args, cwd=storePath, stdout=subprocess.PIPE)
    subResult.wait()
    returnCode = subResult.returncode
    subOut, subErr = subResult.communicate()
    subOut = str(subOut)
    subErr = str(subErr)

    if returnCode == 0:
        out += datetime.datetime.now().strftime('[%H:%M:%S-%Y%m%d] ')
        out += 'wget success for file: ' + url + '\n\n'
    else:
        out += datetime.datetime.now().strftime('[%H:%M:%S-%Y%m%d] ')
        out += 'wget failed, exit status "' + str(returnCode) \
             + '" for file: ' + url + '\n\n'

    return out


"""
Wget exit codes:
0   No problems occurred.
1   Generic error code.
2   Parse error---for instance, when parsing command-line options, the .wgetrc or .netrc...
3   File I/O error.
4   Network failure.
5   SSL verification failure.
6   Username/password authentication failure.
7   Protocol errors.
8   Server issued an error response.
"""
