from django.contrib import admin

from .models import DumpTask


class DumpTaskAdmin(admin.ModelAdmin):
    list_display = (
        'timestamp', 'status', 'file_name', 'elapsed_time',)
    radio_fields = {'status': admin.VERTICAL}


admin.site.register(DumpTask, DumpTaskAdmin)
