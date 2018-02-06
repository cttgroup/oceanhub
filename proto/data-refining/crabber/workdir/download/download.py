import os
import datetime
import logging
import logging.config

import yaml
from dateutil.rrule import rrule, DAILY

from app.settings import DATA_DIR
from .tasks import create_download_quere_task


# Setup logging from logging.yaml
def setup_logging(path='logging.yaml'):
    if os.path.exists(path):
        with open(path, 'rt') as f:
            config = yaml.safe_load(f.read())
        logging.config.dictConfig(config)
    else:
        logging.basicConfig(level=logging.INFO)


setup_logging()
# create logger
logger = logging.getLogger('crabber')


def download(
    serverURL, serverMaxConnections, serverPasses,
    dateStart, dateEnd, storePrefix, filesNamesGenerator,
    status):
    out = ''

    dateEnd = dateEnd if dateEnd else dateStart
    for dt in rrule(DAILY, dtstart=dateStart, until=dateEnd):
        date = str(dt.strftime("%Y-%m-%d"))
        storePath = DATA_DIR + str(date) + '/'
        if storePrefix != '':
            storePath += storePrefix + '/'

        # Check and create data store directory
        if not os.path.exists(storePath):
            os.makedirs(storePath)

        logger.info('download task quered for server: ' + serverURL)
        # create_download_quere_task(
        create_download_quere_task.delay(
            serverURL, serverMaxConnections,
            serverPasses,
            filesNamesGenerator,
            storePath, date
        )
    out += 'download task quered for server: ' + serverURL
    # print("YES", logger.StreamHandler())

    return out
