# world
import os
import json
import timeit
from random import randint

from django.shortcuts import render
from django.http import HttpResponse
from django.core import serializers


def netcdf_get(request):
    response = HttpResponse()
    response.write('Filtering points by given polygon...')

    return response
