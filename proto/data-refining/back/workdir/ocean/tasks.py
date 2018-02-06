import os
import time
import datetime
import timeit
import dateutil.parser
import subprocess
from itertools import chain

from django.core import serializers
from django.contrib.gis.geos import Point
from celery.decorators import task
from celery.utils.log import get_task_logger

from app.settings import DEBUG
from ocean.models import *


logger = get_task_logger(__name__)
