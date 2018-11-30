# back
import time
import os
import logging

# Timer
start_time = time.time()
counter = 0


try:
    if os.environ['DJANGO_ENV'] == 'production':
        print("ENV", os.environ['DJANGO_ENV'])
except:
    pass

# create logger
logger = logging.getLogger('simple_example')
logger.setLevel(logging.DEBUG)

# create console handler and set level to debug
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)

# create formatter
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# add formatter to ch
ch.setFormatter(formatter)

# add ch to logger
logger.addHandler(ch)

# 'application' code
logger.debug('debug message')
logger.info('info message')
logger.warn('warn message')
logger.error('error message')
logger.critical('critical message')


# Timer
print("--- %s seconds ---" % (time.time() - start_time))
