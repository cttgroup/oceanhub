from django.forms import widgets
from django.contrib.auth.models import User
from rest_framework import serializers
from rest_framework.views import APIView
from rest_framework.response import Response

from .models import *



############################################################
# Base models serializers ############################
class RegionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Region
        fields = ('pk', 'title', 'slug', 'mpoly')


class CharacteristicSerializer(serializers.ModelSerializer):
    class Meta:
        model = Characteristic
        fields = ('pk', 'title', 'description', 'model', 'tag',
            'store_prefix', 'file_mask', )


class BathymetricSerializer(serializers.ModelSerializer):
    class Meta:
        model = Bathymetric
        fields = ('pk', 'datetime', 'point', 'value')


class SeaSurfaceTemperatureSerializer(serializers.ModelSerializer):
    class Meta:
        model = SeaSurfaceTemperature
        fields = ('pk', 'datetime', 'point', 'value')


class ChlorophyllSerializer(serializers.ModelSerializer):
    class Meta:
        model = Chlorophyll
        fields = ('pk', 'datetime', 'point', 'value')


class SeaIceCoverSerializer(serializers.ModelSerializer):
    class Meta:
        model = SeaIceCover
        fields = ('pk', 'datetime', 'point', 'value')


class TemperatureSerializer(serializers.ModelSerializer):
    class Meta:
        model = Temperature
        fields = ('pk', 'datetime', 'point', 'value')


class TotalCloudCoverSerializer(serializers.ModelSerializer):
    class Meta:
        model = TotalCloudCover
        fields = ('pk', 'datetime', 'point', 'value')


class SeaWindSpeedSerializer(serializers.ModelSerializer):
    class Meta:
        model = SeaWindSpeed
        fields = ('pk', 'datetime', 'point', 'value')
# /Base models serializers ############################
############################################################





############################################################
# Simplified models serializers ############################

class SimpleChlorophyllSerializer(serializers.ModelSerializer):
    class Meta:
        model = Chlorophyll
        fields = ('pk', 'datetime', 'point', 'value')

# /Simplified models serializers ############################
############################################################


class SSTSerializer(serializers.ModelSerializer):
    class Meta:
        model = SeaSurfaceTemperature
        fields = ('pk', 'datetime', 'point', 'value')
