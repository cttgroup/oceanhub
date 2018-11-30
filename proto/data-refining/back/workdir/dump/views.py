# dump
import json

from celery.result import AsyncResult
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework import generics
from rest_framework.parsers import JSONParser

from ocean.serializers import *
from .models import *
from .serializers import *
from .tasks import *
from app.settings import DEBUG, MEDIA_URL


def test(request, attribute):
    pass


@csrf_exempt
def create(request):
    data = {}
    if request.method == 'POST':
        data = JSONParser().parse(request)
        # create_dump_task(  # 4 testing
        task = create_dump_task.delay(
            data["date"],
            data["region"],
            data["characteristics"],
        )
        try:
            print("ID", task.task_id)
        except:
            print("no task ID")
        # taskResult = AsyncResult(task.task_id)
        # print("READY?", taskResult.ready())
        dumpTask = DumpTask(
            id = task.task_id,
            status = 1,
            )
        dumpTask.save()

    return JsonResponse(data, safe=False)


def update(request):
    update_tasks()

    return JsonResponse({}, safe=False)


def get(request):
    update_tasks()
    data = {'tasks': [],}
    qs = DumpTask.objects.all()
    serializer = DumpTaskSerializer(qs, many=True)

    return JsonResponse(serializer.data, safe=False)


class DumpTaskList(generics.ListCreateAPIView):
    queryset = DumpTask.objects.all()
    serializer_class = DumpTaskSerializer


class DumpTaskDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = DumpTask.objects.all()
    serializer_class = DumpTaskSerializer
