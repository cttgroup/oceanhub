from django.contrib import admin

from .models import Task
from ocean.models import Characteristic


class TaskAdmin(admin.ModelAdmin):
    list_display = ('date_start', 'date_end', 'characteristic', )
    radio_fields = {'process_type': admin.VERTICAL}


admin.site.register(Task, TaskAdmin)
