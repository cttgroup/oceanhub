# ocean
import json
import random
import os
import time
from datetime import datetime
from itertools import chain

from django.shortcuts import render
from django.core import serializers
from django.http import HttpResponse
from django.http import JsonResponse
import django_filters.rest_framework
from django.core.cache import cache
from django.test.utils import override_settings
from django.contrib.gis.geos import Point
from django.contrib.gis.db import models
from django.contrib.gis.geos import GEOSGeometry
from django.contrib.gis.geos import Polygon, MultiPolygon
from rest_framework.parsers import JSONParser
from django.views.decorators.csrf import csrf_exempt

from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import generics
from celery.result import AsyncResult

from .models import *
from .serializers import *
from app.settings import DEBUG


def test(request, attribute):
    day = '2016-09-01'
    date = '2016-09-01 00:00:00'
    model = attribute
    data = {'content': 'Test',}
    data["attribute"] = attribute
    model = 'Chlorophyll'
    model = 'SeaSurfaceTemperature'
    # model = 'SeaIceCover'
    # Getting the region value
    try:
        region = Region.objects.get(slug=region)
    except:
        region = Region.objects.get(slug='default')

    start_time = time.time()
    print('Getting cache...')
    # Updating day cache and get cached result
    # cacheResult = create_qs_cache_byday_and_model(day, model)
    print("--- %s seconds ---" % (time.time() - start_time))

    start_time = time.time()
    startDate = day + ' 00:00:00'
    endDate = day + ' 23:59:59'
    print('Making query...')
    qs = eval(model).objects.filter(
    # qs = cacheResult.filter(
        datetime__range=(startDate, endDate),
        point__intersects=str(region.mpoly),
    # )[0:2000000]
    )[0:1000]
    serializer = eval(model + 'Serializer')(qs, many=True)
    data['qs'] = serializer.data
    print("--- %s seconds ---" % (time.time() - start_time))

    return JsonResponse(data, safe=False)
    # return JsonResponse({}, safe=False)


# Creating chache for query set object
# for default region by given date and model
def create_qs_cache_byday_and_model(date, model, renew=False):
    date = str(date)
    model = str(model)
    dateTime = date + ' 00:00:00'
    # Generating query set cache key
    cacheKey = 'qs' + date + model
    # Trying to get cached data
    if not cache.get(cacheKey) or renew:
        # If cache data in not set, making query
        print("Making day cache...")
        region = Region.objects.get(slug='default')
        qs = eval(model).objects.filter(
            datetime=dateTime,
            point__intersects=str(region.mpoly),
        )
        cache.set(cacheKey, qs)
    else:
        print("Got the cached data from memcached")

    return cache.get(cacheKey)


def get_filter(request):
    data = {'regions': [], 'characteristics': [],}

    qs = Region.objects.all()
    serializer = RegionSerializer(qs, many=True)
    data['regions'] = serializer.data

    qs = Characteristic.objects.all()
    serializer = CharacteristicSerializer(qs, many=True)
    data['characteristics'] = serializer.data

    return JsonResponse(data, safe=False)


def api_get_queryset(
        model, _date, region, needed, createDump=True):
    # Returned result array
    result = []

    # Convert date to datetime format
    date = _date + ' 00:00:00'

    # Max number of DB selects have to be done
    select = 20  # select = int(select)

    # Number of points you need
    needed = int(needed)
    if needed < 500:
        needed = 500

    # Getting the region value
    try:
        region = Region.objects.get(slug=region)
    except:
        region = Region.objects.get(slug='default')

    """
    # Creating query dump task if needed
    if createDump:
        task = create_dump_task.delay(
            model, date, str(region), str(region.mpoly))
    """
    # print("ID", task.task_id)
    # taskResult = AsyncResult(task.task_id)
    # time.sleep(5)
    # print(taskResult.ready())
    # print(task.AsyncResult(task.request.id).state)
    # return result  # 4 testing

    # Generating query set cache key
    cacheKey = str(_date) + '-' + str(region.slug) \
        + '-' + str(model) \
        + '-' + str(needed) + '-' \
        + str(select)

    # Define main DB query, execution is delayed
    qsTotal = eval(model).objects.filter(
        datetime=date,
        point__intersects=region.mpoly,
    )

    # Total points found using query filter
    total = qsTotal.count()

    print("Total points found:", total)  # 4 testing

    # Exit if no points found
    if total < 1:
        print("No points found")
        return result

    # Getting points queryset
    # using normalization algorithm
    step = int(round(total / select, -1))
    pull = int(round(step / (total / needed), -1))
    randStep = step - pull
    randStep = randStep if randStep > 0 else step

    # Trying to get cached data
    result = cache.get(cacheKey)

    # If cache data in not set, making query
    if not result:
        result = []
        print("Making query...")
        for i in range(select):
            shift = i * step + random.randint(0, randStep)
            print("Query shift:", shift)
            qs = qsTotal.filter()[shift:shift + pull]
            result = list(chain(result, qs))
        cache.set(cacheKey, result)
    else:
        print("Got the cached data from memcached")

    print("Points selected:", len(result))

    return result


class ApiChlor(generics.ListCreateAPIView):
    serializer_class = ChlorophyllSerializer

    def get_queryset(self):
        qs = api_get_queryset(
            'Chlorophyll',
            self.kwargs['date'],
            self.kwargs['region'],
            self.kwargs['needed'],
        )

        return qs


class ApiSST(generics.ListCreateAPIView):
    serializer_class = SSTSerializer

    def get_queryset(self):
        qs = api_get_queryset(
            'SST',
            self.kwargs['date'],
            self.kwargs['region'],
            self.kwargs['needed'],
        )

        return qs


class ApiBath(generics.ListCreateAPIView):
    serializer_class = BathymetricSerializer

    def get_queryset(self):
        qs = api_get_queryset(
            'Bathymetric',
            '2015-03-18',
            self.kwargs['region'],
            self.kwargs['needed'],
        )

        return qs


class CharacteristicsList(generics.ListCreateAPIView):
    queryset = Characteristic.objects.all()
    serializer_class = CharacteristicSerializer


class CharacteristicsDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Characteristic.objects.all()
    serializer_class = CharacteristicSerializer


class RegionsList(generics.ListCreateAPIView):
    queryset = Region.objects.all()
    serializer_class = RegionSerializer


class RegionsDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Region.objects.all()
    serializer_class = RegionSerializer


def regions_detail(request, argument):
    data = {}
    try:
        pk = int(argument)
        qs = Region.objects.filter(pk=pk)
    except:
        slug = str(argument)
        qs = Region.objects.filter(slug=slug)

    serializer = RegionSerializer(qs, many=True)
    data = serializer.data[0]
    return JsonResponse(data, safe=False)
