from django.contrib import admin

from .models import *


class CharacteristicAdmin(admin.ModelAdmin):
    list_display = (
        'title',
        'tag',
        'store_prefix',
    )


admin.site.register(Region)
admin.site.register(Characteristic, CharacteristicAdmin)

admin.site.register(SeaSurfaceTemperature)
admin.site.register(Chlorophyll)
admin.site.register(Bathymetric)
admin.site.register(SeaIceCover)
admin.site.register(Temperature)
admin.site.register(TotalCloudCover)
admin.site.register(SeaWindSpeed)
