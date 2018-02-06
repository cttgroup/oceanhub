from rest_framework import serializers
from rest_framework.views import APIView
from rest_framework.response import Response

from .models import *


############################################################
# Base models serializers ############################

class DumpTaskSerializer(serializers.ModelSerializer):
    class Meta:
        model = DumpTask
        fields = ('id', 'timestamp', 'status', 'file_name', 'file_size', 'elapsed_time')

# /Base models serializers ############################
############################################################
