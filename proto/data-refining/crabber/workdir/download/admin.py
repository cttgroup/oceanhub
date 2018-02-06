from django.contrib import admin

from .models import Server
from .models import StorePrefix
from .models import Task


class TaskAdmin(admin.ModelAdmin):
    list_display = ('date_start', 'date_end', 'note', )


admin.site.register(Server)
admin.site.register(StorePrefix)
admin.site.register(Task, TaskAdmin)
